#include <stdio.h>
#include <winsock2.h>
#include "cache.h"
#include "file.c"
#include "cache.c"
#include "mime.c"
#include <time.h>


#define ROOT "root"
#define PORT 8000
#define BUFFER_SIZE 1024

void send_response(SOCKET fd, char *header, char *path , char *content_type, int content_length, void *body ){
    int max_response_size = 262144;
    char response[max_response_size];
    
    time_t now = time(NULL);
    struct tm *gmt = gmtime(&now);
    char date[128];
    strftime(date,sizeof(date),"%a, %d %b %Y %H:%M:%S GMT",gmt);
    
    int header_len = snprintf(response, sizeof(response),
    "%s\r\n"
    "Server: krono\r\n"
    "Date: %s\r\n"
    "Connection: close\r\n"
    "Content-Length: %d\r\n"
    "Content-Type: %s\r\n\r\n"
    , header, date, content_length, content_type);

    int rv = send(fd,response,header_len,0);
    rv+= send(fd,body,content_length,0);

    if(rv==-1){
        printf("Write Failed: %d\n",WSAGetLastError());
        return;
    }
}

void get_file(SOCKET fd,char *filename,int top , int max_size,struct cache_entry *cache[max_size]){
    struct cache_entry *data = find_cache(top,max_size,cache,filename);
    if(data == NULL){
        char filepath[1024];
        struct file_ *filedata;
        char *mime_type;
        
        snprintf(filepath, 1024, "%s/%s",ROOT,filename);

        filedata = load_file(filepath);
        if(filedata == NULL){
            printf("file couldnt be loaded\n");
        }else{
            mime_type = get_mime_type(filepath);
            send_response(fd,"HTTP/1.1 200 OK", filename, mime_type, filedata->file_size, filedata->file_data);
            insert_entry(top, max_size,cache, filename, mime_type, filedata->file_data, filedata->file_size);
            file_free(filedata);
        }
        
    }
}

void handle_http_response(struct cache_entry *cache[],SOCKET fd,int top , int max_size){
    const int max_request_buffer_size = 65536;
    char request[max_request_buffer_size];

    int recieved_bytes = recv(fd,request,max_request_buffer_size-1,0);
    if( recieved_bytes == -1){
        printf("Read Failed: %d\n",WSAGetLastError());
        return;
    }
    
    char method[128],uri[128],version[128];
    sscanf(request,"%s %s %s",method,uri,version);

    if(strcmp(method,"GET")==0){
        if(strcmp(uri,"/")==0){
            get_file(fd,"home.html",top,max_size,cache);
        }else{
        get_file(fd,uri,top,max_size,cache);
        }
    }else{
        printf("POST requests are not handled yet!!!");
    }
    
}

int main(){
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) !=0 ){
        printf("Failed to inialize the Winsock\n");
    }

    SOCKET socketfd = socket(AF_INET,SOCK_STREAM,0);

    char readbuffer[BUFFER_SIZE];

    if(socketfd==-1){
        printf("Socket failed : %d\n",WSAGetLastError());
        return 1;
    }
    
    printf("Socket successfully created.\n");

    struct sockaddr_in host_addr;
    int host_addr_len = sizeof(host_addr);

    host_addr.sin_family = AF_INET;
    host_addr.sin_port = htons(PORT);
    host_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    struct sockaddr_in client_addr;
    int client_addr_len = sizeof(client_addr);

    if(bind(socketfd,(struct sockaddr*)&host_addr,host_addr_len)!=0){
        printf("Binding Failed : %d\n",WSAGetLastError());
        return 1;
    }

    printf("Binding of the socket done successfully.\n");

    if(listen(socketfd,SOMAXCONN)!=0){
        printf("Listen Failed: %d\n",WSAGetLastError());
        return 1;
    }

    printf("Now the passive server socket listens.\n");


    int top = -1;
    int max_size = 10;
    struct cache_entry *cache[max_size];

    while(1){
        SOCKET newsocketfd = accept(socketfd,(struct sockaddr*)&client_addr,(int *)&client_addr_len);

        if(newsocketfd==-1){
            printf("Accept Failed: %d\n",WSAGetLastError());
            continue;
        }

        handle_http_response(cache, newsocketfd,top,max_size);

        shutdown(newsocketfd,SD_SEND); // graceful shutdown

        // closesocket(newsocketfd);
    }

}
