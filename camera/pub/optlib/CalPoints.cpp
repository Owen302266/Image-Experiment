
#include "CalPoints.h"
#include "stdlib.h"
#include "math.h"



CCalPoints::CCalPoints()
{
	point_Num = 0;
	linklines = (pLinkLine)malloc(sizeof(struct LinkLine) * MaxLinkLineNum);
	linkLineNum = 0;
	this->threshold = 255;
	this->threshold_r=255;
	this->threshold_b=255;
	this->threshold_g=255;
	maskvalue = 200; 
//	this->w1=120;
//	this->w2=360;
//	this->h1=160;
//	this->h2=480;


	this->Rx_down=0;
	this->Rx_up=480;
	this->Ry_down=0;
	this->Ry_up=640;
	this->Gx_down=0;
	this->Gx_up=480;
	this->Gy_down=0;
	this->Gy_up=640;
	this->Bx_down=0;
	this->Bx_up=480;
	this->By_down=0;
	this->By_up=640;

	this->R_x=0;
	this->R_y=0;
	this->G_x=0;
	this->G_y=0;
	this->B_x=0;
	this->B_y=0;

	this->R_num=0;
	this->G_num=0;
	this->B_num=0;
//*******************
	linklines_r = (pLinkLine)malloc(sizeof(struct LinkLine) * MaxLinkLineNum);
	linkLineNum_r = 0;
	point_Num_r=0;
	linklines_g = (pLinkLine)malloc(sizeof(struct LinkLine) * MaxLinkLineNum);
	linkLineNum_g = 0;
	point_Num_g=0;
	linklines_b = (pLinkLine)malloc(sizeof(struct LinkLine) * MaxLinkLineNum);
	linkLineNum_b = 0;
	point_Num_b=0;
//***********************

	
}

CCalPoints::~CCalPoints()
{
	free(linklines);
}




bool CCalPoints::OnCalPointsr(CPIC_RGB * pic)
{
	if(pic == NULL)
	{
		return false;
	}
	if(pic->h<=0 || pic->w <=0 || pic->base ==NULL)
	{
		return false;
	}


	//NewCalPointsr(pic);
	/////////////////连接线
	CetLinkLinesr1(pic);
	/////////////////连通性计算
	CalConnect_r(pic);
	CalConnect_g(pic);
	CalConnect_b(pic);
	//CalConnect_b(pic);
//	Filter();

//	Colorize(pic);

//	CalMid(pic);

//Sort_r();
//	Sort_g();
	return true;
}




bool CCalPoints::OnCalPoint4(CPIC_RGB * pic)
{
	if(pic == NULL)
	{
		return false;
	}
	if(pic->h<=0 || pic->w <=0 || pic->base ==NULL)
	{
		return false;
	}
	//NewCalPointsr(pic);
	/////////////////连接线
	CetLinkLinesr1(pic);
	/////////////////连通性计算
	CalConnect_4(pic);
	return true;
}


void CCalPoints::CalConnect_4(CPIC_RGB * pic)
{
//	long xv;
//	long yv;
	long xup=0,yup=0,xdown=480,ydown=640;
//	long num;
	double dxv=0;
	double dyv=0;
	long dnum=0;

	double maxnum=0;
	long max_xup=0,max_yup=0,max_xdown=640,max_ydown=480;
	double max_xv=0,max_yv=0;
	int serial =2;
	point_Num_r = 0;
	for(long i = 0; i < linkLineNum_r ; i ++)
	{
		if(linklines_r[i].use_tag == 1 && point_Num_r < MaxPointNum )
		{
			//xv = 0;
			long xup=0,yup=0,xdown=480,ydown=640;
			//yv = 0;
		//	num = 0;
			FindAndMark_r(i,serial,&xup,&xdown,&yup,&ydown);

			if(xup+15>=480)
				xup = 479;
			else xup = xup+15;
			if(xdown-15<=0)
				xdown = 0;
			else xdown = xdown-15;

			if(yup+15>=640)
				yup = 639;
			else yup = yup+15;
			if(ydown-15<=0)
				ydown = 0;
			else ydown = ydown-15;
		//	dxv = xv;
		//	dyv = yv;
		//	dnum = num;
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
					if(pic->base[pos2].r>threshold_r &&(pic->base[pos2].g < maskvalue && pic->base[pos2].b < maskvalue) && (pic->base[pos2].r >pic->base[pos2].g ) && (pic->base[pos2].r >pic->base[pos2].b ) )
					{
						dxv+=(double)i;
						dyv+=(double)j;
						dnum++;
					}
				}
			}
		//	if(dnum != 0 )
			{
			dxv=dxv/(double)dnum;
			dyv=dyv/(double)dnum;
			}

			if(dnum>maxnum)
			{
				max_xv=dxv;
				max_yv=dyv;
				maxnum=dnum;
				max_xup=xup;
				max_yup=yup;
				max_xdown=xdown;
				max_ydown=ydown;
			}

			//pointsx[point_Num] = dxv / dnum;
		//	pointsy[point_Num] = dyv / dnum;
		//	pointsnum[point_Num] = dnum;
		//	pointsserial[point_Num] = serial;
		//	deletedTag[point_Num] = false;

			point_Num_r ++ ;//总体区域号
			serial ++;//线段上的区域号
		}
	}
	this->R_x=max_xv;
	this->R_y=max_yv;
	this->Rx_down=max_xup;
	this->Rx_up=max_xdown;
	this->Ry_down=max_yup;
	this->Ry_up=max_ydown;
	this->R_num=maxnum;
}











