#ifndef _INI_H
#define _INI_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASH_SIZE 128
#define HASH_DATA_SIZE 64

typedef struct ConfigData{
    char* key;
    char* value;
    struct ConfigData* next;
}ConfigData;

typedef struct ConfigSection{
    char* name;
    ConfigData* data_hash[HASH_DATA_SIZE];
    struct ConfigSection* next;
}ConfigSection;

typedef struct{
    ConfigSection* section_hash[HASH_SIZE];
    ConfigSection* current_section;
}ConfigHandle;

ConfigHandle* ini_load(const char* file_name);
void ini_free(ConfigHandle* handle);
char* ini_get_str(ConfigHandle* handle, const char* section, const char*key);
int ini_get_int(ConfigHandle* handle, const char* section, const char* key, int default_value);
float ini_get_float(ConfigHandle* handle, const char* section, const char* key, float default_value);

#endif // !_INI_H