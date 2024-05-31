#include "mz_board.h"

MZBoard::MZBoard()
{
    lastMove = PieceMove{BoardPosition{-1, -1}, BoardPosition{-1, -1}, MT_MOVE};
    playerTime = 0.0f;
    playerScore = "0";
    fullTurn = 1;
    playerCastling[0] = true;
    playerCastling[1] = true;
}

MZBoard::~MZBoard() {}

//------------------------------------------------------------------------------------
// CELLS
//------------------------------------------------------------------------------------

void MZBoard::setCellState(BoardPosition position, CellState state)
{
    int id = position.file + position.rank * 8;
    boardCells[id].cellState = state;
}

void MZBoard::setAllCellsState(CellState state)
{
    for (Cell &c : boardCells)
    {
        c.cellState = state;
    }
}

//------------------------------------------------------------------------------------
// PIECES
//------------------------------------------------------------------------------------

void MZBoard::addPiece(BoardPosition position, PieceType type, ChessColor color)
{
    int id = position.file + position.rank * 8;

    boardCells[id].pieceType = type;
    boardCells[id].pieceState = PS_IDLE;
    boardCells[id].pieceColor = color;
    boardCells[id].pieceMoves.clear();
}

void MZBoard::setPieceState(BoardPosition position, PieceState state)
{
    int id = position.file + position.rank * 8;
    boardCells[id].pieceState = state;
}

void MZBoard::setAllPiecesState(PieceState state)
{
    for (Cell &c : boardCells)
    {
        if (c.pieceType != PT_NONE)
        {
            c.pieceState = state;
        }
        else
        {
            c.pieceState = PS_NONE;
        }
    }
}

void MZBoard::deletePiece(BoardPosition position)
{
    int id = position.file + position.rank * 8;

    boardCells[id].pieceType = PT_NONE;
    boardCells[id].pieceColor = CC_NONE;
    boardCells[id].pieceState = PS_NONE;
}

void MZBoard::movePiece(BoardPosition origin, BoardPosition target, MoveType mtype)
{
    // GET VARIABELS
    int originId = origin.file + origin.rank * 8;
    int targetId = target.file + target.rank * 8;

    // CHECK PLAYER CASTLING RIGHTS (THE AI ALREADY KNOWS THE RULES)
    if (boardCells[originId].pieceColor == CC_WHITE && boardCells[originId].pieceType == PT_KING)
    {
        playerCastling[0] = false;
        playerCastling[1] = false;
    }
    if (boardCells[originId].pieceColor == CC_WHITE && boardCells[originId].pieceType == PT_ROOK && boardCells[originId].position.file == 0)
    {
        playerCastling[0] = false;
    }
    if (boardCells[originId].pieceColor == CC_WHITE && boardCells[originId].pieceType == PT_ROOK && boardCells[originId].position.file == 7)
    {
        playerCastling[1] = false;
    }

    // PERFORM THE MOVE
    boardCells[targetId].pieceType = boardCells[originId].pieceType;
    boardCells[targetId].pieceColor = boardCells[originId].pieceColor;
    boardCells[targetId].pieceState = PS_IDLE;

    boardCells[originId].pieceType = PT_NONE;
    boardCells[originId].pieceColor = CC_NONE;
    boardCells[originId].pieceState = PS_NONE;

    lastMove = PieceMove{BoardPosition{origin.file, origin.rank},
                         BoardPosition{target.file, target.rank}, mtype};
}

//------------------------------------------------------------------------------------
// PIECE MOVEMENTS
//------------------------------------------------------------------------------------

void MZBoard::addMovementToPiece(BoardPosition origin, BoardPosition target, MoveType moveType)
{
    switch (moveType)
    {
    case MT_LONG_CASTLE:
        if (!isWhiteInCheck())
        {
            if (checkMoveValidity(PieceMove{BoardPosition{4, 0}, BoardPosition{3, 0}, moveType}) &&
                checkMoveValidity(PieceMove{BoardPosition{4, 0}, BoardPosition{2, 0}, moveType}))
            {
                int id = origin.file + origin.rank * 8;
                boardCells[id].pieceMoves.push_back(PieceMove{origin, target, moveType});
            }
        }
        break;
    case MT_SHORT_CASTLE:
        if (!isWhiteInCheck())
        {
            if (checkMoveValidity(PieceMove{BoardPosition{4, 0}, BoardPosition{5, 0}, moveType}) &&
                checkMoveValidity(PieceMove{BoardPosition{4, 0}, BoardPosition{6, 0}, moveType}))
            {
                int id = origin.file + origin.rank * 8;
                boardCells[id].pieceMoves.push_back(PieceMove{origin, target, moveType});
            }
        }
        break;
    default:
        if (checkMoveValidity(PieceMove{origin, target, moveType}))
        {
            int id = origin.file + origin.rank * 8;
            boardCells[id].pieceMoves.push_back(PieceMove{origin, target, moveType});
        }
        break;
    }
}

