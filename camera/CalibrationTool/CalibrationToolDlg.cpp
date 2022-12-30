// CalibrationToolDlg.cpp : implementation file
//
#define EPERM 1 /* Operation not permitted */ 
#define ENOENT 2 /* No such file or directory */
#define ESRCH 3 /* No such process */
#define EIO 5 /* I/O error */
#define ENXIO 6 /* No such device or address */
#define E2BIG 7 /* Argument list too long */
#define ENOEXEC 8 /* Exec format error */
#define EBADF 9 /* Bad file number */
#define ECHILD 10 /* No child processes */

#include "resource.h"
#include "stdafx.h"
#include "CalibrationTool.h"
#include "CalibrationToolDlg.h"

#include <stdio.h>
#include <cerrno>
#include <cstring>



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////保存图片///////
bool jxrSaveBmpFile(char *path, CPIC_RGB *target)
{

	if(target == NULL || path == NULL)
	{
		return false;
	}
	if(target->w <=0 || target->h <= 0 || target->base <=0)
	{
		return false;
	}
	
	
	FILE * fp;
	fp = fopen("D:\\CoderLife\\Image-Experiment\\img_exp4\\tmp.bmp","wb");
    
	if(!fp)
	{
		
        printf("[saveYuvFile] open fail errno = %d, reason = %s", errno, strerror(errno));
		return false;
	}
	

	tagBITMAPFILEHEADER bfh;
	bfh.bfReserved1 = 0;
	bfh.bfReserved2 = 0;
	bfh.bfType = 0x4d42;
	
	
	tagBITMAPINFOHEADER bih;
	bih.biSize = sizeof(tagBITMAPINFOHEADER);
	bih.biWidth = target->w;
	bih.biHeight = target->h;
	bih.biPlanes = 1;
	bih.biBitCount = 24;
	bih.biCompression = 0;
	bih.biSizeImage = 0;
	bih.biXPelsPerMeter = 0;
	bih.biYPelsPerMeter = 0;
	bih.biClrUsed  = 0;
	bih.biClrImportant  = 0;
	
	long span = 3* target->w ; 
	int tag  = span % 4;
	if(tag)
	{
		span += 4;
		span -= tag;
	}
	
	bfh.bfSize = sizeof(tagBITMAPFILEHEADER) 
		+ sizeof(tagBITMAPINFOHEADER)
		+ span * target->h;
	
	bfh.bfOffBits =  sizeof(tagBITMAPFILEHEADER) 
		+ sizeof(tagBITMAPINFOHEADER);
	
	fwrite(&bfh,sizeof(tagBITMAPFILEHEADER),1,fp);
	fwrite(&bih,sizeof(tagBITMAPINFOHEADER),1,fp);
	
	void * buf = malloc(span);
	
	long pos;
	for(long i = 0; i < target->h ; i ++)
	{
		pos = (i)*target->w;
		memcpy(buf,&target->base[pos],target->w * 3);
		fwrite(buf,span,1,fp);
	}
	
	free(buf);
	fclose(fp);
	return true;
}

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCalibrationToolDlg dialog

CCalibrationToolDlg::CCalibrationToolDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCalibrationToolDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCalibrationToolDlg)
	m_camnum = 0;
	m_camserial = 0;
	m_num_in_raw = 11;
	m_num_in_cln = 8;
	m_squre_x = 35;//19.4333;
	m_squre_y = 35;//19.4333;
	m_ts = _T("");
	m_show_p_serial = 0;
	m_o_serial = -1;
	m_x_serial = -1;
	m_y_psp = 1;
	m_x_psp = 1;
	m_y_serial = -1;
	m_show_c = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCalibrationToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCalibrationToolDlg)
	DDX_Control(pDX, IDC_EDIT6, m_e4c);
	DDX_Control(pDX, IDC_EDIT5, m_e3c);
	DDX_Control(pDX, IDC_EDIT4, m_e2c);
	DDX_Control(pDX, IDC_EDIT3, m_e1c);
	DDX_Control(pDX, IDC_STATIC_S, m_show);
	DDX_Text(pDX, IDC_Cam_Num, m_camnum);
	DDX_Text(pDX, IDC_Cam_Serial, m_camserial);
	DDX_Text(pDX, IDC_EDIT3, m_num_in_raw);
	DDX_Text(pDX, IDC_EDIT4, m_num_in_cln);
	DDX_Text(pDX, IDC_EDIT5, m_squre_x);
	DDX_Text(pDX, IDC_EDIT6, m_squre_y);
	DDX_Text(pDX, IDC_Ts, m_ts);
	DDX_Text(pDX, IDC_EDIT1, m_show_p_serial);
	DDX_Text(pDX, IDC_O_Serial, m_o_serial);
	DDX_Text(pDX, IDC_X_Serial, m_x_serial);
	DDX_Text(pDX, IDC_Y_PSP, m_y_psp);
	DDX_Text(pDX, IDC_X_PSP, m_x_psp);
	DDX_Text(pDX, IDC_Y_Serial, m_y_serial);
	DDX_Text(pDX, IDC_EDIT10, m_show_c);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCalibrationToolDlg, CDialog)
	//{{AFX_MSG_MAP(CCalibrationToolDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_Open_Cam, OnOpenCam)
	ON_BN_CLICKED(IDC_GetCamNum, OnGetCamNum)
	ON_BN_CLICKED(IDC_Get_Pic, OnGetPic)
	ON_BN_CLICKED(IDC_BUTTON4, OnButton4)
	ON_BN_CLICKED(IDC_ResetChessboard, OnResetChessboard)
	ON_BN_CLICKED(IDC_ShowChessboard, OnShowChessboard)
	ON_BN_CLICKED(IDC_ShowCorners, OnShowCorners)
	ON_BN_CLICKED(IDC_SaveAsInPic, OnSaveAsInPic)
	ON_BN_CLICKED(IDC_DelPic, OnDelPic)
	ON_BN_CLICKED(IDC_CaliInPara, OnCaliInPara)
	ON_BN_CLICKED(IDC_Save_In_para, OnSaveInpara)
	ON_BN_CLICKED(IDC_LoadInPara, OnLoadInPara)
	ON_BN_CLICKED(IDC_CaliExPara, OnCaliExPara)
	ON_BN_CLICKED(IDC_CaliExParai, OnCaliExParai)
	ON_BN_CLICKED(IDC_SaveExPara, OnSaveExPara)
	ON_BN_CLICKED(IDC_BUTTON17, OnButton17)
	ON_BN_CLICKED(IDC_BUTTON18, OnButton18)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_BN_CLICKED(IDC_BUTTON5, OnButton5)
	ON_BN_CLICKED(IDC_BUTTON6, OnButton6)
	ON_BN_CLICKED(IDC_BUTTON7, OnButton7)
	ON_BN_CLICKED(IDC_BUTTON8, OnButton8)
	ON_BN_CLICKED(IDC_BUTTON9, OnButton9)
	ON_BN_CLICKED(IDC_STATIC_S, OnStaticS)
	ON_WM_LBUTTONDBLCLK()
	ON_BN_CLICKED(IDC_BUTTON10, OnButton10)
	ON_BN_CLICKED(IDC_BUTTON11, OnButton11)
	ON_BN_CLICKED(IDC_BUTTON12, OnButton12)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON13, OnButton13)
	ON_BN_CLICKED(IDC_BUTTON14, OnButton14)
	ON_BN_CLICKED(IDC_BUTTON15, OnButton15)
	ON_BN_CLICKED(IDC_BUTTON16, OnButton16)
	ON_BN_CLICKED(IDC_BUTTON19, OnButton19)
	ON_BN_CLICKED(IDC_BUTTON20, OnButton20)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCalibrationToolDlg message handlers

BOOL CCalibrationToolDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	hdib = NULL;
	shdc = NULL;
	CBCNum = CSDCSIZE;
	for(int i = 0; i < CSDCSIZE; i ++)
	{
		CBCS[i].x = 0;
		CBCS[i].y = 0;
	}

	showss_tmp.init(640,480);

	pic_scale = 1;
	ppnum  = 0;
	picserial = 0;
	showtag = false;
	SetTimer(1,300,NULL);

	m_l_r_o = 0;
	return TRUE;  // return TRUE  unless you set the focus to a control

}

void CCalibrationToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CCalibrationToolDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CCalibrationToolDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


void CCalibrationToolDlg::OnShowPic(CPIC_RGB *  pic,HDC hdc)
{
	if(pic == NULL)
	{
		return ;
	}
	if(hdib == NULL)
	{
		hdib = DrawDibOpen();
	}
	
	
	if(pic->w > 640 )
	{
		
		RGB_CELL tt;
		RGB_CELL tt2;
		tt2.r = 255;
		tt2.g = 255;
		tt2.b = 255;
		
		long scale = pic->w / 640;
		if(scale * pic->w < 640)
		{
			scale += 1.0;
		}
		for(long i = 0; i < 640 ; i ++)
		{
			long ni = i * scale;
			if(ni < pic->w)
			{
				for(long j = 0; j < 480 ; j ++)
				{
					long nj = j * scale;
					if(nj < pic->h)
					{
						pic->GetRGB(ni,nj,&tt);
						showss_tmp.SetRGB(i,j,&tt);
						
					}
					else
					{
						showss_tmp.SetRGB(i,j,&tt2);
					}
				}
			}
			else
			{
				for(long j = 0; j < 480 ; j ++)
				{	
					showss_tmp.SetRGB(i,j,&tt2);	
				}
			}
		}
		
		tagBITMAPINFOHEADER bih;
		bih.biSize = sizeof(tagBITMAPINFOHEADER);
		bih.biWidth = showss_tmp.w;
		bih.biHeight = showss_tmp.h;
		bih.biPlanes = 1;
		bih.biBitCount = 24;
		bih.biCompression = BI_RGB;
		bih.biSizeImage = 0;
		bih.biXPelsPerMeter = 0;
		bih.biYPelsPerMeter = 0;
		bih.biClrUsed  = 0;
		bih.biClrImportant  = 0;
		DrawDibDraw(hdib,hdc,0,0,showss_tmp.w,showss_tmp.h,	&bih,showss_tmp.base,0,0,-1,-1,0);
		
	}
	else
	{
		tagBITMAPINFOHEADER bih;
		bih.biSize = sizeof(tagBITMAPINFOHEADER);
		bih.biWidth = pic->w;
		bih.biHeight = pic->h;
		bih.biPlanes = 1;
		bih.biBitCount = 24;
		bih.biCompression = BI_RGB;
		bih.biSizeImage = 0;
		bih.biXPelsPerMeter = 0;
		bih.biYPelsPerMeter = 0;
		bih.biClrUsed  = 0;
		bih.biClrImportant  = 0;
		DrawDibDraw(hdib,hdc,0,0,pic->w,pic->h,	&bih,pic->base,0,0,-1,-1,0);
	}
	

}

