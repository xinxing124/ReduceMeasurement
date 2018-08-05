#pragma once


// CTestDlg 对话框

class CTestDlg : public CDialog
{
	DECLARE_DYNAMIC(CTestDlg)

public:
	CTestDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTestDlg();

// 对话框数据
	enum { IDD = IDD_TEST_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnParaChooseClicked(unsigned int nCmd);
	afx_msg void OnCheckChooseClicked(unsigned int nCmd);
	CString ReturnPath(void);
	//afx_msg void OnBnClickedCheck4();
	CString Type;
	CString Pos;
	afx_msg void OnBnClickedRadio1();
};
