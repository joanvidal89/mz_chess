#include "scene_game.h"

#include "../mz_director.h"

//------------------------------------------------------------------------------------
// CONSTRUCTOR & DESTRUCTOR
//------------------------------------------------------------------------------------
SceneGame::SceneGame(MZDirector *director) : MZScene(director, SceneType::ST_GAME)
{
    deltaTime = 0.0f;
    currentState = SS_INIT;
    futureScene = ST_GAME;
    currentHoverCell = nullptr;
    lastHoverCell = nullptr;
    currentSelectedCell = nullptr;

    director->board->setAllCellsState(CS_DISABLED);

    if (director->config->currentConfig.video)
        optionButtons[0] = BTN_VID_ON;
    else
        optionButtons[0] = BTN_VID_OFF;

    if (director->config->currentConfig.audio)
        optionButtons[1] = BTN_SND_ON;
    else
        optionButtons[1] = BTN_SND_OFF;

    if (director->config->currentConfig.music)
        optionButtons[2] = BTN_MUS_ON;
    else
        optionButtons[2] = BTN_MUS_OFF;

    optionButtons[3] = BTN_EXIT;
}

SceneGame::~SceneGame() {}

//------------------------------------------------------------------------------------
// SCENE UPDATING
//------------------------------------------------------------------------------------
void SceneGame::update()
{
    // Update Audio
    //--------------------------------------------------------------------------------------
    director->audio->updateAudio();

    // Scene Flow
    //--------------------------------------------------------------------------------------
    switch (currentState)
    {
    case SS_INIT:
        ssInit();
        break;
    case SS_REQUEST_MOVES:
        printf("CLEAR MOVEMENTS\n");
        director->board->clearMovements();
        printf("START PROCESS BOARD\n");
        director->board->startProcessBoard();
        printf("SET ALL CELLS STATE\n");
        director->board->setAllCellsState(CS_DISABLED);
        currentState = SS_AWAIT_MOVES;
        ssAwaitMoves();
        break;
    case SS_AWAIT_MOVES:
        ssAwaitMoves();
        break;
    case SS_PLAYER_TURN:
        ssPlayerTurn();
        break;
    case SS_PLAYER_TURN_PROMOTE:
        ssPlayerTurnPromote();
        break;
    case SS_PLAYER_ANIM_START:
        deltaTime = 0.0f;
        director->board->setAllCellsState(CS_DISABLED);
        currentState = SS_PLAYER_ANIM;
        ssPlayerAnimation();
    case SS_PLAYER_ANIM:
        ssPlayerAnimation();
        break;
    case SS_REQUEST_AI:
        director->uciEngine->startFindBestMove();
        director->board->setAllCellsState(CS_DISABLED);
        currentState = SS_AWAIT_AI;
        ssAwaitAI();
        break;
    case SS_AWAIT_AI:
        ssAwaitAI();
        break;
    case SS_AI_ANIM_START:
        deltaTime = 0.0f;
        currentState = SS_AI_ANIM;
        ssAIAnimation();
    case SS_AI_ANIM:
        ssAIAnimation();
        break;
    case SS_OUT:
        ssOut();
        break;
    default:
        break;
    }

    deltaTime += GetFrameTime();
}

void SceneGame::ssInit()
{
    // Update Camera
    //--------------------------------------------------------------------------------------
    director->composite->updateBaseCamPosition(
        Math::vector3Lerp(menuCamera.position, gameCamera.position, Math::easeInOutExpo(deltaTime)));
    director->composite->updateBaseCamTarget(
        Math::vector3Lerp(menuCamera.target, gameCamera.target, Math::easeInOutExpo(deltaTime)));
    director->composite->updateBaseCamFov(
        Math::floatLerp(menuCamera.fovy, gameCamera.fovy, Math::easeInOutExpo(deltaTime)));
    director->composite->updateFinalCam();

    // Draw Render
    //--------------------------------------------------------------------------------------
    drawDefaultRender();

    // Handle Duration
    //--------------------------------------------------------------------------------------
    if (deltaTime >= 1.0f)
    {
        director->composite->updateBaseCamPosition(gameCamera.position);
        director->composite->updateBaseCamTarget(gameCamera.target);
        director->composite->updateBaseCamFov(gameCamera.fovy);
        director->composite->updateFinalCam();

        deltaTime = 0.0f;
        printf("CURRENT STATE = SS_REQUEST_MOVES\n");
        currentState = SS_REQUEST_MOVES;
    }
}

