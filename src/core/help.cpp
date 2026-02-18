#include <ws_core/ws.h>
#include <ws_core/types/args.h>
#include <ws_core/help.h>

using namespace WS_Core;

void Help::print_usage_info() {
    printf(
        "WeatherStack Core - Version %d.%d.%d - Usage information\n",
        VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH
    );

    printf("%d total available arguments\n", Args::args.size());
}