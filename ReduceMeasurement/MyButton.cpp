// MyButton.cpp : 实现文件
//

#include "stdafx.h"
#include "MyButton.h"


// CMyButton


IMPLEMENT_DYNAMIC(CMyButton, CButton)

CMyButton::CMyButton()
{

}

CMyButton::~CMyButton()
{
}


BEGIN_MESSAGE_MAP(CMyButton, CButton)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()



// CMyButton 消息处理程序



void CMyButton::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
// TODO: Add your message handler code here and/or call default


	CButton::OnLButtonDown(nFlags, point);
	WPARAM wParam = (((DWORD)WM_LBUTTONDOWN) << 16) + ((DWORD)GetDlgCtrlID());
	::SendMessage(this->GetParent()->GetSafeHwnd(),WM_COMMAND, wParam, 0);
}

void CMyButton::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CButton::OnLButtonUp(nFlags, point);
	WPARAM wParam = (((DWORD)WM_LBUTTONUP) << 16) + ((DWORD)GetDlgCtrlID());
	::SendMessage(this->GetParent()->GetSafeHwnd(),WM_COMMAND, wParam, 0);
}
