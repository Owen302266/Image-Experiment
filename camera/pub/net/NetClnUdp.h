
#ifndef AN_NETCLN_UDP_H_ 
#define AN_NETCLN_UDP_H_

/*/�������ݷ��Ϳͻ���/*/
#include <winsock.h>
#define socklen_t int

class CNetClnUdp  
{
public:
	CNetClnUdp();
	virtual ~CNetClnUdp();

	//��ʼ�����绷��
	int InitNet();

	//��ʼ���ͻ��� �����ӷ�������
	int initCln(char * ip,int Port);

	//�������ݵ�������
	int SendSvr(void * p,int len);

	//�ͻ���socket
	
protected:
	char svrip[1000];
	int iplen;
	int RunSocket;
	struct sockaddr_in server_sockaddr_in;
	int svlen;
	int port;


};

#endif 
