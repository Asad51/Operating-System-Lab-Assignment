#include <stdio.h>
#include <unistd.h>
#include <netinet/in.h>	
#include <sys/socket.h>
#include <string.h>
#include <errno.h>

#define MSG_SIZE 512
#define MYPORT 8000

int main(int argc, char const *argv[]){
	//variable declaration
    int client_socket;
    int server_addr_len, error_code;
    struct sockaddr_in server_address;
    struct sockaddr *sockaddr_ptr;
    char server_message[MSG_SIZE], client_message[MSG_SIZE];

    //  Creating a socket.
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(client_socket < 0){
    	perror("Can't create client socket.\n");
    	return 1;
    }

    server_address.sin_family = AF_INET; 
    server_address.sin_port = htons(MYPORT); 
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    //inet_aton(AF_INET, "127.0.0.1", &(server_address.sin_addr));//INADDR_ANY;

    //connecting to server
    server_addr_len = sizeof(server_address);
    sockaddr_ptr = (struct sockaddr *)&server_address;
    if(connect(client_socket, sockaddr_ptr, server_addr_len)<0){
        printf("Can't connect to server.\n");
        return 1;
    }

    printf("Server Connected.\n");
    printf("Write \"bye\" to close connection.\n");
    do{
    	//receiving message from server
    	error_code = recv(client_socket, server_message, MSG_SIZE, 0);
        if (error_code == 0){
            printf("Server disconnected.\n");
        }
        else if(error_code < 0){
    		perror("message received failed\n");
            printf("%d\n", errno );
            return 1;
    	}
    	printf("Server : ");
    	puts(server_message);
    	if(strcmp(server_message,"bye")==0){
    		break;
    	}

    	//sending message to server
    	printf("Client : ");
    	gets(client_message);
    	if(send(client_socket, client_message, MSG_SIZE, 0) < 0){
    		perror("Can't send message to server.\n");
    		return 1;
    	}
    } while(strcmp(client_message,"bye")!=0);

    //closing client socket
    close(client_socket);
	return 0;
}