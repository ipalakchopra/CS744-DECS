#include <string.h>
#include "functions.h"

int get_url_id(const char *url)
{
    if (url == NULL)
        return -1;

    for (int i = 0; i < sizeof(url_map) / sizeof(url_map[0]); i++) {
        if (strcmp(url, url_map[i].url) == 0)
            return url_map[i].id;
    }

    return -1;
}

const char *root(void)
{
    return "Hello";
}

const char *square(void)
{
    return "1";
}

const char *cube(void)
{
    return "1";
}

const char *helloworld(void)
{
    return "Hello";
}

const char *pingpong(void)
{
    return "Pong";
}
