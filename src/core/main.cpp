#include <crow.h>
#include <ws_core/ws.h>
#include <ws_core/utils.h>
#include <ws_core/help.h>
#include <ws_core/types/args.h>

/*
    The WeatherStack Core

    This literally ties everything together, providing
    a central API for communication.

    It runs two HTTP servers, one designed to be accessed
    externally, and one designed to be used mainly internally
    but can be released publicly with some configuration changes.

    Visit the documentation here:
        https://weatherstack.wtdawson.info/docs/core/

    Doxygen documentation (for reference):
        https://ws-core-ref.wtdawson.info/
    NOTE: Doxygen only actually gets generated either
          manually, or by pushing to the master branch
*/

using namespace WS_Core;

#define ENV(x)          Args::get_value(x)
#define ENVD(x, y)      Args::get_value(x, y)
#define ENVS(x, y, z)   Args::set_value(x, y, z)
#define ENVSV(x, y)     Args::set_value(x, y)

int main(int argc, char* const argv[]) {
    // Fetch environment variables
    const int PORT = std::stoi(Utils::GetEnv("PORT", "8000"));
    const int INTERNAL_PORT = std::stoi(Utils::GetEnv("INTERNAL_PORT", "5000"));

    // Set environment variables into arguments
    ENVS("PORT", PORT, true);
    ENVS("INTERNAL_PORT", INTERNAL_PORT, true);

    // Parse arguments
    // Arguments can (sometimes) override environment variables
    Args::parse_args(argc, argv);

    // Check for verbose/debug, output info if needed
    Args::debug_log_values();

    // Before doing anything, check to see some basic flags
    // --version, -v        Display version information
    // --help, -h           Display very basic help information
    if(ENV("VERSION").value.flag == true) {
        printf(
            "WeatherStack Core - Version %d.%d.%d\n",
            VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH
        );

        return 0;
    }

    if(ENV("HELP").value.flag == true) {
        // Display basic help information
        Help::print_usage_info();

        return 0;
    }

    // Create Crow app
    crow::SimpleApp app;
    crow::SimpleApp internal_app;

    // Register all routes
    CROW_ROUTE(app, "/").methods(crow::HTTPMethod::GET)([](const crow::request& req) {
        crow::json::wvalue response;
        response["version"] = STR(VERSION_MAJOR) "." STR(VERSION_MINOR) "." STR(VERSION_PATCH);
        response["name"] = "WeatherStack Core External API";
        response["repository"] = "https://github.com/WeatherStack/Core";
        response["message"] = "Number 5 is alive!";

        response["success"] = true;
        response["code"] = 200;
        return response;
    });

    // Internal information API route
    CROW_ROUTE(internal_app, "/").methods(crow::HTTPMethod::GET)([](const crow::request& req) {
        crow::json::wvalue response;
        response["version"] = STR(VERSION_MAJOR) "." STR(VERSION_MINOR) "." STR(VERSION_PATCH);
        response["name"] = "WeatherStack Core Internal API";
        response["repository"] = "https://github.com/WeatherStack/Core";
        response["message"] = "Number 5 is alive!";

        response["success"] = true;
        response["code"] = 200;

        return response;
    });

    // Begin listening
    std::future<void> app_future = app.port(ENV("PORT").value.integer).multithreaded().run_async();
    internal_app.port(ENV("INTERNAL_PORT").value.integer).multithreaded().run();

    /// @todo Add something here so that the above internal API can run async

    return 0;
}