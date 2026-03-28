#include <ws_frontend/utils.h>

namespace WS_Core {
    namespace Utils {
        std::string GetEnv(std::string_view var_name, std::string_view default_value) {
            const char* val = std::getenv(var_name.data());
            return (val && val[0] != '\0') ? std::string(val) : std::string(default_value);
        }
    }
}