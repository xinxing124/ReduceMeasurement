// ReduceMeasurementDlg.h : 头文件
//

#pragma once
#include "ceseries.h"
#include "afxwin.h"
#include "MyButton.h"

// CReduceMeasurementDlg 对话框
class CReduceMeasurementDlg : public CDialog
{
// 构造
public:
	CReduceMeasurementDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_REDUCEMEASUREMENT_DIALOG };


	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
	afx_msg void OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/);
#endif
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButExit();
	afx_msg void OnBnClickedButSave();
	afx_msg void OnPaint();
	int intExitFlag;
	afx_msg void OnStnClickedStcTitle();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnClose();
private:
	//线程函数
    static  DWORD WINAPI ThreadFunc(LPVOID lparam);
	//关闭读线程
	void CloseThread();
private:
	//读线程句柄
	HANDLE m_hThread;
	//读线程ID标识
	DWORD m_dwThreadID;
	//运行停止标识
	DWORD m_dwRunFlag;
public:
	CCESeries *m_pSerial;
	int m_Pulse;
	unsigned long FlagCount;
	int iNUM;
	int RunFlag;
	int ExcFlag;
	int m_DIR;

	double GetValue(void);
	afx_msg void OnStnClickedStcImage();
	// 这个是全局的。放到include的下面就可以了
	CFont cfont;
	CFont ccfont;
	afx_msg void OnBnClickedButTest();
	afx_msg void OnBnClickedButExtended();
	int FileWrite(CString filename, CString content);
	int FileRead(CString filename,CString* content);
	void Split(CString source, CStringArray& dest, CString division);
	CString ReturnPath(void);
	afx_msg void OnDestroy();
	unsigned long m_SendCout;
	double GetMaxValue(CArray <double> *data, int dstart, int dfinish);
	//int GetStandardPos(CArray <double> * data, int * start,int * middle, int * finish);
	int GetStandardPos(CArray <double> * data, int start1,int  finish1, int start2,int  finish2,int *maxpos1,int *maxpos2,double *maxval1,double *maxval2);
	int GetStandardPos(CArray <double> * data, int startcount,int  finishcount, int *start,int  *finish,int *maxpos1,int *maxpos2,double *maxval1,double *maxval2);
	int GetStandardPos(CArray <double> * data, int * start, int * finish);
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedCheck2();
	
	//afx_msg void OnBnClickedButLeft();
	//afx_msg void OnBnClickedButRight();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButOpen();
protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
//public:
	//CMyButton m_DlgButLeft;
};
