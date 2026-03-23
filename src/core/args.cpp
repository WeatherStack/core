#include <malloc.h>
#include <string.h>
#include <ws_core/ws.h>
#include <ws_core/types/args.h>

// Debugging
#include <iostream>

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
    const char* TypeNames[] = {
        WS_CORE_ARG_TYPE(TO_STR)
    };

    std::array<Arg, 6> args = {
        Arg {
            .name = "External Port",
            .desc = "The external port",

            .internal_name = "PORT",

            .def = {
                .integer = 8080
            },

            .ranges = {
                .integer = {
                    .min = 255,
                    .max = 65535
                }
            },
            .has_range = true,

            .arg_long = "port",
            .arg_short = "p",

            .arg_type = PORT
        },
        Arg {
            .name = "Internal Port",
            .desc = "The internal port",

            .internal_name = "INTERNAL_PORT",

            .def = {
                .integer = 5000
            },

            .ranges = {
                .integer = {
                    .min = 255,
                    .max = 65535
                }
            },
            .has_range = true,

            .arg_long = "port",
            .arg_short = "Ip",

            .arg_type = PORT
        },
        Arg {
            .name = "Version",
            .desc = "Display version information",

            .internal_name = "VERSION",

            .def = {
                .flag = false
            },

            .has_range = false,

            .arg_long = "version",
            .arg_short = "v",

            .arg_type = FLAG
        },
        Arg {
            .name = "Help",
            .desc = "Display usage information",

            .internal_name = "HELP",

            .def = {
                .flag = false
            },

            .has_range = false,

            .arg_long = "help",
            .arg_short = "h",

            .arg_type = FLAG
        },
        Arg {
            .name = "Verbose",
            .desc = "Display all information",

            .internal_name = "VERBOSE",

            .def = {
                .flag = false
            },

            .has_range = false,

            .arg_long = "verbose",
            .arg_short = "vB",

            .arg_type = FLAG
        },
        Arg {
            .name = "Debug",
            .desc = "Display debug information",

            .internal_name = "DEBUG",

            .def = {
                .flag = false
            },

            .has_range = false,

            .arg_long = "debug",
            .arg_short = "d",

            .arg_type = FLAG
        }
    };
    std::map<const char*, Value, compare_strings> values;
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
        strcmp(lower, "true") == 1) {
        return true;
    }
    
    if(strcmp(lower, "off") == 1 ||
        strcmp(lower, "false") == 1) {
        return false;
    }
}

bool str_is_bool(const char* str) {
    char* lower = (char*)malloc(sizeof(str));
    str_to_lower(str, lower);
    return strcmp(lower, "on") == 1 ||
        strcmp(lower, "true") == 1 ||
        strcmp(lower, "off") == 1 ||
        strcmp(lower, "false") == 1;
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
    return false;
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
    return kind;
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
    return false;
}

