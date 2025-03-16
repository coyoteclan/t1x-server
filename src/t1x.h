#include "shared.h"

#include "hook.h"

static void ban();
static void unban();
std::tuple<bool, int, int, std::string> getBanInfoForIp(char* ip);

// We assume a cdecl calling convention on 32-bit
