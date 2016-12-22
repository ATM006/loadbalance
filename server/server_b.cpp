#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>

#define MAX_EVENTS 1024
#define BUFF_SIZE 128
using namespace std;

int setnonblocking( int fd )
{
    int old_option = fcntl( fd, F_GETFL );
    int new_option = old_option | O_NONBLOCK;
    fcntl( fd, F_SETFL, new_option );
    return old_option;
}

void addfd( int epollfd, int fd )
{
    epoll_event event;
    event.data.fd = fd;
    //event.events = EPOLLIN | EPOLLET;
    event.events = EPOLLIN;
    epoll_ctl( epollfd, EPOLL_CTL_ADD, fd, &event );
    setnonblocking( fd );
}



int main()
{
	cout<<"server start.."<<endl;

	struct sockaddr_in saddr;
	bzero(&saddr,sizeof(saddr));

	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	saddr.sin_port = htons(6501);

	int s_socket = socket(AF_INET,SOCK_STREAM,0);
	assert(s_socket != -1);

	int ret = bind(s_socket,(struct sockaddr*)&saddr,sizeof(saddr));
	if( ret == -1)
	{
		printf("bind error\n");
		exit(-1);
	}

	listen(s_socket,5);

	epoll_event events[MAX_EVENTS];
	int epollfd = epoll_create(5);
	assert(epollfd !=-1);
	addfd(epollfd,s_socket);

	
	while(1)
	{
		int num = epoll_wait(epollfd,events,MAX_EVENTS,-1);
		if(num <0 )
		{
			cout<<"epoll_wait error"<<endl;
			break;
		}

		for(int i=0;i<num;++i)
		{
			int retfd = events[i].data.fd;
			if(retfd == s_socket)
			{
				struct sockaddr_in caddr;

				cout<<"accept.."<<endl;
	
				socklen_t len = sizeof(caddr);
				int c = accept(s_socket,(struct sockaddr*)&caddr,&len);
				if(c <0)
				{
					cout<<"accept error"<<endl;
				}
				addfd(epollfd,c);
			}
			else if(events[i].events & EPOLLIN)
			{
				char buff[BUFF_SIZE]={0};
				while(1)
				{
					memset(buff,0,BUFF_SIZE);
					int ret = recv(retfd,buff,BUFF_SIZE,0);
					cout<<buff<<endl;
					
					if(ret < 0)
					{
						if((errno == EAGAIN)||(errno ==EWOULDBLOCK))
						{
							break;
						}
						close(retfd);
						break;
					}
					else if(ret ==0)
					{
						close(retfd);
					}
					else
					{
						send(retfd,"YES",3,0);
					}
				
				}
			}
			else
			{
				cout<<"somthing"<<endl;
			}
		
		}

		
	}	
	close(s_socket);
	exit(0);

}
