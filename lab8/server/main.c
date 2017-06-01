#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <progbase/net.h>

#include <request.h>

#define BUFFER_LEN 10240

struct Memes{
    char name[10];
    int year;
    int id;
};




int main(int argc, char * argv[]) {
	if (argc < 2) {
		puts("Please, specify server port in command line arguments");
		return 1;
	}
	srand(time(0));
	const int port = atoi(argv[1]);

   



	TcpListener * server = TcpListener_init(&(TcpListener){});
    IpAddress * address = IpAddress_initAny(&(IpAddress){}, port);
    if(!TcpListener_bind(server, address)) {
        perror("tcp bind");
        return 1;
    }
    if (!TcpListener_start(server)) {
        perror("tcp server start");
        return 1;
    }
    printf("TCP Server is listening on port %d\n", 
        IpAddress_port(TcpListener_address(server)));
    
    NetMessage * message = NetMessage_init(
        &(NetMessage){},  // value on stack
        (char[BUFFER_LEN]){},  // array on stack 
        BUFFER_LEN);
	//
    // to store information about current client
    TcpClient client;
    while (1) {
        puts(">> Waiting for connection...");
		//
        // wait for someone to connect to server
        TcpListener_accept(server, &client);
        // wait for data from client
        if(!TcpClient_receive(&client, message)) {
			perror("recv");
			return 1;
		}
        IpAddress * clientAddress = TcpClient_address(&client);
        printf(">> Received message from %s:%d (%d bytes): `%s`\n",
            IpAddress_address(clientAddress),  // client IP-address
            IpAddress_port(clientAddress),  // client port
            NetMessage_dataLength(message),
            NetMessage_data(message));
        
        

		// Request req = parseRequest(NetMessage_data(message));
        // Response res;
        // Response_init(&res);
		// processRequest(&req, &res, handlers, sizeof(handlers) / sizeof(handlers[0]));
        // Response_toMessage(&res, message);
		// Response_cleanup(&res);
        // send data back

       char value[15] = " ";
       int n = 0;
       int i = 20;
        if(strncmp(NetMessage_data(message), "GET / ", 6) == 0){
           req1(message); 
        }
        else if(strncmp(NetMessage_data(message), "GET /favorites", 14) == 0){
            if(strncmp(NetMessage_data(message)+14, " ", 1) == 0){
                reqFavorites(message);
            }
            else if(strncmp(NetMessage_data(message)+14, "?", 1) == 0){
                if(strncmp(NetMessage_data(message)+15, "Name=", 5) == 0){
                    n = 0;
                    while(isalpha(NetMessage_data(message)[i])){
                      n++;
                      i++;   
                    } 
                    strncpy(value, NetMessage_data(message)+20, n);
                    reqFavoritesKey(0, value, message);
                }
                else if(strncmp(NetMessage_data(message)+15, "Year=", 5) == 0){
                    n = 0;
                    while(isdigit(NetMessage_data(message)[i])){
                      n++;
                      i++;   
                    }
                    strncpy(value, NetMessage_data(message)+20, n);
                    reqFavoritesKey(1, value, message);
                }
                else reqError(message);
            }
            else if(strncmp(NetMessage_data(message)+14, "/", 1) == 0){
                    strncpy(value, NetMessage_data(message)+15, 2);
               reqFavoritesKey(2, value, message);
            }
            else reqError(message);
        }
        else if(strncmp(NetMessage_data(message), "GET /file", 9) == 0){
            if(strncmp(NetMessage_data(message)+9, " ", 1) == 0){
                reqFile(message);
            }
            else if(strncmp(NetMessage_data(message)+9, "/data", 5) == 0){
                reqFileData(message);
            }
            else reqError(message);
        }
        else{
	        reqError(message);
        }
         if(!TcpClient_send(&client, message)) {
			perror("send");
			return 1;
		}
        TcpClient_close(&client);
    }
    // close listener
    TcpListener_close(server);
	return 0;
}


