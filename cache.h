#ifndef _CACHE_H_
#define _CACHE_H_

struct cache_entry{
    char *path;
    char *content_type;
    void *body;
    int content_length;
};

// struct cache{
//     int top;
//     int bottom;
//     cache_entry cache[]; // flexible arrays must be at the end of the struct;
// };

#endif