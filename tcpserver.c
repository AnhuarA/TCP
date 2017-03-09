/* tcpserver.c */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>


int main(int argc, char *argv[])
{
        int sock, connected, bytes_recieved , true = 1;
        char send_data [1024] , recv_data[1024];
        int serverPort;
        struct sockaddr_in server_addr,client_addr;
        int sin_size;
        char quit[20]; //check that correct number of arguments are given

        if(argc != 2){
            printf("Incorrect number of arguments\nUsage: udpserver [port]\n");
        }
        //Get server port from argument
        serverPort = atoi(argv[1]);
        //Check for valid port number
        if(serverPort > 65535 || serverPort < 1024){
           while(serverPort > 65535 || serverPort < 1024)
           {
              printf("Client port exceeds 16-bit range or is lower than 1024\nTry again\n");
              scanf("%d", &serverPort);
           }
        }
        if ((sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
            perror("Socket");
            exit(1);
        }

        if (setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&true,sizeof(int)) == -1) {
            perror("Setsockopt");
            exit(1);
        }

        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(0);
        server_addr.sin_addr.s_addr = INADDR_ANY;
        bzero(&(server_addr.sin_zero),8);

        if (bind(sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr))
                                                                       == -1) {
            perror("Unable to bind");
            exit(1);
        }

        if (listen(sock, 5) == -1) {
            perror("Listen");
            exit(1);
        }

	printf("TCPServer Waiting for client on port %d\n", serverPort);
        fflush(stdout);


        while(1)
        {

            sin_size = sizeof(struct sockaddr_in);

            connected = accept(sock, (struct sockaddr *)&client_addr,&sin_size);

            printf("I got a connection from (%s , %d)",
                   inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));

            while (1)
            {
              printf(" SEND (q or Q to quit) : ");
			  fgets(send_data, sizeof(send_data), stdin);

              if (strcmp(send_data , "q") == 0 || strcmp(send_data , "Q") == 0)
              {
                sprintf(quit, "QUITTING ON USER REQUEST\n");
                printf("QUITTING ON USER REQUEST\n");
                send(connected, quit,strlen(quit), 0);
                close(connected);
                break;
              }

              else
                 send(connected, send_data,strlen(send_data), 0);

              bytes_recieved = recv(connected,recv_data,1024,0);

              recv_data[bytes_recieved] = '\0';

              if (strcmp(recv_data , "q") == 0 || strcmp(recv_data , "Q") == 0)
              {
                close(connected);
                break;
              }

              else
              printf("RECEIVED DATA = %s " , recv_data);
              fflush(stdout);
            }
        }

      close(sock);
      return 0;
}
