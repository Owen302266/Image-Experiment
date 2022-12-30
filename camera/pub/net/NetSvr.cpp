
#include "NetSvr.h"




CNetSvr::CNetSvr()
{
	recvlen = 0;
}

CNetSvr::~CNetSvr()
{

}



int initAsSvr(void * p)
{
	CNetSvr * ns;
	ns = (CNetSvr *)p;
	if(ns->InitNet()==0)
	{
		return 0;
	}

	int port = ns->port;
	struct sockaddr_in myaddr;
	struct sockaddr_in remoteaddr;
	int addrlen;
	int fdmax;	
	int newfd;
	fd_set read_fds;
	fd_set master;
	SOCKET listener;
	int i;
	char yes=1;
	//////////////////////////////////////////
	FD_ZERO(&master);
	FD_ZERO(&read_fds);
	if((listener=socket(AF_INET,SOCK_STREAM,0))==-1)
	{
	
		return -1;
	}
	if(setsockopt(listener,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int))==-1)
	{
		return -1;
	}
	myaddr.sin_family = AF_INET;
	myaddr.sin_addr.s_addr = INADDR_ANY;
	myaddr.sin_port=htons(port);
	memset(&(myaddr.sin_zero),'\0',sizeof(myaddr.sin_zero));
	if(bind(listener,(struct sockaddr *)&myaddr,sizeof(myaddr))==-1)
	{
		return -1;
	}
	if(listen(listener,10) == -1)
	{
		return -1;
	}
	FD_SET(listener,&master);
	fdmax=listener;
	while(1)
	{
		read_fds=master;
		if(select(fdmax+1,&read_fds,NULL,NULL,NULL)==-1) 
		{
			return -1;
		}
		for(i=0;i<=fdmax;i++)
		{
			if (FD_ISSET(i,&read_fds))
			{
				
				if(i==listener)
				{
					addrlen = sizeof(remoteaddr);
					if ((newfd= accept(listener,(struct sockaddr*)&remoteaddr,(socklen_t *)&addrlen)) == -1)
					{
//						printf("accept new connection but error \n");
					}
					else 
					{
						
						FD_SET(newfd,&master);
						////收到连接请求
						if (newfd > fdmax)      
						{
							fdmax=newfd;
						}
//						printf("accept a new link:%d\n",newfd);
					}
				}
				else
				{ //接收数据
					//						shost->OnRecvData(i);
					ns->recvlen =  recv(i,(char *)ns->recvbuf,RECVBUFLEN,0);
					if(ns->recvlen <= 0 )
					{///关闭soket
						closesocket(i);
						FD_CLR(i,&master);
//						printf("close socket:%d\n",i);
					}
					else
					{
						ns->OnDataRecv();
					}
				}
			}
		}
	}
}



int CNetSvr::InitNet()
{
	WSADATA wsadata;
	int ret;
	ret = WSAStartup ( 0x101, &wsadata );
	if(ret==0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

bool CNetSvr::StartSvr()
{
	DWORD id;
	HANDLE  reth;
	void * para = this;
	reth = CreateThread(NULL,0,(unsigned long (__stdcall *)(void *))initAsSvr,para,0,&id);
	if(reth != NULL)
	{
		return true;
	}
	return false;
}

