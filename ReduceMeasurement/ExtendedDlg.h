#pragma once


// ExtendedDlg 对话框

class ExtendedDlg : public CDialog
{
	DECLARE_DYNAMIC(ExtendedDlg)

public:
	ExtendedDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~ExtendedDlg();

// 对话框数据
	enum { IDD = IDD_EXTENDED_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
