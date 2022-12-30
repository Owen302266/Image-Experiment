
#include "CamSys.h"


CCamCaliSys::CCamCaliSys()
{

}

CCamCaliSys::~CCamCaliSys()
{

}


void CCamCaliSys::EmptyLine(char * path)
{
	FILE * fp;
	fp = fopen(path,"w");
	fclose(fp);
}

bool CCamCaliSys::SaveLine(char * path, char * line)
{
	if(path == NULL || line == NULL)
	{
		return false;
	}
	FILE * fp;
	fp = fopen(path,"a");
	if(fp==NULL)
	{
		fp = fopen(path,"w");
		if(fp== NULL)
		{
			return false;
		}
	}

	fwrite(line,strlen(line),1,fp);
	char c = 0x0d;
	fwrite(&c,1,1,fp);
	c = 0x0a;
	fclose(fp);
	fwrite(&c,1,1,fp);
	return true;
}


bool  CCamCaliSys::GetExtrinsicPara(double uvxy[200][4],long pnum,
					  pIntrinsicPara gp,pDistortionPara gd,
		pRoterMatrix retRoter,pMoveVecter retMv)
{
	Matrix<int> point_counts(1,1,1); 
	Matrix<double> object_points(1,pnum,3); /* 保存定标板上角点的三维坐标 */
	Matrix<double> image_points(1,pnum,2); /* 保存提取的所有角点 */

	long t;

	for (t=0;t<pnum;t++) {
	
		object_points(0,t,0) = uvxy[t][2];
		object_points(0,t,1) = uvxy[t][3];
		object_points(0,t,2) = 0;
		image_points(0,t,0) = uvxy[t][0];
		image_points(0,t,1) = uvxy[t][1];
	
	}


	point_counts(0,0) = pnum;


	Matrix<double> intrinsic_matrix(3,3,1); /* 摄像机内参数矩阵 */
	Matrix<double> distortion_coeffs(1,4,1); /* 摄像机的4个畸变系数：k1,k2,p1,p2 */
	Matrix<double> rotation_vectors(1,1,3); /* 每幅图像的旋转向量 */
	Matrix<double> translation_vectors(1,1,3); /* 每幅图像的平移向量 */

	intrinsic_matrix(2,2) = 1;
	intrinsic_matrix(0,0) = gp->fx;
	intrinsic_matrix(0,2) = gp->cx;
	intrinsic_matrix(1,1) = gp->fy;
	intrinsic_matrix(1,2) = gp->cy;
	

	distortion_coeffs(0,0) = gd->k1;
	distortion_coeffs(0,1) = gd->k2;
	distortion_coeffs(0,2) = gd->p1;
	distortion_coeffs(0,3) = gd->p2;

	
	cvFindExtrinsicCameraParams2(object_points.cvmat,
								 image_points.cvmat,
								 intrinsic_matrix.cvmat,
								 distortion_coeffs.cvmat,
								 rotation_vectors.cvmat,
								 translation_vectors.cvmat
								 );

	/************************************************************************
	       保存定标结果
	*************************************************************************/
	//cout<<"开始保存定标结果………………";
	Matrix<double> rotation_vector(3,1); /* 保存每幅图像的旋转向量 */
	Matrix<double> rotation_matrix(3,3); /* 保存每幅图像的旋转矩阵 */
	

	
	long j;
	for (j=0;j<3;j++) {
		rotation_vector(j,0,0) = rotation_vectors(0,0,j);
	}
	cvRodrigues2(rotation_vector.cvmat,rotation_matrix.cvmat);
	
	retRoter->r11 = rotation_matrix(0,0);
	retRoter->r12 = rotation_matrix(0,1);
	retRoter->r13 = rotation_matrix(0,2);
	
	retRoter->r21 = rotation_matrix(1,0);
	retRoter->r22 = rotation_matrix(1,1);
	retRoter->r23 = rotation_matrix(1,2);
	
	retRoter->r31 = rotation_matrix(2,0);
	retRoter->r32 = rotation_matrix(2,1);
	retRoter->r33 = rotation_matrix(2,2);
	
	retMv->tx = translation_vectors(0,0,0);
	retMv->ty = translation_vectors(0,0,1);
	retMv->tz = translation_vectors(0,0,2);
	
	return true;
}
bool CCamCaliSys::GetExtrinsicPara(char * picpath,
		pIntrinsicPara gp,pDistortionPara gd,
		pRoterMatrix retRoter,pMoveVecter retMv,
		int num_in_raw,int num_in_cln,
		double s_x,double s_y)
{


	
    /************************************************************************
	       读取每一幅图像，从中提取出角点，然后对角点进行亚像素精确化
	*************************************************************************/
	//cout<<"开始提取角点………………";
	int count;
	int image_count=1;  /* 图像数量 */
	CvSize image_size;  /* 图像的尺寸 */
	CvSize board_size = cvSize(num_in_raw,num_in_cln);    /* 定标板上每行、列的角点数 */
	CvPoint2D32f* image_points_buf = new CvPoint2D32f[board_size.width*board_size.height];   /* 缓存每幅图像上检测到的角点 */
	Seq<CvPoint2D32f> image_points_seq;  /* 保存检测到的所有角点 */
	
	Image<uchar> view(picpath);
	
	image_size.width = view.size().width;
	image_size.height = view.size().height;
	
	/* 提取角点 */
	if (0 == cvFindChessboardCorners( view.cvimage, board_size,
		image_points_buf, &count, CV_CALIB_CB_ADAPTIVE_THRESH ))
	{
		//cout<<"can not find chessboard corners!\n";
		delete []image_points_buf;
		return false;
	} 
	else {
		Image<uchar> view_gray(view.size(),8,1);
		rgb2gray(view,view_gray);
		/* 亚像素精确化 */
		cvFindCornerSubPix( view_gray.cvimage, image_points_buf, count, cvSize(11,11),
			cvSize(-1,-1), cvTermCriteria( CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 30, 0.1 ));
		image_points_seq.push_back(image_points_buf,count);
		/* 在图像上显示角点位置 */
		cvDrawChessboardCorners( view.cvimage, board_size, image_points_buf, count, 1);
//		view.show("calib");
//		cvWaitKey();
//		view.close();
	}
	
	delete []image_points_buf;
	//cout<<"角点提取完成！\n";

	/************************************************************************
	       摄像机定标
	*************************************************************************/
	//cout<<"开始定标………………";

	Matrix<double> object_points(1,board_size.width*board_size.height*image_count,3); /* 保存定标板上角点的三维坐标 */
	Matrix<double> image_points(1,image_points_seq.cvseq->total,2); /* 保存提取的所有角点 */
	Matrix<int> point_counts(1,image_count,1); /* 每幅图像中角点的数量 */
	Matrix<double> intrinsic_matrix(3,3,1); /* 摄像机内参数矩阵 */
	Matrix<double> distortion_coeffs(1,4,1); /* 摄像机的4个畸变系数：k1,k2,p1,p2 */
	Matrix<double> rotation_vectors(1,image_count,3); /* 每幅图像的旋转向量 */
	Matrix<double> translation_vectors(1,image_count,3); /* 每幅图像的平移向量 */


	/* 初始化定标板上角点的三维坐标 */
	int i,j,t;
	for (t=0;t<image_count;t++) {
		for (i=0;i<board_size.height;i++) {
			for (j=0;j<board_size.width;j++) {
				/* 假设定标板放在世界坐标系中z=0的平面上 */
				object_points(0,t*board_size.height*board_size.width+i*board_size.width+j,0) = i*s_y;
				object_points(0,t*board_size.height*board_size.width+i*board_size.width+j,1) = j*s_x;
				object_points(0,t*board_size.height*board_size.width+i*board_size.width+j,2) = 0;
			}
		}
	}

	/* 将角点的存储结构转换成矩阵形式 */
	for (i=0;i<image_points_seq.cvseq->total;i++) {
		image_points(0,i,0) = image_points_seq[i].x;
		image_points(0,i,1) = image_points_seq[i].y;
	}

	/* 初始化每幅图像中的角点数量，这里我们假设每幅图像中都可以看到完整的定标板 */
	for (i=0;i<image_count;i++)
		point_counts(0,i) = board_size.width*board_size.height;


	intrinsic_matrix(2,2) = 1;
	intrinsic_matrix(0,0) = gp->fx;
	intrinsic_matrix(0,2) = gp->cx;
	intrinsic_matrix(1,1) = gp->fy;
	intrinsic_matrix(1,2) = gp->cy;
	

	distortion_coeffs(0,0) = gd->k1;
	distortion_coeffs(0,1) = gd->k2;
	distortion_coeffs(0,2) = gd->p1;
	distortion_coeffs(0,3) = gd->p2;

	
	cvFindExtrinsicCameraParams2(object_points.cvmat,
								 image_points.cvmat,
								 intrinsic_matrix.cvmat,
								 distortion_coeffs.cvmat,
								 rotation_vectors.cvmat,
								 translation_vectors.cvmat
								 );

	/************************************************************************
	       保存定标结果
	*************************************************************************/
	//cout<<"开始保存定标结果………………";
	Matrix<double> rotation_vector(3,1); /* 保存每幅图像的旋转向量 */
	Matrix<double> rotation_matrix(3,3); /* 保存每幅图像的旋转矩阵 */
	

	
	for (j=0;j<3;j++) {
		rotation_vector(j,0,0) = rotation_vectors(0,0,j);
	}
	cvRodrigues2(rotation_vector.cvmat,rotation_matrix.cvmat);
	
	retRoter->r11 = rotation_matrix(0,0);
	retRoter->r12 = rotation_matrix(0,1);
	retRoter->r13 = rotation_matrix(0,2);
	
	retRoter->r21 = rotation_matrix(1,0);
	retRoter->r22 = rotation_matrix(1,1);
	retRoter->r23 = rotation_matrix(1,2);
	
	retRoter->r31 = rotation_matrix(2,0);
	retRoter->r32 = rotation_matrix(2,1);
	retRoter->r33 = rotation_matrix(2,2);
	
	retMv->tx = translation_vectors(0,0,0);
	retMv->ty = translation_vectors(0,0,1);
	retMv->tz = translation_vectors(0,0,2);
	
	return true;
}





