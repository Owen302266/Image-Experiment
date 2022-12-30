
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
	Matrix<double> object_points(1,pnum,3); /* ���涨����Ͻǵ����ά���� */
	Matrix<double> image_points(1,pnum,2); /* ������ȡ�����нǵ� */

	long t;

	for (t=0;t<pnum;t++) {
	
		object_points(0,t,0) = uvxy[t][2];
		object_points(0,t,1) = uvxy[t][3];
		object_points(0,t,2) = 0;
		image_points(0,t,0) = uvxy[t][0];
		image_points(0,t,1) = uvxy[t][1];
	
	}


	point_counts(0,0) = pnum;


	Matrix<double> intrinsic_matrix(3,3,1); /* ������ڲ������� */
	Matrix<double> distortion_coeffs(1,4,1); /* �������4������ϵ����k1,k2,p1,p2 */
	Matrix<double> rotation_vectors(1,1,3); /* ÿ��ͼ�����ת���� */
	Matrix<double> translation_vectors(1,1,3); /* ÿ��ͼ���ƽ������ */

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
	       ���涨����
	*************************************************************************/
	//cout<<"��ʼ���涨����������������";
	Matrix<double> rotation_vector(3,1); /* ����ÿ��ͼ�����ת���� */
	Matrix<double> rotation_matrix(3,3); /* ����ÿ��ͼ�����ת���� */
	

	
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
	       ��ȡÿһ��ͼ�񣬴�����ȡ���ǵ㣬Ȼ��Խǵ���������ؾ�ȷ��
	*************************************************************************/
	//cout<<"��ʼ��ȡ�ǵ㡭����������";
	int count;
	int image_count=1;  /* ͼ������ */
	CvSize image_size;  /* ͼ��ĳߴ� */
	CvSize board_size = cvSize(num_in_raw,num_in_cln);    /* �������ÿ�С��еĽǵ��� */
	CvPoint2D32f* image_points_buf = new CvPoint2D32f[board_size.width*board_size.height];   /* ����ÿ��ͼ���ϼ�⵽�Ľǵ� */
	Seq<CvPoint2D32f> image_points_seq;  /* �����⵽�����нǵ� */
	
	Image<uchar> view(picpath);
	
	image_size.width = view.size().width;
	image_size.height = view.size().height;
	
	/* ��ȡ�ǵ� */
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
		/* �����ؾ�ȷ�� */
		cvFindCornerSubPix( view_gray.cvimage, image_points_buf, count, cvSize(11,11),
			cvSize(-1,-1), cvTermCriteria( CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 30, 0.1 ));
		image_points_seq.push_back(image_points_buf,count);
		/* ��ͼ������ʾ�ǵ�λ�� */
		cvDrawChessboardCorners( view.cvimage, board_size, image_points_buf, count, 1);
