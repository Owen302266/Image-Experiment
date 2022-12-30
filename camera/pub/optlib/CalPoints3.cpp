
#include "CalPoints3.h"

#include "stdio.h"
#include "stdlib.h"
#include "math.h"


CCalPoints3::CCalPoints3()
{
	yes_up_limit_r = 255;
	yes_dn_limit_r = 200;
	no_up_limit_r = 199;
	no_dn_limit_r = 0;

	yes_up_limit_g = 255;
	yes_dn_limit_g = 200;
	no_up_limit_g = 199;
	no_dn_limit_g = 0;

	yes_up_limit_b = 255;
	yes_dn_limit_b = 200;
	no_up_limit_b = 199;
	no_dn_limit_b = 0;


	threshold_r = 145;
	threshold_g = 45;
	threshold_b = 45;

	linklines = (pLinkLine3)malloc(sizeof(LinkLine3)*MaxLinkLineNum3);
	
}

CCalPoints3::~CCalPoints3()
{
	free(linklines);
}

double CCalPoints3::GetDst(double x1,double y1,double x2,double y2)
{
	return sqrt((x1 - x2 )*(x1 - x2 ) +  (y1 - y2 )*(y1 - y2 ));
}



bool CCalPoints3::IsOnLine(double l,double m,double n)
{
	if(fabs(l + m - n) < n*0.01)
	{
		return true;
	}
	return false;
}

bool CCalPoints3::OnCalPoint3(CPIC_RGB * pic)
{
	CetLinkLines3(pic);
	CalConnect3(pic);

	//if(point_Num<4)
//	{
//		return false;
//	}

	Sort();
	
	R_x = pointsx[0];
	R_y = pointsy[0];
	R_num = pointsnum[0];

	G_x = 0;
	G_y = 0;
	G_num = 0;

	B_x = 0;
	B_y = 0;
	B_num = 0;
	return true;
}

bool CCalPoints3::Solve(int &rs,int &gs,int &bs)
{
	for(int i = 0; i < 4 ; i ++)
	{
		for(int j = 0 ; j < 4 ; j ++)
		{
			if(j != i)
			{
				for(int k = 0; k < 4 ; k ++)
				{
					if(k != i && k != j)
					{
						double L = GetDst(pointsx[i],pointsy[i],pointsx[j],pointsy[j]);
						double M = GetDst(pointsx[i],pointsy[i],pointsx[k],pointsy[k]);
						double N = GetDst(pointsx[j],pointsy[j],pointsx[k],pointsy[k]);
						if(IsOnLine(L,M,N))
						{
							if(L< M)
							{
								rs = j;
								gs = k;
							}
							else
							{
								rs = k;
								gs = j;
							}
							for(int q = 0 ; q < 4 ;q ++)
							{
								if(q != i && q != j && q != k)
								{
									bs = q;
									return true;
								}
							}
						}
						
					}
				}
			}
		}
	}
	return false;
}

void CCalPoints3::CalConnect3(CPIC_RGB * pic)
{

	
	long xup=0,yup=0,xdown=480,ydown=640;
	double dxv=0;
	double dyv=0;
	long dnum=0;

	double maxnum=0;
	long max_xup=0,max_yup=0,max_xdown=640,max_ydown=480;
	double max_xv=0,max_yv=0;
	int serial =2;
	point_Num = 0;
	for(long i = 0; i < linkLineNum ; i ++)
	{
		if(linklines[i].use_tag == 1 && point_Num < MaxPointNum3 )
		{

			long xup=0,yup=0,xdown=480,ydown=640;

			FindAndMark3(i,serial,&xup,&xdown,&yup,&ydown);

			if(xup+3>=480)
				xup = 479;
			else xup = xup+3;
			if(xdown-3<=0)
				xdown = 0;
			else xdown = xdown-3;

			if(yup+3>=640)
				yup = 639;
			else yup = yup+3;
			if(ydown-3<=0)
				ydown = 0;
			else ydown = ydown-3;

			long pos1;
			long pos2;
			dnum=0;
			dxv = 0;
			dyv = 0;
			for(int i=xdown;i<xup+1;i++)
			{
				pos1 = i*pic->w;
				for(int j=ydown;j<yup+1;j++)
				{
					pos2 = pos1 + j;
					if(judge(pic->base[pos2].r,pic->base[pos2].g,pic->base[pos2].b))
					{
						dxv+=(double)i;
						dyv+=(double)j;
						dnum++;
					}
				}
			}
	
			dxv=dxv/(double)dnum;
			dyv=dyv/(double)dnum;
		

			pointsx[point_Num] = dxv ;
			pointsy[point_Num] = dyv ;
			pointsnum[point_Num] = dnum;

			point_Num ++ ;//总体区域号
			serial ++;//线段上的区域号
		}
	}
}


