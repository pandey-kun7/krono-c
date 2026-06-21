#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "file.h"

struct file_ *load_file(char *filename){
    char *buffer,*p;
    struct stat buf;
    int bytes_read,bytes_left,total_bytes = 0;

    if(stat(filename,&buf) == -1){
        return NULL;
    }
    if(!(buf.st_mode & S_IFREG)){
        return NULL;
    }

    FILE *fp = fopen(filename, "rb");
    if(fp==NULL){
        return NULL;
    }

    bytes_left = buf.st_size;
    p = buffer = malloc(bytes_left);

    if(buffer==NULL){
        return NULL;
    }

    while(bytes_read = fread(p,1,bytes_left,fp),bytes_read !=0 && bytes_left>0){
        if(bytes_read==-1){
            free(buffer);
            return NULL;
        }
        bytes_left -= bytes_read;
        p += bytes_read;
        total_bytes += bytes_read;
    }

    struct file_ *filedata = malloc(sizeof(struct file_));

    if(filedata == NULL){
        free(buffer);
        return NULL;
    }
    filedata->file_data = buffer;
    filedata->file_size = total_bytes;

    return filedata;
} 

void file_free(struct file_ *filedata)
{
    free(filedata->file_data);
    free(filedata);
}