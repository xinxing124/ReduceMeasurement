// ReduceMeasurement.cpp : ����Ӧ�ó��������Ϊ��
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


// CReduceMeasurementApp ����
CReduceMeasurementApp::CReduceMeasurementApp()
	: CWinApp()
	, AppStoragePath(_T("\\Ӳ��\\"))
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
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CReduceMeasurementApp ����
CReduceMeasurementApp theApp;

// CReduceMeasurementApp ��ʼ��

BOOL CReduceMeasurementApp::InitInstance()
{

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	CReduceMeasurementDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}
