// CalibrationToolDlg.h : header file
//

#if !defined(AFX_CALIBRATIONTOOLDLG_H__A2D50993_EA7A_46B0_B4B0_172DBD59507A__INCLUDED_)
#define AFX_CALIBRATIONTOOLDLG_H__A2D50993_EA7A_46B0_B4B0_172DBD59507A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CCalibrationToolDlg dialog

#include "../pub/DirectShow/CameraDS.h"
#include "../pub/optlib/CamSys.h"
#include "../pub/optlib/Geometry.h"
#define CSDCSIZE   400

class CCalibrationToolDlg : public CDialog
{
// Construction
public:
	long m_l_r_o;
	long serialv;
	bool OnGetCord(long pos,long uv[2],double *diff);
	CCalibrationToolDlg(CWnd* pParent = NULL);	// standard constructor

	void OnShowPic(CPIC_RGB *  pic,HDC hdc);
	CPIC_RGB showss_tmp;
	HDRAWDIB    hdib;
	HDC shdc;
	CCameraDS camera;
	long ppnum;

	CPIC_RGB t_rgbpic;
	
	CPIC_RGB t2_rgbpic;
	
	int picserial;
	long pic_scale;

	_2D_Point CBCS[CSDCSIZE];
	_2D_Point CBCS2[CSDCSIZE];
	long cord_x[CSDCSIZE];
	long cord_y[CSDCSIZE];
	double cord_d[CSDCSIZE];
	int CBCNum;

	RoterMatrix rm;
	MoveVecter  mv;
	IntrinsicPara ip;
	DistortionPara dp;
	bool showtag;

	void SaveLine(CString path, CString line);

// Dialog Data
	//{{AFX_DATA(CCalibrationToolDlg)
	enum { IDD = IDD_CALIBRATIONTOOL_DIALOG };
	CEdit	m_e4c;
	CEdit	m_e3c;
	CEdit	m_e2c;
	CEdit	m_e1c;
	CStatic	m_show;
	UINT	m_camnum;
	UINT	m_camserial;
	int		m_num_in_raw;
	int		m_num_in_cln;
	double	m_squre_x;
	double	m_squre_y;
	CString	m_ts;
	long	m_show_p_serial;
	long	m_o_serial;
	long	m_x_serial;
	long	m_y_psp;
	long	m_x_psp;
	long	m_y_serial;
	CString	m_show_c;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCalibrationToolDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CCalibrationToolDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnOpenCam();
	afx_msg void OnGetCamNum();
	afx_msg void OnGetPic();
	afx_msg void OnButton4();
	afx_msg void OnResetChessboard();
	afx_msg void OnShowChessboard();
	afx_msg void OnShowCorners();
	afx_msg void OnSaveAsInPic();
	afx_msg void OnDelPic();
	afx_msg void OnCaliInPara();
	afx_msg void OnSaveInpara();
	afx_msg void OnLoadInPara();
	afx_msg void OnCaliExPara();
	afx_msg void OnCaliExParai();
	afx_msg void OnSaveExPara();
	afx_msg void OnButton17();
	afx_msg void OnButton18();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnButton1();
	afx_msg void OnButton3();
	afx_msg void OnButton5();
	afx_msg void OnButton6();
	afx_msg void OnButton7();
	afx_msg void OnButton8();
	afx_msg void OnButton9();
	afx_msg void OnStaticS();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnButton10();
	afx_msg void OnButton11();
	afx_msg void OnButton12();
	afx_msg void OnButton2();
	afx_msg void OnButton13();
	afx_msg void OnButton14();
	afx_msg void OnButton15();
	afx_msg void OnButton16();
	afx_msg void OnButton19();
	afx_msg void OnButton20();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CALIBRATIONTOOLDLG_H__A2D50993_EA7A_46B0_B4B0_172DBD59507A__INCLUDED_)
