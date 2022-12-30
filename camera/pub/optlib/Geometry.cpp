
#include "Geometry.h"
#include "stdio.h"
#include "math.h"



 void gesture::getPos(double x1,double y1,double z1,double x2,double y2,double z2,double x3,double y3,double z3)
 {
	P1_x= x1;
	P1_y= -y1;
	P1_z= -z1;
	P2_x= x2;
	P2_y= -y2;
	P2_z= -z2;
	P3_x= x3;
	P3_y= -y3;
	P3_z= -z3;
	calNor();
}

 void gesture::calFirst()
 {
	 First_rotate = atan((0.5*(P1_y+P2_y)-P3_y)/(0.5*(P1_x+P2_x)-P3_x));
	 if((0.5*(P1_x+P2_x)-P3_x)<0)
	 {
		 First_rotate+=THEPIIS;
		 
	 }
	 else if((0.5*(P1_y+P2_y)-P3_y)<0)
	 {
		 First_rotate+=2*THEPIIS;
		 
	 }
 }
 void gesture::calSecond()
 {
	 double number = sqrt((0.5*(P1_y+P2_y)-P3_y)*(0.5*(P1_y+P2_y)-P3_y)+(0.5*(P1_x+P2_x)-P3_x)*(0.5*(P1_x+P2_x)-P3_x));
	// cout<<number<<endl;
	 Second_rotate = atan((0.5*(P1_z+P2_z)-P3_z)/number);

 }
 void gesture::calThird()
 {
	 double number = sqrt((0.5*(P1_x-P2_x))*(0.5*(P1_x-P2_x))+(0.5*(P1_y-P2_y))*(0.5*(P1_y-P2_y)));
	 Third_rotate = atan(-(0.5*(P1_z-P2_z))/number);
 }

 void gesture::calNor()
 {
	 Nor_x = (((P3_z-P2_z)*(P2_y-P1_y))-((P2_z-P1_z)*(P3_y-P2_y)))/(((P3_y-P2_y)*(P2_x-P1_x))-((P3_x-P2_x)*(P2_y-P1_y)));
	 Nor_y = (((P3_z-P2_z)*(P2_x-P1_x))-((P2_z-P1_z)*(P3_x-P2_x)))/(((P3_x-P2_x)*(P2_y-P1_y))-((P3_y-P2_y)*(P2_x-P1_x)));
	 Nor_z = 1.00;
 }
 





CGeometry::CGeometry()
{

}

CGeometry::~CGeometry()
{

}


