
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

	/*/�ж�v�Ƿ�ΪO/*/
	static bool Is_Zero(double v);

	/*/������ͷ���Σ������Ϊ�����ĵ㣬���Ϊ����δ����ĵ�
	gpΪ����ͷ�ڲ���
	gdΪ����ϵ��
	p2dΪ����ͷ�����ϵ����������ϵλ�ã�
	���ص�δ�����Ҳ���ڸñ�����
	/*/

	/*/����/*/
	static void Distort(pIntrinsicPara gp,pDistortionPara gd,p_2D_Point src,p_2D_Point dst);

	/*/�����/*/
	static void UnDistort(pIntrinsicPara gp,pDistortionPara gd,p_2D_Point src,p_2D_Point dst);

	
	
	/*/��һ������ͷ�����ϵĵ�����Ӧ�Ŀռ�ֱ�ߵ���������ϵ�µķ���
	RoterΪ����������ϵ������ͷ����ϵ����ת����
	MvΪ����������ϵ������ͷ����ϵ��λ�ƾ���
	gpΪ����ͷ�ڲ���
	gdΪ����ͷ�Ļ���ϵ��
	p2dΪ����ͷ�����ϵ����������ϵλ��
	plΪ���ص�ֱ�߷���  ������ֵ��
	/*/
	static void GetLine(pRoterMatrix Roter,pMoveVecter Mv,pIntrinsicPara gp,pDistortionPara gd,p_2D_Point p2d,p_3D_Line pl);
	
	
	/*/����任 A����ϵ�µ㵽B����ϵ
	cA2cB_r A����ϵ��B����ϵ����ת����
	cA2cb_t A����ϵ��B����ϵ��λ�ƾ���
	_3Dp_inA  ����A����ϵ�µ�λ��
	_3Dp_inB  ����B����ϵ�µ�λ��  ������ֵ��
	/*/
	static void CoordinateA2B(pRoterMatrix cA2cB_r,pMoveVecter cA2cb_t,p_3D_Point _3Dp_inA,p_3D_Point _3Dp_inB);
	
	
	/*/����任 B����ϵ�µ㵽A����ϵ
	cA2cB_r A����ϵ��B����ϵ����ת����
	cA2cb_t A����ϵ��B����ϵ��λ�ƾ���
	_3Dp_inA  ����A����ϵ�µ�λ��  ������ֵ��
	_3Dp_inB  ����B����ϵ�µ�λ��  
	/*/
	static void CoordinateB2A(pRoterMatrix cA2cB_r,pMoveVecter cA2cb_t,p_3D_Point _3Dp_inA,p_3D_Point _3Dp_inB);
	

	/*/��������ֱ�ߵĽ�������޽������ֱ�߼���̾����߶ε��е�
	line1 ֱ��1
	line2 ֱ��2
	cross_point ��õĽ���
	dist ����ֱ��֮��ľ���
	/*/
	static bool GetLineCross(p_3D_Line line1,p_3D_Line line2,p_3D_Point cross_point, double *dist);


		/*/��������ͷ�ڲ��� /*/
	static bool SaveIntrinsicPara(char * path,pIntrinsicPara gp,pDistortionPara gd);
	
	
	/*/��������ͷ�ڲ��� /*/
	static bool LoadIntrinsicPara(char * path,pIntrinsicPara gp,pDistortionPara gd);
	
	
	/*/��������ͷ����� /*/
	static bool SaveExtrinsicPara(char * path,pRoterMatrix Roter,pMoveVecter Mv);
	
	
	/*/��������ͷ����� /*/
	static bool LoadExtrinsicPara(char * path,pRoterMatrix Roter,pMoveVecter Mv);

	/*/��������ֱ�ߵĽ�������޽������ֱ�߼���̾����߶ε��е�
	/*/
	static bool GetLineCross(pRoterMatrix Roter1,pMoveVecter Mv1,pIntrinsicPara gp1,pDistortionPara gd1,p_2D_Point p2d1,
							pRoterMatrix Roter2,pMoveVecter Mv2,pIntrinsicPara gp2,pDistortionPara gd2,p_2D_Point p2d2,
							p_3D_Point cross_point, double *dist);


	/*/
	GetPosePosition 
	������̬��λ��
	red_p ��ɫ��λ��
	green_p��ɫ��λ��
	blue_p��ɫ��λ��
	pose ��̬���������������ᵥλ����
	position_pλ������
	
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
