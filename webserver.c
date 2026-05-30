#include <stdio.h>
#include <winsock2.h>

#define PORT 8000

int main(){
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) !=0 ){
        printf("Failed to inialize the Winsock\n");
    }

    SOCKET socketfd = socket(AF_INET,SOCK_STREAM,0);

    if(socketfd==-1){
        printf("Socket failed : %d\n",WSAGetLastError());
    }
    
    printf("Socket successfully created.\n");

    struct sockaddr_in host_addr;
    int host_addr_len = sizeof(host_addr);

    host_addr.sin_family = AF_INET;
    host_addr.sin_port = htons(PORT);
    host_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(socketfd,(struct sockaddr*)&host_addr,host_addr_len)!=0){
        printf("Socket Binding Error : %s\n",WSAGetLastError());
        return 1;
    }

    printf("Binding of the socket done successfully.");

    // closesocket(socketfd);
}
