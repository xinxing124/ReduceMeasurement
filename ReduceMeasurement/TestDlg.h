#pragma once


// CTestDlg �Ի���

class CTestDlg : public CDialog
{
	DECLARE_DYNAMIC(CTestDlg)

public:
	CTestDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTestDlg();

// �Ի�������
	enum { IDD = IDD_TEST_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
