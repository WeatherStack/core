#pragma once

#include <string>
#include <string_view>

namespace WS_Core {
    namespace Utils {
        std::string GetEnv(std::string_view var_name, std::string_view default_value);
    }
}