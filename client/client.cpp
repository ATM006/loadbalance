#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>	//关闭套接字
#include <string.h>


#define BUFF_SIZE 128

using namespace std;

int main()
{
	int ret = 0;
	struct sockaddr_in caddr;
	bzero(&caddr,sizeof(caddr));

	caddr.sin_family = AF_INET;
	caddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	caddr.sin_port = htons(6500);

	int c_socket = socket(AF_INET,SOCK_STREAM,0);
	assert( c_socket != -1 );
	
	ret = connect(c_socket,(struct sockaddr*)&caddr,sizeof(caddr));
//	assert( ret != -1 );
	if(ret == -1 )
	{
		printf("connect error \n");
		exit(-1);
	}


	char  buff[BUFF_SIZE]="127.0.0.1";
	while(1)
	{
		cin>>buff;
		if(strcmp(buff,"exit")==0)	break;
		send(c_socket,buff,BUFF_SIZE,0);

		bzero(&buff,sizeof(buff));
		recv(c_socket,buff,BUFF_SIZE,0);
		cout<<buff<<endl;
	}
	close(c_socket);

	exit(0);
}