//current use method
bool CGeometry::GetPosePosition(CamerasData datas[20],RoterMatrix Roter[20],MoveVecter Mv[20],
		IntrinsicPara gp[20],DistortionPara gd[20],

		double pose[3][3],p_3D_Point position_p,
		p_3D_Point red_p,p_3D_Point green_p,p_3D_Point  blue_p,
		double ang[3],double *Rdist,_3D_Point  Red15[20])
{
	_3D_Point r_p;
	_3D_Point g_p;
	_3D_Point b_p;

	double max = 0;

	int num1=0;
	int num2=0;
	int num3=0;
	int limit = 50;
	double rlimit = 70;


	if(1)
	{
		double xv[30],yv[30],zv[30];
		
		double x_tmp,y_tmp,z_tmp;
		int num = 0;
		x_tmp = 0;y_tmp = 0;z_tmp = 0;
		_2D_Point p2d1;
		_2D_Point p2d2;
		_3D_Point cross_point;
		double dist=0;
		double distRR=0;
		bool tag;
		int Rcount=0;
		for(int i = 1; i < 6 ; i ++)
		{
			if(datas[i].num_r>1)
			{
				p2d1.x = datas[i].x_r;
				p2d1.y = datas[i].y_r;

				for(int j = 0;j<i ; j ++)
				{
					if(datas[j].num_r>1)
					{
						
						
						p2d2.x = datas[j].x_r;
						p2d2.y = datas[j].y_r;
						
						tag = GetLineCross(&(Roter[i]),&(Mv[i]),&(gp[i]),&(gd[i]),&p2d1,
							&(Roter[j]),&(Mv[j]),&(gp[j]),&(gd[j]),&p2d2,
							&cross_point, &dist);
						if(max < dist)
						{
							max = dist;
						}
						if(tag == true)
						{
							if(dist < limit)
							{
								Red15[Rcount].x=cross_point.x;
								Red15[Rcount].y=cross_point.y;
								Red15[Rcount++].z=cross_point.z;
								xv[num]=cross_point.x;
								yv[num]=cross_point.y;
								zv[num]= cross_point.z;
								distRR+=dist;
								x_tmp += cross_point.x;
								y_tmp += cross_point.y;
								z_tmp += cross_point.z;
								num ++;
								//distance = dist;
							}
						}
					}
				}
				
			}
		}
		Red15[Rcount].x=0;
		Red15[Rcount].y=0;
		Red15[Rcount].z=0;
		if(num!=0)
		{
			int numnew=0;
			double numd = num;
			r_p.x = x_tmp / numd;
			r_p.y = y_tmp / numd;
			r_p.z = z_tmp / numd;
			x_tmp=0;
			y_tmp=0;
			z_tmp=0;
			for(int i =0;i<num;i++)
			{
				int RoundTest=0;//new
			
				{
					//**new

					for(int j=0;j<num;j++)
					{
						double tmpdist=sqrt((xv[i]-xv[j])*(xv[i]-xv[j])+(yv[i]-yv[j])*(yv[i]-yv[j])+(zv[i]-zv[j])*(zv[i]-zv[j]));
						if(tmpdist<=rlimit)
							RoundTest++;
					}
					if(RoundTest>(num/3))
					{
						x_tmp+=xv[i];
						y_tmp+=yv[i];
						z_tmp+=zv[i];
						numnew++;
					}
					//new**
					
					
				}
			}
			if(numnew==0)
			{
				num1=0;
				r_p.x = 0;
				r_p.y = 0;
				r_p.z = 0;
			}
			else
			{
				r_p.x = x_tmp / numnew;
				r_p.y = y_tmp / numnew;
				r_p.z = z_tmp / numnew;
			}
			num1=numnew;
			distRR=distRR/numd;
			*Rdist=distRR;
		}
		else
		{
			r_p.x = 0;
			r_p.y = 0;
			r_p.z = 0;
		}
	
	}





	if(1)
	{
		double xv[30],yv[30],zv[30];
		double x_tmp,y_tmp,z_tmp;
		int num = 0;
		x_tmp = 0;y_tmp = 0;z_tmp = 0;
		_2D_Point p2d1;
		_2D_Point p2d2;
		_3D_Point cross_point;
		double dist;
		bool tag;
		for(int i = 1; i < 20 ; i ++)
		{
			if(datas[i].num_g>1)
			{
				p2d1.x = datas[i].x_g;
				p2d1.y = datas[i].y_g;

				for(int j = 0;j<i ; j ++)
				{
					if(datas[j].num_g>1)
					{
						
						
						p2d2.x = datas[j].x_g;
						p2d2.y = datas[j].y_g;
						
						tag = GetLineCross(&(Roter[i]),&(Mv[i]),&(gp[i]),&(gd[i]),&p2d1,
							&(Roter[j]),&(Mv[j]),&(gp[j]),&(gd[j]),&p2d2,
							&cross_point, &dist);
						if(tag == true)
						{
							if(dist < limit)
							{
								xv[num]=cross_point.x;
								yv[num]=cross_point.y;
								zv[num]= cross_point.z;
								x_tmp += cross_point.x;
								y_tmp += cross_point.y;
								z_tmp += cross_point.z;
								num ++;
							}
						}
					}
				}
				
			}
		}
		if(num!=0)
		{
			int numnew=0;
			double numd = num;
			g_p.x = x_tmp / numd;
			g_p.y = y_tmp / numd;
			g_p.z = z_tmp / numd;
			x_tmp=0;
			y_tmp=0;
			z_tmp=0;
			for(int i =0;i<num;i++)
			{
				int RoundTest=0;//new
				
				{
				//**new

					for(int j=0;j<num;j++)
					{
						double tmpdist=sqrt((xv[i]-xv[j])*(xv[i]-xv[j])+(yv[i]-yv[j])*(yv[i]-yv[j])+(zv[i]-zv[j])*(zv[i]-zv[j]));
						if(tmpdist<=rlimit)
							RoundTest++;
					}
					if(RoundTest>(num/3))
					{
						x_tmp+=xv[i];
						y_tmp+=yv[i];
						z_tmp+=zv[i];
						numnew++;
					}
					//new**
				}
			}
			if(numnew==0)
			{
				num1=0;
				g_p.x = 0;
				g_p.y = 0;
				g_p.z = 0;
			}
			else
			{
				g_p.x = x_tmp / numnew;
				g_p.y = y_tmp / numnew;
				g_p.z = z_tmp / numnew;
			}
			num2=numnew;
		}
		else
		{
			g_p.x = 0;
			g_p.y = 0;
			g_p.z = 0;
		}
	
	}





	if(1)
	{
		double xv[30],yv[30],zv[30];
		double x_tmp,y_tmp,z_tmp;
		int num = 0;
		x_tmp = 0;y_tmp = 0;z_tmp = 0;
		_2D_Point p2d1;
		_2D_Point p2d2;
		_3D_Point cross_point;
		double dist;
		bool tag;
		for(int i = 1; i < 20 ; i ++)
		{
			if(datas[i].num_b>1)
			{
				p2d1.x = datas[i].x_b;
				p2d1.y = datas[i].y_b;

				for(int j = 0;j<i ; j ++)
				{
					if(datas[j].num_b>1)
					{
						
						
						p2d2.x = datas[j].x_b;
						p2d2.y = datas[j].y_b;
						
						tag = GetLineCross(&(Roter[i]),&(Mv[i]),&(gp[i]),&(gd[i]),&p2d1,
							&(Roter[j]),&(Mv[j]),&(gp[j]),&(gd[j]),&p2d2,
							&cross_point, &dist);
						if(tag == true)
						{
							if(dist < limit)
							{
								x_tmp += cross_point.x;
								y_tmp += cross_point.y;
								z_tmp += cross_point.z;
								xv[num]=cross_point.x;
								yv[num]=cross_point.y;
								zv[num]= cross_point.z;
								num ++;
							}
						}
					}
				}
				
			}
		}
		if(num!=0)
		{
			int numnew=0;
			double numd = num;
			b_p.x = x_tmp / numd;
			b_p.y = y_tmp / numd;
			b_p.z = z_tmp / numd;
			x_tmp=0;
			y_tmp=0;
			z_tmp=0;
			for(int i =0;i<num;i++)
			{
					int RoundTest=0;//new
				
				{
					for(int j=0;j<num;j++)
					{
						double tmpdist=sqrt((xv[i]-xv[j])*(xv[i]-xv[j])+(yv[i]-yv[j])*(yv[i]-yv[j])+(zv[i]-zv[j])*(zv[i]-zv[j]));
						if(tmpdist<=rlimit)
							RoundTest++;
					}
					if(RoundTest>(num/3))
					{
						x_tmp+=xv[i];
						y_tmp+=yv[i];
						z_tmp+=zv[i];
						numnew++;
					}
					//new**
				}
			}
			if(numnew==0)
			{
				num1=0;
					b_p.x = 0;
			b_p.y = 0;
			b_p.z = 0;
			}
			else
			{
				b_p.x = x_tmp / numnew;
				b_p.y = y_tmp / numnew;
				b_p.z = z_tmp / numnew;
			}
			num3=numnew;
		}
		else
		{
			b_p.x = 0;
			b_p.y = 0;
			b_p.z = 0;
		}
	
	}

	
	red_p->x = r_p.x;
	red_p->y = r_p.y;
	red_p->z = r_p.z;

	
	green_p->x = g_p.x;
	green_p->y = g_p.y;
	green_p->z = g_p.z;


	
	blue_p->x = b_p.x;
	blue_p->y = b_p.y;
	blue_p->z = b_p.z;
	*Rdist = num1+num2+num3;
	if(num1==0||num2==0||num3==0)
	{
		return false;
	}



	bool rett =  GetPosePosition(&r_p,&g_p,&b_p,pose,position_p);
	if(rett)
	{
		gesture gs;
		gs.P1_x = r_p.x;
		gs.P1_y = r_p.y;
		gs.P1_z = r_p.z;
		gs.P2_x = g_p.x;
		gs.P2_y = g_p.y;
		gs.P2_z = g_p.z;
		gs.P3_x = b_p.x;
		gs.P3_y = b_p.y;
		gs.P3_z = b_p.z;
		
		gs.calNor();
		gs.calFirst();
		gs.calSecond();
		gs.calThird();
		ang[0] = gs.First_rotate;
		ang[1] = gs.Second_rotate;
		ang[2] = gs.Third_rotate;
	}

	*Rdist = num1;
	return rett;
	
}