void SceneGame::ssOut()
{
    // Update Camera
    //--------------------------------------------------------------------------------------
    director->composite->updateBaseCamPosition(
        Math::vector3Lerp(gameCamera.position, menuCamera.position, Math::easeInOutExpo(deltaTime)));
    director->composite->updateBaseCamTarget(
        Math::vector3Lerp(gameCamera.target, menuCamera.target, Math::easeInOutExpo(deltaTime)));
    director->composite->updateBaseCamFov(
        Math::floatLerp(gameCamera.fovy, menuCamera.fovy, Math::easeInOutExpo(deltaTime)));
    director->composite->updateFinalCam();

    // Draw Render
    //--------------------------------------------------------------------------------------
    drawDefaultRender();

    // Handle Duration
    //--------------------------------------------------------------------------------------
    if (deltaTime >= 1.0f)
    {
        director->composite->updateBaseCamPosition(menuCamera.position);
        director->composite->updateBaseCamTarget(menuCamera.target);
        director->composite->updateBaseCamFov(menuCamera.fovy);
        director->composite->updateFinalCam();

        deltaTime = 0.0f;
        director->newScene = futureScene;
    }
}

void SceneGame::ssAwaitMoves()
{
    // Draw Render
    //--------------------------------------------------------------------------------------
    drawDefaultRender();

    // Handle Response
    //--------------------------------------------------------------------------------------
    if (director->board->isProcessBoardReady())
    {
        printf("CURRENT STATE = SS_PLAYER_TURN\n");
        currentState = SS_PLAYER_TURN;
        director->board->setAllCellsState(CS_ENABLED);

        // CHECK LOSS - NO AVAIABLE MOVES
        if (director->board->checkLoss())
        {
            director->audio->playDefeatSound();
            futureScene = ST_MENU_POSTGAME;
            currentState = SS_OUT;
            deltaTime = 0.0f;
        }
    }
}

void SceneGame::ssAwaitAI()
{
    // Draw Render
    //--------------------------------------------------------------------------------------
    drawDefaultRender();

    // Handle Response
    //--------------------------------------------------------------------------------------
    if (director->uciEngine->isReadyBestMove())
    {
        std::string response = director->uciEngine->getBestMoveResponse();
        printf("MOVE RESPONSE: %s\n", response.c_str());

        // HANDLE AI LOSS
        if (response.find("none") != std::string::npos)
        {
            director->audio->playVictorySound();
            futureScene = ST_MENU_POSTGAME;
            printf("CURRENT STATE = SS_OUT\n");
            currentState = SS_OUT;
            deltaTime = 0.0f;
        }
        else
        {
            // DEBUG: I CAN OVERRIDE AI RESPONSE
            // std::getline(std::cin, response);

            director->uciEngine->moves.push_back(response.substr(0, 4));

            currentAIMove = PieceMove{BoardPosition{response[0] - 'a', response[1] - '1'},
                                      BoardPosition{response[2] - 'a', response[3] - '1'}, MT_MOVE};

            Cell *origin = &director->board->getBoardCell(currentAIMove.origin);
            Cell *target = &director->board->getBoardCell(currentAIMove.target);

            origin->pieceState = PS_MOVE;

            // CHECK FOR PAWN DOUBLE MOVE
            if (origin->pieceType == PT_PAWN && origin->position.rank == target->position.rank + 2)
            {
                currentAIMove.type = MT_DOUBLEMOVE;
            }

            // CHECK FOR AI ATTACK
            if (target->pieceColor == CC_WHITE)
            {
                target->pieceState = PS_DIE;
                currentAIMove.type = MT_ATTACK;
            }

            // CHECK FOR AI EN PASSANT ATTACK
            if (origin->pieceType == PT_PAWN && origin->position.file != target->position.file && target->pieceColor == CC_NONE)
            {
                director->board->getBoardCell(BoardPosition{target->position.file, target->position.rank + 1}).pieceState = PS_DIE;
                currentAIMove.type = MT_ENPASSANT;
            }

            // CHECK FOR AI PROMOTION
            if (origin->pieceType == PT_PAWN && target->position.rank == 0)
            {
                if (currentAIMove.type == MT_ATTACK)
                {
                    currentAIMove.type = MT_ATTACKPROMOTE;
                }
                else
                {
                    currentAIMove.type = MT_PROMOTE;
                }
                char promotion = response[4];
                switch (promotion)
                {
                case 'r':
                    origin->pieceState = PS_PROMOTE_ROOK;
                    break;
                case 'n':
                    origin->pieceState = PS_PROMOTE_KNIGHT;
                    break;
                case 'b':
                    origin->pieceState = PS_PROMOTE_BISHOP;
                    break;
                case 'q':
                    origin->pieceState = PS_PROMOTE_QUEEN;
                    break;
                default:
                    break;
                }
            }

            // CHECK FOR CASTLES
            if (origin->pieceType == PT_KING)
            {
                // CHECK FOR LONG CASTLE
                if (origin->position.file == 4 && origin->position.rank == 7 && target->position.file == 2 && target->position.rank == 7)
                {
                    currentAIMove.type = MT_LONG_CASTLE;
                    director->board->getBoardCell(BoardPosition{0, 7}).pieceState = PS_CASTLE_LONG;
                }
                // CHECK FOR SHORT CASTLE
                else if (origin->position.file == 4 && origin->position.rank == 7 && target->position.file == 6 && target->position.rank == 7)
                {
                    currentAIMove.type = MT_SHORT_CASTLE;
                    director->board->getBoardCell(BoardPosition{7, 7}).pieceState = PS_CASTLE_SHORT;
                }
            }

            printf("CURRENT STATE = SS_AI_ANIM_START\n");
            currentState = SS_AI_ANIM_START;
        }
    }
}