std::vector<PieceMove> &MZBoard::getValidMovements(BoardPosition position)
{
    int id = position.file + position.rank * 8;
    return boardCells[id].pieceMoves;
}

void MZBoard::clearMovements()
{
    for (Cell &c : boardCells)
    {
        c.pieceMoves.clear();
    }
}

//------------------------------------------------------------------------------------
// ASYNC
//------------------------------------------------------------------------------------

void MZBoard::startProcessBoard()
{
    processBoardFuture = std::async(std::launch::async, &MZBoard::processBoard, this);
}

bool MZBoard::isProcessBoardReady()
{
    return processBoardFuture.valid() && processBoardFuture.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
}

//------------------------------------------------------------------------------------
// ENDGAME
//------------------------------------------------------------------------------------

bool MZBoard::checkLoss()
{
    for (Cell &c : boardCells)
    {
        if (!c.pieceMoves.empty())
        {
            return false;
        }
    }
    return true;
}

bool MZBoard::checkDrawNoMoves()
{
    return false;
}

//------------------------------------------------------------------------------------
// UTILS
//------------------------------------------------------------------------------------

void MZBoard::initializeBoard()
{
    for (Cell &c : boardCells)
    {
        c.pieceType = PT_NONE;
        c.pieceColor = CC_NONE;
        c.pieceMoves.clear();
    }

    addPiece(BoardPosition{0, 0}, PT_ROOK, CC_WHITE);
    addPiece(BoardPosition{1, 0}, PT_KNIGHT, CC_WHITE);
    addPiece(BoardPosition{2, 0}, PT_BISHOP, CC_WHITE);
    addPiece(BoardPosition{3, 0}, PT_QUEEN, CC_WHITE);
    addPiece(BoardPosition{4, 0}, PT_KING, CC_WHITE);
    addPiece(BoardPosition{5, 0}, PT_BISHOP, CC_WHITE);
    addPiece(BoardPosition{6, 0}, PT_KNIGHT, CC_WHITE);
    addPiece(BoardPosition{7, 0}, PT_ROOK, CC_WHITE);
    addPiece(BoardPosition{0, 7}, PT_ROOK, CC_BLACK);
    addPiece(BoardPosition{2, 7}, PT_BISHOP, CC_BLACK);
    addPiece(BoardPosition{1, 7}, PT_KNIGHT, CC_BLACK);
    addPiece(BoardPosition{3, 7}, PT_QUEEN, CC_BLACK);
    addPiece(BoardPosition{4, 7}, PT_KING, CC_BLACK);
    addPiece(BoardPosition{5, 7}, PT_BISHOP, CC_BLACK);
    addPiece(BoardPosition{6, 7}, PT_KNIGHT, CC_BLACK);
    addPiece(BoardPosition{7, 7}, PT_ROOK, CC_BLACK);

    for (int i = 0; i < 8; i++)
    {
        addPiece(BoardPosition{i, 1}, PT_PAWN, CC_WHITE);
        addPiece(BoardPosition{i, 6}, PT_PAWN, CC_BLACK);
    }
}

bool MZBoard::isBoardPositionValid(BoardPosition position)
{
    return position.rank >= 0 && position.rank < 8 && position.file >= 0 && position.file < 8;
}

Cell &MZBoard::getBoardCell(BoardPosition position)
{
    int id = position.file + position.rank * 8;
    return boardCells[id];
}

Cell &MZBoard::getAuxiliaryBoardCell(BoardPosition position, std::array<Cell, 64> *auxiliaryBoard)
{
    int id = position.file + position.rank * 8;
    return (*auxiliaryBoard)[id];
}

