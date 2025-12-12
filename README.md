# INI 配置文件解析器

## 项目介绍

一个 C 语言实现的 INI 文件解析库，用哈希表存储 section 和键值对，支持基本的读取操作。

主要是练习 C 语言的动态内存管理和哈希表实现。

## 使用方式

### 编译

```bash
make
```

### 主要函数接口

```
ini_load("config.ini")
    加载 INI 文件，返回句柄

ini_get_str(handle, "section", "key")
    读取字符串值

ini_get_int(handle, "section", "key", 8080)
    读取整数值，key 不存在时返回默认值

ini_get_float(handle, "section", "key", 3.14)
    读取浮点数值

ini_free(handle)
    释放所有内存
```

### 示例

```c
ConfigHandle* cfg = ini_load("config.ini");
char* path = ini_get_str(cfg, "system", "log_path");
int port = ini_get_int(cfg, "network", "port", 8080);
ini_free(cfg);
```

## 支持的 INI 格式

```ini
[section1]
key1 = value1
key2 = value2

[section2]
key3 = value3
```

支持 `#` 开头的注释行，自动去除首尾空格。
