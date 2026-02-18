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

namespace WS_Core::Args {
    std::vector<Arg> args;
    std::map<const char*, Value> values;
}

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

bool find_arg(const char* name, Args::Arg* out) {
    // Check if it either begins with the long
    // or short argument thing
    // and because Windows just has to be unique
    // just check both on Windows

    char* arg = (char*)malloc(sizeof(name));
    bool kind = remove_first(name, arg);

    if(arg == NULL) {
        // If arg is NULL, remove_first just didn't
        // like it
        out = NULL;
        return false;
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
            free(arg);
            memcpy(out, &Args::args[i], sizeof(Args::Arg));
            return true;
        }
    }

    free(arg);
    out = NULL;
    return NULL;
}

bool Args::parse_args(int argc, char* const argv[]) {
    Arg current_arg;
    
    for(int i = 1; i < argc; i++) {
        if(find_arg(argv[i], &current_arg)) {
            switch(Args::args[i].arg_type) {
                case Args::Type::FLAG:
                    /// @todo Flag implementation
                    break;
                case Args::Type::INTEGER:
                    if(i == argc - 1) {
                        // Invalid arguments
                        return false;
                    }
                    Args::set_value(
                        Args::args[i].internal_name,
                        std::atoi(argv[i + 1]),
                        false
                    );
                    i++;
                    break;
                case Args::Type::STRING:
                    if(i == argc - 1) {
                        // Invalid arguments
                        return false;
                    }
                    Args::set_value(
                        Args::args[i].internal_name,
                        argv[i + 1],
                        false
                    );
                    i++;
                    break;
                case Args::Type::FLOAT:
                    if(i == argc - 1) {
                        // Invalid arguments
                        return false;
                    }
                    Args::set_value(
                        Args::args[i].internal_name,
                        std::atof(argv[i + 1]),
                        false
                    );
                    i++;
                    break;
                case Args::Type::DOUBLE:
                    if(i == argc - 1) {
                        // Invalid arguments
                        return false;
                    }
                    Args::set_value(
                        Args::args[i].internal_name,
                        std::atof(argv[i + 1]),
                        false
                    );
                    i++;
                    break;
                case Args::Type::PATH:
                    // Need to check that the path is valid
                    // even if the file/directory it is pointed
                    // at does not exist

                    /// @todo Path implementation
                    break;
                case Args::Type::PORT:
                    // This one is a little special
                    // It needs checking to ensure
                    // that it is within valid port
                    // ranges
                    // And isn't something silly

                    /// @todo Port implementation
                    break;
            }
        } else {
            // It couldn't find the argument
            // Failed to parse arguments
            return false;
        }
    }
}

Args::Value Args::get_value(const char* key, Args::Value def) {
    auto i = Args::values.find(key);
    if(i == Args::values.end()) {
        def.using_default = true;
        return def;
    } else {
        Value val = Args::values[key];
        val.using_default = false;
        return val;
    }
}

Args::Value Args::get_value(const char* key) {
    auto i = Args::values.find(key);
    if(i == Args::values.end()) {
        Args::Value val;
        val.success = false;
        return val;
    }
}

void Args::set_value(const char* key, Arg::Default val, Type type) {
    Args::values[key].value = val;
    Args::values[key].type = type;
}

void Args::set_value(const char* key, bool flag) {
    Arg::Default val;
    val.flag = flag;
    Args::set_value(key, val, Type::FLAG);
}

void Args::set_value(const char* key, int val, bool port) {
    Arg::Default _val;
    _val.integer = val;
    Args::set_value(key, _val, port ? Type::PORT : Type::INTEGER);
}

void Args::set_value(const char* key, const char* string, bool path) {
    Arg::Default val;
    strcpy(val.string, string);
    Args::set_value(key, val, path ? Type::PATH : Type::STRING);
}

void Args::set_value(const char* key, float flt) {
    Arg::Default val;
    val.flt = flt;
    Args::set_value(key, val, Type::FLOAT);
}

void Args::set_value(const char* key, double dbl) {
    Arg::Default val;
    val.dbl = dbl;
    Args::set_value(key, val, Type::DOUBLE);
}