bool CCamCaliSys::GetExtrinsicParai(char * picpath,
		pIntrinsicPara gp,pDistortionPara gd,
		pRoterMatrix retRoter,pMoveVecter retMv,
		int num_in_raw,int num_in_cln,
		double s_x,double s_y)
{


	
    /************************************************************************
	       读取每一幅图像，从中提取出角点，然后对角点进行亚像素精确化
	*************************************************************************/
	//cout<<"开始提取角点………………";
	int count;
	int image_count=1;  /* 图像数量 */
	CvSize image_size;  /* 图像的尺寸 */
	CvSize board_size = cvSize(num_in_raw,num_in_cln);    /* 定标板上每行、列的角点数 */
	CvPoint2D32f* image_points_buf = new CvPoint2D32f[board_size.width*board_size.height];   /* 缓存每幅图像上检测到的角点 */
	Seq<CvPoint2D32f> image_points_seq;  /* 保存检测到的所有角点 */
	
	Image<uchar> view(picpath);
	
	image_size.width = view.size().width;
	image_size.height = view.size().height;
	
	/* 提取角点 */
	if (0 == cvFindChessboardCorners( view.cvimage, board_size,
		image_points_buf, &count, CV_CALIB_CB_ADAPTIVE_THRESH ))
	{
		//cout<<"can not find chessboard corners!\n";
		delete []image_points_buf;
		return false;
	} 
	else {
		Image<uchar> view_gray(view.size(),8,1);
		rgb2gray(view,view_gray);
		/* 亚像素精确化 */
		cvFindCornerSubPix( view_gray.cvimage, image_points_buf, count, cvSize(11,11),
			cvSize(-1,-1), cvTermCriteria( CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 30, 0.1 ));
		image_points_seq.push_back(image_points_buf,count);
		/* 在图像上显示角点位置 */
		cvDrawChessboardCorners( view.cvimage, board_size, image_points_buf, count, 1);
		//view.show("calib");
		//cvWaitKey();
		//view.close();
	}
	
	delete []image_points_buf;
	//cout<<"角点提取完成！\n";

	/************************************************************************
	       摄像机定标
	*************************************************************************/
	//cout<<"开始定标………………";

	Matrix<double> object_points(1,board_size.width*board_size.height*image_count,3); /* 保存定标板上角点的三维坐标 */
	Matrix<double> image_points(1,image_points_seq.cvseq->total,2); /* 保存提取的所有角点 */
	Matrix<int> point_counts(1,image_count,1); /* 每幅图像中角点的数量 */
	Matrix<double> intrinsic_matrix(3,3,1); /* 摄像机内参数矩阵 */
	Matrix<double> distortion_coeffs(1,4,1); /* 摄像机的4个畸变系数：k1,k2,p1,p2 */
	Matrix<double> rotation_vectors(1,image_count,3); /* 每幅图像的旋转向量 */
	Matrix<double> translation_vectors(1,image_count,3); /* 每幅图像的平移向量 */


	/* 初始化定标板上角点的三维坐标 */
	int i,j,t;
	for (t=0;t<image_count;t++) {
		for (i=0;i<board_size.height;i++) {
			for (j=0;j<board_size.width;j++) {
				/* 假设定标板放在世界坐标系中z=0的平面上 */
				object_points(0,t*board_size.height*board_size.width+i*board_size.width+j,0) = (board_size.height - i- 1)*s_y;
				object_points(0,t*board_size.height*board_size.width+i*board_size.width+j,1) = (board_size.width - j - 1 )*s_x;
				object_points(0,t*board_size.height*board_size.width+i*board_size.width+j,2) = 0;
			}
		}
	}

	/* 将角点的存储结构转换成矩阵形式 */
	for (i=0;i<image_points_seq.cvseq->total;i++) {
		image_points(0,i,0) = image_points_seq[i].x;
		image_points(0,i,1) = image_points_seq[i].y;
	}

	/* 初始化每幅图像中的角点数量，这里我们假设每幅图像中都可以看到完整的定标板 */
	for (i=0;i<image_count;i++)
		point_counts(0,i) = board_size.width*board_size.height;


	intrinsic_matrix(2,2) = 1;
	intrinsic_matrix(0,0) = gp->fx;
	intrinsic_matrix(0,2) = gp->cx;
	intrinsic_matrix(1,1) = gp->fy;
	intrinsic_matrix(1,2) = gp->cy;
	

	distortion_coeffs(0,0) = gd->k1;
	distortion_coeffs(0,1) = gd->k2;
	distortion_coeffs(0,2) = gd->p1;
	distortion_coeffs(0,3) = gd->p2;

	
	cvFindExtrinsicCameraParams2(object_points.cvmat,
								 image_points.cvmat,
								 intrinsic_matrix.cvmat,
								 distortion_coeffs.cvmat,
								 rotation_vectors.cvmat,
								 translation_vectors.cvmat
								 );

	/************************************************************************
	       保存定标结果
	*************************************************************************/
	//cout<<"开始保存定标结果………………";
	Matrix<double> rotation_vector(3,1); /* 保存每幅图像的旋转向量 */
	Matrix<double> rotation_matrix(3,3); /* 保存每幅图像的旋转矩阵 */
	

	
	for (j=0;j<3;j++) {
		rotation_vector(j,0,0) = rotation_vectors(0,0,j);
	}
	cvRodrigues2(rotation_vector.cvmat,rotation_matrix.cvmat);
	
	retRoter->r11 = rotation_matrix(0,0);
	retRoter->r12 = rotation_matrix(0,1);
	retRoter->r13 = rotation_matrix(0,2);
	
	retRoter->r21 = rotation_matrix(1,0);
	retRoter->r22 = rotation_matrix(1,1);
	retRoter->r23 = rotation_matrix(1,2);
	
	retRoter->r31 = rotation_matrix(2,0);
	retRoter->r32 = rotation_matrix(2,1);
	retRoter->r33 = rotation_matrix(2,2);
	
	retMv->tx = translation_vectors(0,0,0);
	retMv->ty = translation_vectors(0,0,1);
	retMv->tz = translation_vectors(0,0,2);
	
	return true;
}