bool CGeometry::GetPosePosition(CamerasData datas[20],RoterMatrix Roter[20],MoveVecter Mv[20],
		IntrinsicPara gp[20],DistortionPara gd[20],
		double pose[3][3],p_3D_Point position_p,
		p_3D_Point red_p,p_3D_Point green_p,p_3D_Point  blue_p)
{
	_3D_Point r_p;
	_3D_Point g_p;
	_3D_Point b_p;


	int limit = 100;

	if(1)
	{
		double x_tmp,y_tmp,z_tmp;
		int num = 0;
		x_tmp = 0;y_tmp = 0;z_tmp = 0;
		_2D_Point p2d1;
		_2D_Point p2d2;
		_3D_Point cross_point;
		double dist;
		bool tag;
		for(int i = 1; i < 20 ; i ++)
		{
			if(datas[i].num_r>1)
			{
				p2d1.x = datas[i].x_r;
				p2d1.y = datas[i].y_r;

				for(int j = 0;j<i ; j ++)
				{
					if(datas[j].num_r>1)
					{
						
						
						p2d2.x = datas[j].x_r;
						p2d2.y = datas[j].y_r;
						
						tag = GetLineCross(&(Roter[i]),&(Mv[i]),&(gp[i]),&(gd[i]),&p2d1,
							&(Roter[j]),&(Mv[j]),&(gp[j]),&(gd[j]),&p2d2,
							&cross_point, &dist);
						if(tag == true)
						{
							if(dist < limit)
							{
								x_tmp += cross_point.x;
								y_tmp += cross_point.y;
								z_tmp += cross_point.z;
								num ++;
							}
						}
					}
				}
				
			}
		}
		if(num==0)
		{
			return false;
		}
		double numd = num;
		r_p.x = x_tmp / numd;
		r_p.y = y_tmp / numd;
		r_p.z = z_tmp / numd;
	}





	if(1)
	{
		double x_tmp,y_tmp,z_tmp;
		int num = 0;
		x_tmp = 0;y_tmp = 0;z_tmp = 0;
		_2D_Point p2d1;
		_2D_Point p2d2;
		_3D_Point cross_point;
		double dist;
		bool tag;
		for(int i = 1; i < 20 ; i ++)
		{
			if(datas[i].num_g>1)
			{
				p2d1.x = datas[i].x_g;
				p2d1.y = datas[i].y_g;

				for(int j = 0;j<i ; j ++)
				{
					if(datas[j].num_g>1)
					{
						
						
						p2d2.x = datas[j].x_g;
						p2d2.y = datas[j].y_g;
						
						tag = GetLineCross(&(Roter[i]),&(Mv[i]),&(gp[i]),&(gd[i]),&p2d1,
							&(Roter[j]),&(Mv[j]),&(gp[j]),&(gd[j]),&p2d2,
							&cross_point, &dist);
						if(tag == true)
						{
							if(dist < limit)
							{
								x_tmp += cross_point.x;
								y_tmp += cross_point.y;
								z_tmp += cross_point.z;
								num ++;
							}
						}
					}
				}
				
			}
		}
		if(num==0)
		{
			return false;
		}
		double numd = num;
		g_p.x = x_tmp / numd;
		g_p.y = y_tmp / numd;
		g_p.z = z_tmp / numd;
	}





	if(1)
	{
		double x_tmp,y_tmp,z_tmp;
		int num = 0;
		x_tmp = 0;y_tmp = 0;z_tmp = 0;
		_2D_Point p2d1;
		_2D_Point p2d2;
		_3D_Point cross_point;
		double dist;
		bool tag;
		for(int i = 1; i < 20 ; i ++)
		{
			if(datas[i].num_b>1)
			{
				p2d1.x = datas[i].x_b;
				p2d1.y = datas[i].y_b;

				for(int j = 0;j<i ; j ++)
				{
					if(datas[j].num_b>1)
					{
						
						
						p2d2.x = datas[j].x_b;
						p2d2.y = datas[j].y_b;
						
						tag = GetLineCross(&(Roter[i]),&(Mv[i]),&(gp[i]),&(gd[i]),&p2d1,
							&(Roter[j]),&(Mv[j]),&(gp[j]),&(gd[j]),&p2d2,
							&cross_point, &dist);
						if(tag == true)
						{
							if(dist < limit)
							{
								x_tmp += cross_point.x;
								y_tmp += cross_point.y;
								z_tmp += cross_point.z;
								num ++;
							}
						}
					}
				}
				
			}
		}
		if(num==0)
		{
			return false;
		}
		double numd = num;
		b_p.x = x_tmp / numd;
		b_p.y = y_tmp / numd;
		b_p.z = z_tmp / numd;
	}

	red_p->x = r_p.x;
	red_p->y = r_p.y;
	red_p->z = r_p.z;

	
	green_p->x = g_p.x;
	green_p->y = g_p.y;
	green_p->z = g_p.z;


	
	blue_p->x = b_p.x;
	blue_p->y = b_p.y;
	blue_p->z = b_p.z;

	return GetPosePosition(&r_p,&g_p,&b_p,pose,position_p);

}



