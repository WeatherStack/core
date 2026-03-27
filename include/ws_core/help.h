#pragma once

/*
    This provides help to the user, making it as user-friendly as possible,
    it helps explain what went wrong, and tries to offer solutions.
*/

namespace WS_Core {
    namespace Help {
        // Triggered by --help, -h
        void print_usage_info();
    }
}