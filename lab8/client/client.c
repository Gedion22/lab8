#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <progbase.h>
#include <string.h>
#include <ctype.h>
#include <jansson.h>
#include <progbase/net.h>
#include <progbase/console.h>







struct List{
    char word[25];
    struct List *next;
};

typedef struct List List;

List * listNew(char n[25]);
List * listAdd(List * pl, List * node);
void WriteList(List *pl);
void freeList(List *pl);


#define BUFFER_LEN 10000

int main(int argc, char * argv[]) {
	if (argc < 2) {
		puts("Please, specify server port in command line arguments");
		return 1;
	}
	const int port = atoi(argv[1]);
	TcpClient * client = TcpClient_init(&(TcpClient){});
    const char * serverHostname = "progbase.herokuapp.com";
    char ipv4[20] = "127.0.0.1";
    // if (!Ip_resolveHostname(ipv4, serverHostname)) {
    //     perror("resolve address");
    //     return 1;
    // }
    IpAddress * serverAddress = IpAddress_init(
        &(IpAddress){}, 
        ipv4, 
        port);  // default port for web servers
    NetMessage * message = NetMessage_init(
        &(NetMessage){},  // value on stack
        (char[BUFFER_LEN]){},  // array on stack 
        BUFFER_LEN); 
    
    if (!TcpClient_connect(client, serverAddress)) {
        perror("tcp connect");
        return 1;
    }
    //
    // setup message object
    char httpWebRequest[100] = " ";
    char str[30] = " ";
    fgets(str, 30, stdin);
    int i = 0;
    for(i = 0; i < 30; i++){
        if(str[i] == '\n'){
            str[i] = '\0';
            break;
        } 
    }
    sprintf(httpWebRequest, 
        "GET %s HTTP/1.1\r\n"
        "Host: %s\r\n"
        "\r\n",str, serverHostname);
    NetMessage_setDataString(message, httpWebRequest);
    //
    // send string to server
    printf(">> Send message to server %s:%d (%s):\r\n%s\r\n",
        IpAddress_address(serverAddress),
        IpAddress_port(serverAddress),
        serverHostname,
        message->buffer);
    if(!TcpClient_send(client, message)) {
		perror("send");
		return 1;
	}
    //
    // receive response from server
    // use loop to receive big data buffers
    // the end of message if determined by 0 data length
    while (1) {
        if(!TcpClient_receive(client, message)) {
            perror("recv");
            return 1;
        }
        if (NetMessage_dataLength(message) == 0) {
            // no more data to receive from server
            break;
        }
        printf(">> Response  received from server (%d bytes):\r\n%s\r\n", 
            message->dataLength,
            message->buffer);
        
    }
    TcpClient_close(client);
   
	return 0;
}

List * listNew(char n[25]){
    List *node = (List *)malloc(sizeof(List));
    strcpy(node->word, n);
    node->next = NULL;
    return node;
}
List * listAdd(List * pl, List * node){
    if(pl->next == NULL){
        pl->next = node;
        return pl;
    } else{
        List * cur = pl;
        while(cur->next != NULL){
            cur = cur->next;
        }
        cur->next = node;
        return pl;
    }
}
void WriteList(List *pl){
    while(pl != NULL){
        printf("%s ", pl->word);
        //puts(pl->word);
        pl = pl->next;
    }
}
void freeList(List *pl){
     while(pl != NULL){
        List * node = pl;
        pl = pl->next;
        free(*(&node));
        *(&node) = NULL;
    }
}