void CCalPoints3::Sort()
{
	int tag=0;
	double tmp;
	for(int i = 0;i<point_Num;i++)
	{
		for(int j = point_Num-2 ;j >= i; j -- )
		{
			if(pointsnum[j]<pointsnum[j+1])
			{
				tmp = pointsnum[j];
				pointsnum[j] = pointsnum[j+1];
				pointsnum[j+1] = tmp;

				tmp = pointsx[j];
				pointsx[j] = pointsx[j+1];
				pointsx[j+1] = tmp;

				tmp = pointsy[j];
				pointsy[j] = pointsy[j+1];
				pointsy[j+1] = tmp;
			}
		}
	}
}

void CCalPoints3::FindAndMark3(int pos,int tagvalue,long *xup,long *xdown,long *yup,long *ydown)
{
	
	if(linklines[pos].use_tag != 1)
	{
		return ;
	}
	int sp = 3;
	linklines[pos].use_tag = tagvalue;
	int line_p = linklines[pos].line_num - sp;
	int line_next = linklines[pos].line_num + sp;
	int line_number=linklines[pos].line_num;


	if(line_number<(*xdown)) (*xdown)=line_number;
	if(line_number>(*xup)) (*xup)=line_number;
	if(linklines[pos].end_pos>(*yup))(*yup)=linklines[pos].end_pos;
	if(linklines[pos].start_pos<(*ydown))(*ydown)=linklines[pos].start_pos;
	
	int begin;
	int end;

	if(line_p >= 0)
	{
		begin = line_linkLine_pos_begin[line_p];
		end = line_linkLine_pos_end[line_p];
		if(begin != -1)
		{
			for(long i = begin ; i <= end ; i ++)
			{
				if(linklines[i].use_tag == 1)
				{
					if(linklines[i].end_pos+1 < linklines[pos].start_pos   || 
						linklines[pos].end_pos+1 < linklines[i].start_pos )
					{
					}
					else
					{
						
						FindAndMark3(i,tagvalue,xup,xdown,yup,ydown);
					}
				}
			}
		}
	}
	if(line_next <1024 )
	{
		begin = line_linkLine_pos_begin[line_next];
		end = line_linkLine_pos_end[line_next];
		if(begin != -1)
		{
			for(long i = begin ; i <= end ; i ++)
			{
				if(linklines[i].use_tag == 1)
				{
					if(linklines[i].end_pos+1 < linklines[pos].start_pos   || 
						linklines[pos].end_pos+1 < linklines[i].start_pos )
					{///没有联通
					}
					else
					{
					
						FindAndMark3(i,tagvalue,xup,xdown,yup,ydown);
					}
				}
			}
		}
	}
}

void CCalPoints3::CetLinkLines3(CPIC_RGB *pic)
{
	for(long k = 0; k < 1024 ; k ++)
	{
		line_linkLine_pos_begin[k] = -1;
		line_linkLine_pos_end[k] = -1;
	}

	linkLineNum = 0;
	int state;
	int startpos;
	int endpos;

	long pos1;
	long pos2;
	int sp = 3;
	for(long i=0;i<pic->h;i=i+sp)
	{
		pos1 = i*pic->w;
		state=0;
	
		for(long j=0;j<pic->w;j=j+sp)
		{
			pos2 = pos1 + j;
			if(judge(pic->base[pos2].r,pic->base[pos2].g,pic->base[pos2].b))
			{
				
				if(state == 0)
				{
					if(line_linkLine_pos_begin[i] == -1)
					{
						line_linkLine_pos_begin[i] = linkLineNum;
					}
					state =1;
					startpos = j;
				}
			}
			else
			{
				if(state==1)
				{
					state = 0;
					endpos = j-sp;
					if(linkLineNum<MaxLinkLineNum3-1)
					{
						linklines[linkLineNum].serial = linkLineNum;
						linklines[linkLineNum].line_num = i;
						linklines[linkLineNum].start_pos = startpos;
						linklines[linkLineNum].end_pos = endpos;
						linklines[linkLineNum].use_tag = 1;
						linkLineNum ++;
					}
				}
				pic->base[pos2].r = 0;
				pic->base[pos2].g = 0;
				pic->base[pos2].b = 0;
			}	
		}
		if(state==1)
		{
			state = 0;
			endpos = j;
			if(linkLineNum<MaxLinkLineNum3-1)
			{
				linklines[linkLineNum].serial = linkLineNum;
				linklines[linkLineNum].line_num = i;
				linklines[linkLineNum].start_pos = startpos;
				linklines[linkLineNum].end_pos = endpos;
				linklines[linkLineNum].use_tag = 1;
				linkLineNum ++;
			}
		}

		
		if(line_linkLine_pos_begin[i] != -1)
		{
				line_linkLine_pos_end[i] = linkLineNum-1;
		}
	}
}

bool CCalPoints3::judge(unsigned char r,unsigned char g, unsigned char b)
{


	if(r >= threshold_r && (r - g )> threshold_g && (r - b) >threshold_b )
	{
		return true;
	}
	return false;
	if(yes_dn_limit_r <= r && r <= yes_dn_limit_r  && 
		yes_dn_limit_g <= g && g <= yes_dn_limit_g  && 
		yes_dn_limit_b <= b && b <= yes_dn_limit_b)
	{
		return true;
	}
	return false;
}