void CCalibrationToolDlg::OnOpenCam() 
{
	// TODO: Add your control notification handler code here
    
    shdc = m_show.GetDC()->m_hDC;
	camera.CloseCamera();
	UpdateData(true);
	if(m_camserial >= m_camnum)
	{
		AfxMessageBox("error camera selected!");
	}
	camera.CloseCamera();
	if(camera.OpenCamera(m_camserial,true)==false)
	{
		AfxMessageBox("open camera error!");
	}
	else
	{
		CString tmp;
		tmp.Format("open camera %d",m_camserial);
		AfxMessageBox(tmp);
	}

}

void CCalibrationToolDlg::OnGetCamNum() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	this->m_camnum = CCameraDS::CameraCount();
	UpdateData(false);
}

void CCalibrationToolDlg::OnGetPic() 
{
	// TODO: Add your control notification handler code here
	if(camera.m_bConnected == false)
	{
		return ;
	}
	IplImage *pFrame = camera.QueryFrame();

	if(m_l_r_o == 0)
	{
		pic_scale = pFrame->width / 640;
		if(pic_scale * pFrame->width < 640)
		{
			pic_scale += 1.0;
		}


		CPIC_RGB pic_org;	
		pic_org.init(pFrame->width,pFrame->height);
		pic_org.SetBitBYTE(pFrame->width,pFrame->height,pFrame->imageDataOrigin);

		this->OnShowPic(&pic_org,shdc);
	}
	else
	{
		pic_scale = pFrame->width/2 / 640;
		if(pic_scale * pFrame->width < 640)
		{
			pic_scale += 1.0;
		}


		CPIC_RGB pic_org;	
		pic_org.init(pFrame->width,pFrame->height);
		pic_org.SetBitBYTE(pFrame->width,pFrame->height,pFrame->imageDataOrigin);

		CPIC_RGB pic_org1;
		pic_org1.init(pFrame->width/2,pFrame->height);

		if(m_l_r_o == -1)
		{
			pic_org.OnCpyL(&pic_org1);
		}
		else
		{
			pic_org.OnCpyR(&pic_org1);
		}


		this->OnShowPic(&pic_org1,shdc);
	}
	

}

void CCalibrationToolDlg::OnButton4() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	if(m_num_in_raw >1 && m_num_in_cln>1 && m_num_in_cln *m_num_in_raw  > 7 )
	{
		m_e1c.EnableWindow(false);
		m_e2c.EnableWindow(false);
		m_e3c.EnableWindow(false);
		m_e4c.EnableWindow(false);
	}
}

void CCalibrationToolDlg::OnResetChessboard() 
{
	// TODO: Add your control notification handler code here
	
	m_e1c.EnableWindow(true);
	m_e2c.EnableWindow(true);
	m_e3c.EnableWindow(true);
	m_e4c.EnableWindow(true);
	UpdateData(false);
}

void CCalibrationToolDlg::OnShowChessboard() 
{
	// TODO: Add your control notification handler code here
	for(int i = 0; i < CSDCSIZE; i ++)
	{
		CBCS[i].x = 0;
		CBCS[i].y = 0;
	}
	if(this->m_e1c.IsWindowEnabled() != 0)
	{
		AfxMessageBox("请设定标盘规格");
		return;
	}
	if(camera.m_bConnected == false)
	{
		AfxMessageBox("请先启动摄像头");
		return ;
	}
	if( m_num_in_cln *m_num_in_raw  <= 7)
	{
		AfxMessageBox("请设定标盘规格至少8个内点");
		return;
	}

	CString path;
	CString linePath;
	CPIC_RGB rgbpic;

	if(m_l_r_o == 0)
	{
		IplImage *pFrame = camera.QueryFrame();	
		pic_scale = pFrame->width / 640;
		if(pic_scale * pFrame->width < 640)
		{
			pic_scale += 1.0;
		}


		rgbpic.init(pFrame->width,pFrame->height);
		rgbpic.SetBitBYTE(pFrame->width,pFrame->height,pFrame->imageDataOrigin);

		this->OnShowPic(&rgbpic,shdc);
		path.Format("D:\\CoderLife\\Image-Experiment\\img_exp4\\tmp.bmp"); 
		bool tag = jxrSaveBmpFile(path.GetBuffer(path.GetLength()+1),&rgbpic);
		if(tag == false)
		{
			AfxMessageBox("1.保存用于识别的图片失败!");
			return ;
		}
	}


	if(m_l_r_o == -1)
	{
		IplImage *pFrame = camera.QueryFrame();	
		pic_scale = pFrame->width/2 / 640;
		if(pic_scale * pFrame->width/2 < 640)
		{
			pic_scale += 1.0;
		}

		
		CPIC_RGB tmp;
		
		tmp.init(pFrame->width,pFrame->height);
		tmp.SetBitBYTE(pFrame->width,pFrame->height,pFrame->imageDataOrigin);

		rgbpic.init(pFrame->width/2,pFrame->height);
		tmp.OnCpyL(&rgbpic);

		this->OnShowPic(&rgbpic,shdc);
		path.Format("D:\\CoderLife\\Image-Experiment\\img_exp4\\tmp.bmp"); 
		bool tag = jxrSaveBmpFile(path.GetBuffer(path.GetLength()+1),&rgbpic);
		if(tag == false)
		{
			AfxMessageBox("2.保存用于识别的图片失败!");
			return ;
		}
	}

	if(m_l_r_o ==  1)
	{
		IplImage *pFrame = camera.QueryFrame();	
		pic_scale = pFrame->width/2 / 640;
		if(pic_scale * pFrame->width/2 < 640)
		{
			pic_scale += 1.0;
		}

		
		CPIC_RGB tmp;
		
		tmp.init(pFrame->width,pFrame->height);
		tmp.SetBitBYTE(pFrame->width,pFrame->height,pFrame->imageDataOrigin);

		rgbpic.init(pFrame->width/2,pFrame->height);
		tmp.OnCpyR(&rgbpic);

		this->OnShowPic(&rgbpic,shdc);
        path.Format("D:\\CoderLife\\Image-Experiment\\img_exp4\\tmp.bmp"); 
		bool tag = jxrSaveBmpFile(path.GetBuffer(path.GetLength()+1),&rgbpic);
		if(tag == false)
		{
			AfxMessageBox("3.保存用于识别的图片失败!");
			return ;
		}
	}
	
	
	if(CCamCaliSys::FindCross(path.GetBuffer(path.GetLength()+1),m_num_in_raw,m_num_in_cln,CBCS,CBCNum)==false)
	{
		AfxMessageBox("图片的角点不能被完全识别,请调整图片或检查角点设置!");
		return ;
	}
	
	if(hdib != 0)
	{
		SetPixel(shdc,CBCS[0].x/pic_scale,CBCS[0].y/pic_scale,RGB(0,255,0));
		SetPixel(shdc,CBCS[0].x/pic_scale + 1,CBCS[0].y/pic_scale,RGB(0,255,0));
		SetPixel(shdc,CBCS[0].x/pic_scale - 1,CBCS[0].y/pic_scale,RGB(0,255,0));
		
		SetPixel(shdc,CBCS[0].x/pic_scale,CBCS[0].y/pic_scale+1,RGB(0,255,0));
		SetPixel(shdc,CBCS[0].x/pic_scale + 1,CBCS[0].y/pic_scale+1,RGB(0,255,0));
		SetPixel(shdc,CBCS[0].x/pic_scale - 1,CBCS[0].y/pic_scale+1,RGB(0,255,0));
		
		SetPixel(shdc,CBCS[0].x/pic_scale,CBCS[0].y/pic_scale-1,RGB(0,255,0));
		SetPixel(shdc,CBCS[0].x/pic_scale + 1,CBCS[0].y/pic_scale-1,RGB(0,255,0));
		SetPixel(shdc,CBCS[0].x/pic_scale - 1,CBCS[0].y/pic_scale-1,RGB(0,255,0));


		SetPixel(shdc,CBCS[1].x/pic_scale,CBCS[1].y/pic_scale,RGB(0,255,0));
		SetPixel(shdc,CBCS[1].x/pic_scale + 1,CBCS[1].y/pic_scale,RGB(0,255,0));
		SetPixel(shdc,CBCS[1].x/pic_scale - 1,CBCS[1].y/pic_scale,RGB(0,255,0));
		
		SetPixel(shdc,CBCS[1].x/pic_scale,CBCS[1].y/pic_scale+1,RGB(0,255,0));
		SetPixel(shdc,CBCS[1].x/pic_scale + 1,CBCS[1].y/pic_scale+1,RGB(0,255,0));
		SetPixel(shdc,CBCS[1].x/pic_scale - 1,CBCS[1].y/pic_scale+1,RGB(0,255,0));
		
		SetPixel(shdc,CBCS[1].x/pic_scale,CBCS[1].y/pic_scale-1,RGB(0,255,0));
		SetPixel(shdc,CBCS[1].x/pic_scale + 1,CBCS[1].y/pic_scale-1,RGB(0,255,0));
		SetPixel(shdc,CBCS[1].x/pic_scale - 1,CBCS[1].y/pic_scale-1,RGB(0,255,0));


		
		for(int i = 2; i < CBCNum ; i ++)
		{
			SetPixel(shdc,CBCS[i].x/pic_scale,CBCS[i].y/pic_scale,RGB(255,0,0));
			SetPixel(shdc,CBCS[i].x/pic_scale + 1,CBCS[i].y/pic_scale,RGB(255,0,0));
			SetPixel(shdc,CBCS[i].x/pic_scale - 1,CBCS[i].y/pic_scale,RGB(255,0,0));
			
			SetPixel(shdc,CBCS[i].x/pic_scale,CBCS[i].y/pic_scale+1,RGB(255,0,0));
			SetPixel(shdc,CBCS[i].x/pic_scale + 1,CBCS[i].y/pic_scale+1,RGB(255,0,0));
			SetPixel(shdc,CBCS[i].x/pic_scale - 1,CBCS[i].y/pic_scale+1,RGB(255,0,0));
			
			SetPixel(shdc,CBCS[i].x/pic_scale,CBCS[i].y-1,RGB(255,0,0));
			SetPixel(shdc,CBCS[i].x/pic_scale + 1,CBCS[i].y/pic_scale-1,RGB(255,0,0));
			SetPixel(shdc,CBCS[i].x/pic_scale - 1,CBCS[i].y/pic_scale-1,RGB(255,0,0));
		}
	}
}

