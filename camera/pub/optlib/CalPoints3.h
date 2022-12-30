
#ifndef _CALPOINTS3_H_ 
#define _CALPOINTS3_H_


#include "PIC_RGB.h"

#define MaxPointNum3 100
#define MaxLinkLineNum3 10000

typedef struct LinkLine3
{
	long serial;
	long line_num;
	short start_pos;
	short end_pos;
	char use_tag;
}*pLinkLine3;



class CCalPoints3  
{
public:
	CCalPoints3();
	virtual ~CCalPoints3();
	bool OnCalPoint3(CPIC_RGB * pic);
	void CalConnect3(CPIC_RGB * pic);
	void CetLinkLines3(CPIC_RGB *pic);
	inline bool judge(unsigned char r,unsigned char g, unsigned char b);
	inline void FindAndMark3(int pos,int tagvalue,long *xup,long *xdown,long *yup,long *ydown);
	inline void Sort();
	inline double GetDst(double x1,double y1,double x2,double y2);
	inline bool IsOnLine(double l,double m,double n);
	inline bool Solve(int &rs,int &gs,int &bs);

	unsigned char yes_up_limit_r;
	unsigned char yes_dn_limit_r;
	unsigned char no_up_limit_r;
	unsigned char no_dn_limit_r;

	unsigned char yes_up_limit_g;
	unsigned char yes_dn_limit_g;
	unsigned char no_up_limit_g;
	unsigned char no_dn_limit_g;
	
	unsigned char yes_up_limit_b;
	unsigned char yes_dn_limit_b;
	unsigned char no_up_limit_b;
	unsigned char no_dn_limit_b;

	unsigned char threshold_r;
	unsigned char threshold_g;
	unsigned char threshold_b;

	pLinkLine3 linklines;

	int line_linkLine_pos_begin[1024];
	int line_linkLine_pos_end[1024];
	long linkLineNum;
	long point_Num;

	double pointsx[MaxPointNum3];
	double pointsy[MaxPointNum3];
	int		pointsnum[MaxPointNum3];

	double R_x;
	double R_y;
	double R_num;

	double G_x;
	double G_y;
	double G_num;

	double B_x;
	double B_y;
	double B_num;

	


};

#endif 
