#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string>
#include <iostream>
#include "SocketUDP.h"
#include "message.pb.h"

using namespace std;

#define MAX_BUF_LEN 1024

int main(int argc ,char* argv[])
{
	if(argc != 3){
		printf("./client [addr:][port:]\n");
		return 4;
	}

	char buf[MAX_BUF_LEN];
	requestMsg request;
	replyMsg reply;
	CSocketUDP client;
	struct sockaddr_in peer;

	while(true){
		// get input from keyboard
		string str;		
		cout<<"#please enter regular expression#:";
		cin>>str;
		request.set_regstr(str);
		cout<<"#please enter matched string#:";
		cin>>str;
		request.set_targetstr(str);
		request.SerializeToArray(buf,MAX_BUF_LEN);
		
		//cout<<"[client send] regular expression: "<<req.regstr()<<endl;
		//cout<<"[client send] matched string: "<<req.targetstr()<<endl;
		client.send(argv[1],atoi(argv[2]),buf,MAX_BUF_LEN);
		int len = client.recv(buf,MAX_BUF_LEN,&peer);
		if( len < 0){
			perror("recvfrom");
			return 3;
		}else if(len == 0){
			printf("server closed\n");
		}else{
			reply.ParseFromArray(buf,len);
			cout<<"[server reply] match times:"<<reply.result()<<endl;
			memset(buf,0,sizeof(buf));
		}
	}
 
	return 0;
}

