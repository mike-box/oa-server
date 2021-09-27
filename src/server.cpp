#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include <iostream>
#include <sys/epoll.h>
#include "regmatch.h"
#include "SocketUDP.h"
#include "message.pb.h"

using namespace std;

#define MAX_BUF_LEN 1024

int main(int argc ,char* argv[])
{
	if(argc != 3){
		printf("./server [addr:] [port:]\n");
		return 4;
	}
	
    char buf[MAX_BUF_LEN];
	requestMsg request;
	replyMsg reply;
	struct sockaddr_in client;
	CSocketUDP server;

	//bind port
	server.bind(atoi(argv[2]));
 	memset(buf,0,sizeof(buf));	
	
	while(true) {
		//receive msg
		ssize_t len = server.recv(buf,sizeof(buf)-1,&client);
		if(len <= 0){
			printf("client closed\n");
		}else{
			request.ParseFromArray(buf,sizeof(buf)-1);
			printf("client:[%s : %d]:%s\n",inet_ntoa(client.sin_addr),ntohs(client.sin_port),buf);
			cout<<"[server received]: regular expression:"<<request.regstr()<<endl;
			cout<<"[server received]: match string:"<<request.targetstr()<<endl;
			int match = regMatch(request.regstr(),request.targetstr());
			reply.set_result(match);
			reply.SerializeToArray(buf,MAX_BUF_LEN);
			server.send(client.sin_addr.s_addr,client.sin_port,buf,strlen(buf));
		}
 
	}
 
	return 0;
}