void CCalPoints::CetLinkLinesr1(CPIC_RGB *pic)
{
	int judgeR=0;
	int judgeG=0;
	int judgeB=0;
	long pos1;
	long pos2;
	this->Rx_down=0;
	this->Rx_up=480;
	this->Ry_down=0;
	this->Ry_up=640;
	this->Gx_down=0;
	this->Gx_up=480;
	this->Gy_down=0;
	this->Gy_up=640;
	this->Bx_down=0;
	this->Bx_up=480;
	this->By_down=0;
	this->By_up=640;

	this->R_x=0;
	this->R_y=0;
	this->G_x=0;
	this->G_y=0;
	this->B_x=0;
	this->B_y=0;

	this->R_num=0;
	this->G_num=0;
	this->B_num=0;
//**********************
	for(long k = 0; k < 1024 ; k ++)
	{
		line_linkLine_pos_begin_r[k] = -1;
		line_linkLine_pos_end_r[k] = -1;
		line_linkLine_pos_begin_g[k] = -1;
		line_linkLine_pos_end_g[k] = -1;
		line_linkLine_pos_begin_b[k] = -1;
		line_linkLine_pos_end_b[k] = -1;
	}
	linkLineNum_r = 0;
	int state_r;
	int startpos_r;
	int endpos_r;

	linkLineNum_g = 0;
	int state_g;
	int startpos_g;
	int endpos_g;

	linkLineNum_b = 0;
	int state_b;
	int startpos_b;
	int endpos_b;
//*************************

	for(long i=0;i<pic->h;i=i+3)
	{
		pos1 = i*pic->w;
		state_r=0;
		state_g=0;
		state_b=0;
		for(long j=0;j<pic->w;j=j+3)
		{
			pos2 = pos1 + j;
			if(pic->base[pos2].r>threshold_r||pic->base[pos2].g>threshold_g||pic->base[pos2].b>threshold_b)
			{
				if(pic->base[pos2].r>threshold_r && (pic->base[pos2].r >pic->base[pos2].g*1.1 ) && (pic->base[pos2].r >pic->base[pos2].b*1. ) )
				{
					if(state_r == 0)
					{
						if(line_linkLine_pos_begin_r[i] == -1)
						{
							line_linkLine_pos_begin_r[i] = linkLineNum_r;
						}
						state_r =1;
						startpos_r = j;
					}
					if(state_g==1)
					{
						state_g = 0;
						endpos_g = j-3;
						if(linkLineNum_g<MaxLinkLineNum-1)
						{
							linklines_g[linkLineNum_g].serial = linkLineNum_g;
							linklines_g[linkLineNum_g].line_num = i;
							linklines_g[linkLineNum_g].start_pos = startpos_g;
							linklines_g[linkLineNum_g].end_pos = endpos_g;
							linklines_g[linkLineNum_g].use_tag = 1;
							linkLineNum_g ++;
						}
					}
					if(state_b==1)
					{
						state_b = 0;
						endpos_b = j-3;
						if(linkLineNum_b<MaxLinkLineNum-1)
						{
							linklines_b[linkLineNum_b].serial = linkLineNum_b;
							linklines_b[linkLineNum_b].line_num = i;
							linklines_b[linkLineNum_b].start_pos = startpos_b;
							linklines_b[linkLineNum_b].end_pos = endpos_b;
							linklines_b[linkLineNum_b].use_tag = 1;
							linkLineNum_b ++;
						}
					}
				}
				else if(pic->base[pos2].g>threshold_g  && (pic->base[pos2].g >(pic->base[pos2].r+20)) && (pic->base[pos2].g >(pic->base[pos2].b))   )
				{
					if(state_r==1)
					{
						state_r = 0;
						endpos_r = j-3;
						if(linkLineNum_r<MaxLinkLineNum-1)
						{
							linklines_r[linkLineNum_r].serial = linkLineNum_r;
							linklines_r[linkLineNum_r].line_num = i;
							linklines_r[linkLineNum_r].start_pos = startpos_r;
							linklines_r[linkLineNum_r].end_pos = endpos_r;
							linklines_r[linkLineNum_r].use_tag = 1;
							linkLineNum_r ++;
						}
					}
					if(state_b==1)
					{
						state_b = 0;
						endpos_b = j-3;
						if(linkLineNum_b<MaxLinkLineNum-1)
						{
							linklines_b[linkLineNum_b].serial = linkLineNum_b;
							linklines_b[linkLineNum_b].line_num = i;
							linklines_b[linkLineNum_b].start_pos = startpos_b;
							linklines_b[linkLineNum_b].end_pos = endpos_b;
							linklines_b[linkLineNum_b].use_tag = 1;
							linkLineNum_b ++;
						}
					}
					if(state_g == 0)
					{
						if(line_linkLine_pos_begin_g[i] == -1)
						{
							line_linkLine_pos_begin_g[i] = linkLineNum_g;
						}
						state_g =1;
						startpos_g = j;
					}
				}
				else if(pic->base[pos2].b>threshold_b && (pic->base[pos2].b >pic->base[pos2].g+20 ) && (pic->base[pos2].b >pic->base[pos2].r+50 )  )
				{
					if(state_b == 0)
					{
						if(line_linkLine_pos_begin_b[i] == -1)
						{
							line_linkLine_pos_begin_b[i] = linkLineNum_b;
						}
						state_b =1;
						startpos_b = j;
					}
					if(state_r==1)
					{
						state_r = 0;
						endpos_r = j-3;
						if(linkLineNum_r<MaxLinkLineNum-1)
						{
							linklines_r[linkLineNum_r].serial = linkLineNum_r;
							linklines_r[linkLineNum_r].line_num = i;
							linklines_r[linkLineNum_r].start_pos = startpos_r;
							linklines_r[linkLineNum_r].end_pos = endpos_r;
							linklines_r[linkLineNum_r].use_tag = 1;
							linkLineNum_r ++;
						}
					}
					if(state_g==1)
					{
						state_g = 0;
						endpos_g = j-3;
						if(linkLineNum_g<MaxLinkLineNum-1)
						{
							linklines_g[linkLineNum_g].serial = linkLineNum_g;
							linklines_g[linkLineNum_g].line_num = i;
							linklines_g[linkLineNum_g].start_pos = startpos_g;
							linklines_g[linkLineNum_g].end_pos = endpos_g;
							linklines_g[linkLineNum_g].use_tag = 1;
							linkLineNum_g ++;
						}
					}
				}
				else
				{
					if(state_r==1)
					{
						state_r = 0;
						endpos_r = j-3;
						if(linkLineNum_r<MaxLinkLineNum-1)
						{
							linklines_r[linkLineNum_r].serial = linkLineNum_r;
							linklines_r[linkLineNum_r].line_num = i;
							linklines_r[linkLineNum_r].start_pos = startpos_r;
							linklines_r[linkLineNum_r].end_pos = endpos_r;
							linklines_r[linkLineNum_r].use_tag = 1;
							linkLineNum_r ++;
						}
					}
					if(state_g==1)
					{
						state_g = 0;
						endpos_g = j-3;
						if(linkLineNum_g<MaxLinkLineNum-1)
						{
							linklines_g[linkLineNum_g].serial = linkLineNum_g;
							linklines_g[linkLineNum_g].line_num = i;
							linklines_g[linkLineNum_g].start_pos = startpos_g;
							linklines_g[linkLineNum_g].end_pos = endpos_g;
							linklines_g[linkLineNum_g].use_tag = 1;
							linkLineNum_g ++;
						}
					}
					if(state_b==1)
					{
						state_b = 0;
						endpos_b = j-3;
						if(linkLineNum_b<MaxLinkLineNum-1)
						{
							linklines_b[linkLineNum_b].serial = linkLineNum_b;
							linklines_b[linkLineNum_b].line_num = i;
							linklines_b[linkLineNum_b].start_pos = startpos_b;
							linklines_b[linkLineNum_b].end_pos = endpos_b;
							linklines_b[linkLineNum_b].use_tag = 1;
							linkLineNum_b ++;
						}
					}
					pic->base[pos2].r=0;
					pic->base[pos2].g=0;
					pic->base[pos2].b=0;
				}

			}
			else
			{
				if(state_r==1)
					{
						state_r = 0;
						endpos_r = j-3;
						if(linkLineNum_r<MaxLinkLineNum-1)
						{
							linklines_r[linkLineNum_r].serial = linkLineNum_r;
							linklines_r[linkLineNum_r].line_num = i;
							linklines_r[linkLineNum_r].start_pos = startpos_r;
							linklines_r[linkLineNum_r].end_pos = endpos_r;
							linklines_r[linkLineNum_r].use_tag = 1;
							linkLineNum_r ++;
						}
					}
				if(state_g==1)
					{
						state_g = 0;
						endpos_g = j-3;
						if(linkLineNum_g<MaxLinkLineNum-1)
						{
							linklines_g[linkLineNum_g].serial = linkLineNum_g;
							linklines_g[linkLineNum_g].line_num = i;
							linklines_g[linkLineNum_g].start_pos = startpos_g;
							linklines_g[linkLineNum_g].end_pos = endpos_g;
							linklines_g[linkLineNum_g].use_tag = 1;
							linkLineNum_g ++;
						}
					}
				if(state_b==1)
					{
						state_b = 0;
						endpos_b = j-3;
						if(linkLineNum_b<MaxLinkLineNum-1)
						{
							linklines_b[linkLineNum_b].serial = linkLineNum_b;
							linklines_b[linkLineNum_b].line_num = i;
							linklines_b[linkLineNum_b].start_pos = startpos_b;
							linklines_b[linkLineNum_b].end_pos = endpos_b;
							linklines_b[linkLineNum_b].use_tag = 1;
							linkLineNum_b ++;
						}
					}
				pic->base[pos2].r=0;
				pic->base[pos2].g=0;
				pic->base[pos2].b=0;
			}
		}
		if(state_r ==1)
		{
			state_r = 0;
			endpos_r = j-3;
			if(linkLineNum_r<MaxLinkLineNum-1)
			{
				linklines_r[linkLineNum_r].serial = linkLineNum_r;
				linklines_r[linkLineNum_r].line_num = i;
				linklines_r[linkLineNum_r].start_pos = startpos_r;
				linklines_r[linkLineNum_r].end_pos = endpos_r;
				linklines_r[linkLineNum_r].use_tag = 1;
				linkLineNum_r ++;
			}
		}
		if(state_g==1)
		{
			state_g = 0;
			endpos_g = j-3;
			if(linkLineNum_g<MaxLinkLineNum-1)
			{
				linklines_g[linkLineNum_g].serial = linkLineNum_g;
				linklines_g[linkLineNum_g].line_num = i;
				linklines_g[linkLineNum_g].start_pos = startpos_g;
				linklines_g[linkLineNum_g].end_pos = endpos_g;
				linklines_g[linkLineNum_g].use_tag = 1;
				linkLineNum_g ++;
			}
		}
		if(state_b==1)
		{
			state_b = 0;
			endpos_b = j-3;
			if(linkLineNum_b<MaxLinkLineNum-1)
			{
				linklines_b[linkLineNum_b].serial = linkLineNum_b;
				linklines_b[linkLineNum_b].line_num = i;
				linklines_b[linkLineNum_b].start_pos = startpos_b;
				linklines_b[linkLineNum_b].end_pos = endpos_b;
				linklines_b[linkLineNum_b].use_tag = 1;
				linkLineNum_b ++;
			}
		}

		if(line_linkLine_pos_begin_r[i] != -1)
		{
				line_linkLine_pos_end_r[i] = linkLineNum_r-1;
		}
		if(line_linkLine_pos_begin_g[i] != -1)
		{
				line_linkLine_pos_end_g[i] = linkLineNum_g-1;
		}
		if(line_linkLine_pos_begin_b[i] != -1)
		{
				line_linkLine_pos_end_b[i] = linkLineNum_b-1;
		}
	}
}


