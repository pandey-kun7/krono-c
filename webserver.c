#include <stdio.h>
#include <winsock2.h>

#define PORT 8000
#define BUFFER_SIZE 1024

int main(){
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) !=0 ){
        printf("Failed to inialize the Winsock\n");
    }

    SOCKET socketfd = socket(AF_INET,SOCK_STREAM,0);

    char readbuffer[BUFFER_SIZE];

    char standard_resp[] = "HTTP/1.1 200 OK\r\n"
                  "Server: krono\r\n"
                  "Content-Type: text/html; charset=UTF-8\r\n\r\n"
                  "<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"><title>Document</title><style>body{display:flex;justify-content:center;align-items:center }</style></head><body><p>Hello Bro, I am the krono server 😇</p></body></html>\r\n";

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
        printf("Binding Failed : %s\n",WSAGetLastError());
        return 1;
    }

    printf("Binding of the socket done successfully.\n");

    if(listen(socketfd,SOMAXCONN)!=0){
        printf("Listen Failed: %s\n",WSAGetLastError());
        return 1;
    }

    printf("Now the passive server socket listens.\n");

    while(1){
        SOCKET newsocketfd = accept(socketfd,(struct sockaddr*)&host_addr,(int *)&host_addr_len);

        if(newsocketfd==-1){
            printf("Accept Failed: %s\n",WSAGetLastError());
            continue;
        }

        printf("Connection accepted.\n");

        SOCKET sockn = getsockname(newsocketfd,(struct sockaddr *)&client_addr,(int *)&client_addr_len);
        if(sockn==-1){
            printf("Getting Socket Failed: %s\n",WSAGetLastError());
            continue;
        }

        int readval_ = recv(newsocketfd,readbuffer,BUFFER_SIZE,0);
        if(readval_==-1){
            printf("Read Failed: %s\n",WSAGetLastError());
            continue;
        }

        printf("Incoming requests are read.\n");

        char method[BUFFER_SIZE],version[BUFFER_SIZE],uri[BUFFER_SIZE];
        sscanf(readbuffer,"%s %s %s",method,uri,version);

        printf("[%s:%u] %s %s %s\n",inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port),method,uri,version);
        
        // printf("%s",readbuffer); this shows the entire mf req

        int writeval_ = send(newsocketfd,standard_resp,strlen(standard_resp),0);
        if(writeval_ == -1){
            printf("Write Failed: %d\n",WSAGetLastError());
            continue;
        }

        printf("Response sent.\n");

        shutdown(newsocketfd,SD_SEND); // graceful shutdown

        // closesocket(newsocketfd);
    }

}