std::array<Cell, 64> &MZBoard::getBoardCells()
{
    return boardCells;
}

void MZBoard::addPlayerTime(float dt)
{
    playerTime += dt;
}

void MZBoard::setPlayerScore(std::string score)
{
    playerScore = score;
}

void MZBoard::addFullTurn()
{
    fullTurn++;
}

std::string MZBoard::getPlayerTime()
{
    int minutes = static_cast<int>(playerTime) / 60;
    int seconds = static_cast<int>(playerTime) % 60;

    std::ostringstream oss;
    oss << std::setw(2) << std::setfill('0') << minutes << ":" << std::setw(2) << std::setfill('0') << seconds;

    return oss.str();
}

std::string MZBoard::getPlayerScore()
{
    std::ostringstream oss;
    oss << std::setw(4) << std::setfill('0') << playerScore;

    return oss.str();
}

std::string MZBoard::getFullTurn()
{
    std::ostringstream oss;
    oss << std::setw(3) << std::setfill('0') << fullTurn;

    return oss.str();
}

void MZBoard::processBoard()
{
    Cell *aidCell;
    BoardPosition aidBoardPosition;

    printf("PROCESS BOARD - START\n");

    for (Cell c : boardCells)
    {
        if (c.pieceColor == CC_WHITE)
        {
            switch (c.pieceType)
            {
                //------------------------------------------------------------------------------------
                // PAWN LOGIC
                //------------------------------------------------------------------------------------
            case PT_PAWN:
                // CHECK MOVE
                aidBoardPosition = BoardPosition{c.position.file, c.position.rank + 1};
                if (isBoardPositionValid(aidBoardPosition))
                {
                    aidCell = &getBoardCell(aidBoardPosition);
                    if (aidCell->pieceType == PT_NONE)
                    {
                        if (aidCell->position.rank == 7)
                        {
                            addMovementToPiece(c.position, aidCell->position, MT_PROMOTE);
                        }
                        else
                        {
                            addMovementToPiece(c.position, aidCell->position, MT_MOVE);
                        }
                    }
                }
                // CHECK DOUBLE MOVE
                if (c.position.rank == 1)
                {
                    aidBoardPosition = BoardPosition{c.position.file, c.position.rank + 2};
                    aidCell = &getBoardCell(aidBoardPosition);
                    if (c.position.rank == 1 && aidCell->pieceType == PT_NONE)
                    {
                        addMovementToPiece(c.position, aidCell->position, MT_DOUBLEMOVE);
                    }
                }
                // CHECK ATTACK/PROMOTE LEFT
                aidBoardPosition = BoardPosition{c.position.file - 1, c.position.rank + 1};
                if (isBoardPositionValid(aidBoardPosition))
                {
                    aidCell = &getBoardCell(aidBoardPosition);
                    // CHECK EN PASSANT
                    if (lastMove.type == MT_DOUBLEMOVE && lastMove.target.file == aidCell->position.file && lastMove.target.rank == aidCell->position.rank - 1)
                    {
                        addMovementToPiece(c.position, aidCell->position, MT_ENPASSANT);
                    }

                    if (aidCell->pieceColor == CC_BLACK)
                    {
                        if (aidCell->position.rank == 7)
                        {
                            addMovementToPiece(c.position, aidCell->position, MT_ATTACKPROMOTE);
                        }
                        else
                        {
                            addMovementToPiece(c.position, aidCell->position, MT_ATTACK);
                        }
                    }
                }
                // CHECK ATTACK/PROMOTE RIGHT
                aidBoardPosition = BoardPosition{c.position.file + 1, c.position.rank + 1};
                if (isBoardPositionValid(aidBoardPosition))
                {
                    aidCell = &getBoardCell(aidBoardPosition);
                    // CHECK EN PASSANT
                    if (lastMove.type == MT_DOUBLEMOVE && lastMove.target.file == aidCell->position.file && lastMove.target.rank == aidCell->position.rank - 1)
                    {
                        addMovementToPiece(c.position, aidCell->position, MT_ENPASSANT);
                    }

                    if (aidCell->pieceColor == CC_BLACK)
                    {
                        if (aidCell->position.rank == 7)
                        {
                            addMovementToPiece(c.position, aidCell->position, MT_ATTACKPROMOTE);
                        }
                        else
                        {
                            addMovementToPiece(c.position, aidCell->position, MT_ATTACK);
                        }
                    }
                }

                break;
                //------------------------------------------------------------------------------------
                // ROOK LOGIC // ERROR TRYING TO TAKE THE QUEEN
                //------------------------------------------------------------------------------------
            case PT_ROOK:
                for (int i = 0; i < 4; ++i)
                {
                    int df = rookDeltas[i][0];
                    int dr = rookDeltas[i][1];

                    aidBoardPosition = c.position;

                    while (true)
                    {
                        aidBoardPosition.file += df;
                        aidBoardPosition.rank += dr;

                        if (!isBoardPositionValid(aidBoardPosition))
                        {
                            break;
                        }

                        aidCell = &getBoardCell(aidBoardPosition);

                        if (aidCell->pieceColor != CC_NONE)
                        {
                            if (aidCell->pieceColor == CC_BLACK)
                            {
                                addMovementToPiece(c.position, aidBoardPosition, MT_ATTACK);
                            }
                            break;
                        }
                        addMovementToPiece(c.position, aidBoardPosition, MT_MOVE);
                    }
                }
                break;
                //------------------------------------------------------------------------------------
                // KINGHT LOGIC
                //------------------------------------------------------------------------------------
            case PT_KNIGHT:
                for (int i = 0; i < 8; ++i)
                {
                    int df = knightDeltas[i][0];
                    int dr = knightDeltas[i][1];

                    aidBoardPosition = {c.position.file + df, c.position.rank + dr};

                    if (isBoardPositionValid(aidBoardPosition))
                    {
                        aidCell = &getBoardCell(aidBoardPosition);

                        if (aidCell->pieceColor == CC_NONE)
                        {
                            addMovementToPiece(c.position, aidBoardPosition, MT_MOVE);
                        }
                        else if (aidCell->pieceColor == CC_BLACK)
                        {
                            addMovementToPiece(c.position, aidBoardPosition, MT_ATTACK);
                        }
                    }
                }
                break;
                //------------------------------------------------------------------------------------
                // BISHOP LOGIC
                //------------------------------------------------------------------------------------
            case PT_BISHOP:
                for (int i = 0; i < 4; ++i)
                {
                    int df = bishopDeltas[i][0];
                    int dr = bishopDeltas[i][1];

                    aidBoardPosition = c.position;

                    while (true)
                    {
                        aidBoardPosition.file += df;
                        aidBoardPosition.rank += dr;

                        if (!isBoardPositionValid(aidBoardPosition))
                        {
                            break;
                        }

                        aidCell = &getBoardCell(aidBoardPosition);

                        if (aidCell->pieceColor != CC_NONE)
                        {
                            if (aidCell->pieceColor == CC_BLACK)
                            {
                                addMovementToPiece(c.position, aidBoardPosition, MT_ATTACK);
                            }
                            break;
                        }
                        addMovementToPiece(c.position, aidBoardPosition, MT_MOVE);
                    }
                }
                break;
                //------------------------------------------------------------------------------------
                // QUEEN LOGIC
                //------------------------------------------------------------------------------------
            case PT_QUEEN:
                for (int i = 0; i < 8; ++i)
                {
                    int df = queenDeltas[i][0];
                    int dr = queenDeltas[i][1];

                    aidBoardPosition = c.position;

                    while (true)
                    {
                        aidBoardPosition.file += df;
                        aidBoardPosition.rank += dr;

                        if (!isBoardPositionValid(aidBoardPosition))
                        {
                            break;
                        }
                        aidCell = &getBoardCell(aidBoardPosition);

                        if (aidCell->pieceColor != CC_NONE)
                        {
                            if (aidCell->pieceColor == CC_BLACK)
                            {
                                addMovementToPiece(c.position, aidBoardPosition, MT_ATTACK);
                            }
                            break;
                        }
                        addMovementToPiece(c.position, aidBoardPosition, MT_MOVE);
                    }
                }
                break;
                //------------------------------------------------------------------------------------
                // KING LOGIC
                //------------------------------------------------------------------------------------
            case PT_KING:
                for (int dr = -1; dr <= 1; ++dr)
                {
                    for (int df = -1; df <= 1; ++df)
                    {
                        if (dr == 0 && df == 0)
                            continue;

                        aidBoardPosition = {c.position.file + df, c.position.rank + dr};

                        if (isBoardPositionValid(aidBoardPosition))
                        {
                            aidCell = &getBoardCell(aidBoardPosition);

                            if (aidCell->pieceColor == CC_NONE)
                            {
                                addMovementToPiece(c.position, aidBoardPosition, MT_MOVE);
                            }
                            else if (aidCell->pieceColor == CC_BLACK)
                            {
                                addMovementToPiece(c.position, aidBoardPosition, MT_ATTACK);
                            }
                        }
                    }
                }
                if (playerCastling[0])
                {
                    if (getBoardCell(BoardPosition{1, 0}).pieceColor == CC_NONE && getBoardCell(BoardPosition{2, 0}).pieceColor == CC_NONE && getBoardCell(BoardPosition{3, 0}).pieceColor == CC_NONE)
                    {
                        addMovementToPiece(c.position, BoardPosition{2, 0}, MT_LONG_CASTLE);
                    }
                }
                if (playerCastling[1])
                {
                    if (getBoardCell(BoardPosition{5, 0}).pieceColor == CC_NONE && getBoardCell(BoardPosition{6, 0}).pieceColor == CC_NONE)
                    {
                        addMovementToPiece(c.position, BoardPosition{6, 0}, MT_SHORT_CASTLE);
                    }
                }
                break;
            default:
                break;
            }
        }
    }
    printf("PROCESS BOARD - END\n");
}

