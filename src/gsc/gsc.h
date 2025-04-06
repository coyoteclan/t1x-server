#ifndef GSC_H
#define GSC_H

#include "../shared.h"

#define STACK_UNDEFINED 0
#define STACK_STRING 1
#define STACK_LOCALIZED_STRING 2
#define STACK_VECTOR 3
#define STACK_FLOAT 4
#define STACK_INT 5
#define STACK_OBJECT 7
#define STACK_FUNCTION 9

const char * stackGetParamTypeAsString(int param);
int stackGetParams(const char *params, ...);
void stackError(const char *format, ...);

int stackGetParamInt(int param, int *value);
int stackGetParamFunction(int param, int *value);
int stackGetParamString(int param, char **value);
int stackGetParamConstString(int param, unsigned int *value);
int stackGetParamLocalizedString(int param, char **value);
int stackGetParamVector(int param, vec3_t value);
int stackGetParamFloat(int param, float *value);
int stackGetParamObject(int param, unsigned int *value);

//// For tests
void gsc_testfunction();
void gsc_testmethod(scr_entref_t ref);
////

//// Entity

////

//// Player
void gsc_player_processclientcommand(scr_entref_t ref);
void gsc_player_connectionlesspackettoclient(scr_entref_t ref);
////

//// Bots

////

//// Weapons
void gsc_weapons_getweaponstrproperty();
void gsc_weapons_setweaponstrproperty();
void gsc_weapons_getweaponintproperty();
void gsc_weapons_setweaponintproperty();

////

//// Utils
#if COMPILE_SSL == 1
#include <openssl/evp.h>
void gsc_utils_hash();
#endif

void gsc_utils_sendcommandtoclient();
void gsc_utils_logprintconsole();
void gsc_utils_getsubstr();
void gsc_utils_getascii();
void gsc_utils_toupper();
void gsc_utils_tolower();
void gsc_utils_strtok();
void gsc_utils_replace();
void gsc_utils_file_exists();
void gsc_utils_fopen();
void gsc_utils_fread();
void gsc_utils_fwrite();
void gsc_utils_fclose();
void gsc_utils_getserverstarttime();
void gsc_utils_getsystemtime();
void gsc_utils_strftime();
void gsc_utils_getconfigstring();
void gsc_utils_makelocalizedstring();
void gsc_utils_getlocalizedstringindex();
void gsc_utils_makeupdatedlocalizedstring();
void gsc_utils_ban();
void gsc_utils_unban();
void gsc_utils_strip();
void gsc_utils_strstr();
void gsc_utils_monotone();
void gsc_utils_gettype();


////

//// Exec
void gsc_exec();
////

#if COMPILE_CURL == 1
#include <curl/curl.h>
#include <thread>
#include <memory>
void gsc_curl_webhookmessage();
#endif

//// SQLite

////

#endif