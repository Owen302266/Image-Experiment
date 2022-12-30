
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

	/*/获得摄像头外参数  棋盘格
	picpath 为棋盘格图像
	gp 内参数
	gd 畸变系数
	retRoter 从棋盘坐标系到摄像头坐标系的旋转矩阵
	retMv    从棋盘坐标系到摄像头坐标系的位移矩阵
	
	num_in_raw 棋盘格每行内点个数
	num_in_cln 棋盘格每列内点个数
	s_x 棋盘格单元行方向长度
	s_y 棋盘格单元列方向长度
	/*/
	static bool GetExtrinsicPara(char * picpath,
		pIntrinsicPara gp,pDistortionPara gd,
		pRoterMatrix retRoter,pMoveVecter retMv,
		int num_in_raw,int num_in_cln,
		double s_x,double s_y);

	static bool GetExtrinsicPara(double uvxy[200][4],long pnum,
		pIntrinsicPara gp,pDistortionPara gd,
		pRoterMatrix retRoter,pMoveVecter retMv);


	/*/获得摄像头外参数  棋盘格
	picpath 为棋盘格图像
	gp 内参数
	gd 畸变系数
	retRoter 从棋盘坐标系到摄像头坐标系的旋转矩阵
	retMv    从棋盘坐标系到摄像头坐标系的位移矩阵
	
	num_in_raw 棋盘格每行内点个数
	num_in_cln 棋盘格每列内点个数
	s_x 棋盘格单元行方向长度
	s_y 棋盘格单元列方向长度
	/*/
	static bool GetExtrinsicParai(char * picpath,
		pIntrinsicPara gp,pDistortionPara gd,
		pRoterMatrix retRoter,pMoveVecter retMv,
		int num_in_raw,int num_in_cln,
		double s_x,double s_y);

	/*/获得摄像头内参数 采用缺省设置
	picpath 包含图片路径的文件路径
	gp获得的内参数
	gd获得的畸变系数
	num_in_raw 棋盘格每行内点个数
	num_in_cln 棋盘格每列内点个数
	s_x 棋盘格单元行方向长度
	s_y 棋盘格单元列方向长度
	/*/
	static bool GetIntrinsicPara(char * picpath,
		pIntrinsicPara gp,pDistortionPara gd,
		int num_in_raw,int num_in_cln,
		double s_x,double s_y);

	/*/查找目标图片上是否存在num_in_raw * num_in_cln 的棋盘格内点
	path图片路径
	num_in_raw 每行内点个数
	num_in_cln每列内点个数
	retp     返回的点的位置
	retsize  返回点的数组大小/*/
	static bool FindCross(char * path,int num_in_raw,int num_in_cln,p_2D_Point retp,int retsize);
	static long FindCross2(char * path,int num_in_raw,int num_in_cln,p_2D_Point retp,int retsize);
	static long FindCross3(IplImage *pFrame,int num_in_raw,int num_in_cln,p_2D_Point retp,int retsize);

	
	static bool SaveLine(char * path, char * line);
	
	
	static void EmptyLine(char * path);
	
	
};

#endif 