void SceneGame::ssPlayerTurn()
{
    // Handle Input
    //--------------------------------------------------------------------------------------
    director->input->raycastBotScreen(director->composite->getFinalCamera());

    // Handle Back Button
    //--------------------------------------------------------------------------------------
    if (director->input->isVirtualCursorOnRect(BTN_BACK.drawRect))
        BTN_BACK.state = BS_HOVER;
    else
        BTN_BACK.state = BS_ENABLED;

    if (director->input->getMouseButtonDown() == 1 && BTN_BACK.state == BS_HOVER)
    {
        director->audio->playCancelSound();
        futureScene = ST_MENU_POSTGAME;
        printf("CURRENT STATE = SS_OUT\n");
        currentState = SS_OUT;
        deltaTime = 0.0f;
    }

    // Handle Cell Selection
    //--------------------------------------------------------------------------------------
    if (currentSelectedCell == nullptr)
    {
        // Clean values
        //--------------------------------------------------------------------------------------
        if (currentHoverCell != nullptr)
        {
            currentHoverCell->cellState = CS_ENABLED;
            lastHoverCell = currentHoverCell;
            currentHoverCell = nullptr;
        }

        // Check hover
        //--------------------------------------------------------------------------------------
        for (Cell &c : director->board->getBoardCells())
        {
            if (director->input->isVirtualCursorOnRect(c.drawRect))
            {
                currentHoverCell = &c;
                c.cellState = CS_HOVER_ENABLED;
                if (c.pieceColor == CC_WHITE && !c.pieceMoves.empty() && ((lastHoverCell == nullptr) || (lastHoverCell != currentHoverCell)))
                {
                    director->audio->playHoverSound();
                }
                break;
            }
        }

        // Check Input
        //--------------------------------------------------------------------------------------
        if (director->input->getMouseButtonDown() == 1 && currentHoverCell != nullptr &&
            currentHoverCell->pieceColor == CC_WHITE && !currentHoverCell->pieceMoves.empty())
        {
            director->audio->playAcceptSound();
            director->board->setAllCellsState(CS_DISABLED);
            currentHoverCell->cellState = CS_SELECTED;
            currentSelectedCell = currentHoverCell;
            for (PieceMove &pm : currentSelectedCell->pieceMoves)
            {
                switch (pm.type)
                {
                case MT_ATTACK:
                case MT_ATTACKPROMOTE:
                    director->board->setCellState(pm.target, CS_ATTACKED);
                    break;
                default:
                    director->board->setCellState(pm.target, CS_MOVABLE);
                    break;
                }
            }
            currentHoverCell = nullptr;
        }
    }
    // Handle Move Selection
    //--------------------------------------------------------------------------------------
    else
    {
        // Clean Values
        //--------------------------------------------------------------------------------------
        if (currentHoverCell != nullptr)
        {
            switch (currentHoverCell->cellState)
            {
            case CS_HOVER_MOVABLE:
                currentHoverCell->cellState = CS_MOVABLE;
                break;
            case CS_HOVER_ATTACKED:
                currentHoverCell->cellState = CS_ATTACKED;
            default:
                break;
            }
            lastHoverCell = currentHoverCell;
            currentHoverCell = nullptr;
            currentPieceMove = nullptr;
        }

        // Check hover
        //--------------------------------------------------------------------------------------
        for (PieceMove &pm : currentSelectedCell->pieceMoves)
        {
            Cell &c = director->board->getBoardCell(pm.target);

            if (director->input->isVirtualCursorOnRect(c.drawRect))
            {
                currentHoverCell = &c;
                currentPieceMove = &pm;
                switch (c.cellState)
                {
                case CS_MOVABLE:
                    c.cellState = CS_HOVER_MOVABLE;
                    break;
                case CS_ATTACKED:
                    c.cellState = CS_HOVER_ATTACKED;
                default:
                    break;
                }
                if ((lastHoverCell == nullptr) || (lastHoverCell != currentHoverCell))
                {
                    director->audio->playHoverSound();
                }
                break;
            }
        }

        // Check Input
        //--------------------------------------------------------------------------------------
        if (director->input->getMouseButtonDown() == 1 && currentHoverCell != nullptr)
        {
            director->audio->playAcceptSound();

            currentSelectedCell->pieceState = PS_MOVE;
            switch (currentPieceMove->type)
            {
            case MT_ATTACK:
                director->board->getBoardCell(BoardPosition{currentPieceMove->target.file, currentPieceMove->target.rank}).pieceState = PS_DIE;
                break;
            case MT_ATTACKPROMOTE:
                currentSelectedCell->pieceState = PS_PROMOTE;
                director->board->getBoardCell(BoardPosition{currentPieceMove->target.file, currentPieceMove->target.rank}).pieceState = PS_DIE;
                break;
            case MT_SHORT_CASTLE:
                director->board->getBoardCell(BoardPosition{7, 0}).pieceState = PS_CASTLE_SHORT;
                break;
            case MT_LONG_CASTLE:
                director->board->getBoardCell(BoardPosition{0, 0}).pieceState = PS_CASTLE_LONG;
                break;
            case MT_PROMOTE:
                currentSelectedCell->pieceState = PS_PROMOTE;
                break;
            default:
                break;
            }

            director->board->setAllCellsState(CS_DISABLED);

            if (currentSelectedCell->pieceState == PS_PROMOTE)
            {
                director->board->setCellState(currentSelectedCell->position, CS_SELECTED);
                printf("CURRENT STATE = SS_PLAYER_TURN_PROMOTE\n");
                currentState = SS_PLAYER_TURN_PROMOTE;

                // TODO - DELETE THIS LINE
                deltaTime = 0.0f;
            }
            else
            {
                printf("CURRENT STATE = SS_PLAYER_ANIM_START\n");
                currentState = SS_PLAYER_ANIM_START;
            }
        }
        else if (director->input->getMouseButtonDown() == 2)
        {
            director->audio->playCancelSound();
            director->board->setAllCellsState(CS_ENABLED);
            currentSelectedCell = nullptr;
            currentPieceMove = nullptr;
        }
    }

    drawDefaultRender();
    director->board->addPlayerTime(GetFrameTime());
}