bool MZBoard::isWhiteInCheck()
{
    Cell *aidCell;
    BoardPosition aidBoardPosition;

    for (Cell c : boardCells)
    {
        if (c.pieceColor == CC_BLACK)
        {
            switch (c.pieceType)
            {
                //------------------------------------------------------------------------------------
                // PAWN LOGIC
                //------------------------------------------------------------------------------------
            case PT_PAWN:
                aidBoardPosition = BoardPosition{c.position.file - 1, c.position.rank - 1};
                if (isBoardPositionValid(aidBoardPosition))
                {
                    aidCell = &getBoardCell(aidBoardPosition);
                    if (aidCell->pieceColor == CC_WHITE && aidCell->pieceType == PT_KING)
                    {
                        return true;
                    }
                }
                aidBoardPosition = BoardPosition{c.position.file + 1, c.position.rank - 1};
                if (isBoardPositionValid(aidBoardPosition))
                {
                    aidCell = &getBoardCell(aidBoardPosition);
                    if (aidCell->pieceColor == CC_WHITE && aidCell->pieceType == PT_KING)
                    {
                        return true;
                    }
                }
                break;
                //------------------------------------------------------------------------------------
                // ROOK LOGIC
                //------------------------------------------------------------------------------------
            case PT_ROOK:
                for (int i = 0; i < 4; ++i)
                {
                    int df = rookDeltas[i][0];
                    int dr = rookDeltas[i][1];

                    aidBoardPosition = c.position;

                    while (true)
                    {
                        aidBoardPosition.file += df;
                        aidBoardPosition.rank += dr;

                        if (!isBoardPositionValid(aidBoardPosition))
                        {
                            break;
                        }

                        aidCell = &getBoardCell(aidBoardPosition);

                        if (aidCell->pieceColor != CC_NONE)
                        {
                            if (aidCell->pieceColor == CC_WHITE && aidCell->pieceType == PT_KING)
                            {
                                return true;
                            }
                            break;
                        }
                    }
                }
                break;
                //------------------------------------------------------------------------------------
                // KINGHT LOGIC
                //------------------------------------------------------------------------------------
            case PT_KNIGHT:
                for (int i = 0; i < 8; ++i)
                {
                    int df = knightDeltas[i][0];
                    int dr = knightDeltas[i][1];

                    aidBoardPosition = {c.position.file + df, c.position.rank + dr};

                    if (isBoardPositionValid(aidBoardPosition))
                    {
                        aidCell = &getBoardCell(aidBoardPosition);

                        if (aidCell->pieceColor == CC_WHITE && aidCell->pieceType == PT_KING)
                        {
                            return true;
                        }
                    }
                }
                break;
                //------------------------------------------------------------------------------------
                // BISHOP LOGIC
                //------------------------------------------------------------------------------------
            case PT_BISHOP:
                for (int i = 0; i < 4; ++i)
                {
                    int df = bishopDeltas[i][0];
                    int dr = bishopDeltas[i][1];

                    aidBoardPosition = c.position;

                    while (true)
                    {
                        aidBoardPosition.file += df;
                        aidBoardPosition.rank += dr;

                        if (!isBoardPositionValid(aidBoardPosition))
                        {
                            break;
                        }

                        aidCell = &getBoardCell(aidBoardPosition);

                        if (aidCell->pieceColor != CC_NONE)
                        {
                            if (aidCell->pieceColor == CC_WHITE && aidCell->pieceType == PT_KING)
                            {
                                return true;
                            }
                            break;
                        }
                    }
                }
                break;
                //------------------------------------------------------------------------------------
                // QUEEN LOGIC
                //------------------------------------------------------------------------------------
            case PT_QUEEN:
                for (int i = 0; i < 8; ++i)
                {
                    int df = queenDeltas[i][0];
                    int dr = queenDeltas[i][1];

                    aidBoardPosition = c.position;

                    while (true)
                    {
                        aidBoardPosition.file += df;
                        aidBoardPosition.rank += dr;

                        if (!isBoardPositionValid(aidBoardPosition))
                        {
                            break;
                        }
                        aidCell = &getBoardCell(aidBoardPosition);

                        if (aidCell->pieceColor != CC_NONE)
                        {
                            if (aidCell->pieceColor == CC_WHITE && aidCell->pieceType == PT_KING)
                            {
                                return true;
                            }
                            break;
                        }
                    }
                }
                break;
                //------------------------------------------------------------------------------------
                // KING LOGIC
                //------------------------------------------------------------------------------------
            case PT_KING:
                for (int dr = -1; dr <= 1; ++dr)
                {
                    for (int df = -1; df <= 1; ++df)
                    {
                        if (dr == 0 && df == 0)
                            continue;

                        aidBoardPosition = {c.position.file + df, c.position.rank + dr};

                        if (isBoardPositionValid(aidBoardPosition))
                        {
                            aidCell = &getBoardCell(aidBoardPosition);

                            if (aidCell->pieceColor == CC_WHITE && aidCell->pieceType == PT_KING)
                            {
                                return true;
                            }
                        }
                    }
                }
                break;
            default:
                break;
            }
        }
    }

    return false;
}

