// SOCKET PROGRAMMING

/*

1. initiliaze winsock library
2. Create the socket
3. get ip and port
4. bind with the socket (ip and port)
5. listen on the socket
6. accept
7. receave and send
8. close and clean

*/

#include "socketUtil.h"

int main ()
{
    printf("test");
    const char* ip = "142.250.31.100";
    int socketFD = socketTCP();
   
    struct sockaddr_in* address = enderecoIP(ip, 80);
    
    
    int result = connect(socketFD, (struct sockaddr*) &address, sizeof(address));
    
    if(result == 0)
    {
        printf("Connection was successful\n");
    }
    

    char* message;
    message = "GET \\ HTTP/1.1\r\nHost:google.com\r\n\r\n";
    send(socketFD, message, strlen(message), 0);

    char buffer[1024];
    recv(socketFD, buffer, 1024, 0);

    printf("Response was %s\n ", buffer);

    return 0;
}