void CCalPoints::CalConnect_r(CPIC_RGB * pic)
{
//	long xv;
//	long yv;
	long xup=0,yup=0,xdown=480,ydown=640;
//	long num;
	double dxv=0;
	double dyv=0;
	long dnum=0;

	double maxnum=0;
	long max_xup=0,max_yup=0,max_xdown=640,max_ydown=480;
	double max_xv=0,max_yv=0;
	int serial =2;
	point_Num_r = 0;
	for(long i = 0; i < linkLineNum_r ; i ++)
	{
		if(linklines_r[i].use_tag == 1 && point_Num_r < MaxPointNum )
		{
			//xv = 0;
			long xup=0,yup=0,xdown=480,ydown=640;
			//yv = 0;
		//	num = 0;
			FindAndMark_r(i,serial,&xup,&xdown,&yup,&ydown);

			if(xup+15>=480)
				xup = 479;
			else xup = xup+15;
			if(xdown-15<=0)
				xdown = 0;
			else xdown = xdown-15;

			if(yup+15>=640)
				yup = 639;
			else yup = yup+15;
			if(ydown-15<=0)
				ydown = 0;
			else ydown = ydown-15;
		//	dxv = xv;
		//	dyv = yv;
		//	dnum = num;
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
					if(pic->base[pos2].r>threshold_r &&(pic->base[pos2].g < maskvalue && pic->base[pos2].b < maskvalue) && (pic->base[pos2].r >pic->base[pos2].g ) && (pic->base[pos2].r >pic->base[pos2].b ) )
					{
						dxv+=(double)i;
						dyv+=(double)j;
						dnum++;
					}
				}
			}
		//	if(dnum != 0 )
			{
			dxv=dxv/(double)dnum;
			dyv=dyv/(double)dnum;
			}

			if(dnum>maxnum)
			{
				max_xv=dxv;
				max_yv=dyv;
				maxnum=dnum;
				max_xup=xup;
				max_yup=yup;
				max_xdown=xdown;
				max_ydown=ydown;
			}

			//pointsx[point_Num] = dxv / dnum;
		//	pointsy[point_Num] = dyv / dnum;
		//	pointsnum[point_Num] = dnum;
		//	pointsserial[point_Num] = serial;
		//	deletedTag[point_Num] = false;

			point_Num_r ++ ;//总体区域号
			serial ++;//线段上的区域号
		}
	}
	this->R_x=max_xv;
	this->R_y=max_yv;
	this->Rx_down=max_xup;
	this->Rx_up=max_xdown;
	this->Ry_down=max_yup;
	this->Ry_up=max_ydown;
	this->R_num=maxnum;

	
}
void CCalPoints::FindAndMark_r(int pos,int tagvalue,long *xup,long *xdown,long *yup,long *ydown)
{
	if(linklines_r[pos].use_tag != 1)
	{
		return ;
	}
	linklines_r[pos].use_tag = tagvalue;
	int line_p = linklines_r[pos].line_num - 3;
	int line_next = linklines_r[pos].line_num + 3;
	int line_number=linklines_r[pos].line_num;

	
	//long num1 = linklines[pos].end_pos - linklines[pos].start_pos +1;
	//(* yv) +=  linklines[pos].line_num * num1;
	//(* xv) += num1 * linklines[pos].start_pos + num1 *(num1-1) / 2;
	//(* num) += num1;
	if(line_number<(*xdown)) (*xdown)=line_number;
	if(line_number>(*xup)) (*xup)=line_number;
	if(linklines_r[pos].end_pos>(*yup))(*yup)=linklines_r[pos].end_pos;
	if(linklines_r[pos].start_pos<(*ydown))(*ydown)=linklines_r[pos].start_pos;
	
	int begin;
	int end;

	if(line_p >= 0)
	{
		begin = line_linkLine_pos_begin_r[line_p];
		end = line_linkLine_pos_end_r[line_p];
		if(begin != -1)
		{
			for(long i = begin ; i <= end ; i ++)
			{
				if(linklines_r[i].use_tag == 1)
				{
					if(linklines_r[i].end_pos+1 < linklines_r[pos].start_pos   || 
						linklines_r[pos].end_pos+1 < linklines_r[i].start_pos )
					{
					}
					else
					{
						
						FindAndMark_r(i,tagvalue,xup,xdown,yup,ydown);
					}
				}
			}
		}
	}
	if(line_next <1024 )
	{
		begin = line_linkLine_pos_begin_r[line_next];
		end = line_linkLine_pos_end_r[line_next];
		if(begin != -1)
		{
			for(long i = begin ; i <= end ; i ++)
			{
				if(linklines_r[i].use_tag == 1)
				{
					if(linklines_r[i].end_pos+1 < linklines_r[pos].start_pos   || 
						linklines_r[pos].end_pos+1 < linklines_r[i].start_pos )
					{///没有联通
					}
					else
					{
					
						FindAndMark_r(i,tagvalue,xup,xdown,yup,ydown);
					}
				}
			}
		}
	}
}