void SceneGame::ssPlayerTurnPromote()
{
    // Handle Input
    //--------------------------------------------------------------------------------------
    director->input->raycastBotScreen(director->composite->getFinalCamera());

    // Draw 2D
    //--------------------------------------------------------------------------------------
    director->renderVS->beginRT();
    for (UIButton opt : optionButtons)
    {
        director->renderVS->drawOption(opt, BS_DISABLED);
    }
    for (Cell c : director->board->getBoardCells())
    {
        director->renderVS->drawCell(c);
    }

    //TODO INTERFACE

    director->renderVS->drawGameButton(BTN_BACK);
    director->renderVS->endRT();

    // Draw 3D
    //--------------------------------------------------------------------------------------
    director->composite->beginRT();
    director->composite->beginMode3D();
    director->render3D->drawArcade();
    for (Cell c : director->board->getBoardCells())
    {
        if (c.pieceType != PT_NONE)
        {
            director->render3D->drawPieceReflection(c, 1.0f);
        }
    }
    director->renderVS->drawArcadeVS();
    for (Cell c : director->board->getBoardCells())
    {
        if (c.pieceType != PT_NONE)
        {
            director->render3D->drawPiece(c, 1.0f);
        }
    }
    director->composite->endMode3D();
    director->composite->drawCursor();
    director->composite->endRT();

    // Draw Composite
    //--------------------------------------------------------------------------------------
    director->composite->beginDrawing();
    director->composite->drawComposite();
    director->composite->endDrawing();

    if (deltaTime > 0.5f)
    {
        // HANDLE SELECTION
        currentSelectedCell->cellState = CS_DISABLED;
        currentSelectedCell->pieceState = PS_PROMOTE_QUEEN;
        printf("CURRENT STATE = SS_PLAYER_ANIM_START\n");
        currentState = SS_PLAYER_ANIM_START;
    }
}