void CCalibrationToolDlg::OnShowCorners() 
{
	serialv = 0;
	for(int i = 0; i < CSDCSIZE; i ++)
	{
		CBCS[i].x = 0;
		CBCS[i].y = 0;
	}
	if(this->m_e1c.IsWindowEnabled() != 0)
	{
		AfxMessageBox("请设定标盘规格");
		return;
	}
	if(camera.m_bConnected == false)
	{
		AfxMessageBox("请先启动摄像头");
		return ;
	}
	if( m_num_in_cln *m_num_in_raw  <= 7)
	{
		AfxMessageBox("请设定标盘规格至少8个内点");
		return;
	}
	
	CString path;
	CString linePath;
	CPIC_RGB t_rgbpic;
	
	m_o_serial = -1;
	m_x_serial = -1;
	m_y_serial = -1;
	m_x_psp = 0;
	m_y_psp = 0;
	
	IplImage *pFrame = camera.QueryFrame();	
	pic_scale = pFrame->width / 640;
	if(pic_scale * pFrame->width < 640)
	{
		pic_scale += 1.0;
	}
	t_rgbpic.init(pFrame->width,pFrame->height);
	t2_rgbpic.init(pFrame->width,pFrame->height);
	t_rgbpic.SetBitBYTE(pFrame->width,pFrame->height,pFrame->imageDataOrigin);
	t_rgbpic.CpyPic(&t2_rgbpic);
	this->OnShowPic(&t_rgbpic,shdc);
	path.Format("D:\\CoderLife\\Image-Experiment\\img_exp4\\tmp.bmp"); 
	bool tag = jxrSaveBmpFile(path.GetBuffer(path.GetLength()+1),&t_rgbpic);
	if(tag == false)
	{
		AfxMessageBox("4.保存用于识别的图片失败!");
		return ;
	}
	
	ppnum = CCamCaliSys::FindCross2(path.GetBuffer(path.GetLength()+1),m_num_in_raw,m_num_in_cln,CBCS2,CBCNum);
	if(ppnum ==0)
	{
		AfxMessageBox("图片的角点不能被完全识别,请调整图片或检查角点设置!");
		return ;
	}
	
	if(hdib != 0)
	{
		SetPixel(shdc,CBCS2[0].x/pic_scale,CBCS2[0].y/pic_scale,RGB(0,255,0));
		SetPixel(shdc,CBCS2[0].x/pic_scale + 1,CBCS2[0].y/pic_scale,RGB(0,255,0));
		SetPixel(shdc,CBCS2[0].x/pic_scale - 1,CBCS2[0].y/pic_scale,RGB(0,255,0));
		
		SetPixel(shdc,CBCS2[0].x/pic_scale,CBCS2[0].y/pic_scale+1,RGB(0,255,0));
		SetPixel(shdc,CBCS2[0].x/pic_scale + 1,CBCS2[0].y/pic_scale+1,RGB(0,255,0));
		SetPixel(shdc,CBCS2[0].x/pic_scale - 1,CBCS2[0].y/pic_scale+1,RGB(0,255,0));
		
		SetPixel(shdc,CBCS2[0].x/pic_scale,CBCS2[0].y/pic_scale-1,RGB(0,255,0));
		SetPixel(shdc,CBCS2[0].x/pic_scale + 1,CBCS2[0].y/pic_scale-1,RGB(0,255,0));
		SetPixel(shdc,CBCS2[0].x/pic_scale - 1,CBCS2[0].y/pic_scale-1,RGB(0,255,0));
		
		
		SetPixel(shdc,CBCS2[1].x/pic_scale,CBCS2[1].y/pic_scale,RGB(0,255,0));
		SetPixel(shdc,CBCS2[1].x/pic_scale + 1,CBCS2[1].y/pic_scale,RGB(0,255,0));
		SetPixel(shdc,CBCS2[1].x/pic_scale - 1,CBCS2[1].y/pic_scale,RGB(0,255,0));
		
		SetPixel(shdc,CBCS2[1].x/pic_scale,CBCS2[1].y/pic_scale+1,RGB(0,255,0));
		SetPixel(shdc,CBCS2[1].x/pic_scale + 1,CBCS2[1].y/pic_scale+1,RGB(0,255,0));
		SetPixel(shdc,CBCS2[1].x/pic_scale - 1,CBCS2[1].y/pic_scale+1,RGB(0,255,0));
		
		SetPixel(shdc,CBCS2[1].x/pic_scale,CBCS2[1].y/pic_scale-1,RGB(0,255,0));
		SetPixel(shdc,CBCS2[1].x/pic_scale + 1,CBCS2[1].y/pic_scale-1,RGB(0,255,0));
		SetPixel(shdc,CBCS2[1].x/pic_scale - 1,CBCS2[1].y/pic_scale-1,RGB(0,255,0));
		
		
		
		for(int i = 2; i < ppnum ; i ++)
		{
			SetPixel(shdc,CBCS2[i].x/pic_scale,CBCS2[i].y/pic_scale,RGB(255,0,0));
			SetPixel(shdc,CBCS2[i].x/pic_scale + 1,CBCS2[i].y/pic_scale,RGB(255,0,0));
			SetPixel(shdc,CBCS2[i].x/pic_scale - 1,CBCS2[i].y/pic_scale,RGB(255,0,0));
			
			SetPixel(shdc,CBCS2[i].x/pic_scale,CBCS2[i].y/pic_scale+1,RGB(255,0,0));
			SetPixel(shdc,CBCS2[i].x/pic_scale + 1,CBCS2[i].y/pic_scale+1,RGB(255,0,0));
			SetPixel(shdc,CBCS2[i].x/pic_scale - 1,CBCS2[i].y/pic_scale+1,RGB(255,0,0));
			
			SetPixel(shdc,CBCS2[i].x/pic_scale,CBCS2[i].y/pic_scale-1,RGB(255,0,0));
			SetPixel(shdc,CBCS2[i].x/pic_scale + 1,CBCS2[i].y/pic_scale-1,RGB(255,0,0));
			SetPixel(shdc,CBCS2[i].x/pic_scale - 1,CBCS2[i].y/pic_scale-1,RGB(255,0,0));
		}
	}
	
}