void CCalPoints::CalConnect_g(CPIC_RGB * pic)
{
//	long xv;
//	long yv;
	long xup=0,yup=0,xdown=480,ydown=640;
//	long num;
	double dxv=0;
	double dyv=0;
	long dnum=0;

	double maxnum=0;
	long max_xup=0,max_yup=0,max_xdown=640,max_ydown=480;
	double max_xv=0,max_yv=0;
	int serial =2;
	point_Num_g = 0;
	for(long i = 0; i < linkLineNum_g ; i ++)
	{
		if(linklines_g[i].use_tag == 1 && point_Num_g < MaxPointNum )
		{
			//xv = 0;
			xup=0,yup=0,xdown=640,ydown=480;
			//yv = 0;
		//	num = 0;
			FindAndMark_g(i,serial,&xup,&xdown,&yup,&ydown);


				if(xup+15>=480)
				xup = 479;
			else xup = xup+15;
			if(xdown-15<=0)
				xdown = 0;
			else xdown = xdown-15;

			if(yup+15>=640)
				yup = 639;
			else yup = yup+15;
			if(ydown-15<=0)
				ydown = 0;
			else ydown = ydown-15;
		//	dxv = xv;
		//	dyv = yv;
		//	dnum = num;
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
					if(pic->base[pos2].g>threshold_g  && (pic->base[pos2].g >(pic->base[pos2].r+20)) && (pic->base[pos2].g >(pic->base[pos2].b))   )
					{
						dxv+=(double)i;
						dyv+=(double)j;
						dnum++;
					}
				}
			}
		//	if(dnum != 0 )
			{
			dxv=dxv/(double)dnum;
			dyv=dyv/(double)dnum;
			}

			if(dnum>maxnum)
			{
				max_xv=dxv;
				max_yv=dyv;
				maxnum=dnum;
				max_xup=xup;
				max_yup=yup;
				max_xdown=xdown;
				max_ydown=ydown;
			}

			//pointsx[point_Num] = dxv / dnum;
		//	pointsy[point_Num] = dyv / dnum;
		//	pointsnum[point_Num] = dnum;
		//	pointsserial[point_Num] = serial;
		//	deletedTag[point_Num] = false;

			point_Num_g ++ ;//总体区域号
			serial ++;//线段上的区域号
		}
	}
	this->G_x=max_xv;
	this->G_y=max_yv;
	this->Gx_down=max_xup;
	this->Gx_up=max_xdown;
	this->Gy_down=max_yup;
	this->Gy_up=max_ydown;
	this->G_num=maxnum;

	
}
void CCalPoints::FindAndMark_g(int pos,int tagvalue,long *xup,long *xdown,long *yup,long *ydown)
{
	if(linklines_g[pos].use_tag != 1)
	{
		return ;
	}
	linklines_g[pos].use_tag = tagvalue;
	int line_p = linklines_g[pos].line_num - 3;
	int line_next = linklines_g[pos].line_num + 3;
	int line_number=linklines_g[pos].line_num;

	
	//long num1 = linklines[pos].end_pos - linklines[pos].start_pos +1;
	//(* yv) +=  linklines[pos].line_num * num1;
	//(* xv) += num1 * linklines[pos].start_pos + num1 *(num1-1) / 2;
	//(* num) += num1;
	if(line_number<(*xdown)) (*xdown)=line_number;
	if(line_number>(*xup)) (*xup)=line_number;
	if(linklines_g[pos].end_pos>(*yup))(*yup)=linklines_g[pos].end_pos;
	if(linklines_g[pos].start_pos<(*ydown))(*ydown)=linklines_g[pos].start_pos;
	
	int begin;
	int end;

	if(line_p >= 0)
	{
		begin = line_linkLine_pos_begin_g[line_p];
		end = line_linkLine_pos_end_g[line_p];
		if(begin != -1)
		{
			for(long i = begin ; i <= end ; i ++)
			{
				if(linklines_g[i].use_tag == 1)
				{
					if(linklines_g[i].end_pos+1 < linklines_g[pos].start_pos   || 
						linklines_g[pos].end_pos+1 < linklines_g[i].start_pos )
					{
					}
					else
					{
						
						FindAndMark_g(i,tagvalue,xup,xdown,yup,ydown);
					}
				}
			}
		}
	}
	if(line_next <1024 )
	{
		begin = line_linkLine_pos_begin_g[line_next];
		end = line_linkLine_pos_end_g[line_next];
		if(begin != -1)
		{
			for(long i = begin ; i <= end ; i ++)
			{
				if(linklines_g[i].use_tag == 1)
				{
					if(linklines_g[i].end_pos+1 < linklines_g[pos].start_pos   || 
						linklines_g[pos].end_pos+1 < linklines_g[i].start_pos )
					{///没有联通
					}
					else
					{
					
						FindAndMark_g(i,tagvalue,xup,xdown,yup,ydown);
					}
				}
			}
		}
	}
}