bool CCamCaliSys::GetIntrinsicPara(char * picpath,pIntrinsicPara gp,pDistortionPara gd,int num_in_raw,int num_in_cln,double s_x,double s_y)
{
	
	
	ifstream fin(picpath); 			
	int image_count=0;  /* 图像数量 */
	CvSize image_size;  /* 图像的尺寸 */
	CvSize board_size = cvSize(num_in_raw,num_in_cln);    /* 定标板上每行、列的角点数 */
	CvPoint2D32f* image_points_buf = new CvPoint2D32f[board_size.width*board_size.height];   /* 缓存每幅图像上检测到的角点 */
	Seq<CvPoint2D32f> image_points_seq;  /* 保存检测到的所有角点 */
	
	string filename;
	while (getline(fin,filename))
	{	
		image_count++;
		int count;
		Image<uchar> view(filename);
		if (image_count == 1) {
			image_size.width = view.size().width;
			image_size.height = view.size().height;
		}
		/* 提取角点 */
		if (0 == cvFindChessboardCorners( view.cvimage, board_size,
            image_points_buf, &count, CV_CALIB_CB_ADAPTIVE_THRESH ))
		{
			delete []image_points_buf;
			return false;
		} 
		else 
		{
			Image<uchar> view_gray(view.size(),8,1);
			rgb2gray(view,view_gray);
		
			cvFindCornerSubPix( view_gray.cvimage, image_points_buf, count, cvSize(11,11),
				cvSize(-1,-1), cvTermCriteria( CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 30, 0.1 ));
			image_points_seq.push_back(image_points_buf,count);
		
			cvDrawChessboardCorners( view.cvimage, board_size, image_points_buf, count, 1);
		
		}
		
		
	}
	delete []image_points_buf;

	Matrix<double> object_points(1,board_size.width*board_size.height*image_count,3); /* 保存定标板上角点的三维坐标 */
	Matrix<double> image_points(1,image_points_seq.cvseq->total,2); /* 保存提取的所有角点 */
	Matrix<int> point_counts(1,image_count,1); /* 每幅图像中角点的数量 */
	Matrix<double> intrinsic_matrix(3,3,1); /* 摄像机内参数矩阵 */
	Matrix<double> distortion_coeffs(1,4,1); /* 摄像机的4个畸变系数：k1,k2,p1,p2 */
	Matrix<double> rotation_vectors(1,image_count,3); /* 每幅图像的旋转向量 */
	Matrix<double> translation_vectors(1,image_count,3); /* 每幅图像的平移向量 */
	

	double ii;
	double jj;
	int i,j,t;
	for (t=0;t<image_count;t++) {
		for (i=0;i<board_size.height;i++) {
			for (j=0;j<board_size.width;j++) {
			
				ii = i;
				jj = j;
				object_points(0,t*board_size.height*board_size.width+i*board_size.width+j,0) = ii*s_y;
				object_points(0,t*board_size.height*board_size.width+i*board_size.width+j,1) = jj*s_x;
				object_points(0,t*board_size.height*board_size.width+i*board_size.width+j,2) = 0;
			}
		}
	}

	
	
	/* 将角点的存储结构转换成矩阵形式 */
	for (i=0;i<image_points_seq.cvseq->total;i++) {
		image_points(0,i,0) = image_points_seq[i].x;
		image_points(0,i,1) = image_points_seq[i].y;
	}
	
	/* 初始化每幅图像中的角点数量，这里我们假设每幅图像中都可以看到完整的定标板 */
	for (i=0;i<image_count;i++)
	{
		point_counts(0,i) = board_size.width*board_size.height;
	}
	
	/* 开始定标 */
	cvCalibrateCamera2(object_points.cvmat,
					   image_points.cvmat,
                       point_counts.cvmat,
					   image_size,
                       intrinsic_matrix.cvmat,
					   distortion_coeffs.cvmat,
                       rotation_vectors.cvmat,
					   translation_vectors.cvmat,
					   0);

	gp->fx = intrinsic_matrix(0,0);
	gp->cx = intrinsic_matrix(0,2);
	gp->fy = intrinsic_matrix(1,1);
	gp->cy = intrinsic_matrix(1,2);
	gd->k1 = distortion_coeffs(0,0);
	gd->k2 = distortion_coeffs(0,1);
	gd->p1 = distortion_coeffs(0,2);
	gd->p2 = distortion_coeffs(0,3);
	
	///*/
	ofstream fout("c:\\CCamSys_GetIntrinsicPara_err.txt");

		//cout<<"开始评价定标结果………………\n";
	double total_err = 0.0; // 所有图像的平均误差的总和 //
	double err = 0.0; // 每幅图像的平均误差 /
	Matrix<double> image_points2(1,point_counts(0,0,0),2); // 保存重新计算得到的投影点 //

	//cout<<"\t每幅图像的定标误差：\n";
	fout<<"每幅图像的定标误差：\n";
	for (i=0;i<image_count;i++) {
		// 通过得到的摄像机内外参数，对空间的三维点进行重新投影计算，得到新的投影点 //
		cvProjectPoints2(object_points.get_cols(i*point_counts(0,0,0),(i+1)*point_counts(0,0,0)-1).cvmat,
						rotation_vectors.get_col(i).cvmat,
						translation_vectors.get_col(i).cvmat,
						intrinsic_matrix.cvmat,
						distortion_coeffs.cvmat,
						image_points2.cvmat,
						0,0,0,0);
		// 计算新的投影点和旧的投影点之间的误差//
		err = cvNorm(image_points.get_cols(i*point_counts(0,0,0),(i+1)*point_counts(0,0,0)-1).cvmat,
					image_points2.cvmat,
					CV_L1);
		total_err += err/=point_counts(0,0,0);
		//cout<<"\t\t第"<<i+1<<"幅图像的平均误差："<<err<<"像素"<<'\n';
		fout<<"\t第"<<i+1<<"幅图像的平均误差："<<err<<"像素"<<'\n';
	}
	//cout<<"\t总体平均误差："<<total_err/image_count<<"像素"<<'\n';
	fout<<"总体平均误差："<<total_err/image_count<<"像素"<<'\n'<<'\n';
	//cout<<"评价完成！\n";
///*/	
	
	return true;
}



