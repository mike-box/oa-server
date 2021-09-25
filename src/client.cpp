#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string>
#include <iostream>
#include "message.pb.h"

using namespace std;

#define MAX_BUF_LEN 1024

int main(int argc ,char* argv[])
{
	if(argc < 3){
		printf("./client [addr:][port:]\n");
		return 4;
	}
	int sock = socket(AF_INET,SOCK_DGRAM,0);
	if(sock < 0){
		perror("socket");
		return 1;
	}
 
	struct sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_port = htons(atoi(argv[2]));
	server.sin_addr.s_addr = inet_addr(argv[1]);
	
	char buf[MAX_BUF_LEN];
	struct sockaddr_in peer;
	socklen_t slen = sizeof(peer);
	while(1){
		requestMsg req;
		replyMsg reply;
		string str;
		
		cout<<"#please enter regular expression#:";
		cin>>str;
		req.set_regstr(str);
		cout<<"#please enter matched string#:";
		cin>>str;
		req.set_targetstr(str);
		
		//fflush(stdout);
		req.SerializeToArray(buf,MAX_BUF_LEN);
		cout<<"[client send] regular expression: "<<req.regstr()<<endl;
		cout<<"[client send] matched string: "<<req.targetstr()<<endl;
		sendto(sock,buf,sizeof(buf)-1,0,(struct sockaddr*)&server,sizeof(server));
		int len = recvfrom(sock,buf,sizeof(buf)-1,0,(struct sockaddr*)&peer,&slen);
		if( len < 0){
			perror("recvfrom");
			return 3;
		}else if(len == 0){
			printf("server closed\n");
		}else{
			reply.ParseFromArray(buf,len);
			cout<<"[server reply] match times:"<<reply.result()<<endl;
			buf[len] = 0;
			if(strcmp(buf,"quit") == 0)
				break;
		}
	}
 
	return 0;
}