bool CGeometry::GetPosePosition(CamerasData datas[20],RoterMatrix Roter[20],MoveVecter Mv[20],
		IntrinsicPara gp[20],DistortionPara gd[20],
		double pose[3][3],p_3D_Point position_p)
{
	_3D_Point r_p;
	_3D_Point g_p;
	_3D_Point b_p;



	if(1)
	{
		double x_tmp,y_tmp,z_tmp;
		int num = 0;
		x_tmp = 0;y_tmp = 0;z_tmp = 0;
		_2D_Point p2d1;
		_2D_Point p2d2;
		_3D_Point cross_point;
		double dist;
		bool tag;
		for(int i = 1; i < 20 ; i ++)
		{
			if(datas[i].num_r>1)
			{
				p2d1.x = datas[i].x_r;
				p2d1.y = datas[i].y_r;

				for(int j = 0;j<i ; j ++)
				{
					if(datas[j].num_r>1)
					{
						
						
						p2d2.x = datas[j].x_r;
						p2d2.y = datas[j].y_r;
						
						tag = GetLineCross(&(Roter[i]),&(Mv[i]),&(gp[i]),&(gd[i]),&p2d1,
							&(Roter[j]),&(Mv[j]),&(gp[j]),&(gd[j]),&p2d2,
							&cross_point, &dist);
						if(tag == true)
						{
							if(dist < 500)
							{
								x_tmp += cross_point.x;
								y_tmp += cross_point.y;
								z_tmp += cross_point.z;
								num ++;
							}
						}
					}
				}
				
			}
		}
		if(num==0)
		{
			return false;
		}
		double numd = num;
		r_p.x = x_tmp / numd;
		r_p.y = y_tmp / numd;
		r_p.z = z_tmp / numd;
	}





	if(1)
	{
		double x_tmp,y_tmp,z_tmp;
		int num = 0;
		x_tmp = 0;y_tmp = 0;z_tmp = 0;
		_2D_Point p2d1;
		_2D_Point p2d2;
		_3D_Point cross_point;
		double dist;
		bool tag;
		for(int i = 1; i < 20 ; i ++)
		{
			if(datas[i].num_g>1)
			{
				p2d1.x = datas[i].x_g;
				p2d1.y = datas[i].y_g;

				for(int j = 0;j<i ; j ++)
				{
					if(datas[j].num_g>1)
					{
						
						
						p2d2.x = datas[j].x_g;
						p2d2.y = datas[j].y_g;
						
						tag = GetLineCross(&(Roter[i]),&(Mv[i]),&(gp[i]),&(gd[i]),&p2d1,
							&(Roter[j]),&(Mv[j]),&(gp[j]),&(gd[j]),&p2d2,
							&cross_point, &dist);
						if(tag == true)
						{
							if(dist < 500)
							{
								x_tmp += cross_point.x;
								y_tmp += cross_point.y;
								z_tmp += cross_point.z;
								num ++;
							}
						}
					}
				}
				
			}
		}
		if(num==0)
		{
			return false;
		}
		double numd = num;
		g_p.x = x_tmp / numd;
		g_p.y = y_tmp / numd;
		g_p.z = z_tmp / numd;
	}





	if(1)
	{
		double x_tmp,y_tmp,z_tmp;
		int num = 0;
		x_tmp = 0;y_tmp = 0;z_tmp = 0;
		_2D_Point p2d1;
		_2D_Point p2d2;
		_3D_Point cross_point;
		double dist;
		bool tag;
		for(int i = 1; i < 20 ; i ++)
		{
			if(datas[i].num_b>1)
			{
				p2d1.x = datas[i].x_b;
				p2d1.y = datas[i].y_b;

				for(int j = 0;j<i ; j ++)
				{
					if(datas[j].num_b>1)
					{
						
						
						p2d2.x = datas[j].x_b;
						p2d2.y = datas[j].y_b;
						
						tag = GetLineCross(&(Roter[i]),&(Mv[i]),&(gp[i]),&(gd[i]),&p2d1,
							&(Roter[j]),&(Mv[j]),&(gp[j]),&(gd[j]),&p2d2,
							&cross_point, &dist);
						if(tag == true)
						{
							if(dist < 500)
							{
								x_tmp += cross_point.x;
								y_tmp += cross_point.y;
								z_tmp += cross_point.z;
								num ++;
							}
						}
					}
				}
				
			}
		}
		if(num==0)
		{
			return false;
		}
		double numd = num;
		b_p.x = x_tmp / numd;
		b_p.y = y_tmp / numd;
		b_p.z = z_tmp / numd;
	}


	return GetPosePosition(&r_p,&g_p,&b_p,pose,position_p);

}


