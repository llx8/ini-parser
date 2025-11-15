#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include"ini.h"

/*
 * ini_load
 * ini_get_str
 * ini_get_int
 * ini_get_float
 * ini_free
*/

int main(int argc, char const *argv[])
{
    ConfigHandle* handle; //定义一个指向ConfigHandle的指针

    if(argc != 2)
    {
        printf("Usage: %s <config_file>\n", argv[0]);
        return -1;
    }

    handle = ini_load(argv[1]); //加载ini文件
    if(handle == NULL)
    {
        printf("Failed to load ini file: %s\n", argv[1]);
        return -1;
    }

    char* str = ini_get_str(handle, "system", "log_path");
    if(str == NULL)
    {
        printf("Failed to get log.path\n");
        ini_free(handle);
        return -1;
    }
    printf("log.path = %s\n", str);

    int value = ini_get_int(handle, "network", "server_port", -1);
    if(value == -1)
    {
        printf("Failed to get server.port\n");
        ini_free(handle);
        return -1;
    }
    printf("server.port = %d\n", value);

    float fvalue = ini_get_float(handle, "car", "speed", -1.0f);
    if(fvalue == -1.0f)
    {
        printf("Failed to get car.speed\n");
        ini_free(handle);
        return -1;
    }
    printf("car.speed = %f\n", fvalue);

    ini_free(handle);

    return 0;
}
