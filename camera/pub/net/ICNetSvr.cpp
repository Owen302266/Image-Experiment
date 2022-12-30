
#include "ICNetSvr.h"


CICNetSvr::CICNetSvr()
{
	cdp = NULL;
	this->datanum = 0;
}

CICNetSvr::~CICNetSvr()
{

}

void CICNetSvr::OnDataRecv()
{
	if(cdp == NULL || datanum == 0)
	{
		return ;
	}
	if(this->recvlen != 6*sizeof(CamerasData))
	{
		return;
	}
	memcpy(ndp,this->recvbuf,6*sizeof(CamerasData));
	for(int i = 0; i < 6 ; i ++)
	{
		if(ndp[i].serial>=0 &&ndp[i].serial < datanum)
		{
			memcpy(&(cdp[ndp[i].serial]),&(ndp[i]),sizeof(CamerasData));
		}
	}

};
