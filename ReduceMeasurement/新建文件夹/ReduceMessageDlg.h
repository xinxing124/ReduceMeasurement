#pragma once

#include"Resource.h"
// CReduceMessageDlg 对话框

class CReduceMessageDlg : public CDialog
{
	DECLARE_DYNAMIC(CReduceMessageDlg)

public:
	CReduceMessageDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CReduceMessageDlg();

// 对话框数据
	enum { IDD = IDD_MESSAGE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
public:
	
	virtual BOOL OnInitDialog();
	int FileWrite(CString filename,CString content);
	int FileRead(CString filename,CString* content);
	void Split(CString source, CStringArray& dest, CString division);
	afx_msg void OnCbnSelchangeCombo2();
	afx_msg void OnCbnDropdownCombo2();
	afx_msg void OnDestroy();
	CFont   m_font;
	bool existfile;
	CString m_Temp;
};
