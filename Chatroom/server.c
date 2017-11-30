#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <errno.h>

#define MSG_SIZE 512
#define MAX_CLIENT 5
#define MYPORT 8000

int main(int argc, char const *argv[]){
    //variables declaration
	int server_socket, client_socket;
	struct sockaddr_in server_address, client_address;
	int server_addr_len, client_addr_len;
	char server_message[MSG_SIZE], client_message[MSG_SIZE];
    int error_code;

	//creating socket
	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if(server_socket < 0){
		perror("Can't create socket\n");
		return 1;
	}

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(MYPORT);
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");//INADDR_ANY;
    //inet_pton(AF_INET, "127.0.0.1", &(server_address.sin_addr));//INADDR_ANY;

    //binding socket to specific ip and port
    server_addr_len = sizeof(server_address);
    if(bind(server_socket, (struct sockaddr *)&server_address, server_addr_len) < 0){
    	perror("Bind failed\n");
    	return 1;
    }

    // Listening
    if(listen(server_socket, MAX_CLIENT) < 0){
        printf("Can't listen\n");
        return 1;
    }
    printf("Waiting for client connection.\n");

    //client_addr_len = sizeof(client_address);
    // Accept connection
    //client_socket = accept(server_socket, (struct sockaddr *)&client_address, &client_addr_len);
    client_socket = accept(server_socket, NULL, NULL);
    if(client_socket < 0){
        perror("Client connection failed\n");
        return 1;
    }

    printf("Client Connected.\n");
    printf("Write \"bye\" to close connection.\n");
    do{
        //sending message to client
        printf("Server : ");
    	gets(server_message);
    	if(send(client_socket, server_message, MSG_SIZE, 0) < 0){
    		perror("Message send failed\n");
            return 1;
    	}
        if(strcmp(server_message,"bye")==0){
            break;
        }

        //receiving message from client
        error_code = recv(client_socket, client_message, MSG_SIZE, 0);
        if (error_code == 0){
            printf("Disconnected\n");
        }
        else if(error_code < 0){
    		perror("message received failed\n");
            printf("%d\n", errno );
            return 1;
    	}
    	printf("Client : ");
    	puts(client_message);
    } while(strcmp(client_message,"bye")!=0);

    //closing server socket
    close(server_socket);
	return 0;
}
