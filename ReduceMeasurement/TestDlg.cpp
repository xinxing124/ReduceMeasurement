// TestDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ReduceMeasurement.h"
#include "TestDlg.h"


// CTestDlg 对话框

IMPLEMENT_DYNAMIC(CTestDlg, CDialog)

CTestDlg::CTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestDlg::IDD, pParent)
	, Type("")
	, Pos("")
{

}

CTestDlg::~CTestDlg()
{
}

void CTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTestDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CTestDlg::OnBnClickedOk)
	//ON_BN_CLICKED(IDC_RADIO1, &CTestDlg::OnBnClickedRadio1)
	ON_CONTROL_RANGE(BN_CLICKED,IDC_RADIO1,IDC_RADIO9,OnParaChooseClicked)
	//ON_BN_CLICKED(IDC_CHECK4, &CTestDlg::OnBnClickedCheck4)
	ON_CONTROL_RANGE(BN_CLICKED,IDC_RADIO13,IDC_RADIO14,OnCheckChooseClicked)
	ON_BN_CLICKED(IDC_RADIO1, &CTestDlg::OnBnClickedRadio1)
END_MESSAGE_MAP()


// CTestDlg 消息处理程序

void CTestDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	SYSTEMTIME st;
	CString strDate,strTime;
	GetLocalTime(&st);
	strDate.Format(_T("%02d%02d%02d%02d%02d"),st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond);

	//CString strTemp_01,strTemp_02,strTemp_03,strTemp_04,strTemp_05,strTemp_06;
	//((CComboBox*)GetDlgItem(IDC_COMBO1))->GetWindowTextW(strTemp_01);
	//((CComboBox*)GetDlgItem(IDC_COMBO2))->GetWindowTextW(strTemp_02);
	//((CComboBox*)GetDlgItem(IDC_COMBO3))->GetWindowTextW(strTemp_03);
	//((CComboBox*)GetDlgItem(IDC_COMBO4))->GetWindowTextW(strTemp_04);
	//strTemp_05.Format(_T("%.3f"),theApp.Val);
	theApp.AppFilePath= ReturnPath() +strDate +Type +Pos +_T(".txt");

	OnOK();
}

void CTestDlg::OnParaChooseClicked(unsigned int nCmd)
{
switch(nCmd)
{
case IDC_RADIO1:
	Pos="_0";
	break;
case IDC_RADIO2:
	Pos="_1";
	break;
case IDC_RADIO3:
	Pos="_2";
	break;
case IDC_RADIO4:
	Pos="_3";
	break;
case IDC_RADIO5:
	Pos="_4";
	break;
case IDC_RADIO6:
	Pos="_5";
	break;
case IDC_RADIO7:
	Pos="_6";
	break;
case IDC_RADIO8:
	Pos="_7";
	break;
case IDC_RADIO9:
	Pos="_8";
	break;
default:
break;
}
} 
void CTestDlg::OnCheckChooseClicked(unsigned int nCmd)
{
switch(nCmd)
{
case IDC_RADIO13:
	Type="_0";
	break;
case IDC_RADIO14:
	Type="_1";
	break;
default:
break;
}
} 

CString CTestDlg::ReturnPath(void)
{
	  CString    sPath;   
	  GetModuleFileName(NULL,sPath.GetBufferSetLength(MAX_PATH+1),MAX_PATH);   
	  sPath.ReleaseBuffer();   
	  int    nPos;   
	  nPos=sPath.ReverseFind('\\');   
	  sPath=sPath.Left(nPos+1);   
	  return    sPath;   
}
void CTestDlg::OnBnClickedRadio1()
{
	// TODO: 在此添加控件通知处理程序代码
}
