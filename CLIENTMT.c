#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

void* clientthread(void* args)
{
    int client_request=*((int*)args);
    int network_socket;

    network_socket=socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in server_address;
    server_address.sin_family=AF_INET;
    server_address.sin_addr.s_addr=INADDR_ANY;
    server_address.sin_port=htons(8989);
    int connection_status=connect(network_socket,(struct sockaddr*)&server_address,sizeof(server_address));
    if(connection_status <0)
    {
        puts("error\n");
        return 0;
    }
    printf("connection established\n");
    send(network_socket,&client_request,sizeof(client_request),0);
    close(network_socket);
    pthread_exit(NULL);

    return 0;
}
int main()
{
    printf("1. Read\n");
    printf("2. Write\n");

    int choice;
    scanf("%d",&choice);
    pthread_t tid;

    switch(choice)
    {
        case 1:
        {
            int client_request=1;

            sleep(20);
            break;
        }
        case 2:
        {
            int client_request=2;
            pthread_create(&tid,NULL,clientthread,&client_request);
            sleep(20);
            break;

        }
        default:
            printf("invalid Input\n");
            break;
    }
    pthread_join(tid,NULL);
}
