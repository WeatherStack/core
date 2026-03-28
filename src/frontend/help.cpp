#include <iostream>
#include <ws_frontend/ws.h>
#include <ws_frontend/types/args.h>
#include <ws_frontend/help.h>

using namespace WS_Core;

void Help::print_usage_info() {
    printf(std::format("WeatherStack Core - Version {}.{}.{} - Usage information\n", VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH).c_str());
    printf(std::format("{} total available arguments\n", Args::args.size()).c_str());
}