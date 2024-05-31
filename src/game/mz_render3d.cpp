#include "mz_render3d.h"

MZRender3D::MZRender3D() {}

MZRender3D::~MZRender3D()
{
    UnloadModel(mshArcadeMachine);
    UnloadModel(mshWhiteP);
    UnloadModel(mshWhiteR);
    UnloadModel(mshWhiteN);
    UnloadModel(mshWhiteB);
    UnloadModel(mshWhiteQ);
    UnloadModel(mshWhiteK);
    UnloadModel(mshBlackP);
    UnloadModel(mshBlackR);
    UnloadModel(mshBlackN);
    UnloadModel(mshBlackB);
    UnloadModel(mshBlackQ);
    UnloadModel(mshBlackK);
    UnloadModel(mshOutlnP);
    UnloadModel(mshOutlnR);
    UnloadModel(mshOutlnN);
    UnloadModel(mshOutlnB);
    UnloadModel(mshOutlnQ);
    UnloadModel(mshOutlnK);

    UnloadTexture(texArcadeMachine);
    UnloadTexture(texPiecesWhite);
    UnloadTexture(texPiecesBlack);
}

void MZRender3D::loadResources()
{
    mshArcadeMachine = LoadModel("res/msh_arcade_machine.obj");
    mshWhiteP = LoadModel("res/msh_piece_p.obj");
    mshWhiteR = LoadModel("res/msh_piece_r.obj");
    mshWhiteN = LoadModel("res/msh_piece_n.obj");
    mshWhiteB = LoadModel("res/msh_piece_b.obj");
    mshWhiteK = LoadModel("res/msh_piece_k.obj");
    mshWhiteQ = LoadModel("res/msh_piece_q.obj");
    mshBlackP = LoadModel("res/msh_piece_p.obj");
    mshBlackR = LoadModel("res/msh_piece_r.obj");
    mshBlackN = LoadModel("res/msh_piece_n.obj");
    mshBlackB = LoadModel("res/msh_piece_b.obj");
    mshBlackK = LoadModel("res/msh_piece_k.obj");
    mshBlackQ = LoadModel("res/msh_piece_q.obj");
    mshOutlnP = LoadModel("res/msh_outline_p.obj");
    mshOutlnN = LoadModel("res/msh_outline_n.obj");
    mshOutlnB = LoadModel("res/msh_outline_b.obj");
    mshOutlnK = LoadModel("res/msh_outline_k.obj");
    mshOutlnQ = LoadModel("res/msh_outline_q.obj");
    mshOutlnR = LoadModel("res/msh_outline_r.obj");

    texArcadeMachine = LoadTexture("res/tex_arcade_machine.png");
    texPiecesBlack = LoadTexture("res/tex_pieces_black.png");
    texPiecesWhite = LoadTexture("res/tex_pieces_white.png");

    GenTextureMipmaps(&texPiecesBlack);
    GenTextureMipmaps(&texPiecesWhite);
    GenTextureMipmaps(&texArcadeMachine);

    SetTextureFilter(texPiecesBlack, TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(texPiecesWhite, TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(texArcadeMachine, TEXTURE_FILTER_BILINEAR);

    SetMaterialTexture(&mshBlackP.materials[0], MATERIAL_MAP_DIFFUSE, texPiecesBlack);
    SetMaterialTexture(&mshBlackR.materials[0], MATERIAL_MAP_DIFFUSE, texPiecesBlack);
    SetMaterialTexture(&mshBlackN.materials[0], MATERIAL_MAP_DIFFUSE, texPiecesBlack);
    SetMaterialTexture(&mshBlackB.materials[0], MATERIAL_MAP_DIFFUSE, texPiecesBlack);
    SetMaterialTexture(&mshBlackQ.materials[0], MATERIAL_MAP_DIFFUSE, texPiecesBlack);
    SetMaterialTexture(&mshBlackK.materials[0], MATERIAL_MAP_DIFFUSE, texPiecesBlack);
    SetMaterialTexture(&mshWhiteP.materials[0], MATERIAL_MAP_DIFFUSE, texPiecesWhite);
    SetMaterialTexture(&mshWhiteR.materials[0], MATERIAL_MAP_DIFFUSE, texPiecesWhite);
    SetMaterialTexture(&mshWhiteN.materials[0], MATERIAL_MAP_DIFFUSE, texPiecesWhite);
    SetMaterialTexture(&mshWhiteB.materials[0], MATERIAL_MAP_DIFFUSE, texPiecesWhite);
    SetMaterialTexture(&mshWhiteQ.materials[0], MATERIAL_MAP_DIFFUSE, texPiecesWhite);
    SetMaterialTexture(&mshWhiteK.materials[0], MATERIAL_MAP_DIFFUSE, texPiecesWhite);
    SetMaterialTexture(&mshArcadeMachine.materials[0], MATERIAL_MAP_DIFFUSE, texArcadeMachine);
}

void MZRender3D::drawArcade() { DrawModel(mshArcadeMachine, WORLD_ORIGIN, 1.0f, WHITE); }

void MZRender3D::drawPieceReflection(const Cell &cell, float opacity)
{
    Model *model = getModelByPieceType(cell.pieceType, cell.pieceColor);
    if (model == nullptr)
        return;
    DrawModelEx(*model, {cell.drawModel.x, 0.0f, cell.drawModel.y}, {0}, 0, {1.0f, -1.0f, 1.0f},
                ColorAlpha(DARKGRAY, opacity));
}

void MZRender3D::drawPiece(const Cell &cell, float opacity)
{
    Model *model = getModelByPieceType(cell.pieceType, cell.pieceColor);
    Model *outline = getOutlineByPieceType(cell.pieceType);
    if (model == nullptr || outline == nullptr)
        return;
    DrawModelEx(*model, {cell.drawModel.x, 0.0f, cell.drawModel.y}, {0}, 0, {1.0f, 1.0f, 1.0f}, ColorAlpha(WHITE, opacity));
    switch (cell.cellState)
    {
    case CS_SELECTED:
        DrawModelEx(*outline, {cell.drawModel.x, 0.0f, cell.drawModel.y}, {0}, 0, {1.0f, 1.0f, 1.0f}, ColorAlpha(WHITE, opacity));
        break;
    case CS_HOVER_SELECTABLE:
        DrawModelEx(*outline, {cell.drawModel.x, 0.0f, cell.drawModel.y}, {0}, 0, {1.0f, 1.0f, 1.0f}, ColorAlpha(SKYBLUE, opacity));
        break;
    case CS_HOVER_ATTACKED:
        DrawModelEx(*outline, {cell.drawModel.x, 0.0f, cell.drawModel.y}, {0}, 0, {1.0f, 1.0f, 1.0f}, ColorAlpha(RED, opacity));
        break;
    default:
        DrawModelEx(*outline, {cell.drawModel.x, 0.0f, cell.drawModel.y}, {0}, 0, {1.0f, 1.0f, 1.0f}, ColorAlpha(BLACK, opacity));
        break;
    }
}

void MZRender3D::drawPieceReflectionEx(const Cell &cell, PieceMove move, float dt, float opacity)
{
    Model *model = getModelByPieceType(cell.pieceType, cell.pieceColor);

    if (model == nullptr)
        return;

    Vector3 exTransform =
        Math::vector3Lerp(Vector3{(float)cell.position.file, 0.0f, (float)cell.position.rank},
                          Vector3{(float)move.target.file, 0.0f, (float)move.target.rank}, Math::easeInOutExpo(dt));
    DrawModelEx(*model, {exTransform.x + 4.5f, 0.0f, -1.0f - exTransform.z}, {0}, 0, {1.0f, -1.0f, 1.0f}, ColorAlpha(DARKGRAY, opacity));
}

void MZRender3D::drawPieceEx(const Cell &cell, PieceMove move, float dt, float opacity)
{
    Model *model = getModelByPieceType(cell.pieceType, cell.pieceColor);
    Model *outline = getOutlineByPieceType(cell.pieceType);
    if (model == nullptr || outline == nullptr)
        return;

    Vector3 exTransform =
        Math::vector3Lerp(Vector3{(float)cell.position.file, 0.0f, (float)cell.position.rank},
                          Vector3{(float)move.target.file, 0.0f, (float)move.target.rank}, Math::easeInOutExpo(dt));

    DrawModelEx(*model, {exTransform.x + 4.5f, 0.0f, -1.0f - exTransform.z}, {0}, 0, {1.0f, 1.0f, 1.0f}, ColorAlpha(WHITE, opacity));
    DrawModelEx(*outline, {exTransform.x + 4.5f, 0.0f, -1.0f - exTransform.z}, {0}, 0, {1.0f, 1.0f, 1.0f}, ColorAlpha(WHITE, opacity));
}

Model *MZRender3D::getModelByPieceType(PieceType type, ChessColor color)
{
    switch (type)
    {
    case PieceType::PT_PAWN:
        return (color == CC_BLACK) ? &mshBlackP : &mshWhiteP;
    case PieceType::PT_ROOK:
        return (color == CC_BLACK) ? &mshBlackR : &mshWhiteR;
    case PieceType::PT_KNIGHT:
        return (color == CC_BLACK) ? &mshBlackN : &mshWhiteN;
    case PieceType::PT_BISHOP:
        return (color == CC_BLACK) ? &mshBlackB : &mshWhiteB;
    case PieceType::PT_QUEEN:
        return (color == CC_BLACK) ? &mshBlackQ : &mshWhiteQ;
    case PieceType::PT_KING:
        return (color == CC_BLACK) ? &mshBlackK : &mshWhiteK;
    default:
        return nullptr;
    }
}

Model *MZRender3D::getOutlineByPieceType(PieceType type)
{
    switch (type)
    {
    case PieceType::PT_PAWN:
        return &mshOutlnP;
    case PieceType::PT_ROOK:
        return &mshOutlnR;
    case PieceType::PT_KNIGHT:
        return &mshOutlnN;
    case PieceType::PT_BISHOP:
        return &mshOutlnB;
    case PieceType::PT_QUEEN:
        return &mshOutlnQ;
    case PieceType::PT_KING:
        return &mshOutlnK;
    default:
        return nullptr;
    }
}