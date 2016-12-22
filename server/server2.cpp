#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <unistd.h>
#include <string.h>


#define BUFF_SIZE 128
using namespace std;


int main()
{
	cout<<"server start.."<<endl;

	struct sockaddr_in saddr;
	bzero(&saddr,sizeof(saddr));

	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	saddr.sin_port = htons(6500);

	int s_socket = socket(AF_INET,SOCK_STREAM,0);
	assert(s_socket != -1);

	int ret = bind(s_socket,(struct sockaddr*)&saddr,sizeof(saddr));
	if( ret == -1)
	{
		printf("bind error\n");
		exit(-1);
	}

	listen(s_socket,5);

	cout<<"accept.."<<endl;
	
	char buff[BUFF_SIZE]={0};
	socklen_t len = sizeof(saddr);
	int c = accept(s_socket,(struct sockaddr*)&saddr,&len);
	if(c <0)
	{
		printf("accept error\n");
	}

	while(1)
	{

		recv(c,buff,BUFF_SIZE,0);
		printf("buff = %s \n",buff);

		bzero(&buff,sizeof(buff));
		send(c,"YES",3,0);

	}
	close(s_socket);
	exit(0);

}
