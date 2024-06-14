#include "raylib.h"
#include "game/mz_director.h"

int main(int argc, char const *argv[])
{
    if (argc != 3)
        return 1;

    int accountId = std::stoi(argv[1]);
    int skipIntro = std::stoi(argv[2]);
    printf("ACCOUNT ID = %i", accountId);
    MZDirector(accountId, skipIntro).run();
    CloseWindow();

    return 0;
}
