// ReduceMeasurement.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#ifdef STANDARDSHELL_UI_MODEL
#include "resource.h"
#endif

// CReduceMeasurementApp:
// �йش����ʵ�֣������ ReduceMeasurement.cpp
//

class CReduceMeasurementApp : public CWinApp
{
public:
	CReduceMeasurementApp();
	CString AppStoragePath;
	CString AppFilePath;
	CString AppFileContent;
	CString AppStandardContent;
	CString AppParameterContent;
	CString AppOtherContent;
	CString AppIdentification;
	CString AppTurnoutNumber;
	CString AppMeasurementType;
	CString AppMeasurementPos;
	CString AppStandardVal;
	CString AppMeasurementVal;
	CString AppDifferenceVal;
	CString AppPositionContent;
	CString AppFigureNumber;
	int AppFirstSectionStart;
	int AppFirstSectionFinish;
	int AppSecondSectionStart;
	int AppSecondSectionFinish;

	int AppFirstSectionPosition;
	int AppSecondSectionPosition;

	int AppMeasurementTypeIndex;
	int AppMeasurementPosIndex;
	double AppZoomX;
	double AppZoomY;
	double AppAngleVal;
	double AppLeft;
	double AppDown;
	double AppTempVal;
	double Val;
	int AppPosStart;
	int AppPosFinish;
	
	int AppComboIndex_01;
	int AppComboIndex_02;
	int AppComboIndex_03;
	int AppComboIndex_04;
// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CReduceMeasurementApp theApp;