bool CCamCaliSys::FindCross(char * path,int num_in_raw,int num_in_cln,p_2D_Point  retp,int retsize)
{
	CvSize image_size;  /* 图像的尺寸 */
	CvSize board_size = cvSize(num_in_raw,num_in_cln);    /* 定标板上每行、列的角点数 */
	CvPoint2D32f* image_points_buf = new CvPoint2D32f[board_size.width*board_size.height];   /* 缓存每幅图像上检测到的角点 */
	Seq<CvPoint2D32f> image_points_seq;  /* 保存检测到的所有角点 */

	int count;
	Image<uchar> view(path);
	
	image_size.width = view.size().width;
	image_size.height = view.size().height;
	
	/* 提取角点 */
	if (0 == cvFindChessboardCorners( view.cvimage, board_size,
		image_points_buf, &count, CV_CALIB_CB_ADAPTIVE_THRESH ))
	{
		//cout<<"can not find chessboard corners!\n";
		
		delete []image_points_buf;
		return false;
	} 
	else 
	{

		Image<uchar> view_gray(view.size(),8,1);
		rgb2gray(view,view_gray);
		
		cvFindCornerSubPix( view_gray.cvimage, image_points_buf, count, cvSize(11,11),
			cvSize(-1,-1), cvTermCriteria( CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 30, 0.1 ));
		image_points_seq.push_back(image_points_buf,count);
		
		cvDrawChessboardCorners( view.cvimage, board_size, image_points_buf, count, 1);
		if(retp != NULL)
		{
			for(int i = 0; i < num_in_raw * num_in_cln && i < retsize; i ++)
			{
				retp[i].x = image_points_seq[i].x;
				retp[i].y = image_points_seq[i].y;
			}
		}
		
	}
	
	delete []image_points_buf;
	return true;
}

