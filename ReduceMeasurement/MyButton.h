#pragma once


// CMyButton

class CMyButton : public CButton
{
	DECLARE_DYNAMIC(CMyButton)

public:
	CMyButton();
	virtual ~CMyButton();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};

/*
������ʹ���������OnCommand��Ϣ
BOOL CReduceMeasurementDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: �ڴ����ר�ô����/����û���
	if((LOWORD(wParam)) == IDC_BUT_LEFT)
	{
		if((HIWORD(wParam)) == WM_LBUTTONDOWN){
			//AfxMessageBox(_T("OK"));
			API_GPIO_Output(0,1);
			Sleep(1);
			API_GPIO_Output(1,0);
		}
		else if((HIWORD(wParam)) == WM_LBUTTONUP){
			//AfxMessageBox(_T("_OK"));
			API_GPIO_Output(0,0);
			Sleep(1);
			API_GPIO_Output(1,0);
		}
	}
	return CDialog::OnCommand(wParam, lParam);
}
*/