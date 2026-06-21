#include "cache.h"
#include <string.h>
#include <stdlib.h>

struct cache_entry *create_cache_entry(char *path, char *content_type, void *body, int content_length){
    struct cache_entry *entry = malloc(sizeof(struct cache_entry));
    entry->path = path;
    entry->content_length = content_length;
    entry->content_type = content_type;
    entry->body = body;
    return entry;
}

struct cache_entry *move_to_head(int top,int n,int max_size, struct cache_entry *cache[max_size]){
    if(top!=-1 && top!=n){
        struct cache_entry *target_entry = cache[n];
        for(int i = n+1 ; i <= top ; i++){
            cache[i-1] = cache[i];
        }
        cache[top] = target_entry;
        return target_entry;
    }
    return NULL;
}

struct cache_entry *insert_entry(int top, int max_size, struct cache_entry *cache[max_size], char *path, char *content_type, void *body, int content_length) {
    struct cache_entry *entry = create_cache_entry(path,content_type,body,content_length);
  if (top == max_size) {
      for(int i = 1 ; i < max_size ; i++){
          cache[i-1] = cache[i];
      }
      cache[top] = entry;
  }else{
      top++;
      cache[top] = entry;
  }

  return *cache;
}

struct cache_entry *find_cache(int top,int max_size, struct cache_entry *cache[max_size],char *request_path){
    if(top==-1){
        return NULL;
    }
    if(strcmp(request_path,cache[top]->path)==0){
        return cache[top];
    }else{
        int high = top-1;
        int low = 0;
        while(low<=high){
            if(strcmp(request_path,cache[high]->path)==0){
                move_to_head(top,high,max_size,cache);
            }else if (strcmp(request_path,cache[low]->path)==0){
                move_to_head(top,low,max_size,cache);
            }else{
                high--;
                low++;
            }
        }
        if(high==low){
            return NULL;
        }
    }
    return NULL;
}