long CCamCaliSys::FindCross3(IplImage *pFrame,int num_in_raw,int num_in_cln,p_2D_Point retp,int retsize )
{
	CvSize board_size = cvSize(num_in_raw,num_in_cln);    /* 定标板上每行、列的角点数 */
	CvPoint2D32f* image_points_buf = new CvPoint2D32f[board_size.width*board_size.height];   /* 缓存每幅图像上检测到的角点 */
	Seq<CvPoint2D32f> image_points_seq;  /* 保存检测到的所有角点 */
	
	int count;
	
	
	
	long i;
	
	cvFindChessboardCorners( pFrame, board_size,
		image_points_buf, &count, CV_CALIB_CB_ADAPTIVE_THRESH );
	if (count == 0)
	{
		//cout<<"can not find chessboard corners!\n";
		
		delete []image_points_buf;
		return 0;
	} 
	else 
	{
		IplImage *dst_gray = cvCreateImage(cvGetSize(pFrame),pFrame->depth,1);//灰度图

		cvCvtColor(pFrame,dst_gray,CV_BGR2GRAY);//得到灰度图
		
		cvFindCornerSubPix( dst_gray, image_points_buf, count, board_size,
			cvSize(-1,-1), cvTermCriteria( CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 30, 0.1 ));
		image_points_seq.push_back(image_points_buf,count);
		
		cvReleaseImage(&dst_gray);
		if(retp != NULL)
		{
			for( i = 0; i < count && i < retsize; i ++)
			{
				retp[i].x = image_points_seq[i].x;
				retp[i].y = pFrame->height - image_points_seq[i].y;
			}
		}
		
	}
	
	delete []image_points_buf;
	return i;
}
long CCamCaliSys::FindCross2(char * path,int num_in_raw,int num_in_cln,p_2D_Point  retp,int retsize)
{
	CvSize image_size;  /* 图像的尺寸 */
	CvSize board_size = cvSize(num_in_raw,num_in_cln);    /* 定标板上每行、列的角点数 */
	CvPoint2D32f* image_points_buf = new CvPoint2D32f[board_size.width*board_size.height];   /* 缓存每幅图像上检测到的角点 */
	Seq<CvPoint2D32f> image_points_seq;  /* 保存检测到的所有角点 */
	
	int count;
	Image<uchar> view(path);
	
	image_size.width = view.size().width;
	image_size.height = view.size().height;
	
	long i;
	
	cvFindChessboardCorners( view.cvimage, board_size,
		image_points_buf, &count, CV_CALIB_CB_ADAPTIVE_THRESH );
	if (count == 0)
	{
		//cout<<"can not find chessboard corners!\n";
		
		delete []image_points_buf;
		return 0;
	} 
	else 
	{
		
		Image<uchar> view_gray(view.size(),8,1);
		rgb2gray(view,view_gray);
		
		cvFindCornerSubPix( view_gray.cvimage, image_points_buf, count, cvSize(11,11),
			cvSize(-1,-1), cvTermCriteria( CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 30, 0.1 ));
		image_points_seq.push_back(image_points_buf,count);
		
		cvDrawChessboardCorners( view.cvimage, board_size, image_points_buf, count, 1);
		if(retp != NULL)
		{
			for( i = 0; i < count && i < retsize; i ++)
			{
				retp[i].x = image_points_seq[i].x;
				retp[i].y = image_points_seq[i].y;
			}
		}
		
	}
	
	delete []image_points_buf;
	return i;
}