bool Args::parse_args(int argc, char* const argv[]) {
    Arg current_arg;

    // Implement default values
    for(int i = 0; i < args.size(); i++) {
        values[args[i].internal_name] = Value {
            .value = args[i].def,
            .type = args[i].arg_type,
            .using_default = true,
            .success = true
        };
    }
    
    /// @bug This is for some reason picking up flags as ports
    for(int i = 1; i < argc; i++) {
        if(find_arg(argv[i], &current_arg)) {
            printf("%s %s\n", argv[i], TypeNames[current_arg.arg_type]);
            switch(current_arg.arg_type) {
                case FLAG:
                    if(i > argc - 1) {
                        // Check to see if it is a form of boolean
                        if(str_is_bool(argv[i + 1])) {
                            bool val = str_to_bool(argv[i + 1]);

                            Args::set_value(
                                current_arg.internal_name,
                                val
                            );

                            i++;
                        }
                    }

                    // Maybe in the future warn the user
                    // that specifying a flag that is by
                    // default true, without specifying
                    // false will do nothing

                    Args::set_value(
                        current_arg.internal_name,
                        true
                    );
                    break;
                case INTEGER:
                    if(i == argc - 1) {
                        // Invalid arguments
                        return false;
                    }
                    Args::set_value(
                        current_arg.internal_name,
                        std::atoi(argv[i + 1]),
                        false
                    );
                    i++;
                    break;
                case STRING:
                    if(i == argc - 1) {
                        // Invalid arguments
                        return false;
                    }
                    Args::set_value(
                        current_arg.internal_name,
                        argv[i + 1],
                        false
                    );
                    i++;
                    break;
                case FLOAT:
                    if(i == argc - 1) {
                        // Invalid arguments
                        return false;
                    }
                    Args::set_value(
                        current_arg.internal_name,
                        std::atof(argv[i + 1]),
                        false
                    );
                    i++;
                    break;
                case DOUBLE:
                    if(i == argc - 1) {
                        // Invalid arguments
                        return false;
                    }
                    Args::set_value(
                        current_arg.internal_name,
                        std::atof(argv[i + 1]),
                        false
                    );
                    i++;
                    break;
                case PATH:
                    // Need to check that the path is valid
                    // even if the file/directory it is pointed
                    // at does not exist

                    /// @todo Path implementation
                    break;
                case PORT:
                    // This one is a little special
                    // It needs checking to ensure
                    // that it is within valid port
                    // ranges
                    // And isn't something silly

                    if(i == argc - 1) {
                        // Invalid arguments
                        return false;
                    }
                    auto port = strtol(argv[i + 1], nullptr, 10);

                    // Do checks
                    if(port >= 255 && port <= 65535) {
                        Args::set_value(
                            current_arg.internal_name,
                            port,
                            true
                        );
                    } else {
                        // Error, invalid port
                    }
                    
                    i++;
                    break;
            }
        } else {
            // It couldn't find the argument
            // Failed to parse arguments
            return false;
        }
        printf("\n");
    }

    return true;
}

Args::Value Args::get_value(const char* key, Value def) {
    auto i = values.find(key);
    if(i == values.end()) {
        def.using_default = true;
        return def;
    } else {
        Value val = values[key];
        val.using_default = false;
        return val;
    }
}

Args::Value Args::get_value(const char* key) {
    auto i = values.find(key);
    if(i == values.end()) {
        Args::Value val;
        val.success = false;
        return val;
    }
    Args::Value _val = i->second;
    _val.success = true;
    return _val;
}

void Args::set_value(const char* key, Arg::Default val, Type type) {
    values[key].value = val;
    values[key].type = type;
}

void Args::set_value(const char* key, bool flag) {
    Arg::Default val;
    val.flag = flag;
    set_value(key, val, Type::FLAG);
}

void Args::set_value(const char* key, int val, bool port) {
    Arg::Default _val;
    _val.integer = val;
    set_value(key, _val, port ? Type::PORT : Type::INTEGER);
}

void Args::set_value(const char* key, const char* string, bool path) {
    Arg::Default val;
    val.string = (char*)malloc(sizeof(string));
    strcpy(val.string, string);
    set_value(key, val, path ? Type::PATH : Type::STRING);
}

void Args::set_value(const char* key, float flt) {
    Arg::Default val;
    val.flt = flt;
    set_value(key, val, Type::FLOAT);
}

void Args::set_value(const char* key, double dbl) {
    Arg::Default val;
    val.dbl = dbl;
    set_value(key, val, Type::DOUBLE);
}

void Args::debug_log_values() {
    printf("Current argument values:\n");
    for(auto it = values.begin(); it != values.end(); it++) {
        printf("%s\n\t", it->first);
        printf("Type:    %d (%s)\n\t", it->second.type, TypeNames[it->second.type]);
        printf("Value:   ");
        switch(it->second.type) {
            case FLAG:
                printf(it->second.value.flag ? "true" : "false");
                break;
            case INTEGER:
            case PORT:
                printf("%d", it->second.value.integer);
                break;
            case STRING:
            case PATH:
                printf(it->second.value.string);
                break;
            case FLOAT:
                printf("%d", it->second.value.flt);
                break;
            case DOUBLE:
                printf("%d", it->second.value.dbl);
                break;
        }
        printf("\n\tDefault: %s", it->second.using_default ? "true" : "false");
        printf("\n\tSuccess: %s\n", it->second.success ? "true" : "false");

        printf("\n");
    }
}