#include<arpa/inet.h>
#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<unistd.h>
#define PORT 8080
int main(int argc,char const* argv[])
{
    int sock=0,valread,client_fd;
    struct sockaddr_in serv_addr;
    char* hello="hello I got your message";
    char buffer[1024]={0};
    if((sock=socket(AF_INET,SOCK_STREAM,0))<0)
    {
        printf("\n socket creation error \n");
        return -1;

    }
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_port=htons(PORT);
    if(inet_pton(AF_INET,"127.0.0.1",&serv_addr.sin_addr)<=0)
    {
        printf("\nInvalid address/Adress not supported \n");
        return -1;

    }
    if((client_fd=connect(sock,(struck sockaddr*)&serv_addr,sizeof(serv_addr)))<0)
    {
        printf("\nConnection Failed \n");
        return -1;

    }
    send(sock,hello,strlen(hello),0);
    printf("Hello I got your message\n");
    valread=read(sock,buffer,1024);
    printf("%s\n",buffer);
    close(client_fd);
    return 0;
}