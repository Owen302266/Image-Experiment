
#if !defined AN_CAMSYS_H_ 
#define AN_CAMSYS_H_


#include "def.h"
#include "cvut/cvut.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace cvut;
using namespace std;

#pragma comment(lib,"cxcore.lib")
#pragma comment(lib,"cv.lib")
#pragma comment(lib,"highgui.lib")



class CCamCaliSys  
{
public:
	CCamCaliSys();
	virtual ~CCamCaliSys();
	
	IplImage * m_pFrame;

	/*/�������ͷ�����  ���̸�
	picpath Ϊ���̸�ͼ��
	gp �ڲ���
	gd ����ϵ��
	retRoter ����������ϵ������ͷ����ϵ����ת����
	retMv    ����������ϵ������ͷ����ϵ��λ�ƾ���
	
	num_in_raw ���̸�ÿ���ڵ����
	num_in_cln ���̸�ÿ���ڵ����
	s_x ���̸�Ԫ�з��򳤶�
	s_y ���̸�Ԫ�з��򳤶�
	/*/
	static bool GetExtrinsicPara(char * picpath,
		pIntrinsicPara gp,pDistortionPara gd,
		pRoterMatrix retRoter,pMoveVecter retMv,
		int num_in_raw,int num_in_cln,
		double s_x,double s_y);

	static bool GetExtrinsicPara(double uvxy[200][4],long pnum,
		pIntrinsicPara gp,pDistortionPara gd,
		pRoterMatrix retRoter,pMoveVecter retMv);


	/*/�������ͷ�����  ���̸�
	picpath Ϊ���̸�ͼ��
	gp �ڲ���
	gd ����ϵ��
	retRoter ����������ϵ������ͷ����ϵ����ת����
	retMv    ����������ϵ������ͷ����ϵ��λ�ƾ���
	
	num_in_raw ���̸�ÿ���ڵ����
	num_in_cln ���̸�ÿ���ڵ����
	s_x ���̸�Ԫ�з��򳤶�
	s_y ���̸�Ԫ�з��򳤶�
	/*/
	static bool GetExtrinsicParai(char * picpath,
		pIntrinsicPara gp,pDistortionPara gd,
		pRoterMatrix retRoter,pMoveVecter retMv,
		int num_in_raw,int num_in_cln,
		double s_x,double s_y);

	/*/�������ͷ�ڲ��� ����ȱʡ����
	picpath ����ͼƬ·�����ļ�·��
	gp��õ��ڲ���
	gd��õĻ���ϵ��
	num_in_raw ���̸�ÿ���ڵ����
	num_in_cln ���̸�ÿ���ڵ����
	s_x ���̸�Ԫ�з��򳤶�
	s_y ���̸�Ԫ�з��򳤶�
	/*/
	static bool GetIntrinsicPara(char * picpath,
		pIntrinsicPara gp,pDistortionPara gd,
		int num_in_raw,int num_in_cln,
		double s_x,double s_y);

	/*/����Ŀ��ͼƬ���Ƿ����num_in_raw * num_in_cln �����̸��ڵ�
	pathͼƬ·��
	num_in_raw ÿ���ڵ����
	num_in_clnÿ���ڵ����
	retp     ���صĵ��λ��
	retsize  ���ص�������С/*/
	static bool FindCross(char * path,int num_in_raw,int num_in_cln,p_2D_Point retp,int retsize);
	static long FindCross2(char * path,int num_in_raw,int num_in_cln,p_2D_Point retp,int retsize);
	static long FindCross3(IplImage *pFrame,int num_in_raw,int num_in_cln,p_2D_Point retp,int retsize);

	
	static bool SaveLine(char * path, char * line);
	
	
	static void EmptyLine(char * path);
	
	
};

#endif 
