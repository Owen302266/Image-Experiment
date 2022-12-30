
#if !defined AN_GEOMETRY_H_ 
#define AN_GEOMETRY_H_

#include "def.h"


class gesture
{
public:
	double P1_x,P1_y,P1_z,P2_x,P2_y,P2_z,P3_x,P3_y,P3_z;
	double First_rotate,Second_rotate,Third_rotate;
	double C1_x,C1_y,C1_z,C2_x,C2_y,C2_z,C3_x,C3_y,C3_z;
	double Nor_x,Nor_y,Nor_z;
public:
	gesture(){};
	virtual ~gesture(){};
	void calFirst();
	void calSecond();
	void calNor();
	void calThird();
    void getPos(double x1,double y1,double z1,double x2,double y2,double z2,double x3,double y3,double z3);
};


class CGeometry  
{
public:
	//static double distance;
	CGeometry();
	virtual ~CGeometry();


	static void SolveLinePoint_Z0(p_3D_Line line,p_3D_Point retp);
	static void SolveLinePoint_Z(p_3D_Line line,p_3D_Point retp,double z);

	/*/判定v是否为O/*/
	static bool Is_Zero(double v);

	/*/解摄像头变形，输入的为畸变后的点，输出为反解未畸变的点
	gp为摄像头内参数
	gd为畸变系数
	p2d为摄像头成像上点的像素坐标系位置，
	返回的未畸变点也放在该变量内
	/*/

	/*/畸变/*/
	static void Distort(pIntrinsicPara gp,pDistortionPara gd,p_2D_Point src,p_2D_Point dst);

	/*/解畸变/*/
	static void UnDistort(pIntrinsicPara gp,pDistortionPara gd,p_2D_Point src,p_2D_Point dst);

	
	
	/*/从一个摄像头成像上的点解出对应的空间直线的世界坐标系下的方程
	Roter为从世界坐标系到摄像头坐标系的旋转矩阵
	Mv为从世界坐标系到摄像头坐标系的位移矩阵
	gp为摄像头内参数
	gd为摄像头的畸变系数
	p2d为摄像头成像上点的像素坐标系位置
	pl为返回的直线方程  （返回值）
	/*/
	static void GetLine(pRoterMatrix Roter,pMoveVecter Mv,pIntrinsicPara gp,pDistortionPara gd,p_2D_Point p2d,p_3D_Line pl);
	
	
	/*/坐标变换 A坐标系下点到B坐标系
	cA2cB_r A坐标系到B坐标系的旋转矩阵
	cA2cb_t A坐标系到B坐标系的位移矩阵
	_3Dp_inA  点在A坐标系下的位置
	_3Dp_inB  点在B坐标系下的位置  （返回值）
	/*/
	static void CoordinateA2B(pRoterMatrix cA2cB_r,pMoveVecter cA2cb_t,p_3D_Point _3Dp_inA,p_3D_Point _3Dp_inB);
	
	
	/*/坐标变换 B坐标系下点到A坐标系
	cA2cB_r A坐标系到B坐标系的旋转矩阵
	cA2cb_t A坐标系到B坐标系的位移矩阵
	_3Dp_inA  点在A坐标系下的位置  （返回值）
	_3Dp_inB  点在B坐标系下的位置  
	/*/
	static void CoordinateB2A(pRoterMatrix cA2cB_r,pMoveVecter cA2cb_t,p_3D_Point _3Dp_inA,p_3D_Point _3Dp_inB);
	

	/*/计算两条直线的交叉点若无交叉点解出直线间最短距离线段的中点
	line1 直线1
	line2 直线2
	cross_point 获得的交点
	dist 两条直线之间的距离
	/*/
	static bool GetLineCross(p_3D_Line line1,p_3D_Line line2,p_3D_Point cross_point, double *dist);


		/*/保存摄像头内参数 /*/
	static bool SaveIntrinsicPara(char * path,pIntrinsicPara gp,pDistortionPara gd);
	
	
	/*/导入摄像头内参数 /*/
	static bool LoadIntrinsicPara(char * path,pIntrinsicPara gp,pDistortionPara gd);
	
	
	/*/保存摄像头外参数 /*/
	static bool SaveExtrinsicPara(char * path,pRoterMatrix Roter,pMoveVecter Mv);
	
	
	/*/导入摄像头外参数 /*/
	static bool LoadExtrinsicPara(char * path,pRoterMatrix Roter,pMoveVecter Mv);

	/*/计算两条直线的交叉点若无交叉点解出直线间最短距离线段的中点
	/*/
	static bool GetLineCross(pRoterMatrix Roter1,pMoveVecter Mv1,pIntrinsicPara gp1,pDistortionPara gd1,p_2D_Point p2d1,
							pRoterMatrix Roter2,pMoveVecter Mv2,pIntrinsicPara gp2,pDistortionPara gd2,p_2D_Point p2d2,
							p_3D_Point cross_point, double *dist);


	/*/
	GetPosePosition 
	计算姿态和位置
	red_p 红色灯位置
	green_p绿色灯位置
	blue_p蓝色灯位置
	pose 姿态向量机体坐标三轴单位向量
	position_p位置坐标
	
	/*/
	static bool GetPosePosition(p_3D_Point red_p,p_3D_Point green_p,p_3D_Point  blue_p,double pose[3][3],p_3D_Point position_p);


	
	static bool GetPosePosition(CamerasData datas[20],RoterMatrix Roter[20],MoveVecter Mv[20],
		IntrinsicPara gp[20],DistortionPara gd[20],
		double pose[3][3],p_3D_Point position_p);

	
	
	static bool GetPosePosition(CamerasData datas[20],RoterMatrix Roter[20],MoveVecter Mv[20],
		IntrinsicPara gp[20],DistortionPara gd[20],
		double pose[3][3],p_3D_Point position_p,
		p_3D_Point red_p,p_3D_Point green_p,p_3D_Point  blue_p);


	
	static bool GetPosePosition(CamerasData datas[20],RoterMatrix Roter[20],MoveVecter Mv[20],
		IntrinsicPara gp[20],DistortionPara gd[20],
		double pose[3][3],p_3D_Point position_p,
		p_3D_Point red_p,p_3D_Point green_p,p_3D_Point  blue_p,
		double ang[3],double *dist,_3D_Point R15[20]);




};

#endif 
