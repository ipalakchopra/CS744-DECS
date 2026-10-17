#ifndef FUNCTIONS_H
#define FUNCTIONS_H

struct url_mapping {
    const char *url;
    int id;
};

#define NUM_FUNCTIONS 5

static const struct url_mapping url_map[] = {
    {"/",           0},
    {"/square",     1},
    {"/cube",       2},
    {"/helloworld", 3},
    {"/pingpong",   4}
};

int get_url_id(const char *url);

#endif