void CCalibrationToolDlg::OnSaveAsInPic() 
{
	// TODO: Add your control notification handler code here
	for(int i = 0; i < CSDCSIZE; i ++)
	{
		CBCS[i].x = 0;
		CBCS[i].y = 0;
	}
	if(this->m_e1c.IsWindowEnabled() != 0)
	{
		AfxMessageBox("请设定标盘规格");
		return;
	}
	if(camera.m_bConnected == false)
	{
		AfxMessageBox("请先启动摄像头");
		return ;
	}

	CString path;
	CString linePath;
	CPIC_RGB rgbpic;


	if(m_l_r_o == 0)
	{
		IplImage *pFrame = camera.QueryFrame();	
		pic_scale = pFrame->width / 640;
		if(pic_scale * pFrame->width < 640)
		{
			pic_scale += 1.0;
		}


		rgbpic.init(pFrame->width,pFrame->height);
		rgbpic.SetBitBYTE(pFrame->width,pFrame->height,pFrame->imageDataOrigin);

		this->OnShowPic(&rgbpic,shdc);
	}


	if(m_l_r_o == -1)
	{
		IplImage *pFrame = camera.QueryFrame();	
		pic_scale = pFrame->width/2 / 640;
		if(pic_scale * pFrame->width/2 < 640)
		{
			pic_scale += 1.0;
		}

		
		CPIC_RGB tmp;
		
		tmp.init(pFrame->width,pFrame->height);
		tmp.SetBitBYTE(pFrame->width,pFrame->height,pFrame->imageDataOrigin);

		rgbpic.init(pFrame->width/2,pFrame->height);
		tmp.OnCpyL(&rgbpic);

		this->OnShowPic(&rgbpic,shdc);
	}

	if(m_l_r_o ==  1)
	{
		IplImage *pFrame = camera.QueryFrame();	
		pic_scale = pFrame->width/2 / 640;
		if(pic_scale * pFrame->width/2 < 640)
		{
			pic_scale += 1.0;
		}

		
		CPIC_RGB tmp;
		
		tmp.init(pFrame->width,pFrame->height);
		tmp.SetBitBYTE(pFrame->width,pFrame->height,pFrame->imageDataOrigin);

		rgbpic.init(pFrame->width/2,pFrame->height);
		tmp.OnCpyR(&rgbpic);

		this->OnShowPic(&rgbpic,shdc);
	
	}

	
	path.Format("D:\\CoderLife\\Image-Experiment\\img_exp4\\tmp.bmp"); 
    bool tag = jxrSaveBmpFile(path.GetBuffer(path.GetLength()+1),&rgbpic);
	if(tag == false)
	{
		AfxMessageBox("5.保存用于识别的图片失败!");
		return ;
	}
	
	if(CCamCaliSys::FindCross(path.GetBuffer(path.GetLength()+1),m_num_in_raw,m_num_in_cln,CBCS,CBCNum)==false)
	{
	//	AfxMessageBox("图片的角点不能被完全识别,请调整图片或检查角点设置!");
		return ;
	}
	
	if(hdib != 0)
	{
		SetPixel(shdc,CBCS[0].x/pic_scale,CBCS[0].y/pic_scale,RGB(0,255,0));
		SetPixel(shdc,CBCS[0].x/pic_scale + 1,CBCS[0].y/pic_scale,RGB(0,255,0));
		SetPixel(shdc,CBCS[0].x/pic_scale - 1,CBCS[0].y/pic_scale,RGB(0,255,0));
		
		SetPixel(shdc,CBCS[0].x/pic_scale,CBCS[0].y/pic_scale+1,RGB(0,255,0));
		SetPixel(shdc,CBCS[0].x/pic_scale + 1,CBCS[0].y/pic_scale+1,RGB(0,255,0));
		SetPixel(shdc,CBCS[0].x/pic_scale - 1,CBCS[0].y/pic_scale+1,RGB(0,255,0));
		
		SetPixel(shdc,CBCS[0].x/pic_scale,CBCS[0].y/pic_scale-1,RGB(0,255,0));
		SetPixel(shdc,CBCS[0].x/pic_scale + 1,CBCS[0].y/pic_scale-1,RGB(0,255,0));
		SetPixel(shdc,CBCS[0].x/pic_scale - 1,CBCS[0].y/pic_scale-1,RGB(0,255,0));
		
		for(int i = 1; i < CBCNum ; i ++)
		{
			SetPixel(shdc,CBCS[i].x/pic_scale,CBCS[i].y/pic_scale,RGB(255,0,0));
			SetPixel(shdc,CBCS[i].x/pic_scale + 1,CBCS[i].y/pic_scale,RGB(255,0,0));
			SetPixel(shdc,CBCS[i].x/pic_scale - 1,CBCS[i].y/pic_scale,RGB(255,0,0));
			
			SetPixel(shdc,CBCS[i].x/pic_scale,CBCS[i].y/pic_scale+1,RGB(255,0,0));
			SetPixel(shdc,CBCS[i].x/pic_scale + 1,CBCS[i].y/pic_scale+1,RGB(255,0,0));
			SetPixel(shdc,CBCS[i].x/pic_scale - 1,CBCS[i].y/pic_scale+1,RGB(255,0,0));
			
			SetPixel(shdc,CBCS[i].x/pic_scale,CBCS[i].y/pic_scale-1,RGB(255,0,0));
			SetPixel(shdc,CBCS[i].x/pic_scale + 1,CBCS[i].y/pic_scale-1,RGB(255,0,0));
			SetPixel(shdc,CBCS[i].x/pic_scale - 1,CBCS[i].y/pic_scale-1,RGB(255,0,0));
		}
	}

	path.Format("D:\\CoderLife\\Image-Experiment\\img_exp4\\tmp.bmp",picserial); //"E:\\firstlesson\\pics\\%d.bmp"
	tag = jxrSaveBmpFile(path.GetBuffer(path.GetLength()+1),&rgbpic);
	if(tag == false)
	{
		AfxMessageBox("保存失败");
		return ;
	}

	linePath = "D:\\CoderLife\\Image-Experiment\\img_exp4\\line.txt";
	if(picserial == 0)
	{
		CCamCaliSys::EmptyLine(linePath.GetBuffer(linePath.GetLength()+1));
	}
	SaveLine(linePath,path);

	picserial ++;
	CString aaa;
	aaa = "保存至:";
	aaa += path;
	m_ts = aaa;
	UpdateData(false);
}

void CCalibrationToolDlg::OnDelPic() 
{
	// TODO: Add your control notification handler code here
	picserial = 0;
}

void CCalibrationToolDlg::OnCaliInPara() 
{
	// TODO: Add your control notification handler code here
	if(picserial < 5)
	{
	//	AfxMessageBox("请采用至少5张不同的图片，建议20张以上");
	//	return;
	}
	UpdateData(true);
	UpdateData(false);
	
	CString path = "D:\\CoderLife\\Image-Experiment\\img_exp4\\line.txt";

	CCamCaliSys::GetIntrinsicPara(path.GetBuffer(path.GetLength()+1),
		&ip,&dp,
		m_num_in_raw,
		m_num_in_cln
		,this->m_squre_x,this->m_squre_y);
	
	CString aaa;
	char c1,c2;
	c1 = 0x0d;
	c2 = 0x0a;
	aaa.Format("内参数:fx=%.12f  fy=%.12f  cx=%.12f  cy=%.12f%c%c畸变系数:  k1=%.12f  k2=%.12f  p1=%.12f  p2=%.12f ",
		ip.fx,ip.fy,ip.cx,ip.cy,
		c1,c2,
		dp.k1,dp.k2,dp.p1,dp.p2);
	this->m_ts = aaa;
	UpdateData(false);
}

void CCalibrationToolDlg::OnSaveInpara() 
{
	// TODO: Add your control notification handler code here
	CFileDialog fd(false,NULL,NULL,0,".in|*.in");
	if(fd.DoModal()==IDOK)
	{
		CString path2 = fd.GetPathName();
			if(path2.Find(".in") == -1 || path2.Find(".IN") == -1)
		{
			path2 += ".in";
		}
		CGeometry::SaveIntrinsicPara(path2.GetBuffer(path2.GetLength()+1),&ip,&dp);
		CString aaa = "保存至:";
		aaa += path2;
		m_ts = aaa;
		UpdateData(false);
	}
}

void CCalibrationToolDlg::OnLoadInPara() 
{
	// TODO: Add your control notification handler code here
	CFileDialog fd(true,NULL,NULL,0,".in|*.in");
	if(fd.DoModal()==IDOK)
	{
		CString path = fd.GetPathName();
		CGeometry::LoadIntrinsicPara(path.GetBuffer(path.GetLength()+1),&ip,&dp);
	}
}