bool CGeometry::GetPosePosition(p_3D_Point red_p,p_3D_Point green_p,p_3D_Point  blue_p,double pose[3][3],p_3D_Point position_p)
{

	double p_red[3];
	p_red[0] = red_p->x;
	p_red[1] = red_p->y;
	p_red[2] = red_p->z;

	double p_green[3];

	p_green[0] = green_p->x;
	p_green[1] = green_p->y;
	p_green[2] = green_p->z;

	double p_blue[3];
	
	p_blue[0] = blue_p->x;
	p_blue[1] = blue_p->y;
	p_blue[2] = blue_p->z;

	double position[3];
	double span_r_g;
	double middle[3];

	span_r_g = sqrt( 
		(p_red[0] -p_green[0] ) * (p_red[0] -p_green[0] ) + 
		(p_red[1] -p_green[1] ) * (p_red[1] -p_green[1] ) + 
		(p_red[2] -p_green[2] ) * (p_red[2] -p_green[2] )  
		);
	if(Is_Zero(span_r_g) == true)
	{
		return false;
	}

	middle[0] = (p_red[0] + p_green[0] ) /2.0;
	middle[1] = (p_red[1] + p_green[1] ) /2.0;
	middle[2] = (p_red[2] + p_green[2] ) /2.0;
	double span_m_b;
	span_m_b = sqrt( 
		(middle[0] -p_blue[0] ) * (middle[0] -p_blue[0] ) + 
		(middle[1] -p_blue[1] ) * (middle[1] -p_blue[1] ) + 
		(middle[2] -p_blue[2] ) * (middle[2] -p_blue[2] )  
		);

	if(Is_Zero(span_r_g) == true)
	{
		return false;
	}

	position[0] = middle[0];
	position[1] = middle[1];
	position[2] = middle[2];


	position_p->x = position[0];
	position_p->y = position[1];
	position_p->z = position[2];


	pose[0][0] = (middle[0] -p_blue[0] ) / span_m_b;
	pose[0][1] = (middle[1] -p_blue[1] ) / span_m_b;
	pose[0][2] = (middle[2] -p_blue[2] ) / span_m_b;



	pose[1][0] = (p_green[0] -p_red[0] ) / span_r_g;
	pose[1][1] = (p_green[1] -p_red[1] ) / span_r_g;
	pose[1][2] = (p_green[2] -p_red[2] ) / span_r_g;



	double a1 =  pose[0][0];
	double b1 =  pose[0][1];
	double c1 =  pose[0][2];


	double a2 =  pose[1][0];
	double b2 =  pose[1][1];
	double c2 =  pose[1][2];

	pose[2][0] = b1*c2-b2*c1 ;
	pose[2][1] = c1*a2-a1*c2;
	pose[2][2] = a1*b2-a2*b1;

	return true;
}



bool CGeometry::GetLineCross(pRoterMatrix Roter1,pMoveVecter Mv1,pIntrinsicPara gp1,pDistortionPara gd1,p_2D_Point p2d1,
							pRoterMatrix Roter2,pMoveVecter Mv2,pIntrinsicPara gp2,pDistortionPara gd2,p_2D_Point p2d2,
							p_3D_Point cross_point, double *dist)
{
	_3D_Line pl1;
	_3D_Line pl2;

	GetLine( Roter1, Mv1, gp1, gd1,p2d1,&pl1);
	GetLine( Roter2, Mv2, gp2, gd2,p2d2,&pl2);
	return GetLineCross(&pl1,&pl2,cross_point, dist);
}

void CGeometry::SolveLinePoint_Z(p_3D_Line line,p_3D_Point retp,double z)
{
	_3D_Point v;
	v.x = line->p2.x - line->p1.x;
	v.y = line->p2.y - line->p1.y;
	v.z = line->p2.z - line->p1.z;
	if(Is_Zero(v.z))
	{
		return ;
	}
	double s;
	s = line->p1.z + z;
	retp->x = line->p1.x;
	retp->y = line->p1.y;
	retp->z = line->p1.z;
	if(Is_Zero(s))
	{
		return ;
	}
	else
	{
		retp->z -= (s/v.z) *  v.z;
		retp->x -= (s/v.z) *  v.x;
		retp->y -= (s/v.z) *  v.y;
	}
}

void CGeometry::SolveLinePoint_Z0(p_3D_Line line,p_3D_Point retp)
{
	_3D_Point v;
	v.x = line->p2.x - line->p1.x;
	v.y = line->p2.y - line->p1.y;
	v.z = line->p2.z - line->p1.z;
	if(Is_Zero(v.z))
	{
		return ;
	}
	double s;
	s = line->p1.z;
	retp->x = line->p1.x;
	retp->y = line->p1.y;
	retp->z = line->p1.z;
	if(Is_Zero(s))
	{
		return ;
	}
	else
	{
		retp->z -= (s/v.z) *  v.z;
		retp->x -= (s/v.z) *  v.x;
		retp->y -= (s/v.z) *  v.y;
	}
}




bool CGeometry::Is_Zero(double v)
{
	if(v < 0.000000001 && v>-0.000000001 )
	{
		return true;
	}
	return false;
}



bool CGeometry::SaveIntrinsicPara(char * path,pIntrinsicPara gp,pDistortionPara gd)
{
	FILE * fp;
	fp = fopen(path,"wb");
	if(!fp)
	{
		return false;
	}
	fwrite(&gp->cx,sizeof(double),1,fp);
	fwrite(&gp->cy,sizeof(double),1,fp);
	fwrite(&gp->fx,sizeof(double),1,fp);
	fwrite(&gp->fy,sizeof(double),1,fp);

	fwrite(&gd->k1,sizeof(double),1,fp);
	fwrite(&gd->k2,sizeof(double),1,fp);
	
	fwrite(&gd->p1,sizeof(double),1,fp);
	fwrite(&gd->p2,sizeof(double),1,fp);

	fclose(fp);
	return true;
}


