// ReduceMeasurement.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#ifdef STANDARDSHELL_UI_MODEL
#include "resource.h"
#endif

// CReduceMeasurementApp:
// 有关此类的实现，请参阅 ReduceMeasurement.cpp
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
// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
};

extern CReduceMeasurementApp theApp;