void CCalibrationToolDlg::OnCaliExPara() 
{
	// TODO: Add your control notification handler code here


	for(int i = 0; i < CSDCSIZE; i ++)
	{
		CBCS[i].x = 0;
		CBCS[i].y = 0;
	}
	if(this->m_e1c.IsWindowEnabled() != 0)
	{
		AfxMessageBox("请设定标盘规格");
		return;
	}
	if(camera.m_bConnected == false)
	{
		AfxMessageBox("请先启动摄像头");
		return ;
	}

	CString path;
	CString linePath;
	CPIC_RGB rgbpic;


	if(m_l_r_o == 0)
	{
		IplImage *pFrame = camera.QueryFrame();	
		pic_scale = pFrame->width / 640;
		if(pic_scale * pFrame->width < 640)
		{
			pic_scale += 1.0;
		}


		rgbpic.init(pFrame->width,pFrame->height);
		rgbpic.SetBitBYTE(pFrame->width,pFrame->height,pFrame->imageDataOrigin);

		this->OnShowPic(&rgbpic,shdc);
	}


	if(m_l_r_o == -1)
	{
		IplImage *pFrame = camera.QueryFrame();	
		pic_scale = pFrame->width/2 / 640;
		if(pic_scale * pFrame->width/2 < 640)
		{
			pic_scale += 1.0;
		}

		
		CPIC_RGB tmp;
		
		tmp.init(pFrame->width,pFrame->height);
		tmp.SetBitBYTE(pFrame->width,pFrame->height,pFrame->imageDataOrigin);

		rgbpic.init(pFrame->width/2,pFrame->height);
		tmp.OnCpyL(&rgbpic);

		this->OnShowPic(&rgbpic,shdc);
	}

	if(m_l_r_o ==  1)
	{
		IplImage *pFrame = camera.QueryFrame();	
		pic_scale = pFrame->width/2 / 640;
		if(pic_scale * pFrame->width/2 < 640)
		{
			pic_scale += 1.0;
		}

		
		CPIC_RGB tmp;
		
		tmp.init(pFrame->width,pFrame->height);
		tmp.SetBitBYTE(pFrame->width,pFrame->height,pFrame->imageDataOrigin);

		rgbpic.init(pFrame->width/2,pFrame->height);
		tmp.OnCpyR(&rgbpic);

		this->OnShowPic(&rgbpic,shdc);
	
	}

	
    path.Format("D:\\CoderLife\\Image-Experiment\\img_exp4\\tmp.bmp"); 
	bool tag = jxrSaveBmpFile(path.GetBuffer(path.GetLength()+1),&rgbpic);
	if(tag == false)
	{
		AfxMessageBox("6.保存用于识别的图片失败!");
		return ;
	}
	
	if(CCamCaliSys::FindCross(path.GetBuffer(path.GetLength()+1),m_num_in_raw,m_num_in_cln,CBCS,CBCNum)==false)
	{
	//	AfxMessageBox("图片的角点不能被完全识别,请调整图片或检查角点设置!");
		return ;
	}
	
	if(hdib != 0)
	{
		SetPixel(shdc,CBCS[0].x/pic_scale,CBCS[0].y/pic_scale,RGB(0,255,0));
		SetPixel(shdc,CBCS[0].x/pic_scale + 1,CBCS[0].y/pic_scale,RGB(0,255,0));
		SetPixel(shdc,CBCS[0].x/pic_scale - 1,CBCS[0].y/pic_scale,RGB(0,255,0));
		
		SetPixel(shdc,CBCS[0].x/pic_scale,CBCS[0].y/pic_scale+1,RGB(0,255,0));
		SetPixel(shdc,CBCS[0].x/pic_scale + 1,CBCS[0].y/pic_scale+1,RGB(0,255,0));
		SetPixel(shdc,CBCS[0].x/pic_scale - 1,CBCS[0].y/pic_scale+1,RGB(0,255,0));
		
		SetPixel(shdc,CBCS[0].x/pic_scale,CBCS[0].y/pic_scale-1,RGB(0,255,0));
		SetPixel(shdc,CBCS[0].x/pic_scale + 1,CBCS[0].y/pic_scale-1,RGB(0,255,0));
		SetPixel(shdc,CBCS[0].x/pic_scale - 1,CBCS[0].y/pic_scale-1,RGB(0,255,0));
		
		for(int i = 1; i < CBCNum ; i ++)
		{
			SetPixel(shdc,CBCS[i].x/pic_scale,CBCS[i].y/pic_scale,RGB(255,0,0));
			SetPixel(shdc,CBCS[i].x/pic_scale + 1,CBCS[i].y/pic_scale,RGB(255,0,0));
			SetPixel(shdc,CBCS[i].x/pic_scale - 1,CBCS[i].y/pic_scale,RGB(255,0,0));
			
			SetPixel(shdc,CBCS[i].x/pic_scale,CBCS[i].y/pic_scale+1,RGB(255,0,0));
			SetPixel(shdc,CBCS[i].x/pic_scale + 1,CBCS[i].y/pic_scale+1,RGB(255,0,0));
			SetPixel(shdc,CBCS[i].x/pic_scale - 1,CBCS[i].y/pic_scale+1,RGB(255,0,0));
			
			SetPixel(shdc,CBCS[i].x/pic_scale,CBCS[i].y/pic_scale-1,RGB(255,0,0));
			SetPixel(shdc,CBCS[i].x/pic_scale + 1,CBCS[i].y/pic_scale-1,RGB(255,0,0));
			SetPixel(shdc,CBCS[i].x/pic_scale - 1,CBCS[i].y/pic_scale-1,RGB(255,0,0));
		}
	}
	FILE * fps = fopen("D:\\CoderLife\\Image-Experiment\\img_exp4\\expoint.txt","w");
	
	if(hdib != 0)
	{
		SetPixel(shdc,CBCS[0].x/pic_scale,CBCS[0].y/pic_scale,RGB(0,255,0));
		SetPixel(shdc,CBCS[0].x/pic_scale + 1,CBCS[0].y/pic_scale,RGB(0,255,0));
		SetPixel(shdc,CBCS[0].x/pic_scale - 1,CBCS[0].y/pic_scale,RGB(0,255,0));
		
		SetPixel(shdc,CBCS[0].x/pic_scale,CBCS[0].y/pic_scale+1,RGB(0,255,0));
		SetPixel(shdc,CBCS[0].x/pic_scale + 1,CBCS[0].y/pic_scale+1,RGB(0,255,0));
		SetPixel(shdc,CBCS[0].x/pic_scale - 1,CBCS[0].y/pic_scale+1,RGB(0,255,0));
		
		SetPixel(shdc,CBCS[0].x/pic_scale,CBCS[0].y/pic_scale-1,RGB(0,255,0));
		SetPixel(shdc,CBCS[0].x/pic_scale + 1,CBCS[0].y/pic_scale-1,RGB(0,255,0));
		SetPixel(shdc,CBCS[0].x/pic_scale - 1,CBCS[0].y/pic_scale-1,RGB(0,255,0));

		if(fps != NULL)
		{
			fprintf(fps,"x%lfy%lf\n",CBCS[0].x/pic_scale,CBCS[0].y/pic_scale);
		}
		
		for(int i = 1; i < CBCNum ; i ++)
		{
			if(fps != NULL)
			{
				fprintf(fps,"x%lfy%lf\n",CBCS[i].x/pic_scale,CBCS[i].y/pic_scale);
			}
		
			
			SetPixel(shdc,CBCS[i].x/pic_scale,CBCS[i].y/pic_scale,RGB(255,0,0));
			SetPixel(shdc,CBCS[i].x/pic_scale + 1,CBCS[i].y/pic_scale,RGB(255,0,0));
			SetPixel(shdc,CBCS[i].x/pic_scale - 1,CBCS[i].y/pic_scale,RGB(255,0,0));
			
			SetPixel(shdc,CBCS[i].x/pic_scale,CBCS[i].y/pic_scale+1,RGB(255,0,0));
			SetPixel(shdc,CBCS[i].x/pic_scale + 1,CBCS[i].y/pic_scale+1,RGB(255,0,0));
			SetPixel(shdc,CBCS[i].x/pic_scale - 1,CBCS[i].y/pic_scale+1,RGB(255,0,0));
			
			SetPixel(shdc,CBCS[i].x/pic_scale,CBCS[i].y/pic_scale-1,RGB(255,0,0));
			SetPixel(shdc,CBCS[i].x/pic_scale + 1,CBCS[i].y/pic_scale-1,RGB(255,0,0));
			SetPixel(shdc,CBCS[i].x/pic_scale - 1,CBCS[i].y/pic_scale-1,RGB(255,0,0));
		}
	}

	if(fps != NULL)
	{
		fclose(fps);
	}


	CCamCaliSys::GetExtrinsicPara(path.GetBuffer(path.GetLength()+1),
		&ip,&dp,
		&rm,&mv,
		m_num_in_raw,
		m_num_in_cln,
		this->m_squre_x,this->m_squre_y);


	RoterMatrix rmtmp;
	MoveVecter  mvtmp;

	rmtmp.r11 = rm.r11;
	rmtmp.r12 = rm.r21;
	rmtmp.r13 = rm.r31;

	
	rmtmp.r21 = rm.r12;
	rmtmp.r22 = rm.r22;
	rmtmp.r23 = rm.r32;


	
	rmtmp.r31 = rm.r13;
	rmtmp.r32 = rm.r23;
	rmtmp.r33 = rm.r33;


	mvtmp.tx = -mv.tx * rmtmp.r11 -mv.ty * rmtmp.r12 -mv.tz * rmtmp.r13;
	mvtmp.ty = -mv.tx * rmtmp.r21 -mv.ty * rmtmp.r22 -mv.tz * rmtmp.r23;
	mvtmp.tz = -mv.tx * rmtmp.r31 -mv.ty * rmtmp.r32 -mv.tz * rmtmp.r33; 


	CString sss;
	char c1,c2;
	c1 = 0x0d;
	c2 = 0x0a;
	m_ts.Empty();
	CString ttt;
	ttt.Format("外参数:%c%c",c1,c2);
	m_ts += ttt;
	sss.Format("%.12f	%.12f	%.12f	|	%.12f%c%c",rm.r11,rm.r12,rm.r13,mv.tx,c1,c2);
	m_ts += sss;
	sss.Format("%.12f	%.12f	%.12f	|	%.12f%c%c",rm.r21,rm.r22,rm.r23,mv.ty,c1,c2);
	m_ts += sss;
	sss.Format("%.12f	%.12f	%.12f	|	%.12f%c%c",rm.r31,rm.r32,rm.r33,mv.tz,c1,c2);
	m_ts += sss;

	
	ttt.Format("外参数逆变换:%c%c",c1,c2);
	m_ts += ttt;
	sss.Format("%.12f	%.12f	%.12f	|	%.12f%c%c",rmtmp.r11,rmtmp.r12,rmtmp.r13,mvtmp.tx,c1,c2);
	m_ts += sss;
	sss.Format("%.12f	%.12f	%.12f	|	%.12f%c%c",rmtmp.r21,rmtmp.r22,rmtmp.r23,mvtmp.ty,c1,c2);
	m_ts += sss;
	sss.Format("%.12f	%.12f	%.12f	|	%.12f%c%c",rmtmp.r31,rmtmp.r32,rmtmp.r33,mvtmp.tz,c1,c2);
	m_ts += sss;

	UpdateData(false);

}