void SceneGame::ssPlayerAnimation()
{
    // Draw 2D
    //--------------------------------------------------------------------------------------
    director->renderVS->beginRT();
    for (Cell c : director->board->getBoardCells())
    {
        director->renderVS->drawCell(c);
    }

    // TODO INTERFACE

    director->renderVS->drawGameButton(BTN_BACK);
    director->renderVS->endRT();

    // Draw 3D
    //--------------------------------------------------------------------------------------
    director->composite->beginRT();
    director->composite->beginMode3D();
    director->render3D->drawArcade();
    for (Cell &c : director->board->getBoardCells())
    {
        if (c.pieceType != PT_NONE)
        {
            switch (c.pieceState)
            {
            case PS_MOVE:
                director->render3D->drawPieceReflectionEx(c, *currentPieceMove, deltaTime * 2, 1.0f);
                break;
            case PS_PROMOTE_ROOK:
                director->render3D->drawPieceReflectionEx(c, *currentPieceMove, deltaTime * 2, 1.0f - deltaTime * 2);
                c.pieceType = PT_ROOK;
                director->render3D->drawPieceReflectionEx(c, *currentPieceMove, deltaTime * 2, deltaTime * 2);
                c.pieceType = PT_PAWN;
                break;
            case PS_PROMOTE_KNIGHT:
                director->render3D->drawPieceReflectionEx(c, *currentPieceMove, deltaTime * 2, 1.0f - deltaTime * 2);
                c.pieceType = PT_KNIGHT;
                director->render3D->drawPieceReflectionEx(c, *currentPieceMove, deltaTime * 2, deltaTime * 2);
                c.pieceType = PT_PAWN;
                break;
            case PS_PROMOTE_BISHOP:
                director->render3D->drawPieceReflectionEx(c, *currentPieceMove, deltaTime * 2, 1.0f - deltaTime * 2);
                c.pieceType = PT_BISHOP;
                director->render3D->drawPieceReflectionEx(c, *currentPieceMove, deltaTime * 2, deltaTime * 2);
                c.pieceType = PT_PAWN;
                break;
            case PS_PROMOTE_QUEEN:
                director->render3D->drawPieceReflectionEx(c, *currentPieceMove, deltaTime * 2, 1.0f - deltaTime * 2);
                c.pieceType = PT_QUEEN;
                director->render3D->drawPieceReflectionEx(c, *currentPieceMove, deltaTime * 2, deltaTime * 2);
                c.pieceType = PT_PAWN;
                break;
            case PS_CASTLE_SHORT:
                director->render3D->drawPieceReflectionEx(c, PieceMove{BoardPosition{7, 0}, BoardPosition{5, 0}, MT_SHORT_CASTLE}, deltaTime * 2, 1.0f);
                break;
            case PS_CASTLE_LONG:
                director->render3D->drawPieceReflectionEx(c, PieceMove{BoardPosition{0, 0}, BoardPosition{3, 0}, MT_LONG_CASTLE}, deltaTime * 2, 1.0f);
                break;
            case PS_DIE:
                director->render3D->drawPieceReflection(c, 1.0f - deltaTime * 2);
                break;
            default:
                director->render3D->drawPieceReflection(c, 1.0f);
                break;
            }
        }
    }
    director->renderVS->drawArcadeVS();
    for (Cell c : director->board->getBoardCells())
    {
        if (c.pieceType != PT_NONE)
        {
            switch (c.pieceState)
            {
            case PS_MOVE:
                director->render3D->drawPieceEx(c, *currentPieceMove, deltaTime * 2, 1.0f);
                break;
            case PS_PROMOTE_ROOK:
                director->render3D->drawPieceEx(c, *currentPieceMove, deltaTime * 2, 1.0f - deltaTime * 2);
                c.pieceType = PT_ROOK;
                director->render3D->drawPieceEx(c, *currentPieceMove, deltaTime * 2, deltaTime * 2);
                c.pieceType = PT_PAWN;
                break;
            case PS_PROMOTE_KNIGHT:
                director->render3D->drawPieceEx(c, *currentPieceMove, deltaTime * 2, 1.0f - deltaTime * 2);
                c.pieceType = PT_KNIGHT;
                director->render3D->drawPieceEx(c, *currentPieceMove, deltaTime * 2, deltaTime * 2);
                c.pieceType = PT_PAWN;
                break;
            case PS_PROMOTE_BISHOP:
                director->render3D->drawPieceEx(c, *currentPieceMove, deltaTime * 2, 1.0f - deltaTime * 2);
                c.pieceType = PT_BISHOP;
                director->render3D->drawPieceEx(c, *currentPieceMove, deltaTime * 2, deltaTime * 2);
                c.pieceType = PT_PAWN;
                break;
            case PS_PROMOTE_QUEEN:
                director->render3D->drawPieceEx(c, *currentPieceMove, deltaTime * 2, 1.0f - deltaTime * 2);
                c.pieceType = PT_QUEEN;
                director->render3D->drawPieceEx(c, *currentPieceMove, deltaTime * 2, deltaTime * 2);
                c.pieceType = PT_PAWN;
                break;
            case PS_CASTLE_SHORT:
                director->render3D->drawPieceEx(c, PieceMove{BoardPosition{7, 0}, BoardPosition{5, 0}, MT_SHORT_CASTLE}, deltaTime * 2, 1.0f);
                break;
            case PS_CASTLE_LONG:
                director->render3D->drawPieceEx(c, PieceMove{BoardPosition{0, 0}, BoardPosition{3, 0}, MT_LONG_CASTLE}, deltaTime * 2, 1.0f);
                break;
            case PS_DIE:
                director->render3D->drawPiece(c, 1.0f - deltaTime * 2);
                break;
            default:
                director->render3D->drawPiece(c, 1.0f);
                break;
            }
        }
    }
    director->composite->endMode3D();
    director->composite->drawCursor();
    director->composite->endRT();

    // Draw Composite
    //--------------------------------------------------------------------------------------
    director->composite->beginDrawing();
    director->composite->drawComposite();
    director->composite->endDrawing();

    if (deltaTime >= 0.5f)
    {
        deltaTime = 0.0f;

        // HANDLE NOTATION

        std::string notation;
        notation += 'a' + currentPieceMove->origin.file;
        notation += '1' + currentPieceMove->origin.rank;
        notation += 'a' + currentPieceMove->target.file;
        notation += '1' + currentPieceMove->target.rank;

        // HANDLE PROMOTION
        if (currentPieceMove->type == MT_PROMOTE || currentPieceMove->type == MT_ATTACKPROMOTE)
        {
            Cell *c = &director->board->getBoardCell(currentPieceMove->origin);
            switch (c->pieceState)
            {
            case PS_PROMOTE_BISHOP:
                c->pieceType = PT_BISHOP;
                notation += 'b';
                break;
            case PS_PROMOTE_ROOK:
                c->pieceType = PT_ROOK;
                notation += 'r';
                break;
            case PS_PROMOTE_KNIGHT:
                c->pieceType = PT_KNIGHT;
                notation += 'n';
                break;
            case PS_PROMOTE_QUEEN:
                c->pieceType = PT_QUEEN;
                notation += 'q';
                break;
            default:
                break;
            }
        }

        director->uciEngine->moves.push_back(notation);

        // DELETE ENPASSANT PAWN
        if (currentPieceMove->type == MT_ENPASSANT)
        {
            director->board->deletePiece(BoardPosition{currentPieceMove->target.file, currentPieceMove->target.rank - 1});
        }

        // DELETE ATTACKED PIECE
        if (currentPieceMove->type == MT_ATTACK ||
            currentPieceMove->type == MT_ATTACKPROMOTE)
        {
            director->board->deletePiece(BoardPosition{currentPieceMove->target.file, currentPieceMove->target.rank});
        }

        // MOVE THE PIECE
        director->board->movePiece(BoardPosition{currentPieceMove->origin.file, currentPieceMove->origin.rank},
                                   BoardPosition{currentPieceMove->target.file, currentPieceMove->target.rank},
                                   currentPieceMove->type);

        // MOVE THE CASTLED PIECES
        if (currentPieceMove->type == MT_SHORT_CASTLE)
        {
            director->board->movePiece(BoardPosition{7, 0}, BoardPosition{5, 0}, MT_SHORT_CASTLE);
        }
        if (currentPieceMove->type == MT_LONG_CASTLE)
        {
            director->board->movePiece(BoardPosition{0, 0}, BoardPosition{3, 0}, MT_LONG_CASTLE);
        }

        // HANDLE EOT
        director->board->setAllPiecesState(PS_IDLE);
        printf("CURRENT STATE = SS_REQUEST_AI\n");
        currentState = SS_REQUEST_AI;

        currentHoverCell = nullptr;
        currentSelectedCell = nullptr;
        currentPieceMove = nullptr;
    }
}

