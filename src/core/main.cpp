#include <crow.h>
#include <ws_core/ws.h>
#include <ws_core/utils.h>

using namespace WS_Core;

int main(int argc, char* const argv[]) {
    // Fetch environment variables
    const int PORT = std::stoi(Utils::GetEnv("PORT", "8000"));

    // Create Crow app
    crow::SimpleApp app;

    // Register all routes
    CROW_ROUTE(app, "/").methods(crow::HTTPMethod::GET)([](const crow::request& req) {
        std::string res = "Welcome to WeatherStack Core!";
        return res;
    });

    // Begin listening
    app.port(PORT).multithreaded().run();

    return 0;
}