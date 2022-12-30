
#if !defined AN_ICNETSVR_H_ 
#define AN_ICNETSVR_H_

#include "NetSvrUdp.h"
#include "../optlib/def.h"

class CICNetSvr : public CNetSvrUdp  
{
public:
	CICNetSvr();
	virtual ~CICNetSvr();
	virtual void OnDataRecv();
	int datanum;
	CamerasData * cdp;
	CamerasData  ndp[6];
};

#endif 
