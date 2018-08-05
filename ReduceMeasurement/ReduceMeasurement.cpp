// ReduceMeasurement.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "ReduceMeasurement.h"
#include "ReduceMeasurementDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CReduceMeasurementApp

BEGIN_MESSAGE_MAP(CReduceMeasurementApp, CWinApp)
END_MESSAGE_MAP()


// CReduceMeasurementApp 构造
CReduceMeasurementApp::CReduceMeasurementApp()
	: CWinApp()
	, AppStoragePath(_T("\\硬盘\\"))
	, AppFilePath(_T(""))
	, AppFileContent(_T(""))
	, AppStandardContent(_T(""))
	, AppParameterContent(_T(""))
	, AppOtherContent(_T(""))
	, AppIdentification(_T(""))
	, AppTurnoutNumber(_T(""))
	, AppMeasurementType(_T(""))
	, AppMeasurementPos(_T(""))
	, AppStandardVal(_T(""))
	, AppMeasurementVal(_T(""))
	, AppDifferenceVal(_T(""))
	, AppPositionContent(_T(""))
	, AppFigureNumber(_T(""))
	, AppPosStart(0)
	, AppPosFinish(0)
	, AppTempVal(0)
	, AppMeasurementTypeIndex(0)
	, AppMeasurementPosIndex(0)
	, AppFirstSectionStart(0)
	, AppFirstSectionFinish(0)
	, AppSecondSectionStart(0)
	, AppSecondSectionFinish(0)
	, AppFirstSectionPosition(0)
	, AppSecondSectionPosition(0)
	, AppAngleVal(0)
	, AppLeft(0)
	, AppDown(0)
	, AppZoomX(1)
	, AppZoomY(1)
	, AppComboIndex_01(-1)
	, AppComboIndex_02(-1)
	, AppComboIndex_03(-1)
	, AppComboIndex_04(-1)
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CReduceMeasurementApp 对象
CReduceMeasurementApp theApp;

// CReduceMeasurementApp 初始化

BOOL CReduceMeasurementApp::InitInstance()
{

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

	CReduceMeasurementDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 在此放置处理何时用
		//  “确定”来关闭对话框的代码
	}

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}
