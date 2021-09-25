#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include <iostream>
#include <sys/epoll.h>
#include "message.pb.h"

using namespace std;

#define MAX_BUF_LEN 1024

extern int regMatch(const char *,int,const char *,int);

int main(int argc ,char* argv[])
{
	if(argc < 3){
		printf("./server [addr:] [port:]\n");
		return 4;
	}

	//create socket
	int sock = socket(AF_INET,SOCK_DGRAM,0);//SOCK_DGRAM代表UDP
	if(sock < 0){
		perror("socket");
		return 1;
	}
 
	struct sockaddr_in local;
	local.sin_family = AF_INET;
	local.sin_port = htons(atoi(argv[2]));
	local.sin_addr.s_addr = inet_addr(argv[1]);

	//bind port
	if(bind(sock,(struct sockaddr*)&local,sizeof(local))<0)
	{
		perror("bind");
		return 2;
	}
 
	char buf[1024];
	struct sockaddr_in client;
	socklen_t len = sizeof(client);
	while(1){
		//receive msg
		requestMsg request;
		replyMsg reply;
		ssize_t s = recvfrom(sock,buf,sizeof(buf)-1,0,(struct sockaddr*)&client,&len);
		if(s<0)
		{
			perror("recvfrom");
			return 3;
		}
		else if(s == 0)
		{
			printf("client closed\n");
		}
		else
		{
			request.ParseFromArray(buf,sizeof(buf)-1);
			printf("client:[%s : %d]:%s\n",inet_ntoa(client.sin_addr),ntohs(client.sin_port),buf);
			cout<<"[server received]: regular expression:"<<request.regstr()<<endl;
			cout<<"[server received]: match string:"<<request.targetstr()<<endl;
			int match = regMatch(request.regstr().c_str(),request.regstr().size(),\
				request.targetstr().c_str(),request.targetstr().size());
			reply.set_result(match);
			reply.SerializeToArray(buf,1024);
			sendto(sock,buf,strlen(buf),0,(struct sockaddr*)&client,strlen(buf));
			if(strcmp(buf,"quit") == 0)
				break;
		}
 
	}
 
	return 0;
}