void CCalibrationToolDlg::OnCaliExParai() 
{
	// TODO: Add your control notification handler code here
		for(int i = 0; i < CSDCSIZE; i ++)
	{
		CBCS[i].x = 0;
		CBCS[i].y = 0;
	}
	if(this->m_e1c.IsWindowEnabled() != 0)
	{
		AfxMessageBox("请设定标盘规格");
		return;
	}
	if(camera.m_bConnected == false)
	{
//		AfxMessageBox("请先启动摄像头");
//		return ;
	}

	CString path;
	CString linePath;
	CPIC_RGB rgbpic;
	
//	IplImage *pFrame = camera.QueryFrame();	
//	rgbpic.init(pFrame->width,pFrame->height);
//	rgbpic.SetBitBYTE(pFrame->width,pFrame->height,pFrame->imageDataOrigin);

//	this->OnShowPic(&rgbpic,shdc);
	path.Format("D:\\CoderLife\\Image-Experiment\\img_exp4\\tmp.bmp"); 
//	bool tag = CCameraDS::SaveBmpFile(path.GetBuffer(path.GetLength()+1),&rgbpic);
//	if(tag == false)
	{
//		AfxMessageBox("保存用于识别的图片失败!");
//		return ;
	}
	
	if(CCamCaliSys::FindCross(path.GetBuffer(path.GetLength()+1),m_num_in_raw,m_num_in_cln,CBCS,CBCNum)==false)
	{
		AfxMessageBox("图片的角点不能被完全识别,请调整图片或检查角点设置!");
		return ;
	}
	
	if(hdib != 0)
	{
		SetPixel(shdc,CBCS[0].x/pic_scale,CBCS[0].y/pic_scale,RGB(0,255,0));
		SetPixel(shdc,CBCS[0].x/pic_scale + 1,CBCS[0].y/pic_scale,RGB(0,255,0));
		SetPixel(shdc,CBCS[0].x/pic_scale - 1,CBCS[0].y/pic_scale,RGB(0,255,0));
		
		SetPixel(shdc,CBCS[0].x/pic_scale,CBCS[0].y/pic_scale+1,RGB(0,255,0));
		SetPixel(shdc,CBCS[0].x/pic_scale + 1,CBCS[0].y/pic_scale+1,RGB(0,255,0));
		SetPixel(shdc,CBCS[0].x/pic_scale - 1,CBCS[0].y/pic_scale+1,RGB(0,255,0));
		
		SetPixel(shdc,CBCS[0].x/pic_scale,CBCS[0].y/pic_scale-1,RGB(0,255,0));
		SetPixel(shdc,CBCS[0].x/pic_scale + 1,CBCS[0].y/pic_scale-1,RGB(0,255,0));
		SetPixel(shdc,CBCS[0].x/pic_scale - 1,CBCS[0].y/pic_scale-1,RGB(0,255,0));
		
		for(int i = 1; i < CBCNum ; i ++)
		{
			SetPixel(shdc,CBCS[i].x/pic_scale,CBCS[i].y/pic_scale,RGB(255,0,0));
			SetPixel(shdc,CBCS[i].x/pic_scale + 1,CBCS[i].y/pic_scale,RGB(255,0,0));
			SetPixel(shdc,CBCS[i].x/pic_scale - 1,CBCS[i].y/pic_scale,RGB(255,0,0));
			
			SetPixel(shdc,CBCS[i].x/pic_scale,CBCS[i].y/pic_scale+1,RGB(255,0,0));
			SetPixel(shdc,CBCS[i].x/pic_scale + 1,CBCS[i].y/pic_scale+1,RGB(255,0,0));
			SetPixel(shdc,CBCS[i].x/pic_scale - 1,CBCS[i].y/pic_scale+1,RGB(255,0,0));
			
			SetPixel(shdc,CBCS[i].x/pic_scale,CBCS[i].y/pic_scale-1,RGB(255,0,0));
			SetPixel(shdc,CBCS[i].x/pic_scale + 1,CBCS[i].y/pic_scale-1,RGB(255,0,0));
			SetPixel(shdc,CBCS[i].x/pic_scale - 1,CBCS[i].y/pic_scale-1,RGB(255,0,0));
		}
	}



	CCamCaliSys::GetExtrinsicParai(path.GetBuffer(path.GetLength()+1),
		&ip,&dp,
		&rm,&mv,
		m_num_in_raw,
		m_num_in_cln,
		this->m_squre_x,this->m_squre_y);


	RoterMatrix rmtmp;
	MoveVecter  mvtmp;

	rmtmp.r11 = rm.r11;
	rmtmp.r12 = rm.r21;
	rmtmp.r13 = rm.r31;

	
	rmtmp.r21 = rm.r12;
	rmtmp.r22 = rm.r22;
	rmtmp.r23 = rm.r32;


	
	rmtmp.r31 = rm.r13;
	rmtmp.r32 = rm.r23;
	rmtmp.r33 = rm.r33;


	mvtmp.tx = -mv.tx * rmtmp.r11 -mv.ty * rmtmp.r12 -mv.tz * rmtmp.r13;
	mvtmp.ty = -mv.tx * rmtmp.r21 -mv.ty * rmtmp.r22 -mv.tz * rmtmp.r23;
	mvtmp.tz = -mv.tx * rmtmp.r31 -mv.ty * rmtmp.r32 -mv.tz * rmtmp.r33; 


	CString sss;
	char c1,c2;
	c1 = 0x0d;
	c2 = 0x0a;
	m_ts.Empty();
	CString ttt;
	ttt.Format("外参数:%c%c",c1,c2);
	m_ts += ttt;
	sss.Format("%.12f	%.12f	%.12f	|	%.12f%c%c",rm.r11,rm.r12,rm.r13,mv.tx,c1,c2);
	m_ts += sss;
	sss.Format("%.12f	%.12f	%.12f	|	%.12f%c%c",rm.r21,rm.r22,rm.r23,mv.ty,c1,c2);
	m_ts += sss;
	sss.Format("%.12f	%.12f	%.12f	|	%.12f%c%c",rm.r31,rm.r32,rm.r33,mv.tz,c1,c2);
	m_ts += sss;

	
	ttt.Format("外参数逆变换:%c%c",c1,c2);
	m_ts += ttt;
	sss.Format("%.12f	%.12f	%.12f	|	%.12f%c%c",rmtmp.r11,rmtmp.r12,rmtmp.r13,mvtmp.tx,c1,c2);
	m_ts += sss;
	sss.Format("%.12f	%.12f	%.12f	|	%.12f%c%c",rmtmp.r21,rmtmp.r22,rmtmp.r23,mvtmp.ty,c1,c2);
	m_ts += sss;
	sss.Format("%.12f	%.12f	%.12f	|	%.12f%c%c",rmtmp.r31,rmtmp.r32,rmtmp.r33,mvtmp.tz,c1,c2);
	m_ts += sss;

	UpdateData(false);
}

void CCalibrationToolDlg::OnSaveExPara() 
{
	// TODO: Add your control notification handler code here
	CFileDialog fd(false,NULL,NULL,0,".ex|*.ex");
	if(fd.DoModal()==IDOK)
	{
		CString path = fd.GetPathName();
		if(path.Find(".ex") == -1 || path.Find(".EX") == -1)
		{
			path += ".ex";
		}
		CGeometry::SaveExtrinsicPara(path.GetBuffer(path.GetLength()+1),&rm,&mv);

		CString aaa = "保存至:";
		aaa += path;
		m_ts = aaa;
		UpdateData(false);
	}
}

void CCalibrationToolDlg::SaveLine(CString path, CString line)
{
	FILE * fp;
	fp = fopen(path.GetBuffer(path.GetLength()+1),"a");
	if(fp==NULL)
	{
		fp = fopen(path.GetBuffer(path.GetLength()+1),"w");
		if(fp== NULL)
		{
			return ;
		}
	}

	line += "\n";
	fwrite(line.GetBuffer(line.GetLength()+1),line.GetLength(),1,fp);
	fclose(fp);
}

void CCalibrationToolDlg::OnButton17() 
{
	// TODO: Add your control notification handler code here
	CFileDialog fd(true,NULL,NULL,0,".ex|*.ex");
	if(fd.DoModal()==IDOK)
	{
		CString path = fd.GetPathName();
		CGeometry::LoadExtrinsicPara(path.GetBuffer(path.GetLength()+1),&rm,&mv);

		CString aaa = "导入:";
		aaa += path;
		m_ts = aaa;
		UpdateData(false);
	}
}

void CCalibrationToolDlg::OnButton18() 
{
	// TODO: Add your control notification handler code here


    if(0)
    {
		
		
		CString path = "D:\\CoderLife\\Image-Experiment\\img_exp4\\u.bmp";
		if(CCamCaliSys::FindCross(path.GetBuffer(path.GetLength()+1),m_num_in_raw,m_num_in_cln,CBCS,CBCNum)==false)
		{
			AfxMessageBox("图片的角点不能被完全识别,请调整图片或检查角点设置!");
			return ;
		}
		else
		{
			AfxMessageBox("图片的角点识别OK!");
			return ;
		}
		return;
    }
    ////////////////
	m_ts.Empty();
	CString tmp;
	char c1,c2;
	c1 = 0x0d;
	c2 = 0x0a;
	
	FILE * fps = fopen("D:\\CoderLife\\Image-Experiment\\img_exp4\\expoint.txt","w");
	
	
	for(int i = 0; i < this->m_num_in_raw * this->m_num_in_cln ; i ++)
	{
		_3D_Line line;
		_3D_Point retp;
		_2D_Point _2dp;
		_2dp.x = CBCS[i].x;
		_2dp.y = CBCS[i].y;
		
		CGeometry::GetLine(&rm,&mv,&ip,&dp,&_2dp,&line);
		CGeometry::SolveLinePoint_Z0(&line,&retp);
		
		
		if(fps != NULL)
		{
			fprintf(fps,"x%lfy%lf\n",_2dp.x,_2dp.y);
		}
        if(i == this->m_num_in_raw * this->m_num_in_cln - 1)
        {
			
            tmp.Format("[%f,%f]-> [%f,%f,%f][%f,%f,%f]%c%c",_2dp.x,_2dp.y,line.p1.x,
                line.p1.y,
                line.p1.z,
                line.p2.x,
                line.p2.y,
                line.p2.z,c1,c2);
			m_ts += tmp;
        }
		tmp.Format("[%f,%f]-> [%f,%f]%c%c",_2dp.x,_2dp.y,retp.x,retp.y,c1,c2);
		m_ts += tmp;
	}
	if(fps != NULL)
	{
		fclose(fps);
	}
	UpdateData(false);
}

void CCalibrationToolDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == 1)
	{
		
		if(camera.m_bConnected == true && showtag)
		{
			OnGetPic();
		}
	}
	CDialog::OnTimer(nIDEvent);
}

void CCalibrationToolDlg::OnButton1() 
{
	// TODO: Add your control notification handler code here
	if(showtag==false)
	{showtag = true;
	}else
	{
		showtag = false;
	}
}

void CCalibrationToolDlg::OnButton3() 
{
	// TODO: Add your control notification handler code here
	if(ppnum == 0)
	{
		return;
	}

	this->OnShowPic(&t2_rgbpic,shdc);
	
	if(hdib != 0)
	{
		SetPixel(shdc,CBCS2[m_show_p_serial].x/pic_scale,CBCS2[m_show_p_serial].y/pic_scale,RGB(255,0,0));
		SetPixel(shdc,CBCS2[m_show_p_serial].x/pic_scale + 1,CBCS2[m_show_p_serial].y/pic_scale,RGB(255,0,0));
		SetPixel(shdc,CBCS2[m_show_p_serial].x/pic_scale - 1,CBCS2[m_show_p_serial].y/pic_scale,RGB(255,0,0));
		
		SetPixel(shdc,CBCS2[m_show_p_serial].x/pic_scale,CBCS2[m_show_p_serial].y/pic_scale+1,RGB(255,0,0));
		SetPixel(shdc,CBCS2[m_show_p_serial].x/pic_scale + 1,CBCS2[m_show_p_serial].y/pic_scale+1,RGB(255,0,0));
		SetPixel(shdc,CBCS2[m_show_p_serial].x/pic_scale - 1,CBCS2[m_show_p_serial].y/pic_scale+1,RGB(255,0,0));
		
		SetPixel(shdc,CBCS2[m_show_p_serial].x/pic_scale,CBCS2[m_show_p_serial].y/pic_scale-1,RGB(255,0,0));
		SetPixel(shdc,CBCS2[m_show_p_serial].x/pic_scale + 1,CBCS2[m_show_p_serial].y/pic_scale-1,RGB(255,0,0));
		SetPixel(shdc,CBCS2[m_show_p_serial].x/pic_scale - 1,CBCS2[m_show_p_serial].y/pic_scale-1,RGB(255,0,0));
		
	
		
		
	
	}
}

void CCalibrationToolDlg::OnButton5() 
{
	// TODO: Add your control notification handler code here
	
	if(ppnum == 0)
	{
		return;
	}
	UpdateData(true);
	m_show_p_serial --;
	if( m_show_p_serial < 0)
	{
		m_show_p_serial = 0;
	}
	UpdateData(false);
	OnButton3();
	OnButton7();
}

