#include "shared.h"

#include "hook.h"

std::tuple<bool, int, int, std::string> getBanInfoForIp(char* ip);

// We assume a cdecl calling convention on 32-bit
