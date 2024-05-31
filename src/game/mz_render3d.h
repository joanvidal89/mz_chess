#ifndef MZ_RENDER3D
#define MZ_RENDER3D

#include "mz_math.h"
#include "mz_struct.h"
#include "raylib.h"

class MZRender3D
{
public:
    MZRender3D();
    ~MZRender3D();

    void loadResources();

    void drawArcade();
    void drawPieceReflection(const Cell &cell, float opacity);
    void drawPiece(const Cell &cell, float opacity);

    void drawPieceReflectionEx(const Cell &cell, PieceMove move, float dt, float opacity);
    void drawPieceEx(const Cell &cell, PieceMove move, float dt, float opacity);

private:
    Model mshArcadeMachine;
    Model mshWhiteP;
    Model mshWhiteR;
    Model mshWhiteN;
    Model mshWhiteB;
    Model mshWhiteK;
    Model mshWhiteQ;
    Model mshBlackP;
    Model mshBlackR;
    Model mshBlackN;
    Model mshBlackB;
    Model mshBlackK;
    Model mshBlackQ;
    Model mshOutlnP;
    Model mshOutlnN;
    Model mshOutlnB;
    Model mshOutlnK;
    Model mshOutlnQ;
    Model mshOutlnR;

    Texture2D texPiecesBlack;
    Texture2D texPiecesWhite;
    Texture2D texArcadeMachine;

    Model *getModelByPieceType(PieceType type, ChessColor color);
    Model *getOutlineByPieceType(PieceType type);
};

#endif