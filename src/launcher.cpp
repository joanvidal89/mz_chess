#include "launcher/mz_launcher.h"
#include "launcher/mz_execute.h"

int main(int argc, char const *argv[])
{
    MZLauncher *launcher = new MZLauncher();

    if (launcher->run())
    {
        CloseWindow();
        int accountId = launcher->accountId;
        delete (launcher);

        MZExecute().executeProcess(accountId);
    }
    else
    {
        CloseWindow();
        delete (launcher);
    }

    return 0;
}