void CCalPoints::CalConnect_b(CPIC_RGB * pic)
{
//	long xv;
//	long yv;
	long xup=0,yup=0,xdown=640,ydown=480;
//	long num;
	double dxv=0;
	double dyv=0;
	long dnum=0;

	double maxnum=0;
	long max_xup=0,max_yup=0,max_xdown=640,max_ydown=480;
	double max_xv=0,max_yv=0;
	int serial =2;
	point_Num_b = 0;
	for(long i = 0; i < linkLineNum_b ; i ++)
	{
		if(linklines_b[i].use_tag == 1 && point_Num_b < MaxPointNum )
		{
			//xv = 0;
			xup=0,yup=0,xdown=480,ydown=640;
			//yv = 0;
		//	num = 0;
			FindAndMark_b(i,serial,&xup,&xdown,&yup,&ydown);

			if(xup+15>=480)
				xup = 479;
			else xup = xup+15;
			if(xdown-15<=0)
				xdown = 0;
			else xdown = xdown-15;

			if(yup+15>=640)
				yup = 639;
			else yup = yup+15;
			if(ydown-15<=0)
				ydown = 0;
			else ydown = ydown-15;


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
					if(pic->base[pos2].b>threshold_b && (pic->base[pos2].b >pic->base[pos2].g ) && (pic->base[pos2].b >pic->base[pos2].r+20 )  )
					{
						dxv+=(double)i;
						dyv+=(double)j;
						dnum++;
					}
				}
			}
		//	if(dnum != 0 )
			{
			dxv=dxv/(double)dnum;
			dyv=dyv/(double)dnum;
			}

			if(dnum>maxnum)
			{
				max_xv=dxv;
				max_yv=dyv;
				maxnum=dnum;
				max_xup=xup;
				max_yup=yup;
				max_xdown=xdown;
				max_ydown=ydown;
			}

			//pointsx[point_Num] = dxv / dnum;
		//	pointsy[point_Num] = dyv / dnum;
		//	pointsnum[point_Num] = dnum;
		//	pointsserial[point_Num] = serial;
		//	deletedTag[point_Num] = false;

			point_Num_b ++ ;//总体区域号
			serial ++;//线段上的区域号
		}
	}
	this->B_x=max_xv;
	this->B_y=max_yv;
	this->Bx_down=max_xup;
	this->Bx_up=max_xdown;
	this->By_down=max_yup;
	this->By_up=max_ydown;
	this->B_num=maxnum;

	
}
void CCalPoints::FindAndMark_b(int pos,int tagvalue,long *xup,long *xdown,long *yup,long *ydown)
{
	if(linklines_b[pos].use_tag != 1)
	{
		return ;
	}
	linklines_b[pos].use_tag = tagvalue;
	int line_p = linklines_b[pos].line_num - 3;
	int line_next = linklines_b[pos].line_num + 3;
	int line_number=linklines_b[pos].line_num;

	
	//long num1 = linklines[pos].end_pos - linklines[pos].start_pos +1;
	//(* yv) +=  linklines[pos].line_num * num1;
	//(* xv) += num1 * linklines[pos].start_pos + num1 *(num1-1) / 2;
	//(* num) += num1;
	if(line_number<(*xdown)) (*xdown)=line_number;
	if(line_number>(*xup)) (*xup)=line_number;
	if(linklines_b[pos].end_pos>(*yup))(*yup)=linklines_b[pos].end_pos;
	if(linklines_b[pos].start_pos<(*ydown))(*ydown)=linklines_b[pos].start_pos;
	
	int begin;
	int end;

	if(line_p >= 0)
	{
		begin = line_linkLine_pos_begin_b[line_p];
		end = line_linkLine_pos_end_b[line_p];
		if(begin != -1)
		{
			for(long i = begin ; i <= end ; i ++)
			{
				if(linklines_b[i].use_tag == 1)
				{
					if(linklines_b[i].end_pos+1 < linklines_b[pos].start_pos   || 
						linklines_b[pos].end_pos+1 < linklines_b[i].start_pos )
					{
					}
					else
					{
						
						FindAndMark_b(i,tagvalue,xup,xdown,yup,ydown);
					}
				}
			}
		}
	}
	if(line_next <1024 )
	{
		begin = line_linkLine_pos_begin_b[line_next];
		end = line_linkLine_pos_end_b[line_next];
		if(begin != -1)
		{
			for(long i = begin ; i <= end ; i ++)
			{
				if(linklines_b[i].use_tag == 1)
				{
					if(linklines_b[i].end_pos+1 < linklines_b[pos].start_pos   || 
						linklines_b[pos].end_pos+1 < linklines_b[i].start_pos )
					{///没有联通
					}
					else
					{
					
						FindAndMark_b(i,tagvalue,xup,xdown,yup,ydown);
					}
				}
			}
		}
	}
}









