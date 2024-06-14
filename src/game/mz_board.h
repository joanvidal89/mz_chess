#ifndef MZ_BOARD
#define MZ_BOARD

#include <array>
#include <future>
#include <iomanip>
#include <sstream>
#include <vector>

#include "mz_struct.h"
#include "raylib.h"

class MZBoard
{
public:
    MZBoard();
    ~MZBoard();

    // CELLS
    void setCellState(BoardPosition position, CellState state);
    void setAllCellsState(CellState state);
    void markCheckCells();

    // PIECES
    void addPiece(BoardPosition position, PieceType type, ChessColor color);
    void deletePiece(BoardPosition position);
    void movePiece(BoardPosition origin, BoardPosition target, MoveType mtype);
    void setPieceState(BoardPosition position, PieceState state);
    void setAllPiecesState(PieceState state);

    // MOVEMENT
    std::vector<PieceMove> &getValidMovements(BoardPosition position);
    void clearMovements();

    // ASYNC
    void processBoard();


    // ENDGAME
    bool checkNoAvaiableMoves();
    bool checkDrawByInactivity();
    bool checkDrawByMaterial();
    bool checkAILoss();
    void setGameResult(GameResult result);
    GameResult getGameResult();

    // UTILS
    void initializeBoard(bool blitz);
    bool isBoardPositionValid(BoardPosition position);
    Cell &getBoardCell(BoardPosition position);
    std::array<Cell, 64> &getBoardCells();
    void addPlayerTime(float dt);
    void addFullTurn();
    int calculateScore();

    // UI
    std::string getPlayerTime();
    std::string getFullTurn();
    std::string getWhiteDeaths();
    std::string getBlackDeaths();

private:
    std::future<void> processBoardFuture;

    PieceMove lastMove = PieceMove{BoardPosition{-1, -1}, BoardPosition{-1, -1}, MT_MOVE};
    bool isBlitz = false;
    float playerTime = 2400.0f;
    int fullTurn = 0;
    int turnsToDraw = 50;

    GameResult gameResult = GS_NONE;
    int whiteDeaths[5] = {0, 0, 0, 0, 0};
    int blackDeaths[5] = {0, 0, 0, 0, 0};

    void addMovementToPiece(BoardPosition origin, BoardPosition target, MoveType moveType);
    Cell &getAuxiliaryBoardCell(BoardPosition position, std::array<Cell, 64> *auxiliaryBoard);
    bool checkMoveValidity(PieceMove pmove);