bool CGeometry::LoadIntrinsicPara(char * path,pIntrinsicPara gp,pDistortionPara gd)
{
	FILE * fp;
	fp = fopen(path,"rb");
	if(!fp)
	{
		return false;
	}
	fread(&gp->cx,sizeof(double),1,fp);
	fread(&gp->cy,sizeof(double),1,fp);
	fread(&gp->fx,sizeof(double),1,fp);
	fread(&gp->fy,sizeof(double),1,fp);

	fread(&gd->k1,sizeof(double),1,fp);
	fread(&gd->k2,sizeof(double),1,fp);
	
	fread(&gd->p1,sizeof(double),1,fp);
	fread(&gd->p2,sizeof(double),1,fp);

	fclose(fp);
	return true;
}




bool CGeometry::SaveExtrinsicPara(char * path,pRoterMatrix Roter,pMoveVecter Mv)
{
	FILE * fp;
	fp = fopen(path,"wb");
	if(!fp)
	{
		return false;
	}

	fwrite(&Roter->r11,sizeof(double),1,fp);
	fwrite(&Roter->r12,sizeof(double),1,fp);
	fwrite(&Roter->r13,sizeof(double),1,fp);
	fwrite(&Roter->r21,sizeof(double),1,fp);
	fwrite(&Roter->r22,sizeof(double),1,fp);
	fwrite(&Roter->r23,sizeof(double),1,fp);
	fwrite(&Roter->r31,sizeof(double),1,fp);
	fwrite(&Roter->r32,sizeof(double),1,fp);
	fwrite(&Roter->r33,sizeof(double),1,fp);

	
	fwrite(&Mv->tx,sizeof(double),1,fp);
	fwrite(&Mv->ty,sizeof(double),1,fp);
	fwrite(&Mv->tz,sizeof(double),1,fp);

	fclose(fp);
	return true;
}


bool CGeometry::LoadExtrinsicPara(char * path,pRoterMatrix Roter,pMoveVecter Mv)
{
	FILE * fp;
	fp = fopen(path,"rb");
	if(!fp)
	{
		return false;
	}

	fread(&Roter->r11,sizeof(double),1,fp);
	fread(&Roter->r12,sizeof(double),1,fp);
	fread(&Roter->r13,sizeof(double),1,fp);
	fread(&Roter->r21,sizeof(double),1,fp);
	fread(&Roter->r22,sizeof(double),1,fp);
	fread(&Roter->r23,sizeof(double),1,fp);
	fread(&Roter->r31,sizeof(double),1,fp);
	fread(&Roter->r32,sizeof(double),1,fp);
	fread(&Roter->r33,sizeof(double),1,fp);

	
	fread(&Mv->tx,sizeof(double),1,fp);
	fread(&Mv->ty,sizeof(double),1,fp);
	fread(&Mv->tz,sizeof(double),1,fp);

	fclose(fp);
	return true;
}



void CGeometry::UnDistort(pIntrinsicPara gp,pDistortionPara gd,p_2D_Point src,p_2D_Point dst)
{

/*//////////////////////////////////////////////////////////////////////////////////////////////
	[x]     [X]
	[y] = R*[Y] + t
	[z]     [Z]

	x' = x/z
	y' = y/z

	x" = x'*(1 + k1r2 + k2r4) + 2*p1x'*y' + p2(r2+2*x'2)
	y" = y'*(1 + k1r2 + k2r4) + p1(r2+2*y'2) + 2*p2*x'*y'
	where r2 = x'2+y'2

	u = fx*x" + cx
	v = fy*y" + cy
	k1, k2 are radial distortion coefficients, p1, p2 are tangential distortion coefficients.
/*///////////////////////////////////////////////////////////////////////////////////////////////

	double u_image = src->x;
	double v_image = src->y;

	double x_tmp2 = ( u_image - gp->cx )/gp->fx;
	double y_tmp2 = ( v_image - gp->cy )/gp->fy;
	double r2 = x_tmp2*x_tmp2 + y_tmp2*y_tmp2;
/*/
x" = x'*(1 + k1r2 + k2r4) + 2*p1x'*y' + p2(r2+2*x'2)
x" = x'+  x'(k1r2 + k2r4) + 2*p1x'*y' + p2(r2+2*x'2)
x' =  x" - x'(k1r2 + k2r4) - 2*p1x'*y' - p2(r2+2*x'2);
because the x"  y" is near the x' y'
so use the x" and y" replace the x' y'in the equation right
x' =  x" - x"(k1r2 + k2r4) - 2*p1x"*y" - p2(r2+2*x"2);
y' = y" - y"*(k1r2 + k2r4) - p1(r2+2*y"2) - 2*p2*x"*y"/*/

	double x_tmp1 = x_tmp2 - x_tmp2 * (gd->k1*r2 + gd->k2*r2*r2) 
		- ( 2 *  gd->p1 *  x_tmp2 *  y_tmp2)
		- (gd->p2*(r2 + 2 * x_tmp2 * x_tmp2 ) );
	double y_tmp1 = y_tmp2 - y_tmp2 * (gd->k1*r2 + gd->k2*r2*r2) 
		-  ( 2 *  gd->p2 *  x_tmp2 *  y_tmp2)
		- (gd->p1*(r2 + 2 * y_tmp2 * y_tmp2 ) );

//consider the (x_tmp1 y_tmp1 )is the correct point then get the distort point (x_tmp3 y_tmp3)
	r2 = x_tmp1*x_tmp1 + y_tmp1*y_tmp1;

	double x_tmp3 = x_tmp1 + x_tmp1 * (gd->k1*r2 + gd->k2*r2*r2) 
		+ ( 2 *  gd->p1 *  x_tmp1 *  y_tmp1)
		+ (gd->p2*(r2 + 2 * x_tmp1 * x_tmp1 ) );
	double y_tmp3 = y_tmp1 + y_tmp1 * (gd->k1*r2 + gd->k2*r2*r2) 
		+  ( 2 *  gd->p2 *  x_tmp1 *  y_tmp1)
		+ (gd->p1*(r2 + 2 * y_tmp1 * y_tmp1 ) );
//the error between (x_tmp1 y_tmp1 ) and (x_tmp3 y_tmp3) is almost the error between (x_tmp4 y_tmp4) and (x_tmp2 y_tmp2)
	double x_tmp4 = x_tmp2 + (x_tmp1 - x_tmp3);
	double y_tmp4 = y_tmp2 + (y_tmp1 - y_tmp3);
//consider the (x_tmp4 y_tmp4 )is the correct point then get the distort point (x_tmp5 y_tmp5)
	r2 = x_tmp4*x_tmp4 + y_tmp4*y_tmp4;

	double x_tmp5 = x_tmp4 + x_tmp4 * (gd->k1*r2 + gd->k2*r2*r2) 
		+ ( 2 *  gd->p1 *  x_tmp4 *  y_tmp4)
		+ (gd->p2*(r2 + 2 * x_tmp4 * x_tmp4 ) );
	double y_tmp5 = y_tmp4 + y_tmp4 * (gd->k1*r2 + gd->k2*r2*r2) 
		+  ( 2 *  gd->p2 *  x_tmp4 *  y_tmp4)
		+ (gd->p1*(r2 + 2 * y_tmp4 * y_tmp4 ) );

	double x_tmp6 = x_tmp2 + (x_tmp4 - x_tmp5);
	double y_tmp6 = y_tmp2 + (y_tmp4 - y_tmp5); 
	
	dst->x = x_tmp6 * gp->fx + gp->cx;
	dst->y = y_tmp6 * gp->fy + gp->cy;
/*/	
	dst->x = x_tmp6;
	dst->y = y_tmp6;/*/
}