//**********************************
//**********************************

//**********************************

//**********************************


//**********************************


//**********************************

void CCalPoints::Colorize(CPIC_RGB *pic)
{
	
	RGB_CELL rgbcell;
	rgbcell.r = 0;
	rgbcell.g = 0;
	rgbcell.b = 0;
	for(int j = 0 ;j < point_Num;j++)
	{
		if(deletedTag[j])
		{
			for(long i = 0; i < linkLineNum ; i ++)
			{
				if(linklines[i].use_tag == pointsserial[j] )
				{
					for(int k = linklines[i].start_pos ; k <= linklines[i].end_pos ; k ++)
					{
						pic->SetRGB(k,linklines[i].line_num,&rgbcell);
					}

				}
			}
		}
	}
}




void CCalPoints::Filter()
{
	double r ;
	double span;
	for(int i = 0 ;i < point_Num;i++)
	{
		if(!deletedTag[i])
		{
			r = sqrt(pointsnum[i] / 3.14159);
			r = (r/2) > 5 ? r+5 : r+3 ;
			

			for( int j = 0 ; j < point_Num;j++)
			{
				if(j != i)
				{
					if(pointsnum[j] < (pointsnum[i]/6))
					{
						span = sqrt((pointsx[i] - pointsx[j]) * (pointsx[i] - pointsx[j]) + 
							(pointsy[i] - pointsy[j]) * (pointsy[i] - pointsy[j]));
						if(span < r)
						{
							deletedTag[j] = true;
						}
					}
				}
			}
		}


	}
}
void CCalPoints::CalMid(CPIC_RGB * pic)
{
	double sum_x,sum_y;
	int pos1=0;
	int pos2=0;
	int count=0;
	for(int i = 0; i < pic->h ; i ++)
	{
		pos1=i*pic->w;
		for(int j =0;j<pic->w;j++)
		{
			pos2=pos1+j;
			if(pic->base[pos2].r>threshold_r ||pic->base[pos2].g>threshold_g ||pic->base[pos2].b>threshold_b  )
			{
				sum_x+=j;
				sum_y+=i;
				count++;
			}
		}
	}
	mid_x=sum_x/count;
	mid_y=sum_x/count;

}