void CCalibrationToolDlg::OnButton6() 
{
	// TODO: Add your control notification handler code here
	if(ppnum == 0)
	{
		return;
	}
	UpdateData(true);
	m_show_p_serial ++;
	if( m_show_p_serial >= ppnum-1)
	{
		m_show_p_serial = ppnum-1;
	}
	UpdateData(false);
	OnButton3();
	OnButton7();
}


bool CCalibrationToolDlg::OnGetCord(long pos,long uv[2],double *diff)
{
	UpdateData(true);
	m_show_c.Empty();
	UpdateData(false);
	
	
	long i,j,k;

	if(m_o_serial < 0 || m_x_serial < 0 || m_y_serial < 0 )
	{
		return false;
	}
	if(m_o_serial >=  ppnum|| m_x_serial  >=  ppnum|| m_y_serial >=  ppnum)
	{
		return false;
	}
	if(m_x_psp <= 0 || m_y_psp <=0 )
	{
		return false;
	}

	for( k = 0; k < CSDCSIZE ; k ++)
	{
		cord_x[k] = -1000;
		cord_y[k] = -1000;
	}

	cord_x[m_o_serial] = 0;
	cord_y[m_o_serial] = 0;
	cord_d[m_o_serial] = 0;
	
	
	
	double po[2];
	po[0] = CBCS2[m_o_serial].x;
	po[1] = CBCS2[m_o_serial].y;
	double px[2];
	px[0] =  CBCS2[m_x_serial].x;
	px[1] =  CBCS2[m_x_serial].y;
	double py[2];
	py[0] =  CBCS2[m_y_serial].x;
	py[1] =  CBCS2[m_y_serial].y;
	
	double pt[2];
	pt[0] =  CBCS2[pos].x;
	pt[1] =  CBCS2[pos].y;
	
	
	double dx[2];
	dx[0] = px[0] - po[0];
	dx[1] = px[1] - po[1];
	
	double dy[2];
	dy[0] = py[0] - po[0];
	dy[1] = py[1] - po[1];
	
	dx[0] /= m_x_psp;
	dx[1] /= m_x_psp;
	dy[0] /= m_y_psp;
	dy[1] /= m_y_psp;



	bool ftag = false;
	while(ftag == false)
	{
		double min_r = 10000000000;
		long min_ok_pos = -1;
		long min_ng_pos = -1;
		for( i= 0 ; i < ppnum ; i ++)
		{
			for( j = 0; j < ppnum ; j++)
			{
				if(i != j)
				{
					if(cord_x[i] != -1000 && cord_x[j] == -1000)
					{
						double dd1 = CBCS2[i].x - CBCS2[j].x;
						double dd2 = CBCS2[i].y - CBCS2[j].y;
						double rr = sqrt(dd1 * dd1 + dd2 * dd2);
						if(rr < min_r)
						{
							min_r = rr;
							min_ok_pos = i;
							min_ng_pos = j;
						}

					}
				}
			}
		}

		if(min_ok_pos != -1)
		{
			double dd1 = CBCS2[min_ng_pos].x - CBCS2[min_ok_pos].x;
			double dd2 = CBCS2[min_ng_pos].y - CBCS2[min_ok_pos].y;

			double minxxx = 1000000000;


			long min_pos_x;
			long min_pos_y;
			
			for(long i = -20; i < 20; i ++)
			{
				for(long j = -20; j < 20; j ++)
				{
					double t[2];
					t[0] =  CBCS2[min_ok_pos].x + i * dx[0] + j * dy[0] - CBCS2[min_ng_pos].x;
					t[1] =  CBCS2[min_ok_pos].y + i * dx[1] + j * dy[1] - CBCS2[min_ng_pos].y;
					double r = t[0] * t[0] + t[1] * t[1];
					if(r < minxxx)
					{
						minxxx = r;
						min_pos_x = i;
						min_pos_y = j;
					}
				}
			}

			cord_x[min_ng_pos] = cord_x[min_ok_pos] + min_pos_x;
			cord_y[min_ng_pos] = cord_y[min_ok_pos] + min_pos_y;
			cord_d[min_ng_pos] = sqrt(minxxx);


		}

		ftag = true;
		for( i= 0 ; i < ppnum ; i ++)
		{
			if(cord_x[i] == -1000)
			{
				ftag = false;
			}
		}
	}

	uv[0] = cord_x[pos];
	uv[1] = cord_y[pos];
	*diff = cord_d[pos];
	return true;
}

void CCalibrationToolDlg::OnButton7() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	m_show_c.Empty();
	UpdateData(false);


	if(m_o_serial < 0 || m_x_serial < 0 || m_y_serial < 0 )
	{
		return ;
	}
	if(m_o_serial >=  ppnum|| m_x_serial  >=  ppnum|| m_y_serial >=  ppnum)
	{
		return ;
	}
	if(m_x_psp <= 0 || m_y_psp <=0 )
	{
		return ;
	}
	


	
	long uv[2];
	double r;
	if(OnGetCord(m_show_p_serial,uv,&r) == false)
	{
		AfxMessageBox("ERROR\n");
		return;
	}

	m_show_c.Format("[%d,%d] %lf",uv[0],uv[1],r);
	UpdateData(false);
}

void CCalibrationToolDlg::OnButton8() 
{
	// TODO: Add your control notification handler code here
	double points[200][4];
	for(long i = 0; i < ppnum ; i ++)
	{
		points[i][0] = CBCS2[i].x;
		points[i][1] = CBCS2[i].y;
		long uv[2];
		double r;
		if(OnGetCord(i,uv,&r) == false)
		{
			AfxMessageBox("ERROR\n");
			return;
		}
		points[i][2] = uv[0]*this->m_squre_x;
		points[i][3] = uv[1]*this->m_squre_y;
	}


	CCamCaliSys::GetExtrinsicPara(points,ppnum,
		&ip,&dp,
		&rm,&mv);
	
	
	RoterMatrix rmtmp;
	MoveVecter  mvtmp;
	
	rmtmp.r11 = rm.r11;
	rmtmp.r12 = rm.r21;
	rmtmp.r13 = rm.r31;
	
	
	rmtmp.r21 = rm.r12;
	rmtmp.r22 = rm.r22;
	rmtmp.r23 = rm.r32;
	
	
	
	rmtmp.r31 = rm.r13;
	rmtmp.r32 = rm.r23;
	rmtmp.r33 = rm.r33;
	
	
	mvtmp.tx = -mv.tx * rmtmp.r11 -mv.ty * rmtmp.r12 -mv.tz * rmtmp.r13;
	mvtmp.ty = -mv.tx * rmtmp.r21 -mv.ty * rmtmp.r22 -mv.tz * rmtmp.r23;
	mvtmp.tz = -mv.tx * rmtmp.r31 -mv.ty * rmtmp.r32 -mv.tz * rmtmp.r33; 
	
	
	CString sss;
	char c1,c2;
	c1 = 0x0d;
	c2 = 0x0a;
	m_ts.Empty();
	CString ttt;
	ttt.Format("外参数:%c%c",c1,c2);
	m_ts += ttt;
	sss.Format("%.12f	%.12f	%.12f	|	%.12f%c%c",rm.r11,rm.r12,rm.r13,mv.tx,c1,c2);
	m_ts += sss;
	sss.Format("%.12f	%.12f	%.12f	|	%.12f%c%c",rm.r21,rm.r22,rm.r23,mv.ty,c1,c2);
	m_ts += sss;
	sss.Format("%.12f	%.12f	%.12f	|	%.12f%c%c",rm.r31,rm.r32,rm.r33,mv.tz,c1,c2);
	m_ts += sss;
	
	
	ttt.Format("外参数逆变换:%c%c",c1,c2);
	m_ts += ttt;
	sss.Format("%.12f	%.12f	%.12f	|	%.12f%c%c",rmtmp.r11,rmtmp.r12,rmtmp.r13,mvtmp.tx,c1,c2);
	m_ts += sss;
	sss.Format("%.12f	%.12f	%.12f	|	%.12f%c%c",rmtmp.r21,rmtmp.r22,rmtmp.r23,mvtmp.ty,c1,c2);
	m_ts += sss;
	sss.Format("%.12f	%.12f	%.12f	|	%.12f%c%c",rmtmp.r31,rmtmp.r32,rmtmp.r33,mvtmp.tz,c1,c2);
	m_ts += sss;
	
	UpdateData(false);

//	OnButton9();



}

void CCalibrationToolDlg::OnButton9() 
{
	// TODO: Add your control notification handler code here

	UpdateData(true);
	m_ts.Empty();
	CString tmp;
	char c1,c2;
	c1 = 0x0d;
	c2 = 0x0a;
	
	for(int i = 0; i < ppnum ; i ++)
	{
		_3D_Line line;
		_3D_Point retp;
		_2D_Point _2dp;
		_2dp.x = CBCS2[i].x;
		_2dp.y = CBCS2[i].y;
		
		CGeometry::GetLine(&rm,&mv,&ip,&dp,&_2dp,&line);
		CGeometry::SolveLinePoint_Z0(&line,&retp);
		tmp.Format("[%f,%f]-> [%f,%f]%c%c",_2dp.x,_2dp.y,retp.x,retp.y,c1,c2);
		m_ts += tmp;
	}
	
	UpdateData(false);
}

void CCalibrationToolDlg::OnStaticS() 
{
	// TODO: Add your control notification handler code here
	
}

void CCalibrationToolDlg::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CString s;
	if(point.x > 11 && point.x < 660 && point.y > 11 && point.y < 490 )
	{
		double vx = point.x - 11;
		double vy = point.y - 11;

		vx *= pic_scale;
		vy *= pic_scale;

		long i;
		double min_r = 10000000000;
		long min_ok_pos = -1;
		for( i= 0 ; i < ppnum ; i ++)
		{
		
			double dd1 = CBCS2[i].x - vx;
			double dd2 = CBCS2[i].y - vy;
			double rr = sqrt(dd1 * dd1 + dd2 * dd2);
			if(rr < min_r)
			{
				min_r = rr;
				min_ok_pos = i;
			}
		}

		if(min_ok_pos >= 0)
		{
			UpdateData(true);
			m_show_p_serial = min_ok_pos;
			if( m_show_p_serial >= ppnum-1)
			{
				m_show_p_serial = ppnum-1;
			}
			UpdateData(false);
			OnButton3();
			OnButton7();
			switch (serialv)
			{
			case 0:
				OnButton10();
				break;
			case 1:
				OnButton11();
				break;
			case 2:
				OnButton12();
				break;
			}
			serialv ++;

			
		}

		

	}
	
	CDialog::OnLButtonDblClk(nFlags, point);
}

