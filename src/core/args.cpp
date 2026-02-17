#include <malloc.h>
#include <string.h>
#include <ws_core/ws.h>
#include <ws_core/types/args.h>

using namespace WS_Core;

/*
    Parsing arguments

    Rules:
    Flags - Flags have a default value, and because
            the default value can be true, there has
            to be a way of setting it to false.

            Flags can EITHER just be as the flag, or
            it can be the flag, then one of the
            following: "on", "off", "true", or "false"

            E.g. "--debug"      (Default: false)
                 "--debug on"   (Default: false)
                 "--debug off"  (Default: true)
    
    Pretty much everything else is self-explanatory,
    so there's no need for an explanation.

    Args::parse_args does most of the work
*/

void str_to_lower(const char* o, char* n) {
    strcpy(n, o);
    for(int i = 0; i < strlen(n); i++) {
        n[i] = tolower(o[i]);
    }
}

bool str_to_bool(const char* str) {
    char* lower = (char*)malloc(sizeof(str));
    str_to_lower(str, lower);
    if(strcmp(lower, "on") == 1 ||
        strcmp(lower, "true")) {
        return true;
    } else if(strcmp(lower, "off") == 1 ||
        strcmp(lower, "false")) {
        return false;
    }
}

bool starts_with(const char* prefix, const char* str) {
    return strstr(str, prefix) == str;
}

// Removes either the first 1 or 2 characters
// returns if it is a long argument or not
bool remove_first(const char* o, char* n) {
    #if defined(_WIN32) || defined(_WIN64)
    if(starts_with(WS_CLI_ARG_LONG_BEGIN, o)) {
        strcpy(n, o);
        n++;
    } else {
        n = NULL;
        return false;
    }
    #else
    bool kind = false;
    if(starts_with(WS_CLI_ARG_LONG_BEGIN, o)) {
        kind = true;
        strcpy(n, o);
        n += 2;
    } else if(starts_with(WS_CLI_ARG_SHORT_BEGIN, o)) {
        strcpy(n, o);
        n++;
    } else {
        n = NULL;
        return false;
    }
    #endif
}

Args::Arg* find_arg(const char* name) {
    // Check if it either begins with the long
    // or short argument thing
    // and because Windows just has to be unique
    // just check both on Windows

    char* arg = (char*)malloc(sizeof(name));
    bool kind = remove_first(name, arg);

    if(arg == NULL) {
        // If arg is NULL, remove_first just didn't
        // like it
        return NULL;
    }

    for(int i = 0; i < Args::args.size(); i++) {
        bool is_arg = false;
    #if defined(_WIN32) || defined(_WIN64)
        // Completely discard "kind"
        if(strcmp(Args::args[i].arg_long, name) == 1 ||
            strcmp(Args::args[i].arg_short, name) == 1) {
            is_arg = true;
        }
    #else
        if(kind &&
            strcmp(Args::args[i].arg_long, name) == 1) {
            is_arg = true;
        } else if(strcmp(Args::args[i].arg_short, name) == 1) {
            is_arg = true;
        }
    #endif

        if(is_arg) {
            
        }
    }

    free(arg);
}

bool Args::parse_args(int argc, char* const argv[]) {
    for(int i = 1; i < argc; i++) {

    }
}