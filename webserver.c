#include <stdio.h>
#include <winsock2.h>

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

    closesocket(socketfd);
}
