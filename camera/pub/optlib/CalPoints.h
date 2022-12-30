
#if !defined AN_CALPOINTS_H_ 
#define AN_CALPOINTS_H_

#include "PIC_RGB.h"

#define MaxPointNum 100
#define MaxLinkLineNum 10000

typedef struct LinkLine
{
	long serial;
	short line_num;
	short start_pos;
	short end_pos;
	char use_tag;
}*pLinkLine;

class CCalPoints  
{
public:
	
	void Filter();

	void CetLinkLines(CPIC_RGB * pic);
	void CetLinkLinesr(CPIC_RGB * pic);
	void CetLinkLinesr1(CPIC_RGB * pic);
	void CetLinkLinesg(CPIC_RGB * pic);
	void CetLinkLinesb(CPIC_RGB * pic);
	void NewCalPointsr(CPIC_RGB * pic);
	void Colorize(CPIC_RGB *pic);

	double pointsx[MaxPointNum];
	double pointsy[MaxPointNum];
	double pointsnum[MaxPointNum];
	int    pointsserial[MaxPointNum];
	bool   deletedTag[MaxPointNum];

	int line_linkLine_pos_begin[1024];
	int line_linkLine_pos_end[1024];
	int point_Num;

//*******************
	int Rx_up,Rx_down,Ry_up,Ry_down;
	int Gx_up,Gx_down,Gy_up,Gy_down;
	int Bx_up,Bx_down,By_up,By_down;
	int R_num,G_num,B_num;
	double R_x,R_y,G_x,G_y,B_x,B_y;
	//int test_r,test_g,test_b;
//**************************************
	void CalConnect_r(CPIC_RGB * pic);
	//void Sort_r();
	void FindAndMark_r(int pos,int tagvalue,long *xup,long *xdown,long *yup,long *ydown);
	void CalConnect_g(CPIC_RGB * pic);
//	void Sort_g();
	void FindAndMark_g(int pos,int tagvalue,long *xup,long *xdown,long *yup,long *ydown);
	void CalConnect_b(CPIC_RGB * pic);
//	void Sort_g();
	void FindAndMark_b(int pos,int tagvalue,long *xup,long *xdown,long *yup,long *ydown);
//*****************************
	int linkLineNum_r,linkLineNum_g,linkLineNum_b;//
	pLinkLine linklines_r;
	pLinkLine linklines_g;
	pLinkLine linklines_b;
	int line_linkLine_pos_begin_r[1024];
	int line_linkLine_pos_end_r[1024];
	int line_linkLine_pos_begin_g[1024];
	int line_linkLine_pos_end_g[1024];
	int line_linkLine_pos_begin_b[1024];
	int line_linkLine_pos_end_b[1024];
	double pointsx_r[MaxPointNum];
	double pointsx_g[MaxPointNum];
	double pointsx_b[MaxPointNum];
	double pointsy_r[MaxPointNum];
	double pointsy_g[MaxPointNum];
	double pointsy_b[MaxPointNum];
	int pointsnum_r[MaxPointNum];
	int pointsnum_g[MaxPointNum];
	int pointsnum_b[MaxPointNum];
	int point_Num_r;
	int point_Num_g;
	int point_Num_b;
//*******************



	CCalPoints();
	virtual ~CCalPoints();
	bool OnCalPoints(CPIC_RGB * pic);
	bool OnCalPointsr(CPIC_RGB * pic);
	bool OnCalPointsg(CPIC_RGB * pic);
	bool OnCalPointsb(CPIC_RGB * pic);


	bool OnCalPoint4(CPIC_RGB * pic);
	void CalConnect_4(CPIC_RGB * pic);

	unsigned char threshold;
	unsigned char threshold_r;
	unsigned char threshold_g;
	unsigned char threshold_b;
	pLinkLine linklines;
	int linkLineNum;
	double mid_x;
	double mid_y;
	void CalMid(CPIC_RGB * pic);
	int maskvalue;
	//int w1,w2,h1,h2;
	
};

#endif 