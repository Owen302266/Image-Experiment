

#if !defined   AN_OPT_DEFINE_H
#define AN_OPT_DEFINE_H

#define THEPIIS 3.141592653589793

typedef struct RoterMatrix
{
	double r11;
	double r12;
	double r13;
	double r21;
	double r22;
	double r23;
	double r31;
	double r32;
	double r33;
}* pRoterMatrix;

typedef struct MoveVecter
{
	double tx;
	double ty;
	double tz;
}*pMoveVecter;

typedef struct IntrinsicPara
{
	double cx;
	double cy;
	double fx;
	double fy;
}*pIntrinsicPara;


typedef struct DistortionPara
{
	double k1;
	double k2;
	double p1;
	double p2;
}*pDistortionPara;


typedef struct _3D_Point
{
	double x;
	double y;
	double z;
}*p_3D_Point;

typedef struct _2D_Point
{
	double x;
	double y;
}*p_2D_Point;

typedef struct _3D_Line
{
	_3D_Point p1;
	_3D_Point p2;
}*p_3D_Line;


typedef struct _3d_2d_pair
{
	double _3d_x;
	double _3d_y;
	double _3d_z;
	
	double _2d_x;
	double _2d_y;
}*p_3d_2d_pair;


typedef struct CamerasData
{
	int serial;
	double x_r;
	double y_r;
	int num_r;

	double x_g;
	double y_g;
	int num_g;

	double x_b;
	double y_b;
	int num_b;
}*p_CamerasData;

typedef struct RealData
{
	
	char test;

	float x;
	float y;
	float z;

	float sp_x;
	float sp_y;
	float sp_z;


	float acc_x;
	float acc_y;
	float acc_z;

	float first;
	float second;
	float third;

	float acc_first;
	float acc_second;
	float acc_third;
    //int ss;
	//int valid;int ss;
}*p_RealData;


typedef struct RGB_CELL
{
	unsigned char b;
	unsigned char g;
	unsigned char r;
} * p_RGB_CELL;


#endif