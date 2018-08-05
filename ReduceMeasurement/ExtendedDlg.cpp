// ExtendedDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ReduceMeasurement.h"
#include "ExtendedDlg.h"


// ExtendedDlg 对话框

IMPLEMENT_DYNAMIC(ExtendedDlg, CDialog)

ExtendedDlg::ExtendedDlg(CWnd* pParent /*=NULL*/)
	: CDialog(ExtendedDlg::IDD, pParent)
{

}

ExtendedDlg::~ExtendedDlg()
{
}

void ExtendedDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(ExtendedDlg, CDialog)
END_MESSAGE_MAP()


// ExtendedDlg 消息处理程序
