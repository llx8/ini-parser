#include "ini.h"
#include <ctype.h>

static char* trim(char* str)
{
    char* end;
    while(isspace((unsigned char)*str)) str++;
    if(*str == 0) return str;
    end = str + strlen(str) - 1;
    while(end > str && isspace((unsigned char)*end)) end--;
    *(end + 1) = 0;
    return str;
}

void Handle_Init(ConfigHandle** handle)
{
    if(handle == NULL)
        return;
    *handle = (ConfigHandle*)malloc(sizeof(ConfigHandle));
    if(*handle == NULL)
    {
        printf("handle malloc failed\n");
        return;
    }
    (*handle)->current_section = NULL;
    for(int i = 0; i < HASH_SIZE; i++)
    {
        (*handle)->section_hash[i] = NULL;
    }
}

unsigned int hash(const char* str)
{
    unsigned int hash = 5381;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    return hash;
}

int Creat_newsection(ConfigHandle* handle, char* buf)  
{
    if(buf == NULL || handle == NULL)
        return -1;
    
    int index = hash(buf) % HASH_SIZE;
    ConfigSection* tmp = (ConfigSection*)malloc(sizeof(ConfigSection));
    if(tmp == NULL)
    {
        printf("tmp malloc failed\n");
        return -1;
    }
    tmp->next = NULL;
    tmp->name = strdup(buf);
    for(int i = 0; i< HASH_DATA_SIZE; i++)
    {
        tmp->data_hash[i] = NULL;
    }

    if(handle->section_hash[index] == NULL)
    {
        handle->section_hash[index] = tmp;
    }
    else
    {
        tmp->next = handle->section_hash[index];
        handle->section_hash[index] = tmp;
    }

    handle->current_section = tmp;

    return 0;
}

void parse_key_value(char* buf, char** key, char** value)
{
    char* eq = strchr(buf, '=');
    if(eq == NULL)
    {
        *key = NULL;
        *value = NULL;
        return;
    }
    *eq = '\0';
    *key = trim(buf);
    *value = trim(eq + 1);
}

int Creat_newkey(ConfigSection* section, char* buf)
{
    char* key;
    char* value;

    if(section == NULL || buf == NULL)
        return -1;
    
    parse_key_value(buf, &key, &value);
    
    if(key == NULL || value == NULL)
        return -1;

    ConfigData* tmp = (ConfigData*)malloc(sizeof(ConfigData));
    if(tmp == NULL)
    {
        printf("tmp malloc failed\n");
        return -1;
    }

    int index = hash(key) % HASH_DATA_SIZE;

    tmp->key = strdup(key);
    tmp->value = strdup(value);
    tmp->next = NULL;

    if(section->data_hash[index] == NULL)
    {
        section->data_hash[index] = tmp;
    }
    else
    {
        tmp->next = section->data_hash[index];
        section->data_hash[index] = tmp;
    }

    return 0;
}

ConfigHandle* ini_load(const char* file_name)
{
    int ret;
    FILE* fp;
    char buf[256] = {0};

    ConfigHandle* handle;
    Handle_Init(&handle);

    fp = fopen(file_name, "r");
    if(fp == NULL)
    {
        printf("fopen failed\n");
        ini_free(handle);
        return NULL;
    }

    while(fgets(buf, sizeof(buf), fp))
    {
        char* line = trim(buf);
        
        if(line[0] == '\0' || line[0] == '#')
        {
            continue;
        }
        else if(line[0] == '[')
        {
            char *section_start = line + 1;
            char *section_end = strchr(section_start, ']');
            if (section_end) *section_end = '\0';
            ret = Creat_newsection(handle, section_start);
            if(ret == -1)
            {
                printf("Creat_newsection failed\n");
                fclose(fp);
                ini_free(handle);
                return NULL;
            }
        }
        else
        {
            ret = Creat_newkey(handle->current_section, line);
            if(ret == -1)
            {
                printf("Creat_newkey failed\n");
                fclose(fp);
                ini_free(handle);
                return NULL;
            }
        }
    }

    fclose(fp);

    return handle;
}

ConfigData* ini_get_key(ConfigHandle* handle, const char* section, const char* key)
{
    if(handle == NULL || section == NULL || key == NULL)
    {
        return NULL;
    }

    int sec_index = hash(section) % HASH_SIZE;
    ConfigSection* sec_tmp = handle->section_hash[sec_index];

    while(sec_tmp != NULL)
    {
        if(strcmp(sec_tmp->name, section) == 0)
        {
            int key_index = hash(key) % HASH_DATA_SIZE;
            ConfigData* key_tmp = sec_tmp->data_hash[key_index];
            while(key_tmp != NULL)
            {
                if(strcmp(key_tmp->key, key) == 0)
                {
                    return key_tmp;
                }
                key_tmp = key_tmp->next;
            }
            return NULL;
        }
        sec_tmp = sec_tmp->next;
    }

    return NULL;
}

char* ini_get_str(ConfigHandle* handle, const char* section, const char* key)
{
    if(handle == NULL || section == NULL || key == NULL)
        return NULL;

    ConfigData* key_tmp = ini_get_key(handle, section, key);
    if(key_tmp == NULL)
        return NULL;

    return key_tmp->value;
}

int ini_get_int(ConfigHandle* handle, const char* section, const char* key, int default_value)
{
    if(handle == NULL || section == NULL || key == NULL)
        return default_value;

    ConfigData* key_tmp = ini_get_key(handle, section, key);
    if(key_tmp == NULL)
        return default_value;

    return atoi(key_tmp->value);
}

float ini_get_float(ConfigHandle* handle, const char* section, const char* key, float default_value)
{
    if(handle == NULL || section == NULL || key == NULL)
        return default_value;

    ConfigData* key_tmp = ini_get_key(handle, section, key);
    if(key_tmp == NULL)
        return default_value;

    return atof(key_tmp->value);
}

void ini_free(ConfigHandle* handle)
{
    if(handle == NULL)
        return;

    for(int i = 0; i < HASH_SIZE; i++)
    {
        ConfigSection* sec_tmp = handle->section_hash[i];
        
        while(sec_tmp != NULL)
        {
            for(int j = 0; j < HASH_DATA_SIZE; j++)
            {
                ConfigData* key_tmp = sec_tmp->data_hash[j];
                
                while(key_tmp != NULL)
                {
                    ConfigData* data_to_free = key_tmp;
                    key_tmp = key_tmp->next;
                    free(data_to_free->key);
                    free(data_to_free->value);
                    free(data_to_free);
                }
            }
            
            ConfigSection* sec_to_free = sec_tmp;
            sec_tmp = sec_tmp->next;
            free(sec_to_free->name);
            free(sec_to_free);
        }
    }
    
    free(handle);
}