bool MZBoard::checkMoveValidity(PieceMove pmove)
{
    std::array<Cell, 64> auxiliarBoard = boardCells;

    int originId = pmove.origin.file + pmove.origin.rank * 8;
    int targetId = pmove.target.file + pmove.target.rank * 8;

    auxiliarBoard[targetId].pieceType = auxiliarBoard[originId].pieceType;
    auxiliarBoard[targetId].pieceColor = auxiliarBoard[originId].pieceColor;
    auxiliarBoard[originId].pieceType = PT_NONE;
    auxiliarBoard[originId].pieceColor = CC_NONE;

    Cell *aidCell;
    BoardPosition aidBoardPosition;

    for (Cell c : auxiliarBoard)
    {
        if (c.pieceColor == CC_BLACK)
        {
            switch (c.pieceType)
            {
                //------------------------------------------------------------------------------------
                // PAWN LOGIC
                //------------------------------------------------------------------------------------
            case PT_PAWN:
                aidBoardPosition = BoardPosition{c.position.file - 1, c.position.rank - 1};
                if (isBoardPositionValid(aidBoardPosition))
                {
                    aidCell = &getAuxiliaryBoardCell(aidBoardPosition, &auxiliarBoard);
                    if (aidCell->pieceColor == CC_WHITE && aidCell->pieceType == PT_KING)
                    {
                        return false;
                    }
                }
                aidBoardPosition = BoardPosition{c.position.file + 1, c.position.rank - 1};
                if (isBoardPositionValid(aidBoardPosition))
                {
                    aidCell = &getAuxiliaryBoardCell(aidBoardPosition, &auxiliarBoard);
                    if (aidCell->pieceColor == CC_WHITE && aidCell->pieceType == PT_KING)
                    {
                        return false;
                    }
                }
                break;
                //------------------------------------------------------------------------------------
                // ROOK LOGIC
                //------------------------------------------------------------------------------------
            case PT_ROOK:
                for (int i = 0; i < 4; ++i)
                {
                    int df = rookDeltas[i][0];
                    int dr = rookDeltas[i][1];

                    aidBoardPosition = c.position;

                    while (true)
                    {
                        aidBoardPosition.file += df;
                        aidBoardPosition.rank += dr;

                        if (!isBoardPositionValid(aidBoardPosition))
                        {
                            break;
                        }

                        aidCell = &getAuxiliaryBoardCell(aidBoardPosition, &auxiliarBoard);

                        if (aidCell->pieceColor != CC_NONE)
                        {
                            if (aidCell->pieceColor == CC_WHITE && aidCell->pieceType == PT_KING)
                            {
                                return false;
                            }
                            break;
                        }
                    }
                }
                break;
                //------------------------------------------------------------------------------------
                // KINGHT LOGIC
                //------------------------------------------------------------------------------------
            case PT_KNIGHT:
                for (int i = 0; i < 8; ++i)
                {
                    int df = knightDeltas[i][0];
                    int dr = knightDeltas[i][1];

                    aidBoardPosition = {c.position.file + df, c.position.rank + dr};

                    if (isBoardPositionValid(aidBoardPosition))
                    {
                        aidCell = &getAuxiliaryBoardCell(aidBoardPosition, &auxiliarBoard);

                        if (aidCell->pieceColor == CC_WHITE && aidCell->pieceType == PT_KING)
                        {
                            return false;
                        }
                    }
                }
                break;
                //------------------------------------------------------------------------------------
                // BISHOP LOGIC
                //------------------------------------------------------------------------------------
            case PT_BISHOP:
                for (int i = 0; i < 4; ++i)
                {
                    int df = bishopDeltas[i][0];
                    int dr = bishopDeltas[i][1];

                    aidBoardPosition = c.position;

                    while (true)
                    {
                        aidBoardPosition.file += df;
                        aidBoardPosition.rank += dr;

                        if (!isBoardPositionValid(aidBoardPosition))
                        {
                            break;
                        }

                        aidCell = &getAuxiliaryBoardCell(aidBoardPosition, &auxiliarBoard);

                        if (aidCell->pieceColor != CC_NONE)
                        {
                            if (aidCell->pieceColor == CC_WHITE && aidCell->pieceType == PT_KING)
                            {
                                return false;
                            }
                            break;
                        }
                    }
                }
                break;
                //------------------------------------------------------------------------------------
                // QUEEN LOGIC
                //------------------------------------------------------------------------------------
            case PT_QUEEN:
                for (int i = 0; i < 8; ++i)
                {
                    int df = queenDeltas[i][0];
                    int dr = queenDeltas[i][1];

                    aidBoardPosition = c.position;

                    while (true)
                    {
                        aidBoardPosition.file += df;
                        aidBoardPosition.rank += dr;

                        if (!isBoardPositionValid(aidBoardPosition))
                        {
                            break;
                        }
                        aidCell = &getAuxiliaryBoardCell(aidBoardPosition, &auxiliarBoard);

                        if (aidCell->pieceColor != CC_NONE)
                        {
                            if (aidCell->pieceColor == CC_WHITE && aidCell->pieceType == PT_KING)
                            {
                                return false;
                            }
                            break;
                        }
                    }
                }
                break;
                //------------------------------------------------------------------------------------
                // KING LOGIC
                //------------------------------------------------------------------------------------
            case PT_KING:
                for (int dr = -1; dr <= 1; ++dr)
                {
                    for (int df = -1; df <= 1; ++df)
                    {
                        if (dr == 0 && df == 0)
                            continue;

                        aidBoardPosition = {c.position.file + df, c.position.rank + dr};

                        if (isBoardPositionValid(aidBoardPosition))
                        {
                            aidCell = &getAuxiliaryBoardCell(aidBoardPosition, &auxiliarBoard);

                            if (aidCell->pieceColor == CC_WHITE && aidCell->pieceType == PT_KING)
                            {
                                return false;
                            }
                        }
                    }
                }
                break;
            default:
                break;
            }
        }
    }

    return true;
}