/*void CCalPoints::Sort_g()
{
	int tag=0;
	double max = 0;
	for(int i =0;i<point_Num_r;i++)
	{
		if(pointsnum_r[i]>max)
		{
			tag=i;
			max = pointsnum_r[i];
		}
	}
	double temp=pointsx_r[0];
	pointsx_r[0]=pointsx_r[tag];
	pointsx_r[tag]=temp;

	temp=pointsy_r[0];
	pointsy_r[0]=pointsy_r[tag];
	pointsy_r[tag]=temp;

	temp=pointsnum_r[0];
	pointsnum_r[0]=pointsnum_r[tag];
	pointsnum_r[tag]=temp;
}*/
/*

void CCalPoints::CetLinkLinesr(CPIC_RGB *pic)
{
		///计算所有连接线
	for(long k = 0; k < 1024 ; k ++)
	{
		line_linkLine_pos_begin[k] = -1;
		line_linkLine_pos_end[k] = -1;
	}
	linkLineNum = 0;
	int state;
	long pos1;
	long pos2;
	int startpos;
	int endpos;
	for(long i = 0; i < pic->h ; i =i+2)
	{
		pos1 = i*pic->w;
		state = 0;
		for(long j = 0; j < pic->w ; j=j+2)
		{
			pos2 = pos1 + j;
			if(pic->base[pos2].r>threshold_r &&(pic->base[pos2].g < maskvalue && pic->base[pos2].b < maskvalue) && (pic->base[pos2].r >pic->base[pos2].g ) && (pic->base[pos2].r >pic->base[pos2].b ) )
			{
				pic->base[pos2].g = 0;
				pic->base[pos2].b = 0;
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
				if(state ==1)
				{
					state = 0;
					endpos = j-2;
					if(linkLineNum<MaxLinkLineNum-1)
					{
						linklines[linkLineNum].serial = linkLineNum;
						linklines[linkLineNum].line_num = i;
						linklines[linkLineNum].start_pos = startpos;
						linklines[linkLineNum].end_pos = endpos;
						linklines[linkLineNum].use_tag = 1;
						linkLineNum ++;
					}
				}
				pic->base[pos2].g = 0;
				pic->base[pos2].b = 0;
				pic->base[pos2].r = 0;

			}
		}	
		if(state ==1)
		{
			state = 0;
			endpos = j-2;
			if(linkLineNum<MaxLinkLineNum-1)
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
*/