//		view.show("calib");
//		cvWaitKey();
//		view.close();
	}
	
	delete []image_points_buf;
	//cout<<"�ǵ���ȡ��ɣ�\n";

	/************************************************************************
	       ���������
	*************************************************************************/
	//cout<<"��ʼ���ꡭ����������";

	Matrix<double> object_points(1,board_size.width*board_size.height*image_count,3); /* ���涨����Ͻǵ����ά���� */
	Matrix<double> image_points(1,image_points_seq.cvseq->total,2); /* ������ȡ�����нǵ� */
	Matrix<int> point_counts(1,image_count,1); /* ÿ��ͼ���нǵ������ */
	Matrix<double> intrinsic_matrix(3,3,1); /* ������ڲ������� */
	Matrix<double> distortion_coeffs(1,4,1); /* �������4������ϵ����k1,k2,p1,p2 */
	Matrix<double> rotation_vectors(1,image_count,3); /* ÿ��ͼ�����ת���� */
	Matrix<double> translation_vectors(1,image_count,3); /* ÿ��ͼ���ƽ������ */


	/* ��ʼ��������Ͻǵ����ά���� */
	int i,j,t;
	for (t=0;t<image_count;t++) {
		for (i=0;i<board_size.height;i++) {
			for (j=0;j<board_size.width;j++) {
				/* ���趨��������������ϵ��z=0��ƽ���� */
				object_points(0,t*board_size.height*board_size.width+i*board_size.width+j,0) = i*s_y;
				object_points(0,t*board_size.height*board_size.width+i*board_size.width+j,1) = j*s_x;
				object_points(0,t*board_size.height*board_size.width+i*board_size.width+j,2) = 0;
			}
		}
	}

	/* ���ǵ�Ĵ洢�ṹת���ɾ�����ʽ */
	for (i=0;i<image_points_seq.cvseq->total;i++) {
		image_points(0,i,0) = image_points_seq[i].x;
		image_points(0,i,1) = image_points_seq[i].y;
	}

	/* ��ʼ��ÿ��ͼ���еĽǵ��������������Ǽ���ÿ��ͼ���ж����Կ��������Ķ���� */
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
	       ���涨����
	*************************************************************************/
	//cout<<"��ʼ���涨����������������";
	Matrix<double> rotation_vector(3,1); /* ����ÿ��ͼ�����ת���� */
	Matrix<double> rotation_matrix(3,3); /* ����ÿ��ͼ�����ת���� */
	

	
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
	       ��ȡÿһ��ͼ�񣬴�����ȡ���ǵ㣬Ȼ��Խǵ���������ؾ�ȷ��
	*************************************************************************/
	//cout<<"��ʼ��ȡ�ǵ㡭����������";
	int count;
	int image_count=1;  /* ͼ������ */
	CvSize image_size;  /* ͼ��ĳߴ� */
	CvSize board_size = cvSize(num_in_raw,num_in_cln);    /* �������ÿ�С��еĽǵ��� */
	CvPoint2D32f* image_points_buf = new CvPoint2D32f[board_size.width*board_size.height];   /* ����ÿ��ͼ���ϼ�⵽�Ľǵ� */
	Seq<CvPoint2D32f> image_points_seq;  /* �����⵽�����нǵ� */
	
	Image<uchar> view(picpath);
	
	image_size.width = view.size().width;
	image_size.height = view.size().height;
	
	/* ��ȡ�ǵ� */
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
		/* �����ؾ�ȷ�� */
		cvFindCornerSubPix( view_gray.cvimage, image_points_buf, count, cvSize(11,11),
			cvSize(-1,-1), cvTermCriteria( CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 30, 0.1 ));
		image_points_seq.push_back(image_points_buf,count);
		/* ��ͼ������ʾ�ǵ�λ�� */
		cvDrawChessboardCorners( view.cvimage, board_size, image_points_buf, count, 1);
		//view.show("calib");
		//cvWaitKey();
		//view.close();
	}
	
	delete []image_points_buf;
	//cout<<"�ǵ���ȡ��ɣ�\n";

	/************************************************************************
	       ���������
	*************************************************************************/
	//cout<<"��ʼ���ꡭ����������";

	Matrix<double> object_points(1,board_size.width*board_size.height*image_count,3); /* ���涨����Ͻǵ����ά���� */
	Matrix<double> image_points(1,image_points_seq.cvseq->total,2); /* ������ȡ�����нǵ� */
	Matrix<int> point_counts(1,image_count,1); /* ÿ��ͼ���нǵ������ */
	Matrix<double> intrinsic_matrix(3,3,1); /* ������ڲ������� */
	Matrix<double> distortion_coeffs(1,4,1); /* �������4������ϵ����k1,k2,p1,p2 */
	Matrix<double> rotation_vectors(1,image_count,3); /* ÿ��ͼ�����ת���� */
	Matrix<double> translation_vectors(1,image_count,3); /* ÿ��ͼ���ƽ������ */


	/* ��ʼ��������Ͻǵ����ά���� */
	int i,j,t;
	for (t=0;t<image_count;t++) {
		for (i=0;i<board_size.height;i++) {
			for (j=0;j<board_size.width;j++) {
				/* ���趨��������������ϵ��z=0��ƽ���� */
				object_points(0,t*board_size.height*board_size.width+i*board_size.width+j,0) = (board_size.height - i- 1)*s_y;
				object_points(0,t*board_size.height*board_size.width+i*board_size.width+j,1) = (board_size.width - j - 1 )*s_x;
				object_points(0,t*board_size.height*board_size.width+i*board_size.width+j,2) = 0;
			}
		}
	}

	/* ���ǵ�Ĵ洢�ṹת���ɾ�����ʽ */
	for (i=0;i<image_points_seq.cvseq->total;i++) {
		image_points(0,i,0) = image_points_seq[i].x;
		image_points(0,i,1) = image_points_seq[i].y;
	}

	/* ��ʼ��ÿ��ͼ���еĽǵ��������������Ǽ���ÿ��ͼ���ж����Կ��������Ķ���� */
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
	       ���涨����
	*************************************************************************/
	//cout<<"��ʼ���涨����������������";
	Matrix<double> rotation_vector(3,1); /* ����ÿ��ͼ�����ת���� */
	Matrix<double> rotation_matrix(3,3); /* ����ÿ��ͼ�����ת���� */
	

	
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
	int image_count=0;  /* ͼ������ */
	CvSize image_size;  /* ͼ��ĳߴ� */
	CvSize board_size = cvSize(num_in_raw,num_in_cln);    /* �������ÿ�С��еĽǵ��� */
	CvPoint2D32f* image_points_buf = new CvPoint2D32f[board_size.width*board_size.height];   /* ����ÿ��ͼ���ϼ�⵽�Ľǵ� */
	Seq<CvPoint2D32f> image_points_seq;  /* �����⵽�����нǵ� */
	
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
		/* ��ȡ�ǵ� */
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

	Matrix<double> object_points(1,board_size.width*board_size.height*image_count,3); /* ���涨����Ͻǵ����ά���� */
	Matrix<double> image_points(1,image_points_seq.cvseq->total,2); /* ������ȡ�����нǵ� */
	Matrix<int> point_counts(1,image_count,1); /* ÿ��ͼ���нǵ������ */
	Matrix<double> intrinsic_matrix(3,3,1); /* ������ڲ������� */
	Matrix<double> distortion_coeffs(1,4,1); /* �������4������ϵ����k1,k2,p1,p2 */
	Matrix<double> rotation_vectors(1,image_count,3); /* ÿ��ͼ�����ת���� */
	Matrix<double> translation_vectors(1,image_count,3); /* ÿ��ͼ���ƽ������ */
	

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

	
	
	/* ���ǵ�Ĵ洢�ṹת���ɾ�����ʽ */
	for (i=0;i<image_points_seq.cvseq->total;i++) {
		image_points(0,i,0) = image_points_seq[i].x;
		image_points(0,i,1) = image_points_seq[i].y;
	}
	
	/* ��ʼ��ÿ��ͼ���еĽǵ��������������Ǽ���ÿ��ͼ���ж����Կ��������Ķ���� */
	for (i=0;i<image_count;i++)
	{
		point_counts(0,i) = board_size.width*board_size.height;
	}
	
	/* ��ʼ���� */
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

		//cout<<"��ʼ���۶�����������������\n";
	double total_err = 0.0; // ����ͼ���ƽ�������ܺ� //
	double err = 0.0; // ÿ��ͼ���ƽ����� /
	Matrix<double> image_points2(1,point_counts(0,0,0),2); // �������¼���õ���ͶӰ�� //

	//cout<<"\tÿ��ͼ��Ķ�����\n";
	fout<<"ÿ��ͼ��Ķ�����\n";
	for (i=0;i<image_count;i++) {
		// ͨ���õ������������������Կռ����ά���������ͶӰ���㣬�õ��µ�ͶӰ�� //
		cvProjectPoints2(object_points.get_cols(i*point_counts(0,0,0),(i+1)*point_counts(0,0,0)-1).cvmat,
						rotation_vectors.get_col(i).cvmat,
						translation_vectors.get_col(i).cvmat,
						intrinsic_matrix.cvmat,
						distortion_coeffs.cvmat,
						image_points2.cvmat,
						0,0,0,0);
		// �����µ�ͶӰ��;ɵ�ͶӰ��֮������//
		err = cvNorm(image_points.get_cols(i*point_counts(0,0,0),(i+1)*point_counts(0,0,0)-1).cvmat,
					image_points2.cvmat,
					CV_L1);
		total_err += err/=point_counts(0,0,0);
		//cout<<"\t\t��"<<i+1<<"��ͼ���ƽ����"<<err<<"����"<<'\n';
		fout<<"\t��"<<i+1<<"��ͼ���ƽ����"<<err<<"����"<<'\n';
	}
	//cout<<"\t����ƽ����"<<total_err/image_count<<"����"<<'\n';
	fout<<"����ƽ����"<<total_err/image_count<<"����"<<'\n'<<'\n';
	//cout<<"������ɣ�\n";
