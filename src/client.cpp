#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string>
#include <iostream>
#include "message.pb.h"

using namespace std;


int main(int argc ,char* argv[])
{
	if(argc < 3)
	{
		printf("./client [addr:][port:]\n");
		return 4;
	}
	int sock = socket(AF_INET,SOCK_DGRAM,0);
	if(sock < 0)
	{
		perror("socket");
		return 1;
	}
 
	struct sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_port = htons(atoi(argv[2]));
	server.sin_addr.s_addr = inet_addr(argv[1]);
	
	char buf[1024];
	struct sockaddr_in peer;
	socklen_t len = sizeof(peer);
	while(1){
		printf("#please enter#  ");
		fflush(stdout);
		
		ssize_t s = read(0,buf,sizeof(buf)-1);
		if(s < 0)
		{
			perror("read");
			return 2;
		}
		buf[s-1] = 0;
		requestMsg req;
		replyMsg reply;
		string tt;
		req.set_regstr("[0-9]");
		req.set_targetstr("000001121299mmsandasdansdm");
		req.SerializeToString(&tt);
		cout<<"[regular expression]:"<<req.regstr()<<endl;
		cout<<"[matched expression]:"<<req.targetstr()<<endl;
		memset(buf,0,sizeof(buf));
		memcpy(buf,tt.c_str(),tt.size());
		sendto(sock,buf,sizeof(buf)-1,0,(struct sockaddr*)&server,sizeof(buf));
		s = recvfrom(sock,buf,sizeof(buf)-1,0,(struct sockaddr*)&peer,&len);
		if(s<0)
		{
			perror("recvfrom");
			return 3;
		}
		else if(s == 0)
		{
			printf("server closed\n");
		}
		else
		{
			reply.ParseFromArray(buf,len);
			cout<<"[match times]:"<<reply.result()<<endl;
			buf[s] = 0;
			//printf("#server echo#  %s\n",buf);
			if(strcmp(buf,"quit") == 0)
				break;
		}
	}
 
	return 0;
}

