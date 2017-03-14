/* tcpclient.c */

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>


int main(int argc, char *argv[])

{

        int sock, bytes_recieved;
        char send_data[1024],recv_data[1024];
        char *hostInput;
        int clientPort;
        int ipVal = 0;
        unsigned long ip;
        struct hostent *host;
        struct sockaddr_in server_addr;
        char *quit = "QUITTING ON USER REQUEST\n"; //check that correct number of arguments are given

        if(argc != 3){
         fprintf(stderr,"Incorrect number of arguments\nUsage: %s [host] [client]\n", argv[0]);
         return -1;
        }
        hostInput = argv[1];
        clientPort = atoi(argv[2]);

        ipVal = inet_pton(AF_INET, hostInput,& ip);//host ip validation
        while(ipVal != 1){
            printf("%s is an invalid IPv4 address.\nTry again.\n",hostInput);
            scanf("%s", hostInput);
            ipVal = inet_pton(AF_INET, hostInput,& ip);//host ip validation
        }
        if(clientPort > 65535 || clientPort < 1024){
        while(clientPort > 65535 || clientPort < 1024)
        {
           printf("Client port exceeds 16-bit range or is lower than 1024\nTry again\n");
           scanf("%d", &clientPort);
        }
        }

        //host = gethostbyname("127.0.0.1");
        host = (struct hostent *) gethostbyname((char *)hostInput);
        if(host == NULL){
            printf("Host %s does not exist\n", hostInput);
            return 0;
        }

        if ((sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
            perror("Socket");
            exit(1);
        }

        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(clientPort);
        server_addr.sin_addr = *((struct in_addr *)host->h_addr);
        bzero(&(server_addr.sin_zero),8);
        bcopy((char *)host->h_addr, (char *)&server_addr.sin_addr.s_addr, host->h_length);

        if (connect(sock, (struct sockaddr *)&server_addr,
                    sizeof(struct sockaddr)) == -1)
        {
            perror("Connect");
            exit(1);
        }

        while(1){
         bytes_recieved=recv(sock,recv_data,1024,0);
         recv_data[bytes_recieved] = '\0';
        //if the client receives a "q" or "Q" close
        if (strcmp(recv_data , "QUITTING ON USER REQUEST\n\n") == 0 ||
        strcmp(recv_data , "QUITTING ON USER REQUEST\n") == 0)
                {
                        printf("QUITTING ON SERVER REQUEST\n");
                        close(sock);
                        break;
                }
        //Prints out received data, asks to send data
        else
        printf("Recieved data = %s " , recv_data);

        printf("SEND (q or Q to quit) : ");
        fgets(send_data, sizeof(send_data), stdin);

        if (strcmp(send_data , "q\n") != 0 && strcmp(send_data , "Q\n") != 0)
                send(sock,send_data,strlen(send_data), 0);
        else
           {
                send(sock,quit,strlen(quit), 0);
                close(sock);
                break;
           }
        }
return 0;
}