void SceneGame::ssAIAnimation()
{
    // Draw 2D
    //--------------------------------------------------------------------------------------
    director->renderVS->beginRT();
    for (Cell c : director->board->getBoardCells())
    {
        director->renderVS->drawCell(c);
    }


    //TODO INTERFACE

    director->renderVS->drawGameButton(BTN_BACK);
    director->renderVS->endRT();

    // Draw 3D
    //--------------------------------------------------------------------------------------
    director->composite->beginRT();
    director->composite->beginMode3D();
    director->render3D->drawArcade();
    for (Cell &c : director->board->getBoardCells())
    {
        if (c.pieceType != PT_NONE)
        {
            switch (c.pieceState)
            {
            case PS_MOVE:
                director->render3D->drawPieceReflectionEx(c, currentAIMove, deltaTime * 2, 1.0f);
                break;
            case PS_PROMOTE_ROOK:
                director->render3D->drawPieceReflectionEx(c, currentAIMove, deltaTime * 2, 1.0f - deltaTime * 2);
                c.pieceType = PT_ROOK;
                director->render3D->drawPieceReflectionEx(c, currentAIMove, deltaTime * 2, deltaTime * 2);
                c.pieceType = PT_PAWN;
                break;
            case PS_PROMOTE_KNIGHT:
                director->render3D->drawPieceReflectionEx(c, currentAIMove, deltaTime * 2, 1.0f - deltaTime * 2);
                c.pieceType = PT_KNIGHT;
                director->render3D->drawPieceReflectionEx(c, currentAIMove, deltaTime * 2, deltaTime * 2);
                c.pieceType = PT_PAWN;
                break;
            case PS_PROMOTE_BISHOP:
                director->render3D->drawPieceReflectionEx(c, currentAIMove, deltaTime * 2, 1.0f - deltaTime * 2);
                c.pieceType = PT_BISHOP;
                director->render3D->drawPieceReflectionEx(c, currentAIMove, deltaTime * 2, deltaTime * 2);
                c.pieceType = PT_PAWN;
                break;
            case PS_PROMOTE_QUEEN:
                director->render3D->drawPieceReflectionEx(c, currentAIMove, deltaTime * 2, 1.0f - deltaTime * 2);
                c.pieceType = PT_QUEEN;
                director->render3D->drawPieceReflectionEx(c, currentAIMove, deltaTime * 2, deltaTime * 2);
                c.pieceType = PT_PAWN;
                break;
            case PS_CASTLE_SHORT:
                director->render3D->drawPieceReflectionEx(c, PieceMove{BoardPosition{7, 7}, BoardPosition{5, 7}, MT_SHORT_CASTLE}, deltaTime * 2, 1.0f);
                break;
            case PS_CASTLE_LONG:
                director->render3D->drawPieceReflectionEx(c, PieceMove{BoardPosition{0, 7}, BoardPosition{3, 7}, MT_LONG_CASTLE}, deltaTime * 2, 1.0f);
                break;
            case PS_DIE:
                director->render3D->drawPieceReflection(c, 1.0f - deltaTime * 2);
                break;
            default:
                director->render3D->drawPieceReflection(c, 1.0f);
                break;
            }
        }
    }
    director->renderVS->drawArcadeVS();
    for (Cell c : director->board->getBoardCells())
    {
        if (c.pieceType != PT_NONE)
        {
            switch (c.pieceState)
            {
            case PS_MOVE:
                director->render3D->drawPieceEx(c, currentAIMove, deltaTime * 2, 1.0f);
                break;
            case PS_PROMOTE_ROOK:
                director->render3D->drawPieceEx(c, currentAIMove, deltaTime * 2, 1.0f - deltaTime * 2);
                c.pieceType = PT_ROOK;
                director->render3D->drawPieceEx(c, currentAIMove, deltaTime * 2, deltaTime * 2);
                c.pieceType = PT_PAWN;
                break;
            case PS_PROMOTE_KNIGHT:
                director->render3D->drawPieceEx(c, currentAIMove, deltaTime * 2, 1.0f - deltaTime * 2);
                c.pieceType = PT_KNIGHT;
                director->render3D->drawPieceEx(c, currentAIMove, deltaTime * 2, deltaTime * 2);
                c.pieceType = PT_PAWN;
                break;
            case PS_PROMOTE_BISHOP:
                director->render3D->drawPieceEx(c, currentAIMove, deltaTime * 2, 1.0f - deltaTime * 2);
                c.pieceType = PT_BISHOP;
                director->render3D->drawPieceEx(c, currentAIMove, deltaTime * 2, deltaTime * 2);
                c.pieceType = PT_PAWN;
                break;
            case PS_PROMOTE_QUEEN:
                director->render3D->drawPieceEx(c, currentAIMove, deltaTime * 2, 1.0f - deltaTime * 2);
                c.pieceType = PT_QUEEN;
                director->render3D->drawPieceEx(c, currentAIMove, deltaTime * 2, deltaTime * 2);
                c.pieceType = PT_PAWN;
                break;
            case PS_DIE:
                director->render3D->drawPiece(c, 1.0f - deltaTime * 2);
                break;
            case PS_CASTLE_SHORT:
                director->render3D->drawPieceEx(c, PieceMove{BoardPosition{7, 7}, BoardPosition{5, 7}, MT_SHORT_CASTLE}, deltaTime * 2, 1.0f);
                break;
            case PS_CASTLE_LONG:
                director->render3D->drawPieceEx(c, PieceMove{BoardPosition{0, 7}, BoardPosition{3, 7}, MT_LONG_CASTLE}, deltaTime * 2, 1.0f);
                break;
            default:
                director->render3D->drawPiece(c, 1.0f);
                break;
            }
        }
    }
    director->composite->endMode3D();
    director->composite->drawCursor();
    director->composite->endRT();

    // Draw Composite
    //--------------------------------------------------------------------------------------
    director->composite->beginDrawing();
    director->composite->drawComposite();
    director->composite->endDrawing();

    if (deltaTime >= 0.5f)
    {
        deltaTime = 0.0f;

        // HANDLE PROMOTION
        if (currentAIMove.type == MT_PROMOTE || currentAIMove.type == MT_ATTACKPROMOTE)
        {
            Cell *c = &director->board->getBoardCell(currentAIMove.origin);
            switch (c->pieceState)
            {
            case PS_PROMOTE_BISHOP:
                c->pieceType = PT_BISHOP;
                break;
            case PS_PROMOTE_ROOK:
                c->pieceType = PT_ROOK;
                break;
            case PS_PROMOTE_KNIGHT:
                c->pieceType = PT_KNIGHT;
                break;
            case PS_PROMOTE_QUEEN:
                c->pieceType = PT_QUEEN;
                break;
            default:
                break;
            }
        }

        // DELETE ATTACKED PIECE
        if (currentAIMove.type == MT_ATTACK ||
            currentAIMove.type == MT_ATTACKPROMOTE)
        {
            director->board->deletePiece(BoardPosition{currentAIMove.target.file, currentAIMove.target.rank});
        }

        // DELETE ENPASSANT PAWN
        if (currentAIMove.type == MT_ENPASSANT)
        {
            director->board->deletePiece(BoardPosition{currentAIMove.target.file, currentAIMove.target.rank + 1});
        }

        // MOVE THE PIECE
        director->board->movePiece(BoardPosition{currentAIMove.origin.file, currentAIMove.origin.rank},
                                   BoardPosition{currentAIMove.target.file, currentAIMove.target.rank},
                                   currentAIMove.type);

        // MOVE THE CASTLED PIECES
        if (currentAIMove.type == MT_SHORT_CASTLE)
        {
            director->board->movePiece(BoardPosition{7, 7}, BoardPosition{5, 7}, MT_SHORT_CASTLE);
        }
        if (currentAIMove.type == MT_LONG_CASTLE)
        {
            director->board->movePiece(BoardPosition{0, 7}, BoardPosition{3, 7}, MT_LONG_CASTLE);
        }

        // HANDLE EOT
        director->board->addFullTurn();
        director->board->setAllPiecesState(PS_IDLE);
        printf("CURRENT STATE = SS_REQUEST_MOVES\n");
        currentState = SS_REQUEST_MOVES;

        currentHoverCell = nullptr;
        currentSelectedCell = nullptr;
        currentPieceMove = nullptr;
        currentAIMove = {0};
    }
}

