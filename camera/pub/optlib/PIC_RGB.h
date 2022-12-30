
#ifndef AN_PIC_RGB_H_ 
#define AN_PIC_RGB_H_

#include "def.h"

/*/rgb图片类/*/

class CPIC_RGB  
{

public:
	int w;//宽
	int h;//高
	int Line_Span;//对齐宽
	p_RGB_CELL base;//数据基地址
	long time_Stamp;//图像时间标
public:
	bool CpyPic(CPIC_RGB * ret);
	CPIC_RGB();

	virtual ~CPIC_RGB();
	bool init(int x_span,int y_span);

	bool SetBitBYTE(int x ,int y,void * target);
	bool SetBitBYTEOrg(int x ,int y,void * target);
	bool GetBitBYTE(int x ,int y,void * target);
	bool SetRGB(int x,int y,p_RGB_CELL t);
	bool GetRGB(int x,int y,p_RGB_CELL t);

	bool OnCpyL(CPIC_RGB * ret);
	bool OnCpyR(CPIC_RGB * ret);

	bool GetR(CPIC_RGB * ret);
	bool GetG(CPIC_RGB * ret);
	bool GetB(CPIC_RGB * ret);

	
	bool GetR_F(CPIC_RGB * ret);
	bool GetG_F(CPIC_RGB * ret);
	bool GetB_F(CPIC_RGB * ret);
	
	 
};

#endif 