    const int rookDeltas[4][2] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
    const int bishopDeltas[4][2] = {{1, 1}, {-1, 1}, {1, -1}, {-1, -1}};
    const int queenDeltas[8][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}, {-1, -1}, {1, -1}, {-1, 1}, {1, 1}};
    int knightDeltas[8][2] = {{-2, -1}, {-2, 1}, {-1, -2}, {-1, 2}, {1, -2}, {1, 2}, {2, -1}, {2, 1}};
    bool playerCastling[2];

    std::array<Cell, 64> boardCells =
        {
            Cell{BoardPosition{0, 0}, Rectangle{160, 660, 40, 40}, Vector2{4.5f, -1.0f}, CC_BLACK, CS_DISABLED, PT_NONE, CC_NONE, PS_NONE, std::vector<PieceMove>()},
            Cell{BoardPosition{1, 0}, Rectangle{200, 660, 40, 40}, Vector2{5.5f, -1.0f}, CC_WHITE, CS_DISABLED, PT_NONE, CC_NONE, PS_NONE, std::vector<PieceMove>()},
            Cell{BoardPosition{2, 0}, Rectangle{240, 660, 40, 40}, Vector2{6.5f, -1.0f}, CC_BLACK, CS_DISABLED, PT_NONE, CC_NONE, PS_NONE, std::vector<PieceMove>()},
            Cell{BoardPosition{3, 0}, Rectangle{280, 660, 40, 40}, Vector2{7.5f, -1.0f}, CC_WHITE, CS_DISABLED, PT_NONE, CC_NONE, PS_NONE, std::vector<PieceMove>()},
            Cell{BoardPosition{4, 0}, Rectangle{320, 660, 40, 40}, Vector2{8.5f, -1.0f}, CC_BLACK, CS_DISABLED, PT_NONE, CC_NONE, PS_NONE, std::vector<PieceMove>()},
            Cell{BoardPosition{5, 0}, Rectangle{360, 660, 40, 40}, Vector2{9.5f, -1.0f}, CC_WHITE, CS_DISABLED, PT_NONE, CC_NONE, PS_NONE, std::vector<PieceMove>()},
            Cell{BoardPosition{6, 0}, Rectangle{400, 660, 40, 40}, Vector2{10.5f, -1.0f}, CC_BLACK, CS_DISABLED, PT_NONE, CC_NONE, PS_NONE, std::vector<PieceMove>()},
            Cell{BoardPosition{7, 0}, Rectangle{440, 660, 40, 40}, Vector2{11.5f, -1.0f}, CC_WHITE, CS_DISABLED, PT_NONE, CC_NONE, PS_NONE, std::vector<PieceMove>()},
            Cell{BoardPosition{0, 1}, Rectangle{160, 620, 40, 40}, Vector2{4.5f, -2.0f}, CC_WHITE, CS_DISABLED, PT_NONE, CC_NONE, PS_NONE, std::vector<PieceMove>()},
            Cell{BoardPosition{1, 1}, Rectangle{200, 620, 40, 40}, Vector2{5.5f, -2.0f}, CC_BLACK, CS_DISABLED, PT_NONE, CC_NONE, PS_NONE, std::vector<PieceMove>()},
            Cell{BoardPosition{2, 1}, Rectangle{240, 620, 40, 40}, Vector2{6.5f, -2.0f}, CC_WHITE, CS_DISABLED, PT_NONE, CC_NONE, PS_NONE, std::vector<PieceMove>()},
            Cell{BoardPosition{3, 1}, Rectangle{280, 620, 40, 40}, Vector2{7.5f, -2.0f}, CC_BLACK, CS_DISABLED, PT_NONE, CC_NONE, PS_NONE, std::vector<PieceMove>()},
            Cell{BoardPosition{4, 1}, Rectangle{320, 620, 40, 40}, Vector2{8.5f, -2.0f}, CC_WHITE, CS_DISABLED, PT_NONE, CC_NONE, PS_NONE, std::vector<PieceMove>()},
            Cell{BoardPosition{5, 1}, Rectangle{360, 620, 40, 40}, Vector2{9.5f, -2.0f}, CC_BLACK, CS_DISABLED, PT_NONE, CC_NONE, PS_NONE, std::vector<PieceMove>()},
            Cell{BoardPosition{6, 1}, Rectangle{400, 620, 40, 40}, Vector2{10.5f, -2.0f}, CC_WHITE, CS_DISABLED, PT_NONE, CC_NONE, PS_NONE, std::vector<PieceMove>()},
            Cell{BoardPosition{7, 1}, Rectangle{440, 620, 40, 40}, Vector2{11.5f, -2.0f}, CC_BLACK, CS_DISABLED, PT_NONE, CC_NONE, PS_NONE, std::vector<PieceMove>()},
            Cell{BoardPosition{0, 2}, Rectangle{160, 580, 40, 40}, Vector2{4.5f, -3.0f}, CC_BLACK, CS_DISABLED, PT_NONE, CC_NONE, PS_NONE, std::vector<PieceMove>()},
            Cell{BoardPosition{1, 2}, Rectangle{200, 580, 40, 40}, Vector2{5.5f, -3.0f}, CC_WHITE, CS_DISABLED, PT_NONE, CC_NONE, PS_NONE, std::vector<PieceMove>()},
            Cell{BoardPosition{2, 2}, Rectangle{240, 580, 40, 40}, Vector2{6.5f, -3.0f}, CC_BLACK, CS_DISABLED, PT_NONE, CC_NONE, PS_NONE, std::vector<PieceMove>()},
            Cell{BoardPosition{3, 2}, Rectangle{280, 580, 40, 40}, Vector2{7.5f, -3.0f}, CC_WHITE, CS_DISABLED, PT_NONE, CC_NONE, PS_NONE, std::vector<PieceMove>()},
            Cell{BoardPosition{4, 2}, Rectangle{320, 580, 40, 40}, Vector2{8.5f, -3.0f}, CC_BLACK, CS_DISABLED, PT_NONE, CC_NONE, PS_NONE, std::vector<PieceMove>()},
            Cell{BoardPosition{5, 2}, Rectangle{360, 580, 40, 40}, Vector2{9.5f, -3.0f}, CC_WHITE, CS_DISABLED, PT_NONE, CC_NONE, PS_NONE, std::vector<PieceMove>()},
            Cell{BoardPosition{6, 2}, Rectangle{400, 580, 40, 40}, Vector2{10.5f, -3.0f}, CC_BLACK, CS_DISABLED, PT_NONE, CC_NONE, PS_NONE, std::vector<PieceMove>()},
            Cell{BoardPosition{7, 2}, Rectangle{440, 580, 40, 40}, Vector2{11.5f, -3.0f}, CC_WHITE, CS_DISABLED, PT_NONE, CC_NONE, PS_NONE, std::vector<PieceMove>()},
            Cell{BoardPosition{0, 3}, Rectangle{160, 540, 40, 40}, Vector2{4.5f, -4.0f}, CC_WHITE, CS_DISABLED, PT_NONE, CC_NONE, PS_NONE, std::vector<PieceMove>()},
            Cell{BoardPosition{1, 3}, Rectangle{200, 540, 40, 40}, Vector2{5.5f, -4.0f}, CC_BLACK, CS_DISABLED, PT_NONE, CC_NONE, PS_NONE, std::vector<PieceMove>()},
            Cell{BoardPosition{2, 3}, Rectangle{240, 540, 40, 40}, Vector2{6.5f, -4.0f}, CC_WHITE, CS_DISABLED, PT_NONE, CC_NONE, PS_NONE, std::vector<PieceMove>()},
            Cell{BoardPosition{3, 3}, Rectangle{280, 540, 40, 40}, Vector2{7.5f, -4.0f}, CC_BLACK, CS_DISABLED, PT_NONE, CC_NONE, PS_NONE, std::vector<PieceMove>()},
            Cell{BoardPosition{4, 3}, Rectangle{320, 540, 40, 40}, Vector2{8.5f, -4.0f}, CC_WHITE, CS_DISABLED, PT_NONE, CC_NONE, PS_NONE, std::vector<PieceMove>()},
            Cell{BoardPosition{5, 3}, Rectangle{360, 540, 40, 40}, Vector2{9.5f, -4.0f}, CC_BLACK, CS_DISABLED, PT_NONE, CC_NONE, PS_NONE, std::vector<PieceMove>()},
            Cell{BoardPosition{6, 3}, Rectangle{400, 540, 40, 40}, Vector2{10.5f, -4.0f}, CC_WHITE, CS_DISABLED, PT_NONE, CC_NONE, PS_NONE, std::vector<PieceMove>()},
            Cell{BoardPosition{7, 3}, Rectangle{440, 540, 40, 40}, Vector2{11.5f, -4.0f}, CC_BLACK, CS_DISABLED, PT_NONE, CC_NONE, PS_NONE, std::vector<PieceMove>()},
            Cell{BoardPosition{0, 4}, Rectangle{160, 500, 40, 40}, Vector2{4.5f, -5.0f}, CC_BLACK, CS_DISABLED, PT_NONE, CC_NONE, PS_NONE, std::vector<PieceMove>()},
            Cell{BoardPosition{1, 4}, Rectangle{200, 500, 40, 40}, Vector2{5.5f, -5.0f}, CC_WHITE, CS_DISABLED, PT_NONE, CC_NONE, PS_NONE, std::vector<PieceMove>()},
            Cell{BoardPosition{2, 4}, Rectangle{240, 500, 40, 40}, Vector2{6.5f, -5.0f}, CC_BLACK, CS_DISABLED, PT_NONE, CC_NONE, PS_NONE, std::vector<PieceMove>()},
            Cell{BoardPosition{3, 4}, Rectangle{280, 500, 40, 40}, Vector2{7.5f, -5.0f}, CC_WHITE, CS_DISABLED, PT_NONE, CC_NONE, PS_NONE, std::vector<PieceMove>()},
            Cell{BoardPosition{4, 4}, Rectangle{320, 500, 40, 40}, Vector2{8.5f, -5.0f}, CC_BLACK, CS_DISABLED, PT_NONE, CC_NONE, PS_NONE, std::vector<PieceMove>()},
            Cell{BoardPosition{5, 4}, Rectangle{360, 500, 40, 40}, Vector2{9.5f, -5.0f}, CC_WHITE, CS_DISABLED, PT_NONE, CC_NONE, PS_NONE, std::vector<PieceMove>()},
            Cell{BoardPosition{6, 4}, Rectangle{400, 500, 40, 40}, Vector2{10.5f, -5.0f}, CC_BLACK, CS_DISABLED, PT_NONE, CC_NONE, PS_NONE, std::vector<PieceMove>()},
            Cell{BoardPosition{7, 4}, Rectangle{440, 500, 40, 40}, Vector2{11.5f, -5.0f}, CC_WHITE, CS_DISABLED, PT_NONE, CC_NONE, PS_NONE, std::vector<PieceMove>()},
            Cell{BoardPosition{0, 5}, Rectangle{160, 460, 40, 40}, Vector2{4.5f, -6.0f}, CC_WHITE, CS_DISABLED, PT_NONE, CC_NONE, PS_NONE, std::vector<PieceMove>()},
            Cell{BoardPosition{1, 5}, Rectangle{200, 460, 40, 40}, Vector2{5.5f, -6.0f}, CC_BLACK, CS_DISABLED, PT_NONE, CC_NONE, PS_NONE, std::vector<PieceMove>()},
            Cell{BoardPosition{2, 5}, Rectangle{240, 460, 40, 40}, Vector2{6.5f, -6.0f}, CC_WHITE, CS_DISABLED, PT_NONE, CC_NONE, PS_NONE, std::vector<PieceMove>()},
            Cell{BoardPosition{3, 5}, Rectangle{280, 460, 40, 40}, Vector2{7.5f, -6.0f}, CC_BLACK, CS_DISABLED, PT_NONE, CC_NONE, PS_NONE, std::vector<PieceMove>()},
            Cell{BoardPosition{4, 5}, Rectangle{320, 460, 40, 40}, Vector2{8.5f, -6.0f}, CC_WHITE, CS_DISABLED, PT_NONE, CC_NONE, PS_NONE, std::vector<PieceMove>()},
            Cell{BoardPosition{5, 5}, Rectangle{360, 460, 40, 40}, Vector2{9.5f, -6.0f}, CC_BLACK, CS_DISABLED, PT_NONE, CC_NONE, PS_NONE, std::vector<PieceMove>()},
            Cell{BoardPosition{6, 5}, Rectangle{400, 460, 40, 40}, Vector2{10.5f, -6.0f}, CC_WHITE, CS_DISABLED, PT_NONE, CC_NONE, PS_NONE, std::vector<PieceMove>()},
            Cell{BoardPosition{7, 5}, Rectangle{440, 460, 40, 40}, Vector2{11.5f, -6.0f}, CC_BLACK, CS_DISABLED, PT_NONE, CC_NONE, PS_NONE, std::vector<PieceMove>()},
            Cell{BoardPosition{0, 6}, Rectangle{160, 420, 40, 40}, Vector2{4.5f, -7.0f}, CC_BLACK, CS_DISABLED, PT_NONE, CC_NONE, PS_NONE, std::vector<PieceMove>()},
            Cell{BoardPosition{1, 6}, Rectangle{200, 420, 40, 40}, Vector2{5.5f, -7.0f}, CC_WHITE, CS_DISABLED, PT_NONE, CC_NONE, PS_NONE, std::vector<PieceMove>()},
            Cell{BoardPosition{2, 6}, Rectangle{240, 420, 40, 40}, Vector2{6.5f, -7.0f}, CC_BLACK, CS_DISABLED, PT_NONE, CC_NONE, PS_NONE, std::vector<PieceMove>()},
            Cell{BoardPosition{3, 6}, Rectangle{280, 420, 40, 40}, Vector2{7.5f, -7.0f}, CC_WHITE, CS_DISABLED, PT_NONE, CC_NONE, PS_NONE, std::vector<PieceMove>()},
            Cell{BoardPosition{4, 6}, Rectangle{320, 420, 40, 40}, Vector2{8.5f, -7.0f}, CC_BLACK, CS_DISABLED, PT_NONE, CC_NONE, PS_NONE, std::vector<PieceMove>()},
            Cell{BoardPosition{5, 6}, Rectangle{360, 420, 40, 40}, Vector2{9.5f, -7.0f}, CC_WHITE, CS_DISABLED, PT_NONE, CC_NONE, PS_NONE, std::vector<PieceMove>()},
            Cell{BoardPosition{6, 6}, Rectangle{400, 420, 40, 40}, Vector2{10.5f, -7.0f}, CC_BLACK, CS_DISABLED, PT_NONE, CC_NONE, PS_NONE, std::vector<PieceMove>()},
            Cell{BoardPosition{7, 6}, Rectangle{440, 420, 40, 40}, Vector2{11.5f, -7.0f}, CC_WHITE, CS_DISABLED, PT_NONE, CC_NONE, PS_NONE, std::vector<PieceMove>()},
            Cell{BoardPosition{0, 7}, Rectangle{160, 380, 40, 40}, Vector2{4.5f, -8.0f}, CC_WHITE, CS_DISABLED, PT_NONE, CC_NONE, PS_NONE, std::vector<PieceMove>()},
            Cell{BoardPosition{1, 7}, Rectangle{200, 380, 40, 40}, Vector2{5.5f, -8.0f}, CC_BLACK, CS_DISABLED, PT_NONE, CC_NONE, PS_NONE, std::vector<PieceMove>()},
            Cell{BoardPosition{2, 7}, Rectangle{240, 380, 40, 40}, Vector2{6.5f, -8.0f}, CC_WHITE, CS_DISABLED, PT_NONE, CC_NONE, PS_NONE, std::vector<PieceMove>()},
            Cell{BoardPosition{3, 7}, Rectangle{280, 380, 40, 40}, Vector2{7.5f, -8.0f}, CC_BLACK, CS_DISABLED, PT_NONE, CC_NONE, PS_NONE, std::vector<PieceMove>()},
            Cell{BoardPosition{4, 7}, Rectangle{320, 380, 40, 40}, Vector2{8.5f, -8.0f}, CC_WHITE, CS_DISABLED, PT_NONE, CC_NONE, PS_NONE, std::vector<PieceMove>()},
            Cell{BoardPosition{5, 7}, Rectangle{360, 380, 40, 40}, Vector2{9.5f, -8.0f}, CC_BLACK, CS_DISABLED, PT_NONE, CC_NONE, PS_NONE, std::vector<PieceMove>()},
            Cell{BoardPosition{6, 7}, Rectangle{400, 380, 40, 40}, Vector2{10.5f, -8.0f}, CC_WHITE, CS_DISABLED, PT_NONE, CC_NONE, PS_NONE, std::vector<PieceMove>()},
            Cell{BoardPosition{7, 7}, Rectangle{440, 380, 40, 40}, Vector2{11.5f, -8.0f}, CC_BLACK, CS_DISABLED, PT_NONE, CC_NONE, PS_NONE, std::vector<PieceMove>()},
    };
};

#endif