void CGeometry::Distort(pIntrinsicPara gp,pDistortionPara gd,p_2D_Point src,p_2D_Point dst)
{


	double u_image = src->x;
	double v_image = src->y;

	double x_tmp2 = ( u_image - gp->cx )/gp->fx;
	double y_tmp2 = ( v_image - gp->cy )/gp->fy;
	double r2 = x_tmp2*x_tmp2 + y_tmp2*y_tmp2;

	double x_tmp1 = x_tmp2 + x_tmp2 * (gd->k1*r2 + gd->k2*r2*r2) 
		+ ( 2 *  gd->p1 *  x_tmp2 *  y_tmp2)
		+ (gd->p2*(r2 + 2 * x_tmp2 * x_tmp2 ) );
	double y_tmp1 = y_tmp2 - y_tmp2 * (gd->k1*r2 + gd->k2*r2*r2) 
		+  ( 2 *  gd->p2 *  x_tmp2 *  y_tmp2)
		+ (gd->p1*(r2 + 2 * y_tmp2 * y_tmp2 ) );

	dst->x = x_tmp1 * gp->fx + gp->cx;
	dst->y = y_tmp1 * gp->fy + gp->cy;
}


bool CGeometry::GetLineCross(p_3D_Line line1,p_3D_Line line2,p_3D_Point cross_point, double * dist)
{
	

	double x11 = line1->p1.x;
	double x12 = line1->p2.x;
	double y11 = line1->p1.y;
	double y12 = line1->p2.y;
	double z11 = line1->p1.z;
	double z12 = line1->p2.z;


	double x21 = line2->p1.x;
	double x22 = line2->p2.x;
	double y21 = line2->p1.y;
	double y22 = line2->p2.y;
	double z21 = line2->p1.z;
	double z22 = line2->p2.z;

	///判定直线1上两点是否重合
	if(Is_Zero(x11-x12) && Is_Zero(y11-y12) && Is_Zero(z11-z12))
	{
		return false;
	}
	///判定直线2上两点是否重合
	if(Is_Zero(x21-x22) && Is_Zero(y21-y22) && Is_Zero(z21-z22))
	{
		return false;
	}
	

	double L1P1[3];
	double L1P2[3];
	double L2P1[3];
	double L2P2[3];

	L1P1[0] = line1->p1.x;
	L1P1[1] = line1->p1.y;
	L1P1[2] = line1->p1.z;

	L1P2[0] = line1->p2.x;
	L1P2[1] = line1->p2.y;
	L1P2[2] = line1->p2.z;


	L2P1[0] = line2->p1.x;
	L2P1[1] = line2->p1.y;
	L2P1[2] = line2->p1.z;

	L2P2[0] = line2->p2.x;
	L2P2[1] = line2->p2.y;
	L2P2[2] = line2->p2.z;

	double v1[3];
	double v2[3];
	double lp[3];
	for(int i = 0; i < 3; i ++)
	{
		v1[i] = L1P1[i] - L1P2[i];
		v2[i] = L2P1[i] - L2P2[i];
		lp[i] = v1[i] + v2[i];
	}

	lp[0] = sqrt( (lp[0] * lp[0] + lp[1] * lp[1] + lp[2] * lp[2] ));
	lp[1] = sqrt( (v1[0] * v1[0] + v1[1] * v1[1] + v1[2] * v1[2] ));
	lp[2] = sqrt( (v2[0] * v2[0] + v2[1] * v2[1] + v2[2] * v2[2] ));

	double tmp;
	for(i = 0; i < 3 ; i ++)
	{
		for(int j=0; j < 2 ; j ++)
		{
			if(lp[j]>lp[j+1])
			{
				tmp = lp[j] ; 
				lp[j] = lp[j+1];
				lp[j+1] = tmp;
			}
		}
	}
	tmp = lp[0] + lp[1] - lp[2];
	///利用三角形短的两边长之和大于第三边判定两条直线是否近似平行
	if(Is_Zero(tmp))
	{
		return false;
	}



	double v1x = v1[0];
	double v1y = v1[1];
	double v1z = v1[2];

	double v2x = v2[0];
	double v2y = v2[1];
	double v2z = v2[2];

	double a1 =  (x11 - x12)*v1x + (y11 - y12)*v1y + (z11 - z12)*v1z; 
	double b1 =  ((x21 - x22)*v1x + (y21 - y22)*v1y + (z21 - z22)*v1z)*(-1); 
	double c1 = (x22 - x12)*v1x + (y22 - y12)*v1y + (z22 - z12)*v1z;

	double a2 =  (x11 - x12)*v2x + (y11 - y12)*v2y + (z11 - z12)*v2z; 
	double b2 =  ((x21 - x22)*v2x + (y21 - y22)*v2y + (z21 - z22)*v2z)*(-1); 
	double c2 = (x22 - x12)*v2x + (y22 - y12)*v2y + (z22 - z12)*v2z;



	double t1 = 
		(c1*b2 - c2*b1)
		/
		(a1*b2 - a2*b1);

	double t2 = 
		(c1*a2 - c2*a1)
		/
		(b1*a2 - b2*a1);

	double p1[3];
	double p2[3];


	p1[0] = t1* L1P1[0] + L1P2[0] - t1*L1P2[0];
	p1[1] = t1* L1P1[1] + L1P2[1] - t1*L1P2[1];
	p1[2] = t1* L1P1[2] + L1P2[2] - t1*L1P2[2];

	p2[0] = t2* L2P1[0] + L2P2[0] - t2*L2P2[0];
	p2[1] = t2* L2P1[1] + L2P2[1] - t2*L2P2[1];
	p2[2] = t2* L2P1[2] + L2P2[2] - t2*L2P2[2];


	cross_point->x = (p1[0] + p2[0])/2;
	cross_point->y = (p1[1] + p2[1])/2;
	cross_point->z = (p1[2] + p2[2])/2;

	*dist = sqrt(
		(p1[0]-p2[0])*(p1[0]-p2[0]) + 
		(p1[1]-p2[1])*(p1[1]-p2[1]) +
		(p1[2]-p2[2])*(p1[2]-p2[2]) );

	return true;
}