///*/	
	
	return true;
}



bool CCamCaliSys::FindCross(char * path,int num_in_raw,int num_in_cln,p_2D_Point  retp,int retsize)
{
	CvSize image_size;  /* ͼ��ĳߴ� */
	CvSize board_size = cvSize(num_in_raw,num_in_cln);    /* �������ÿ�С��еĽǵ��� */
	CvPoint2D32f* image_points_buf = new CvPoint2D32f[board_size.width*board_size.height];   /* ����ÿ��ͼ���ϼ�⵽�Ľǵ� */
	Seq<CvPoint2D32f> image_points_seq;  /* �����⵽�����нǵ� */

	int count;
	Image<uchar> view(path);
	
	image_size.width = view.size().width;
	image_size.height = view.size().height;
	
	/* ��ȡ�ǵ� */
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
	CvSize board_size = cvSize(num_in_raw,num_in_cln);    /* �������ÿ�С��еĽǵ��� */
	CvPoint2D32f* image_points_buf = new CvPoint2D32f[board_size.width*board_size.height];   /* ����ÿ��ͼ���ϼ�⵽�Ľǵ� */
	Seq<CvPoint2D32f> image_points_seq;  /* �����⵽�����нǵ� */
	
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
		IplImage *dst_gray = cvCreateImage(cvGetSize(pFrame),pFrame->depth,1);//�Ҷ�ͼ

		cvCvtColor(pFrame,dst_gray,CV_BGR2GRAY);//�õ��Ҷ�ͼ
		
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
	CvSize image_size;  /* ͼ��ĳߴ� */
	CvSize board_size = cvSize(num_in_raw,num_in_cln);    /* �������ÿ�С��еĽǵ��� */
	CvPoint2D32f* image_points_buf = new CvPoint2D32f[board_size.width*board_size.height];   /* ����ÿ��ͼ���ϼ�⵽�Ľǵ� */
	Seq<CvPoint2D32f> image_points_seq;  /* �����⵽�����нǵ� */
	
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