#ifndef GLOBALS_H
#define GLOBALS_H
#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)
#define MAJOR_VER 1
#define MINOR_VER 2
//#define def_api_url "http://f8api.dev/api/tool/"
#define def_api_url "http://api.f8filter.net/api/tool/"
enum service_type
{
    s_start,
    s_restart,
    s_stop
};
#endif // GLOBALS_H