void CGeometry::CoordinateA2B(pRoterMatrix cA2cB_r,pMoveVecter cA2cb_t,p_3D_Point _3Dp_inA,p_3D_Point _3Dp_inB)
{
	_3Dp_inB->x = cA2cB_r->r11 * _3Dp_inA->x + cA2cB_r->r12 * _3Dp_inA->y + cA2cB_r->r13 * _3Dp_inA->z + cA2cb_t->tx;
	_3Dp_inB->y = cA2cB_r->r21 * _3Dp_inA->x + cA2cB_r->r22 * _3Dp_inA->y + cA2cB_r->r23 * _3Dp_inA->z + cA2cb_t->ty;
	_3Dp_inB->z = cA2cB_r->r31 * _3Dp_inA->x + cA2cB_r->r32 * _3Dp_inA->y + cA2cB_r->r33 * _3Dp_inA->z + cA2cb_t->tz;
}


void CGeometry::CoordinateB2A(pRoterMatrix cA2cB_r,pMoveVecter cA2cb_t,p_3D_Point _3Dp_inA,p_3D_Point _3Dp_inB)
{
	RoterMatrix r;
	r.r11 = cA2cB_r->r11;
	r.r12 = cA2cB_r->r21;
	r.r13 = cA2cB_r->r31;

	r.r21 = cA2cB_r->r12;
	r.r22 = cA2cB_r->r22;
	r.r23 = cA2cB_r->r32;

	r.r31 = cA2cB_r->r13;
	r.r32 = cA2cB_r->r23;
	r.r33 = cA2cB_r->r33;

	_3D_Point bt;
	bt.x = _3Dp_inB->x - cA2cb_t->tx;
	bt.y = _3Dp_inB->y - cA2cb_t->ty;
	bt.z = _3Dp_inB->z - cA2cb_t->tz;

	_3Dp_inA->x = bt.x * r.r11 + bt.y * r.r12 + bt.z * r.r13;
	_3Dp_inA->y = bt.x * r.r21 + bt.y * r.r22 + bt.z * r.r23;
	_3Dp_inA->z = bt.x * r.r31 + bt.y * r.r32 + bt.z * r.r33;
}

void CGeometry::GetLine(pRoterMatrix Roter,pMoveVecter Mv,pIntrinsicPara gp,pDistortionPara gd,p_2D_Point p2d,p_3D_Line pl)
{
	_2D_Point  _2d;
	_2d.x = p2d->x;
	_2d.y = p2d->y;

	_2D_Point  ud_2d;

	UnDistort(gp,gd,&_2d,&ud_2d);

	_3D_Point ccp;
	ccp.x = (((ud_2d.x  - gp->cx)/ gp->fx )*100.0) ;
	ccp.y = (((ud_2d.y  - gp->cy)/ gp->fy )*100.0) ;
	ccp.z = 100.0;

	CoordinateB2A(Roter,Mv,&(pl->p2),&ccp);
	ccp.x = 0;
	ccp.y = 0;
	ccp.z = 0;
	CoordinateB2A(Roter,Mv,&(pl->p1),&ccp);
}