void SceneGame::drawDefaultRender()
{
    // Draw 2D
    //--------------------------------------------------------------------------------------
    director->renderVS->beginRT();
    for (UIButton opt : optionButtons)
    {
        director->renderVS->drawOption(opt, BS_DISABLED);
    }
    for (Cell c : director->board->getBoardCells())
    {
        director->renderVS->drawCell(c);
    }

    director->renderVS->drawGamePanel(GAME_PANEL);

    director->renderVS->drawMonoText(Vector2{520, 426}, director->board->getFullTurn().c_str(), 12, 1.0f);
    director->renderVS->drawMonoText(Vector2{508, 455}, director->board->getPlayerTime().c_str(), 12, 1.0f);
    director->renderVS->drawMonoText(Vector2{504, 496}, "00000", 12, 4.0f);
    director->renderVS->drawMonoText(Vector2{504, 512}, "00000", 12, 4.0f);
    director->renderVS->drawMonoText(Vector2{500, 540}, director->board->getPlayerScore().c_str(), 12, 1.0f);

    director->renderVS->drawGameButton(BTN_BACK);
    director->renderVS->endRT();

    // Draw 3D
    //--------------------------------------------------------------------------------------
    director->composite->beginRT();
    director->composite->beginMode3D();
    director->render3D->drawArcade();
    for (Cell c : director->board->getBoardCells())
    {
        if (c.pieceType != PT_NONE)
        {
            director->render3D->drawPieceReflection(c, 1.0f);
        }
    }
    director->renderVS->drawArcadeVS();
    for (Cell c : director->board->getBoardCells())
    {
        if (c.pieceType != PT_NONE)
        {
            director->render3D->drawPiece(c, 1.0f);
        }
    }
    director->composite->endMode3D();
    director->composite->drawCursor();
    director->composite->endRT();

    // Draw Composite
    //--------------------------------------------------------------------------------------
    director->composite->beginDrawing();
    director->composite->drawComposite();
    director->composite->endDrawing();
}