/*



bool CCalPoints::OnCalPoints(CPIC_RGB * pic)
{
	if(pic == NULL)
	{
		return false;
	}
	if(pic->h<=0 || pic->w <=0 || pic->base ==NULL)
	{
		return false;
	}



	/////////////////连接线
	CetLinkLines(pic);
	/////////////////连通性计算
	CalConnect();

	Filter();

	Colorize(pic);

//	CalMid(pic);
	Sort();
	return true;
}











void CCalPoints::CetLinkLinesg(CPIC_RGB *pic)
{
		///计算所有连接线
	for(long k = 0; k < 1024 ; k ++)
	{
		line_linkLine_pos_begin[k] = -1;
		line_linkLine_pos_end[k] = -1;
	}
	linkLineNum = 0;
	int state;
	long pos1;
	long pos2;
	int startpos;
	int endpos;
	for(long i = 0; i < pic->h ; i =i+2)
	{
		pos1 = i*pic->w;
		state = 0;
		for(long j = 0; j < pic->w ; j =j+2)
		{
			pos2 = pos1 + j;
			if(pic->base[pos2].g>threshold_g  &&(pic->base[pos2].b < maskvalue && pic->base[pos2].r < maskvalue)&& (pic->base[pos2].g >pic->base[pos2].r ) && (pic->base[pos2].g >pic->base[pos2].b )  )
			{
				pic->base[pos2].r = 0;
				pic->base[pos2].b = 0;
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
				if(state ==1)
				{
					state = 0;
					endpos = j-2;
					if(linkLineNum<MaxLinkLineNum-1)
					{
						linklines[linkLineNum].serial = linkLineNum;
						linklines[linkLineNum].line_num = i;
						linklines[linkLineNum].start_pos = startpos;
						linklines[linkLineNum].end_pos = endpos;
						linklines[linkLineNum].use_tag = 1;
						linkLineNum ++;
					}
				}
				pic->base[pos2].g = 0;
				pic->base[pos2].b = 0;
				pic->base[pos2].r = 0;

			}
		}	
		if(state ==1)
		{
			state = 0;
			endpos = j-2;
			if(linkLineNum<MaxLinkLineNum-1)
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

void CCalPoints::CetLinkLines(CPIC_RGB *pic)
{
		///计算所有连接线
	for(long k = 0; k < 1024 ; k ++)
	{
		line_linkLine_pos_begin[k] = -1;
		line_linkLine_pos_end[k] = -1;
	}
	linkLineNum = 0;
	int state;
	long pos1;
	long pos2;
	int startpos;
	int endpos;
	for(long i = 0; i < pic->h ; i ++)
	{
		pos1 = i*pic->w;
		state = 0;
		for(long j = 0; j < pic->w ; j ++)
		{
			pos2 = pos1 + j;
			if(pic->base[pos2].r>threshold_r ||pic->base[pos2].g>threshold_g ||pic->base[pos2].b>threshold_b )
			{
				pic->base[pos2].g = 0;
				pic->base[pos2].b = 0;
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
				if(state ==1)
				{
					state = 0;
					endpos = j-1;
					if(linkLineNum<MaxLinkLineNum-1)
					{
						linklines[linkLineNum].serial = linkLineNum;
						linklines[linkLineNum].line_num = i;
						linklines[linkLineNum].start_pos = startpos;
						linklines[linkLineNum].end_pos = endpos;
						linklines[linkLineNum].use_tag = 1;
						linkLineNum ++;
					}
				}
				pic->base[pos2].g = 0;
				pic->base[pos2].b = 0;
				pic->base[pos2].r = 0;

			}
		}	
		if(state ==1)
		{
			state = 0;
			endpos = j-1;
			if(linkLineNum<MaxLinkLineNum-1)
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





void CCalPoints::CetLinkLinesb(CPIC_RGB *pic)
{
		///计算所有连接线
	for(long k = 0; k < 1024 ; k ++)
	{
		line_linkLine_pos_begin[k] = -1;
		line_linkLine_pos_end[k] = -1;
	}
	linkLineNum = 0;
	int state;
	long pos1;
	long pos2;
	int startpos;
	int endpos;
	for(long i = 0; i < pic->h ; i=i+2)
	{
		pos1 = i*pic->w;
		state = 0;
		for(long j = 0; j < pic->w ; j=j+2)
		{
			pos2 = pos1 + j;
			if(pic->base[pos2].b>threshold_b &&(pic->base[pos2].g < maskvalue && pic->base[pos2].r < maskvalue) && (pic->base[pos2].b >pic->base[pos2].g ) && (pic->base[pos2].b >pic->base[pos2].r )  )
			{
				pic->base[pos2].g = 0;
				pic->base[pos2].r = 0;
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
				if(state ==1)
				{
					state = 0;
					endpos = j-2;
					if(linkLineNum<MaxLinkLineNum-1)
					{
						linklines[linkLineNum].serial = linkLineNum;
						linklines[linkLineNum].line_num = i;
						linklines[linkLineNum].start_pos = startpos;
						linklines[linkLineNum].end_pos = endpos;
						linklines[linkLineNum].use_tag = 1;
						linkLineNum ++;
					}
				}
				pic->base[pos2].g = 0;
				pic->base[pos2].b = 0;
				pic->base[pos2].r = 0;

			}
		}	
		if(state ==1)
		{
			state = 0;
			endpos = j-2;
			if(linkLineNum<MaxLinkLineNum-1)
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
bool CCalPoints::OnCalPointsg(CPIC_RGB * pic)
{
	if(pic == NULL)
	{
		return false;
	}
	if(pic->h<=0 || pic->w <=0 || pic->base ==NULL)
	{
		return false;
	}



	/////////////////连接线
	CetLinkLinesg(pic);
	/////////////////连通性计算
	CalConnect();

//	Filter();

//	Colorize(pic);

//	CalMid(pic);

	Sort();
	return true;
}


bool CCalPoints::OnCalPointsb(CPIC_RGB * pic)
{
	if(pic == NULL)
	{
		return false;
	}
	if(pic->h<=0 || pic->w <=0 || pic->base ==NULL)
	{
		return false;
	}



	/////////////////连接线
	CetLinkLinesb(pic);
	/////////////////连通性计算
	CalConnect();

//	Filter();

//	Colorize(pic);

//	CalMid(pic);
	Sort();
	return true;
}

void CCalPoints::NewCalPointsr(CPIC_RGB *pic)
{

	long pos1;
	long pos2;
    for(long i =0;i<(pic->h);i=i+2)
	{
		pos1 = i*pic->w;
		for(long j=0;j<(pic->w)-2;j=j+2)
		{
			pos2 = pos1 + j;
			if((pic->base[pos2].r>threshold_r)&&(pic->base[pos2].r>pic->base[pos2].g)&&(pic->base[pos2].r>pic->base[pos2].b )&&(pic->base[pos2].g < maskvalue && pic->base[pos2].b < maskvalue) )
			{
				
				pic->base[pos2].g=0;
				pic->base[pos2].b=0;
				if(pic->base[pos2+1].r>threshold_r&&pic->base[pos2+2].r>threshold_r&&pic->base[pos2+3].r>threshold_r&&pic->base[pos2+4].r>threshold_r)
				{
					pic->base[pos2].r=255;
					pic->base[pos2+1].r=255;
					pic->base[pos2+2].r=255;
					pic->base[pos2+3].r=255;
					pic->base[pos2+4].r=255;
				}
				pointsx[0]=i;
				pointsy[0]=j;
				pointsnum[0]=5;
				point_Num=1;
				return ;
			} 
			else{
				pic->base[pos2].b=0;
				pic->base[pos2].g=0;
				pic->base[pos2].r=0;
			}
			
		}
	}
}


*/















