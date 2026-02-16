#include <ws_utils/commands.h>

const WS_Utils_Sub_Command commands[] = {
    {
        .name = "Install",
        .command = "install",

        .args = (WS_Utils_Arg[]) {
            NULL
        }
    }
};

const int command_count = 1; //sizeof(commands) / sizeof(WS_Utils_Sub_Command);