void CCalibrationToolDlg::OnButton10() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	m_o_serial = m_show_p_serial;
	UpdateData(false);
}

void CCalibrationToolDlg::OnButton11() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	m_x_serial = m_show_p_serial;
	UpdateData(false);
	
}

void CCalibrationToolDlg::OnButton12() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	m_y_serial = m_show_p_serial;
	UpdateData(false);
	
}

void CCalibrationToolDlg::OnButton2() 
{
	// TODO: Add your control notification handler code here
	camera.SetFlip();
}

void CCalibrationToolDlg::OnButton13() 
{
	// TODO: Add your control notification handler code here
	camera.SetUnFlip();
}

void CCalibrationToolDlg::OnButton14() 
{
	// TODO: Add your control notification handler code here
		for(int i = 0; i < CSDCSIZE; i ++)
	{
		CBCS[i].x = 0;
		CBCS[i].y = 0;
	}
	if(this->m_e1c.IsWindowEnabled() != 0)
	{
		AfxMessageBox("请设定标盘规格");
		return;
	}
	if(camera.m_bConnected == false)
	{
//		AfxMessageBox("请先启动摄像头");
//		return ;
	}

	CString path;
	CString linePath;
	CPIC_RGB rgbpic;
	
//	IplImage *pFrame = camera.QueryFrame();	
//	rgbpic.init(pFrame->width,pFrame->height);
//	rgbpic.SetBitBYTE(pFrame->width,pFrame->height,pFrame->imageDataOrigin);

//	this->OnShowPic(&rgbpic,shdc);
	

	long se = 0;
	bool run_tag = true;
	while(run_tag == true)
	{
		path.Format("D:\\CoderLife\\Image-Experiment\\img_exp4\\%d.bmp",se);

		if(CCamCaliSys::FindCross(path.GetBuffer(path.GetLength()+1),m_num_in_raw,m_num_in_cln,CBCS,CBCNum)==false)
		{
			AfxMessageBox("图片的角点不能被完全识别,请调整图片或检查角点设置!");
			return ;
		}

		CString s1;
		s1.Format("C:\\PROJ\\PLATFORM_CCD_BASE\\DATA\\%d.odata",se);

	FILE * fps = fopen(s1.GetBufferSetLength(s1.GetLength()+1),"w");
	


		if(fps != NULL)
		{
			
		
		for(int i = 0; i < m_num_in_raw*m_num_in_cln ; i ++)
		{
			if(fps != NULL)
			{
				fprintf(fps,"x%lfy%lf\n",CBCS[i].x/pic_scale,CBCS[i].y/pic_scale);
			}
		
	
		}


	
		fclose(fps);
	}


	CCamCaliSys::GetExtrinsicPara(path.GetBuffer(path.GetLength()+1),
		&ip,&dp,
		&rm,&mv,
		m_num_in_raw,
		m_num_in_cln,
		this->m_squre_x,this->m_squre_y);


	RoterMatrix rmtmp;
	MoveVecter  mvtmp;

	rmtmp.r11 = rm.r11;
	rmtmp.r12 = rm.r21;
	rmtmp.r13 = rm.r31;

	
	rmtmp.r21 = rm.r12;
	rmtmp.r22 = rm.r22;
	rmtmp.r23 = rm.r32;


	
	rmtmp.r31 = rm.r13;
	rmtmp.r32 = rm.r23;
	rmtmp.r33 = rm.r33;


	mvtmp.tx = -mv.tx * rmtmp.r11 -mv.ty * rmtmp.r12 -mv.tz * rmtmp.r13;
	mvtmp.ty = -mv.tx * rmtmp.r21 -mv.ty * rmtmp.r22 -mv.tz * rmtmp.r23;
	mvtmp.tz = -mv.tx * rmtmp.r31 -mv.ty * rmtmp.r32 -mv.tz * rmtmp.r33; 


	CString sss;
	char c1,c2;
	c1 = 0x0d;
	c2 = 0x0a;
	m_ts.Empty();
	CString ttt;
	ttt.Format("外参数:%c%c",c1,c2);
	m_ts += ttt;
	sss.Format("%.12f	%.12f	%.12f	|	%.12f%c%c",rm.r11,rm.r12,rm.r13,mv.tx,c1,c2);
	m_ts += sss;
	sss.Format("%.12f	%.12f	%.12f	|	%.12f%c%c",rm.r21,rm.r22,rm.r23,mv.ty,c1,c2);
	m_ts += sss;
	sss.Format("%.12f	%.12f	%.12f	|	%.12f%c%c",rm.r31,rm.r32,rm.r33,mv.tz,c1,c2);
	m_ts += sss;

	
	ttt.Format("外参数逆变换:%c%c",c1,c2);
	m_ts += ttt;
	sss.Format("%.12f	%.12f	%.12f	|	%.12f%c%c",rmtmp.r11,rmtmp.r12,rmtmp.r13,mvtmp.tx,c1,c2);
	m_ts += sss;
	sss.Format("%.12f	%.12f	%.12f	|	%.12f%c%c",rmtmp.r21,rmtmp.r22,rmtmp.r23,mvtmp.ty,c1,c2);
	m_ts += sss;
	sss.Format("%.12f	%.12f	%.12f	|	%.12f%c%c",rmtmp.r31,rmtmp.r32,rmtmp.r33,mvtmp.tz,c1,c2);
	m_ts += sss;

	UpdateData(false);


		CString s2;
		s2.Format("C:\\PROJ\\PLATFORM_CCD_BASE\\DATA\\%d.ex",se);



	CGeometry::SaveExtrinsicPara(s2.GetBuffer(s2.GetLength()+1),&rm,&mv);

	CString aaa = "保存至:";
		aaa += s2;
		m_ts = aaa;
//		AfxMessageBox(aaa);
		se ++;


	

	}
	

	
	
}

void CCalibrationToolDlg::OnButton15() 
{
	// TODO: Add your control notification handler code here
	m_l_r_o = -1;
}

void CCalibrationToolDlg::OnButton16() 
{
	// TODO: Add your control notification handler code here
	m_l_r_o =  1;
}

void CCalibrationToolDlg::OnButton19() 
{
	// TODO: Add your control notification handler code here
	m_l_r_o = 0;
}

void CCalibrationToolDlg::OnButton20() 
{
	// TODO: Add your control notification handler code here
		for(int i = 0; i < CSDCSIZE; i ++)
	{
		CBCS[i].x = 0;
		CBCS[i].y = 0;
	}
	if(this->m_e1c.IsWindowEnabled() != 0)
	{
		AfxMessageBox("请设定标盘规格");
		return;
	}
	if(camera.m_bConnected == false)
	{
		AfxMessageBox("请先启动摄像头");
		return ;
	}
	if( m_num_in_cln *m_num_in_raw  <= 7)
	{
		AfxMessageBox("请设定标盘规格至少8个内点");
		return;
	}

	CString path;
	CString linePath;
	CPIC_RGB rgbpic;

	if(m_l_r_o == 0)
	{
		IplImage *pFrame = camera.QueryFrame();	
		pic_scale = pFrame->width / 640;
		if(pic_scale * pFrame->width < 640)
		{
			pic_scale += 1.0;
		}


		rgbpic.init(pFrame->width,pFrame->height);
		rgbpic.SetBitBYTE(pFrame->width,pFrame->height,pFrame->imageDataOrigin);

		this->OnShowPic(&rgbpic,shdc);
		path.Format("D:\\CoderLife\\Image-Experiment\\img_exp4\\tmp.bmp"); 
		bool tag = jxrSaveBmpFile(path.GetBuffer(path.GetLength()+1),&rgbpic);
		if(tag == false)
		{
		
            printf("[saveYuvFile] open fail errno = %d, reason = %s", errno, strerror(errno));

			AfxMessageBox("7.保存用于识别的图片失败!");
			return ;
		}
	}


	if(m_l_r_o == -1)
	{
		IplImage *pFrame = camera.QueryFrame();	
		pic_scale = pFrame->width/2 / 640;
		if(pic_scale * pFrame->width/2 < 640)
		{
			pic_scale += 1.0;
		}

		
		CPIC_RGB tmp;
		
		tmp.init(pFrame->width,pFrame->height);
		tmp.SetBitBYTE(pFrame->width,pFrame->height,pFrame->imageDataOrigin);

		rgbpic.init(pFrame->width/2,pFrame->height);
		tmp.OnCpyL(&rgbpic);

		this->OnShowPic(&rgbpic,shdc);
		path.Format("D:\\CoderLife\\Image-Experiment\\img_exp4\\tmp.bmp"); 
		bool tag = jxrSaveBmpFile(path.GetBuffer(path.GetLength()+1),&rgbpic);
		if(tag == false)
		{
			AfxMessageBox("8.保存用于识别的图片失败!");
			return ;
		}
	}

	if(m_l_r_o ==  1)
	{
		IplImage *pFrame = camera.QueryFrame();	
		pic_scale = pFrame->width/2 / 640;
		if(pic_scale * pFrame->width/2 < 640)
		{
			pic_scale += 1.0;
		}

		
		CPIC_RGB tmp;
		
		tmp.init(pFrame->width,pFrame->height);
		tmp.SetBitBYTE(pFrame->width,pFrame->height,pFrame->imageDataOrigin);

		rgbpic.init(pFrame->width/2,pFrame->height);
		tmp.OnCpyR(&rgbpic);

		this->OnShowPic(&rgbpic,shdc);
		path.Format("D:\\CoderLife\\Image-Experiment\\img_exp4\\tmp.bmp"); 
		bool tag = CCameraDS::SaveBmpFile(path.GetBuffer(path.GetLength()+1),&rgbpic);
		if(tag == false)
		{
			AfxMessageBox("9.保存用于识别的图片失败!");
			return ;
		}
	}
}
