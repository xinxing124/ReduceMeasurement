// ReduceMeasurementDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "HDI_API.h"
#include "ReduceMessageDlg.h"
#include "ReduceMeasurement.h"
#include "ReduceMeasurementDlg.h"
#include "TestDlg.h"
#include<math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CReduceMeasurementDlg 对话框
#define pi 3.1415926535898
//读线程退出事件
HANDLE g_hDetIOEvent;
CArray <double> m_Array;
int m_Start=0;
int m_Finish=0;
double m_TempVal=0.0;
int CW=0;
extern BYTE* pRecvBuf;
extern long pRecvLen;

CDC MemDC; //首先定义一个显示设备对象
CBitmap MemBitmap;//定义一个位图对象
CBitmap *pOldBit;

CReduceMeasurementDlg::CReduceMeasurementDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CReduceMeasurementDlg::IDD, pParent)
	, m_pSerial(NULL)
	, intExitFlag(0)
	, m_Pulse(0)
	,m_dwRunFlag(0)
	, FlagCount(0)
	, iNUM(0)
	, RunFlag(0)
	,ExcFlag(0)
	,m_DIR(0)
	, m_SendCout(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CReduceMeasurementDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_BUT_LEFT, m_DlgButLeft);
}

BEGIN_MESSAGE_MAP(CReduceMeasurementDlg, CDialog)
#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
	ON_WM_SIZE()
#endif
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUT_EXIT, &CReduceMeasurementDlg::OnBnClickedButExit)
	ON_BN_CLICKED(IDC_BUT_SAVE, &CReduceMeasurementDlg::OnBnClickedButSave)
	ON_WM_PAINT()
	ON_STN_CLICKED(IDC_STC_TITLE, &CReduceMeasurementDlg::OnStnClickedStcTitle)
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUT_TEST, &CReduceMeasurementDlg::OnBnClickedButTest)
	ON_BN_CLICKED(IDC_BUT_EXTENDED, &CReduceMeasurementDlg::OnBnClickedButExtended)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_CHECK1, &CReduceMeasurementDlg::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK2, &CReduceMeasurementDlg::OnBnClickedCheck2)
	//ON_BN_CLICKED(IDC_BUT_LEFT, &CReduceMeasurementDlg::OnBnClickedButLeft)
	//ON_BN_CLICKED(IDC_BUT_RIGHT, &CReduceMeasurementDlg::OnBnClickedButRight)
	ON_BN_CLICKED(IDC_BUT_OPEN, &CReduceMeasurementDlg::OnBnClickedButOpen)
END_MESSAGE_MAP()

//关闭读线程
void CReduceMeasurementDlg::CloseThread()
{
	SetEvent(g_hDetIOEvent);
    //等待4秒，如果读线程没有退出，则强制退出
    if (WaitForSingleObject(m_hThread,4000) == WAIT_TIMEOUT)
	{
		TerminateThread(m_hThread,0);
	}
	m_hThread = NULL;
}

// CReduceMeasurementDlg 消息处理程序
/*
DWORD CReduceMeasurementDlg::ThreadFunc(LPVOID lparam)
{
	CReduceMeasurementDlg *hDlg = (CReduceMeasurementDlg*)lparam;
	CCESeries *pSerial=hDlg->m_pSerial;
	BYTE buf[]={0x02,0x43,0xB0,0x01,0x03,0xF2};
	BYTE *revbuf=NULL;
	DWORD revlen=0;
	DWORD errlen=0;
	double data=0.0;
	double temp=0.0;
	DWORD dwRet;
	UINT8 Level;
	CString ss;
	CString str;
	bool DropFlag=false;
	int Num=0;
	hDlg->FlagCount=0;
	while (TRUE)
	{   	
		//如果收到读线程退出信号，则退出线程
		dwRet=WaitForSingleObject(g_hDetIOEvent,50);
		if (dwRet== WAIT_OBJECT_0)//调试模式
		{
			//::AfxMessageBox(_T("1"));
			break;
		}
		if ( dwRet== WAIT_TIMEOUT)//无返回超时
		{
			break;
		}
		//if(hDlg->RunFlag==1)
		//{
		//	Sleep(500);
		//	hDlg->RunFlag=2;
		//}
		if(hDlg->RunFlag==2)
		{
			if(hDlg->ExcFlag==2)
			{
				Sleep(0);
				API_GPIO_Input(1,&Level);//读方向信号
				if(Level==0)
				{
					Sleep(100);
					hDlg->FlagCount=0;
					hDlg->m_SendCout=0;
					API_GPIO_Output(0,1);
					Sleep(1);
					API_GPIO_Output(1,0);
					m_Array.RemoveAll();  //删除所有元素
					hDlg->ExcFlag=1;
					TRACE0( "反向信号……\n"  );
				}
			}
			else if(hDlg->ExcFlag==1)
			{
				Sleep(0);
				API_GPIO_Input(2,&Level);//读停止信号
				if(Level==0)
				{       
					Sleep(100);
					((CButton*)(hDlg->GetDlgItem(IDC_BUT_TEST)))->EnableWindow(true);
					((CButton*)(hDlg->GetDlgItem(IDC_BUT_SAVE)))->EnableWindow(true);
					hDlg->RunFlag=0;
					hDlg->ExcFlag=0;

					API_GPIO_Output(0,0);
					API_GPIO_Output(1,0);
					str.Format(L"采集数量：%d\n",hDlg->FlagCount);
					hDlg->GetDlgItem(IDC_STC_CONTENT)->SetWindowTextW(str);
					TRACE(_T("%s"),str);
					hDlg->Invalidate();
					TRACE0( "停止信号……\n"  );
				}
				else
				{
					Sleep(0);
					API_GPIO_Input(0,&Level);//读采集信号
					if(Level==0 && DropFlag==true)
					{
						DropFlag=false;
						//data=rand()%50;
						hDlg->FlagCount++;
						
						revbuf=pSerial->Command((BYTE*)buf,sizeof(buf),&revlen);
						if(revlen==6)
						{
						if(revbuf[0] == 0x02&&revbuf[1] == 0x06&&revbuf[4] == 0x03)
							data=(revbuf[2]*256+revbuf[3])/100.0;
							if(data>327.67) data=data-655.35;
							if(data==327.67)data=theApp.AppTempVal;
							//if(data==327.67)data=m_TempVal;
							temp=data;
							theApp.AppTempVal=temp;
							//m_TempVal=temp;
							hDlg->m_SendCout++;
						}
						else
						{
							data=temp;
						}
						data=data*(-1.0);
						m_Array.Add(data);
						
					}
					else if(Level==1)
					{
						DropFlag=true;
					}
				}
			}

			//////CW=1;
			//////API_GPIO_Input(2,&Level);//读停止信号
			//////if(Level==1)
			//////{       
			//////	Sleep(100);
			//////	hDlg->RunFlag=0;
			//////	//API_GPIO_Output(2, 0);
			//////	API_GPIO_Output(0,0);
			//////	API_GPIO_Output(1,0);
			//////	CW=0;
			//////	str.Format(L"采集数量：%d\n发送数量：%d",hDlg->FlagCount,Num);
			//////	TRACE(_T("%s"),str);
			//////	hDlg->FlagCount=0;
			//////	hDlg->Invalidate();
			//////	//str="";
			//////	//for(int i=0;i<(int)m_Array.GetSize();i++)
			//////	//{
			//////	//	ss.Format(_T("%d:%f;\n"),i,m_Array.GetAt(i));
			//////	//	TRACE(_T("%s"),ss);
			//////	//	//str.Append(ss);
			//////	//}
			//////	//str1.append (str2);   //将str2连接到str1后
			//////	//TRACE(_T("%s"),str);
			//////	TRACE0( "停止信号……\n"  );

			//////}
			//////API_GPIO_Input(1,&Level);//读方向信号
			//////if(Level==1)
			//////{
			//////	revbuf=pSerial->Command((BYTE*)buf,sizeof(buf),&revlen);
			//////	Sleep(100);
			//////	//CW=CW!=0?0:1;
			//////	CW=1;
			//////	//API_GPIO_Output(1,CW);
			//////	API_GPIO_Output(0,0);
			//////	API_GPIO_Output(1,1);
			//////	hDlg->FlagCount=0;
			//////	m_Array.RemoveAll();  //删除所有元素
			//////	//hDlg->Invalidate();
			//////	hDlg->m_SendCout=0;
			//////	TRACE0( "反向信号……\n"  );
			//////}

			//if(CW==1)
			//{
			//	Sleep(0);
			//	API_GPIO_Input(0,&Level);//读采集信号
			//	if(Level==0 && DropFlag==true)
			//	{
			//		DropFlag=false;
			//		//data=rand()%50;
			//		hDlg->FlagCount++;
			//		
			//		revbuf=pSerial->Command((BYTE*)buf,sizeof(buf),&revlen);
			//		if(revlen==6)
			//		{
			//		if(revbuf[0] == 0x02&&revbuf[1] == 0x06&&revbuf[4] == 0x03)
			//			data=(revbuf[2]*256+revbuf[3])/100.0;
			//			if(data>327.67) data=data-655.35;
			//			if(data==327.67)data=theApp.AppTempVal;
			//			//if(data==327.67)data=m_TempVal;
			//			temp=data;
			//			theApp.AppTempVal=temp;
			//			//m_TempVal=temp;
			//			hDlg->m_SendCout++;
			//		}
			//		else
			//		{
			//			data=temp;
			//		}
			//		data=data*(-1.0);
			//		m_Array.Add(data);
			//		
			//	}
			//	else if(Level==1)
			//	{
			//		DropFlag=true;
			//	}
			//}
			//else
			//{
			//		//TRACE0( "Start Dump of MyClass members:\n"  );
			//		//hDlg->FlagCount++;
			//		Sleep(0);
			//		hDlg->FlagCount++;
			//		
			//		revbuf=pSerial->Command((BYTE*)buf,sizeof(buf),&revlen);
			//		if(revlen==6)
			//		{
			//		if(revbuf[0] == 0x02&&revbuf[1] == 0x06&&revbuf[4] == 0x03)
			//			data=(revbuf[2]*256+revbuf[3])/100.0;
			//			if(data>327.67) data=data-655.35;
			//			//if(data==327.67)data=theApp.AppTempVal;
			//			//if(data==327.67)data=m_TempVal;
			//			temp=data;
			//			//theApp.AppTempVal=temp;
			//			//m_TempVal=temp;
			//			hDlg->m_SendCout++;
			//		}
			//		else
			//		{
			//			data=temp;
			//		}
			//		data=data*(-1.0);
			//		m_Array.Add(data);
			//}
					////data=hDlg->GetValue();
					////if(CW==1)
					////{
					//
					////str.Format(L"采集序号：%d\n",hDlg->FlagCount);
					////TRACE(_T("%s"),str);
					//m_Array.Add(data);
					//hDlg->Invalidate();
					//}
					//double x,y;
					//CListBox *plist=(CListBox*)GetDlgItem(IDC_LIST1);
					//CString str;
					//str.Format(L"%lf",data);
					//plist->AddString((LPCTSTR)str); 
					//CWnd   *pWnd   =   ReduceMeasurementDlg->GetDlgItem(IDC_STC_IMAGE);     

					//pWnd->Invalidate();  
					//pWnd->UpdateWindow();
					//hDlg->Invalidate();
		}
		else
		{
			Sleep(1);
		}
			
		//if(CW==0) Sleep(1);
	}
	return 0;
}
*/

DWORD CReduceMeasurementDlg::ThreadFunc(LPVOID lparam)
{
	CReduceMeasurementDlg *hDlg = (CReduceMeasurementDlg*)lparam;
	CCESeries *pSerial=hDlg->m_pSerial;
	BYTE buf[]={0x02,0x43,0xB0,0x01,0x03,0xF2};
	BYTE *revbuf=NULL;
	DWORD revlen=0;
	DWORD errlen=0;
	double data=0.0;
	double temp=0.0;
	DWORD dwRet;
	UINT8 Level;
	CString ss;
	CString str;
	bool DropFlag=false;
	int Num=0;
	hDlg->FlagCount=0;
	while (TRUE)
	{   	
		//如果收到读线程退出信号，则退出线程
		dwRet=WaitForSingleObject(g_hDetIOEvent,50);
		if (dwRet== WAIT_OBJECT_0)//调试模式
		{
			//::AfxMessageBox(_T("1"));
			break;
		}
		if ( dwRet== WAIT_TIMEOUT)//无返回超时
		{
			break;
		}
		//if(hDlg->RunFlag==1)
		//{
		//	Sleep(500);
		//	hDlg->RunFlag=2;
		//}
		if(hDlg->RunFlag==2)
		{
			//CW=1;
			//API_GPIO_Input(2,&Level);//读停止信号
			//if(Level==1)
			//{       
			//	Sleep(100);
			//	hDlg->RunFlag=0;
			//	//API_GPIO_Output(2, 0);
			//	API_GPIO_Output(0,0);
			//	API_GPIO_Output(1,0);
			//	CW=0;
			//	str.Format(L"采集数量：%d\n发送数量：%d",hDlg->FlagCount,Num);
			//	TRACE(_T("%s"),str);
			//	hDlg->FlagCount=0;
			//	hDlg->Invalidate();
			//	//str="";
			//	//for(int i=0;i<(int)m_Array.GetSize();i++)
			//	//{
			//	//	ss.Format(_T("%d:%f;\n"),i,m_Array.GetAt(i));
			//	//	TRACE(_T("%s"),ss);
			//	//	//str.Append(ss);
			//	//}
			//	//str1.append (str2);   //将str2连接到str1后
			//	//TRACE(_T("%s"),str);
			//	TRACE0( "停止信号……\n"  );

			//}
			//API_GPIO_Input(1,&Level);//读方向信号
			//if(Level==1)
			//{
			//	revbuf=pSerial->Command((BYTE*)buf,sizeof(buf),&revlen);
			//	Sleep(100);
			//	//CW=CW!=0?0:1;
			//	CW=1;
			//	//API_GPIO_Output(1,CW);
			//	API_GPIO_Output(0,0);
			//	API_GPIO_Output(1,1);
			//	hDlg->FlagCount=0;
			//	m_Array.RemoveAll();  //删除所有元素
			//	//hDlg->Invalidate();
			//	hDlg->m_SendCout=0;
			//	TRACE0( "反向信号……\n"  );
			//}
			if(CW==1)
			{
				Sleep(0);
				API_GPIO_Input(0,&Level);//读采集信号
				if(Level==0 && DropFlag==true)
				{
					DropFlag=false;
					//data=rand()%50;
					hDlg->FlagCount++;
					
					revbuf=pSerial->Command((BYTE*)buf,sizeof(buf),&revlen);
					if(revlen==6)
					{
					if(revbuf[0] == 0x02&&revbuf[1] == 0x06&&revbuf[4] == 0x03)
						data=(revbuf[2]*256+revbuf[3])/100.0;
						if(data>327.67) data=data-655.35;
						if(data==327.67)data=theApp.AppTempVal;
						//if(data==327.67)data=m_TempVal;
						temp=data;
						theApp.AppTempVal=temp;
						//m_TempVal=temp;
						hDlg->m_SendCout++;
					}
					else
					{
						data=temp;
					}
					data=data*(-1.0);
					m_Array.Add(data);
					
				}
				else if(Level==1)
				{
					DropFlag=true;
				}
			}
			else
			{
					//TRACE0( "Start Dump of MyClass members:\n"  );
					//hDlg->FlagCount++;
					Sleep(0);
					hDlg->FlagCount++;
					
					revbuf=pSerial->Command((BYTE*)buf,sizeof(buf),&revlen);
					if(revlen==6)
					{
					if(revbuf[0] == 0x02&&revbuf[1] == 0x06&&revbuf[4] == 0x03)
						data=(revbuf[2]*256+revbuf[3])/100.0;
						if(data>327.67) data=data-655.35;
						if(data==327.67)data=theApp.AppTempVal;
						//if(data==327.67)data=m_TempVal;
						temp=data;
						theApp.AppTempVal=temp;
						//m_TempVal=temp;
						hDlg->m_SendCout++;
					}
					else
					{
						data=temp;
					}
					data=data*(-1.0);
					m_Array.Add(data);
			}
					////data=hDlg->GetValue();
					////if(CW==1)
					////{
					//
					////str.Format(L"采集序号：%d\n",hDlg->FlagCount);
					////TRACE(_T("%s"),str);
					//m_Array.Add(data);
					//hDlg->Invalidate();
					//}
					//double x,y;
					//CListBox *plist=(CListBox*)GetDlgItem(IDC_LIST1);
					//CString str;
					//str.Format(L"%lf",data);
					//plist->AddString((LPCTSTR)str); 
					//CWnd   *pWnd   =   ReduceMeasurementDlg->GetDlgItem(IDC_STC_IMAGE);     

					//pWnd->Invalidate();  
					//pWnd->UpdateWindow();
					//hDlg->Invalidate();
		}
		else
		{
			Sleep(1);
		}
			
		//if(CW==0) Sleep(1);
	}
	return 0;
}

int CReduceMeasurementDlg::GetStandardPos(CArray <double> * data, int startcount,int  finishcount, int *start,int  *finish,int *maxpos1,int *maxpos2,double *maxval1,double *maxval2)
{
	int i,pos;
	int start1,finish1;
	int start2,finish2;
	double tval;
	double *dTemp;
	int dlen=(int)data->GetSize();
	dTemp=new double[dlen];

	dTemp[0]=(data->GetAt(0));
	for(i=1;i<dlen;i++)
	{
		//if(data->GetAt(i)==-327.67)
		//	dTemp[i]=dTemp[i-1];
		//else
			dTemp[i]=(data->GetAt(i));
	}

	tval=dTemp[dlen-1];
	for(i=dlen-1;i>1;i--)
	{
		if(fabs(tval-dTemp[i])>0.03)
		{
			pos=i;
			break;
		}
	}
	
	start2=pos-finishcount+1;
	finish2=pos;
	start1=pos-finishcount-startcount;
	finish1=pos-finishcount;

	 *start=start1;
	 *finish=finish2;

	*maxval1=dTemp[start1];
	*maxpos1=start1;
	for(i=start1;i<finish1-1;i++)
	{
		if(*maxval1<dTemp[i])
		{
			*maxval1=dTemp[i];
			*maxpos1=i;
		}
	}
	if(*maxpos1==finish1-2)
	{
		for(i=finish1-2;i>start1;i--)
		{
			pos=finish1-1;
			if(dTemp[i-1]<dTemp[i]&&dTemp[i]>dTemp[i+1])
			{
				pos=i;
				break;
			}
		}
		*maxval1=dTemp[pos];
		*maxpos1=pos;
	}

	*maxval2=dTemp[start2];
	*maxpos2=start2;
	for(i=start2;i<finish2-1;i++)
	{
		if(*maxval2<dTemp[i])
		{
			*maxval2=dTemp[i];
			*maxpos2=i;
		}
	}
	
	delete []dTemp;

	return 0;


	//for(i=2;i<dlen-2;i++)
	//{
	//	dTemp1[i]=(dTemp[i+1]-dTemp[i-1])/2.0;
	//	TRACE(_T("%f "),dTemp1[i]);
	//}

	//TRACE(_T("\n"));
	//for(i=3;i<dlen-3;i++)
	//{
	//	dTemp2[i]=(dTemp1[i+1]-dTemp1[i-1])/2.0;
	//	TRACE(_T("%f "),dTemp2[i]);
	//}
	//
	//for(i=start;i<finish-1;i++)
	//{
	//	if(dTemp2[i]>0.2)	
	//		break;
	//}
	//pos=i;

	//*maxval1=dTemp[start];
	//*maxpos1=start;
	//for(i=start;i<pos;i++)
	//{
	//	if(*maxval1<dTemp[i])
	//	{
	//		*maxval1=dTemp[i];
	//		*maxpos1=i;
	//	}
	//}

	//*maxval2=dTemp[pos];
	//*maxpos2=pos;
	//for(i=pos;i<finish-1;i++)
	//{
	//	if(*maxval2<dTemp[i])
	//	{
	//		*maxval2=dTemp[i];
	//		*maxpos2=i;
	//	}
	//}
	//
	//delete []dTemp;
	//delete []dTemp1;
	//delete []dTemp2;

	//return 0;
}
int CReduceMeasurementDlg::GetStandardPos(CArray <double> * data, int start1,int  finish1, int start2,int  finish2,int *maxpos1,int *maxpos2,double *maxval1,double *maxval2)
{
	int i,pos;
	double *dTemp;
	double *dTemp1,*dTemp2;
	int dlen=(int)data->GetSize();
	dTemp=new double[dlen];
	dTemp1=new double[dlen];
	dTemp2=new double[dlen];

	dTemp[0]=(data->GetAt(0));
	for(i=1;i<dlen-1;i++)
	{
		if(data->GetAt(i)==-327.67)
			dTemp[i]=dTemp[i-1];
		else
			dTemp[i]=(data->GetAt(i));
	}
/*
	for(i=2;i<dlen-2;i++)
	{
		dTemp1[i]=(dTemp[i+1]-dTemp[i-1])/2.0;
		TRACE(_T("%f "),dTemp1[i]);
	}

	TRACE(_T("\n"));
	for(i=3;i<dlen-3;i++)
	{
		dTemp2[i]=(dTemp1[i+1]-dTemp1[i-1])/2.0;
		TRACE(_T("%f "),dTemp2[i]);
	}
	
	for(i=start1;i<finish1-1;i++)
	{
		if(dTemp2[i]>0.2)	
			break;
	}
	pos=i;
	*maxval1=dTemp[pos];
	*maxpos1=pos;

	*maxval1=dTemp[start1];
	*maxpos1=start1;
	for(i=start1;i<finish1;i++)
	{
		if(*maxval1<dTemp[i])
		{
			*maxval1=dTemp[i];
			*maxpos1=i;
		}
	}




	//取得图形起点
	for(i=start1+1;i<finish1-1;i++)
	{
		//if(fabs(dTemp[i-2]-(-327.67))<0.001 &&fabs(dTemp[i-1]-(-327.67))<0.001 &&fabs(dTemp[i]-(-327.67))<0.001&&fabs(dTemp[i+1]-(-327.67))<0.001&&fabs(dTemp[i+2]-(-327.67))<0.001)
		if(dTemp[i-1]<dTemp[i]&&dTemp[i]>dTemp[i+1])
		{
			pos=i;
			break;
		}
	}

	*maxval1=dTemp[pos];
	*maxpos1=pos;
*/

	*maxval1=dTemp[start1];
	*maxpos1=start1;
	for(i=start1;i<finish1-1;i++)
	//for(i=finish1-2;i>start1;i--)
	{
		if(*maxval1<dTemp[i])
		{
			*maxval1=dTemp[i];
			*maxpos1=i;
		}
		if(i==1001){
			pos=i;
		}
	}
	if(*maxpos1==finish1-2)
	{
		//for(i=start1+1;i<finish1-1;i++)
		for(i=finish1-2;i>start1;i--)
		{
			pos=finish1-1;
			if(dTemp[i-1]<dTemp[i]&&dTemp[i]>dTemp[i+1])
			{
				pos=i;
				break;
			}
		}
		*maxval1=dTemp[pos];
		*maxpos1=pos;
	}

	*maxval2=dTemp[start2];
	*maxpos2=start2;
	for(i=start2;i<finish2-1;i++)
	{
		if(*maxval2<dTemp[i])
		{
			*maxval2=dTemp[i];
			*maxpos2=i;
		}
	}
	
	delete []dTemp;
	delete []dTemp1;
	delete []dTemp2;

	return 0;
}

BOOL CReduceMeasurementDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	CWnd*  pWnd=NULL;
	cfont.CreateFont(36,        //这个数字就是字体大小了
		  0,0,0,FW_NORMAL,  
		  FALSE,FALSE,  
		  0,  
		  ANSI_CHARSET,              // nCharSet
		  OUT_DEFAULT_PRECIS,        // nOutPrecision
		  CLIP_DEFAULT_PRECIS,       // nClipPrecision
		  DEFAULT_QUALITY,           // nQuality
		  DEFAULT_PITCH | FF_SWISS, L"Arial");
		pWnd = GetDlgItem(IDC_STC_TITLE);
		pWnd->SetFont(&cfont);

 
	//ccfont.CreateFont(24,        //这个数字就是字体大小了
	//  0,0,0,FW_NORMAL,  
	//  FALSE,FALSE,  
	//  0,  
	//  ANSI_CHARSET,              // nCharSet
	//  OUT_DEFAULT_PRECIS,        // nOutPrecision
	//  CLIP_DEFAULT_PRECIS,       // nClipPrecision
	//  DEFAULT_QUALITY,           // nQuality
	//  DEFAULT_PITCH | FF_SWISS, L"宋体");//Arial
	//pWnd = GetDlgItem(IDC_STC_CONTENT);
	//pWnd->SetFont(&ccfont);

	if(::GetFileAttributes(_T("\\硬盘\\parameter.txt")) != 0xFFFFFFFF)
		::CopyFile(_T("\\硬盘\\parameter.txt"),ReturnPath()  +_T("parameter.txt"),false);
	if(::GetFileAttributes(_T("\\硬盘\\standard.txt")) != 0xFFFFFFFF)
		::CopyFile(_T("\\硬盘\\standard.txt"),ReturnPath()  +_T("standard.txt"),false);
	if(::GetFileAttributes(_T("\\硬盘\\position.txt")) != 0xFFFFFFFF)
			::CopyFile(_T("\\硬盘\\position.txt"),ReturnPath()  +_T("position.txt"),false);
	if(::GetFileAttributes(_T("\\硬盘\\angle.txt")) != 0xFFFFFFFF)
			::CopyFile(_T("\\硬盘\\other.txt"),ReturnPath()  +_T("other.txt"),false);

	FileRead(ReturnPath()  +_T("parameter.txt"),&theApp.AppParameterContent);
	FileRead(ReturnPath()  +_T("standard.txt"),&theApp.AppStandardContent);
	FileRead(ReturnPath()  +_T("position.txt"),&theApp.AppPositionContent);
	FileRead(ReturnPath()  +_T("other.txt"),&theApp.AppOtherContent);

	//打开文件
	double val,vTmp;
	CString content;
	CStringArray dest;
	int Count;
	wchar_t   *stopstring;

	Split(theApp.AppOtherContent,dest,_T(","));
	Count= dest.GetSize();
	theApp.AppLeft =wcstod(dest[0],&stopstring);
	theApp.AppDown =wcstod(dest[1],&stopstring);
	theApp.AppZoomX=wcstod(dest[2],&stopstring);
	theApp.AppZoomY=wcstod(dest[3],&stopstring);
	theApp.AppAngleVal =wcstod(dest[4],&stopstring);
	

	//GetDlgItem(IDC_STC_TITLE)->MoveWindow(7,6,354,40) ; 

	//CString FileFlt = _T("data file(*.txt)|*.txt");
 //   FileFlt += _T("||");
 //   CFileDialog FileDlg(TRUE,NULL,NULL,OFN_HIDEREADONLY,FileFlt,this,0);
 //   FileDlg.m_ofn.lpstrInitialDir = ReturnPath();
 //   FileDlg.m_ofn.lpstrTitle = L"请选择数据文件...";
 //   INT_PTR nResponse = FileDlg.DoModal();
 //   if(nResponse == IDCANCEL)
 //       return false;
	////filePath = FileDlg.GetPathName();   // filePath即为所打开的文件的路径  
	//
	//AfxMessageBox(FileDlg.GetPathName());

	CString strFileName=_T("2015_0_1_CN-6118AB.txt");
	FileRead(ReturnPath() +strFileName,&content);
	//Split(theApp.AppParameterContent,souce,_T("}"));
	Split(content,dest,_T("\n"));
	Count = dest.GetSize();
	for(int i=0;i<Count-1;i++)
	{
		val=wcstod(dest[i],&stopstring);
		if(i>0)
		{
			val=(val==-327.67?vTmp:val);
		}
		vTmp=val;
		m_Array.Add(val);
	}
	
	int first,last;
	//strFileName.MakeLower();
	first=strFileName.Find(_T('_'))+1; 
	if(first>0)
	{
		last=strFileName.Find(_T('_'),first); 

		CString sMeasurementType = strFileName.Mid(first,last-first);
		
		first=strFileName.Find(_T("_"),last)+1; 
		last=strFileName.Find(_T('_'),first); 
		CString sMeasurementPos = strFileName.Mid(first,last-first);
		
		first=strFileName.Find(_T("_"),last)+1; 
		last=strFileName.Find(_T('.'),first); 
		CString sFigureNumber = strFileName.Mid(first,last-first);
		int lMeasurementTypeIndex=wcstol(sMeasurementType,&stopstring,0);
		int lMeasurementPosIndex=wcstol(sMeasurementPos,&stopstring,0);
		
		CStringArray soucest,destst,deststr;
		Split(theApp.AppStandardContent,soucest,_T("}"));
		int Num=soucest.GetSize();
		int nNum;
		for(int m=0;m<Num-1;m++)
		{
			if(soucest[m].Find(sFigureNumber)>0)
			{
				Split(soucest[m],destst,_T(";"));
				Split(destst[lMeasurementTypeIndex+1],deststr,_T(","));
				theApp.AppStandardVal=deststr[lMeasurementPosIndex*2+1];
				break;
			}
		}

		CStringArray souce,dest;
		CStringArray possouce,posmiddle,posdest;
		
		Split(theApp.AppPositionContent,possouce,_T("}"));
		CString strTemp;
		CString strValue;
		int Count = possouce.GetSize();
		int nCount;
		for(int i=0;i<Count;i++)
		{	
			if(possouce[i].Find(sFigureNumber)>0)
			{
				Split(possouce[i],posmiddle,_T(";"));
				Split(posmiddle[lMeasurementTypeIndex+1],posdest,_T(","));
				TRACE(_T("%s\n"),posmiddle[theApp.AppMeasurementTypeIndex+1]);
				nCount = posdest.GetSize();
	
				wchar_t   *stopstring;
				theApp.AppFirstSectionStart=(int)wcstol(posdest[lMeasurementPosIndex*4],&stopstring,10);
				theApp.AppFirstSectionFinish=(int)wcstol(posdest[lMeasurementPosIndex*4+1],&stopstring,10);
				theApp.AppSecondSectionStart=(int)wcstol(posdest[lMeasurementPosIndex*4+2],&stopstring,10);
				theApp.AppSecondSectionFinish=(int)wcstol(posdest[lMeasurementPosIndex*4+3],&stopstring,10);
				break;			
			}
		}

		if(theApp.AppFirstSectionStart!=0&&theApp.AppSecondSectionFinish!=0)
		{
			CString str;
			int s1,s2,s3,s4;
			double v1,v2,v3,v4;
			//GetStandardPos(&m_Array,theApp.AppFirstSectionStart,theApp.AppSecondSectionFinish,&s1,&s2,&v1,&v2);
			//GetStandardPos(&m_Array,theApp.AppFirstSectionStart,theApp.AppSecondSectionFinish,&s3,&s4,&s1,&s2,&v1,&v2);
			GetStandardPos(&m_Array,theApp.AppFirstSectionStart,theApp.AppFirstSectionFinish ,theApp.AppSecondSectionStart,theApp.AppSecondSectionFinish,&s1,&s2,&v1,&v2);
			theApp.AppFirstSectionPosition=s1;
			theApp.AppSecondSectionPosition=s2;
			theApp.AppPosStart= theApp.AppFirstSectionStart ;
			theApp.AppPosFinish=theApp.AppSecondSectionFinish ; 
			//theApp.AppPosStart= s3;
			//theApp.AppPosFinish=s4; 
			
			//y=m_Array.GetAt(i)*cos(theApp.AppAngleVal*pi/180.0)+theApp.AppDown;
			v3=fabs(v2-v1)*theApp.AppZoomY;
			v3=fabs(v2-v1)*cos(theApp.AppAngleVal*pi/180.0);
			wchar_t   *stopstring;
			//v4=wcstod(theApp.AppStandardVal,&stopstring)*theApp.AppZoomY;
			v4=wcstod(theApp.AppStandardVal,&stopstring);
			theApp.AppMeasurementVal.Format(L"%.3f",v3);
			theApp.AppDifferenceVal.Format(L"%.3f",v3-v4);

			str.Format(L"标准值：%s\n测量值：%s\n测量差：%s\n第一点：%d\n第二点：%d\n第一点值：%.3f\n第二点值：%.3f\n采集数量：%d\n发送数量：%d\n",theApp.AppStandardVal,theApp.AppMeasurementVal,theApp.AppDifferenceVal,s1,s2,v1,v2,FlagCount,m_SendCout);
			GetDlgItem(IDC_STC_CONTENT)->SetWindowTextW(str);
			TRACE(_T("%s"),str);
			
		}

		//CStringArray soucest;
		//CStringArray destst;
		//CString str;
		//int pos=0;
		//Split(theApp.AppPositionContent,souce,_T("}"));
		//Split(theApp.AppStandardContent,soucest,_T("}"));
		//CString strTemp;
		//CString strValue;
		//int Count = souce.GetSize();
		//int nCount;
		//int Num=soucest.GetSize();
		//int nNum;
		//for(int i=0;i<Count;i++)
		//{
		//	Split(souce[i],dest,_T(","));
		//}
	}




	//m_Array.Add(0.1);
	//m_Array.Add(0.2);
	//m_Array.Add(0.5);
	//m_Array.Add(0.6);
	//int s1,s2;
	//double sc=GetMaxValue(&m_Array,2,4); 
	//GetStandardPos(&m_Array,&s1,&s2);

//int s1=0,s2=0,s3=0;
////GetStandardPos(&m_Array,&s1,&s2,&s3);
////GetStandardPos(&m_Array,&s1,&s2);
//
//GetStandardPos(&m_Array,&s1,&s2);
//theApp.AppPosStart= s1;
//theApp.AppPosFinish=s2; 

//CString str;
//str.Format(L"起始位：%d\n结束位：%d\n采集数量：%d\n发送数量：%d\n",theApp.AppPosStart,theApp.AppPosFinish,FlagCount,m_SendCout);
//GetDlgItem(IDC_STC_CONTENT)->SetWindowTextW(str);

		//double v1,v2;
		//v2=GetMaxValue(&m_Array,s1,s2);
		//v1=GetMaxValue(&m_Array,s1-150,s1);

	//if(::GetFileAttributes(_T("\\硬盘\\parameter.txt")) != 0xFFFFFFFF)
	//	::CopyFile(_T("\\硬盘\\parameter.txt"),_T("\\ResidentFlash2\\GUI\\parameter.txt"),false);
	//if(::GetFileAttributes(_T("\\硬盘\\standard.txt")) != 0xFFFFFFFF)
	//	::CopyFile(_T("\\硬盘\\standard.txt"),_T("\\ResidentFlash2\\GUI\\standard.txt"),false);

	//FileRead(_T("\\ResidentFlash2\\GUI\\parameter.txt"),&theApp.AppParameterContent);
	//FileRead(_T("\\ResidentFlash2\\GUI\\standard.txt"),&theApp.AppStandardContent);


	//FileWrite(ReturnPath() +strDate +_T(".txt"),m_Temp);
	//SetTimer(1,20,NULL);

	if (m_pSerial != NULL)
	{
		m_pSerial->ClosePort();

		delete m_pSerial;
		m_pSerial = NULL;
	}
	API_GPIO_Output(0, 0);
	API_GPIO_Output(1, 0);
	API_GPIO_Output(2, 0);
	API_GPIO_Output(3, 1);

	//新建串口通讯对象
	m_pSerial = new CCESeries();
	//m_pSerial->m_OnSeriesRead = OnSerialRead;

	//打开串口  9600
	if(m_pSerial->OpenPort(this,3,115200,0,8,0,0))
	{
		//AfxMessageBox(L"串口打开成功");
		Sleep(200);
	}
	else
	{
		AfxMessageBox(L"串口打开失败");
	}
	
	m_hThread = CreateThread(NULL,0,ThreadFunc,this,0,&m_dwThreadID);
	

	pWnd   =GetDlgItem(IDC_STC_IMAGE);     
	RECT   rect;  
	pWnd->GetClientRect(&rect);  
	CDC   *pDC   =   pWnd->GetDC();  
	int nWidth=rect.right;
	int nHeight=rect.bottom ;

	//随后建立与屏幕显示兼容的内存显示设备
MemDC.CreateCompatibleDC(NULL);
//这时还不能绘图，因为没有地方画 ^_^
//下面建立一个与屏幕显示兼容的位图，至于位图的大小嘛，可以用窗口的大小
//MemBitmap.CreateCompatibleBitmap(pDC,nWidth,nHeight);
MemBitmap.CreateBitmap(nWidth*10,nHeight*10,1,16,NULL);
//MemBitmap.CreateBitmap(nWidth,nHeight,1,16,NULL);
//将位图选入到内存显示设备中
//只有选入了位图的内存显示设备才有地方绘图，画到指定的位图上
pOldBit=MemDC.SelectObject(&MemBitmap);
//先用背景色将位图清除干净，这里我用的是白色作为背景
//你也可以用自己应该用的颜色
MemDC.FillSolidRect(0,0,nWidth*10,nHeight*10,RGB(255,255,255));

srand(GetTickCount());

	int iFullWidth = GetSystemMetrics(SM_CXSCREEN);
	int iFullHeight = GetSystemMetrics(SM_CYSCREEN); 
	::SetWindowPos(this->m_hWnd, HWND_TOPMOST, 0, 0, iFullWidth, iFullHeight, SWP_NOOWNERZORDER|SWP_SHOWWINDOW);

	::SetCursorPos(iFullHeight,iFullWidth);
	ShowCursor(FALSE);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
void CReduceMeasurementDlg::OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/)
{
	if (AfxIsDRAEnabled())
	{
		DRA::RelayoutDialog(
			AfxGetResourceHandle(), 
			this->m_hWnd, 
			DRA::GetDisplayMode() != DRA::Portrait ? 
			MAKEINTRESOURCE(IDD_REDUCEMEASUREMENT_DIALOG_WIDE) : 
			MAKEINTRESOURCE(IDD_REDUCEMEASUREMENT_DIALOG));
	}
}
#endif


void CReduceMeasurementDlg::OnBnClickedButExit()
{
	// TODO: 在此添加控件通知处理程序代码
	//m_Array.RemoveAll();  //删除所有元素

	////m_hThread = CreateThread(NULL,0,ThreadFunc,this,0,&m_dwThreadID);

	//RunFlag=true;
	//m_DIR=0;
	//API_GPIO_Output(1,m_DIR);
	////SetTimer(1,100,NULL);
	//API_GPIO_Output(2, 1);
	//iNUM=0;

	//	CReduceMessageDlg dlg;
	////m_pMainWnd = &dlg;
	//INT_PTR nResponse = dlg.DoModal();
	//if (nResponse == IDOK)
	//{
	//	// TODO: 在此放置处理何时用
	//	//  “确定”来关闭对话框的代码
	//}


	CloseThread();
	this->OnOK();
	//this->OnDestroy(); 
}

void CReduceMeasurementDlg::OnBnClickedButSave()
{
	// TODO: 在此添加控件通知处理程序代码
	//CWnd   *pWnd   =   GetDlgItem(IDC_STC_IMAGE);     
	//RECT   rect;  
	//pWnd->GetClientRect(&rect);  
	//CDC   *pControlDC   =   pWnd->GetDC();  
	//pWnd->Invalidate();  
	//pWnd->UpdateWindow();  

	//pControlDC->SelectStockObject(WHITE_BRUSH);  
	//pControlDC->Rectangle(&rect);  
	//pControlDC->SelectStockObject(BLACK_BRUSH);  
	//pControlDC->MoveTo(0 ,rect.bottom/4); 
	//pControlDC->LineTo(rect.right ,rect.bottom/4);

	//pControlDC->MoveTo(rect.right/2  ,0); 
	//pControlDC->LineTo(rect.right/2 ,rect.bottom);

	((CButton*)GetDlgItem(IDC_BUT_TEST))->EnableWindow(false);
	((CButton*)GetDlgItem(IDC_BUT_SAVE))->EnableWindow(false);
	int Num;
	CString str;
	double dTemp=0.0;
	CString  m_Temp=_T("");
	SYSTEMTIME st;
	CString strDate,strTime;
	GetLocalTime(&st);

	 //if(MessageBox(_T("是否要保存本组数据？"),_T("保存"),MB_YESNO||MB_ICONQUESTION)==IDYES)
	 //{
		CString m_Str=_T(""),m_Tmp=_T("");
		wchar_t   *stopstring;
		
		strDate.Format(_T("%4d%02d%02d"),st.wYear,st.wMonth,st.wDay);
		theApp.AppFilePath= theApp.AppStoragePath +strDate +_T(".txt");
		if(theApp.AppFilePath.Compare(_T(""))!=0)
		{
			//this->MessageBoxW(L"SSS");
				
				if(::GetFileAttributes(theApp.AppFilePath) == 0xFFFFFFFF){
					m_Str=_T("位置标识\t道岔编号\t测量位置(m)\t标准值(mm)\t测量值(mm)\t差值(mm)\r\n");
				}
				else{
					m_Str=_T("");
				}
				//		位置标识			道岔编号			测量位置			标准值				测量值				差值
				m_Tmp.Format(_T("%f"),wcstod(theApp.AppMeasurementVal,&stopstring)-wcstod(theApp.AppStandardVal,&stopstring));
				m_Str+=theApp.AppIdentification +_T("\t")+theApp.AppTurnoutNumber +_T("\t")+theApp.AppMeasurementPos +_T("\t")+theApp.AppStandardVal +_T("\t")+theApp.AppMeasurementVal +_T("\t")+m_Tmp+_T("\r\n");
				//FileWrite(_T("\\ResidentFlash2\\GUI\\") +strDate +_T(".txt"),m_Temp);
				FileWrite(theApp.AppFilePath,m_Str);
		}
	 //}



	strDate.Format(_T("%4d%02d%02d%02d%02d%02d"),st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond);
	
	Num=m_Array.GetCount();
	for(int i=0;i<Num;i++)
	{
		if(m_Array.GetAt(i)!=0.0) dTemp=m_Array.GetAt(i);
	str.Format(_T("%lf"),m_Array.GetAt(i)); 
	m_Temp+=str+_T("\r\n");//+_T(";");
	}
	//FileWrite(_T("\\ResidentFlash2\\GUI\\") +strDate +_T(".txt"),m_Temp);
	if(dTemp!=0.0)
	{
		if(theApp.AppFigureNumber.Compare(_T(""))!=0)
		{
			strTime.Format(_T("_%d_%d_%s"),theApp.AppMeasurementTypeIndex, theApp.AppMeasurementPosIndex,theApp.AppFigureNumber);   
		}
	//FileWrite(ReturnPath() +strDate + strTime +_T(".txt"),m_Temp);
	//FileWrite(_T("\\硬盘\\") +strDate + strTime +_T(".txt"),m_Temp);
	FileWrite(theApp.AppStoragePath +strDate + strTime +_T(".txt"),m_Temp);
		
	//FileWrite(_T("\\硬盘\\") +strDate +_T(".txt"),m_Temp);
	MessageBoxW(L"当前保存文件为：" +strDate,L"标题", MB_ICONEXCLAMATION|MB_YESNO);
	Invalidate();
	}
	else
	{
		if(MessageBoxW(L"测试数据为空,重新测试！",L"标题", MB_ICONEXCLAMATION|MB_YESNO)==IDYES)
			OnBnClickedButTest();
	}

	((CButton*)GetDlgItem(IDC_BUT_TEST))->EnableWindow(true);
	((CButton*)GetDlgItem(IDC_BUT_SAVE))->EnableWindow(true);

	//double data;
	//data=GetValue();
	//CString str;
	//str.Format(L"%lf\n",data);
	////plist->AddString((LPCTSTR)str); 
	//TRACE(_T("%s"),str);
	//BYTE buf[]={0x02,0x43,0xB0,0x01,0x03,0xF2};
	//BYTE *revbuf=NULL;
	//DWORD revlen=0;
	//DWORD errlen=0;
	//double data=0.0;
	//revbuf=m_pSerial->Command((BYTE*)buf,sizeof(buf),&revlen);
	//if(revbuf[0] == 0x02&&revbuf[1] == 0x06&&revbuf[4] == 0x03)
	//	data=revbuf[2]*256+revbuf[3];
}

void CReduceMeasurementDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialog::OnPaint()
	//CString str;
	//str.Format(_T("测量值：\n%d\n发送量：\n%d"),(int)m_Array.GetSize(),m_SendCout); 
	//GetDlgItem(IDC_STC_CONTENT)->SetWindowTextW(str);
	//dc.SetGraphicsMode .SetWorldTransform .GetViewportExt

	double x,y;
	CString str;
	CWnd   *pWnd   =   GetDlgItem(IDC_STC_IMAGE);     
	RECT   rect;  
	pWnd->GetClientRect(&rect);  
	CDC   *pDC   =   pWnd->GetDC();  
	pWnd->UpdateWindow();  
	int nWidth=rect.right;
	int nHeight=rect.bottom ;
	int px,py;
	px=GetDeviceCaps(pDC->m_hDC,LOGPIXELSX);
	py=GetDeviceCaps(pDC->m_hDC,LOGPIXELSY);


	//pWnd->Invalidate();  
	
//CDC MemDC; //首先定义一个显示设备对象
//CBitmap MemBitmap;//定义一个位图对象
////随后建立与屏幕显示兼容的内存显示设备
//MemDC.CreateCompatibleDC(NULL);
////这时还不能绘图，因为没有地方画 ^_^
////下面建立一个与屏幕显示兼容的位图，至于位图的大小嘛，可以用窗口的大小
//MemBitmap.CreateCompatibleBitmap(pDC,nWidth,nHeight);
////将位图选入到内存显示设备中
////只有选入了位图的内存显示设备才有地方绘图，画到指定的位图上
//CBitmap *pOldBit=MemDC.SelectObject(&MemBitmap);
////先用背景色将位图清除干净，这里我用的是白色作为背景
////你也可以用自己应该用的颜色
//MemDC.FillSolidRect(0,0,nWidth,nHeight,RGB(255,255,255));
////绘图
//MemDC.MoveTo(0 ,nHeight/4); 
//MemDC.LineTo(nWidth ,nHeight/4);
////将内存中的图拷贝到屏幕上进行显示
//pDC->BitBlt(0,0,nWidth,nHeight,&MemDC,0,0,SRCCOPY);
////绘图完成后的清理
//MemBitmap.DeleteObject(); 
//MemDC.SelectObject(pOldBit);
//MemDC.DeleteDC();

//		//绘图
//MemDC.MoveTo(0 ,nHeight/4); 
//MemDC.LineTo(nWidth ,nHeight/4);
//MemDC.MoveTo(0 ,0); 
//for(int i=0;i<100;i++)
//{
//MemDC.LineTo(i*2 ,rand()%50);
//}
////将内存中的图拷贝到屏幕上进行显示
//pDC->BitBlt(0,0,nWidth,nHeight,&MemDC,0,0,SRCCOPY);
////pDC->StretchBlt(0,0,nWidth,nHeight,&MemDC,0,0,nWidth,nHeight,SRCCOPY);


////	PAINTSTRUCT paintStruct;
////    pWnd->BeginPaint(&paintStruct);
////
////	CDC dcMem;                                                  //用于缓冲作图的内存DC
////	CBitmap bmp;                                                 //内存中承载临时图象的位图
////	dcMem.CreateCompatibleDC(pDC);               //依附窗口DC创建兼容内存DC
////	bmp.CreateCompatibleBitmap(&dcMem,rect.right*1000,rect.bottom*1000);//创建兼容位图
////	dcMem.SelectObject(&bmp);                          //将位图选择进内存DC
////	dcMem.FillSolidRect(&rect,pDC->GetBkColor());//按原来背景填充客户区，不然会是黑色
////	//for(int i=20;i>0;i--)                                          //在内存DC上做同样的同心圆图象
////	//{
////	//	ellipseRect.SetRect(ptCenter,ptCenter);
////	//	ellipseRect.InflateRect(i*10,i*10);
////	//	dcMem.Ellipse(ellipseRect);
////	//}
////	dcMem.MoveTo(0 ,rect.bottom*1000/4); 
////	dcMem.LineTo(rect.right*1000 ,rect.bottom*1000/4);
////
////	dcMem.MoveTo(rect.right*1000/2  ,0); 
////	dcMem.LineTo(rect.right*1000/2 ,rect.bottom*1000);
////	CBrush brush,*oldbrush;
////	brush.CreateSolidBrush(RGB(255,0,0));
////	oldbrush=dcMem.SelectObject(&brush);
////
////	//pDC->Ellipse(10,10,100,100);
////	//pDC->SetPixel(10,10,RGB(255,0,0)); 
////
////	for(int i=0;i<(int)m_Array.GetSize();i++)
////	{
////		x=i*0.15*1000;y=m_Array.GetAt(i)*1000;
////		dcMem.SetPixel(int(x),int(20.0+y),RGB(255,0,0)); 
////		//x=-50.0+i*0.35;y=m_Data.GetAt(i);
////		//pDC->SetPixel(int(x+rect.right/2),int(rect.bottom/4-y),RGB(255,0,0));//rect.bottom-(pst[0].y/2.5+rect.bottom/4));  
////		//pDC->Ellipse(int((x-r)/2.5+rect.right/2),int(rect.bottom/4-(y-r)/2.5),int((x+r)/2.5+rect.right/2),int(rect.bottom/4-(y+r)/2.5));
////	}
////
////	dcMem.SelectObject(oldbrush);
////
////
////	pDC->BitBlt(0,0,rect.right ,rect.bottom ,&dcMem,0,0,SRCCOPY);//将内存DC上的图象拷贝到前台
////	dcMem.DeleteDC();                                       //删除DC
////	bmp.DeleteObject();                                        //删除位图
////	pWnd->ReleaseDC(pDC); 
////
////pWnd->EndPaint(&paintStruct);

	pDC->SelectStockObject(WHITE_BRUSH);  
	pDC->Rectangle(&rect);  
	pDC->SelectStockObject(BLACK_BRUSH);  
	pDC->MoveTo(0 ,rect.bottom/2); 
	pDC->LineTo(rect.right ,rect.bottom/2);

	pDC->MoveTo(rect.right/2  ,0); 
	pDC->LineTo(rect.right/2 ,rect.bottom);

	//m_Array
	CBrush brush,*oldbrush;
	brush.CreateSolidBrush(RGB(0,0,0));
	oldbrush=pDC->SelectObject(&brush);

	//pDC->Ellipse(10,10,100,100);
	//pDC->SetPixel(10,10,RGB(255,0,0)); 

	//if(theApp.AppPosStart>0&&theApp.AppPosFinish>0)
	//{
	//	//this->MessageBoxW(L"eee");
	//	TRACE(_T("%d__%d"),theApp.AppPosStart,theApp.AppPosFinish);
	//	for(int i=theApp.AppPosStart ;i<theApp.AppPosFinish;i++)
	//	{
	//	//x=i*sin(35.0*pi/180.0);y=m_Array.GetAt(i)*cos(35.0*pi/180.0);
	//	//x=i*0.15;y=m_Array.GetAt(i)*cos(35.0*pi/180.0);
	//	//pDC->SetPixel(int(10.0+x),int(20.0+y),RGB(255,0,0)); 
	//	//x=-50.0+i*5.0;y=m_Array.GetAt(i)*5.0;
	//	x=-100.0+i*sin(35.0*pi/180.0);y=m_Array.GetAt(i)*cos(35.0*pi/180.0)+20.0;
	//	if(i==0) pDC->MoveTo((x+rect.right/2),(rect.bottom/2-y));//pDC->MoveTo((x+rect.right/2),(rect.bottom/2-y));
	//	//pDC->SetPixel((x+rect.right/2),(rect.bottom/4-y),RGB(255,0,0));//rect.bottom-(pst[0].y/2.5+rect.bottom/4));  
	//	pDC->LineTo((x+rect.right/2),(rect.bottom/2-y));
	//	//pDC->LineTo((int)x,(int)y);
	//	//pDC->Ellipse(int((x-r)/2.5+rect.right/2),int(rect.bottom/4-(y-r)/2.5),int((x+r)/2.5+rect.right/2),int(rect.bottom/4-(y+r)/2.5));
	//	}
	//}
	//else
	//{
		
		//for(int i=0;i<(int)m_Array.GetSize();i++)
		if(m_Array.GetSize()>0)
		{
			double sh;
			if(m_Array.GetSize()>1000)
			{
				sh=theApp.AppZoomX;
			}
			else
			{
				sh=1.2;
			}

			for(int i=theApp.AppPosStart;i<(int)theApp.AppPosFinish;i++)
			{
			//x=i*sin(35.0*pi/180.0);y=m_Array.GetAt(i)*cos(35.0*pi/180.0);
			//x=i*0.15;y=m_Array.GetAt(i)*cos(35.0*pi/180.0);
			//pDC->SetPixel(int(10.0+x),int(20.0+y),RGB(255,0,0)); 
			//x=-50.0+i*5.0;y=m_Array.GetAt(i)*5.0;

			x=theApp.AppLeft+(i-theApp.AppPosStart)*sh*sin(theApp.AppAngleVal*pi/180.0);y=m_Array.GetAt(i)*cos(theApp.AppAngleVal*pi/180.0)+theApp.AppDown;
			if(i==theApp.AppPosStart) pDC->MoveTo((x+rect.right/2+0.5f),(rect.bottom/2-y+0.5f));
			if(i==theApp.AppFirstSectionPosition ||i==theApp.AppSecondSectionPosition)
			{
				pDC->LineTo((x+rect.right/2+0.5f),(rect.bottom/2-y+0.5f));
				pDC->LineTo((x+rect.right/2+0.5f),(0));
				pDC->MoveTo((x+rect.right/2+0.5f),(rect.bottom/2-y+0.5f));
			}
			//pDC->SetPixel((x+rect.right/2),(rect.bottom/4-y),RGB(255,0,0));//rect.bottom-(pst[0].y/2.5+rect.bottom/4));  
			pDC->LineTo((x+rect.right/2+0.5f),(rect.bottom/2-y+0.5f));
			//pDC->LineTo((int)x,(int)y);
			//pDC->Ellipse(int((x-r)/2.5+rect.right/2),int(rect.bottom/4-(y-r)/2.5),int((x+r)/2.5+rect.right/2),int(rect.bottom/4-(y+r)/2.5));
			}
		}
	//}
	
	//x=10.0+m_TimerNum*0.35;y=rev/1000.0;
	//pDC->Ellipse(int((x-r)/2.5+rect.right/2),int(rect.bottom/4-(y-r)/2.5),int((x+r)/2.5+rect.right/2),int(rect.bottom/4-(y+r)/2.5));

	//for (int i=0;i<2;i++)  
	//{  
	//    pDC->LineTo(int(pst[i].x/2.5+rect.right/2),int(rect.bottom/4-pst[i].y/2.5));//rect.bottom-(pst[0].y/2.5+rect.bottom/4));  
	//}

	//pDC->DrawText(_T("12.12"), &rect,DT_CENTER|DT_VCENTER);
	//DrawArc(pDC->m_hDC,100,100,200,200,300); 
	pDC->SelectObject(oldbrush);
	pWnd->ReleaseDC(pDC); 
	CDC::DeleteTempMap();


 //long nWidth=rect.right;
 //long nHigth=rect.bottom;
 //CDC tempdc;//B桌子
 //CBitmap MemBitmap;//新桌布
 //CBitmap*OldBitmap;//旧桌布

 //tempdc.CreateCompatibleDC(pDC); //实体化B桌子
 //MemBitmap.CreateCompatibleBitmap(pDC,nWidth,nHigth);//实体化给B使用的桌布
 ////这里是拷贝了A的位图，自己用张图片也行，大小注意下
 //OldBitmap=tempdc.SelectObject(&MemBitmap);
 ////给B使用
 //tempdc.FillSolidRect(0,0,nWidth,nHigth,RGB(0,255,255));
 ////默认黑色，自己按需改变
 //	tempdc.MoveTo(0 ,rect.bottom/4); 
	//tempdc.LineTo(rect.right ,rect.bottom/4);

	//tempdc.MoveTo(rect.right/2  ,0); 
	//tempdc.LineTo(rect.right/2 ,rect.bottom);
 ////...............
 ////在B桌布上画画，tempdc
 //pDC->BitBlt(0,0,nWidth,nHigth,&tempdc,0,0,SRCCOPY);
 ////拷贝给A
 //tempdc.SelectObject(OldBitmap);
 ////恢复B原有的桌布（习惯吧，就像申请内存后记得释放）
 ////删除临时对象
 //MemBitmap.DeleteObject();
 //tempdc.DeleteDC();

	 // CDC   MemDC;   //首先定义一个显示设备对象  
  //CBitmap   MemBitmap;//定义一个位图对象  
  // 
  ////随后建立与屏幕显示兼容的内存显示设备  
  //MemDC.CreateCompatibleDC(NULL);  
  ////这时还不能绘图，因为没有地方画   ^_^  
  ////下面建立一个与屏幕显示兼容的位图，至于位图的大小嘛，可以用窗口的大小  
  //MemBitmap.CreateCompatibleBitmap(pDC,nWidth,nHeight);  
  //   
  ////将位图选入到内存显示设备中  
  ////只有选入了位图的内存显示设备才有地方绘图，画到指定的位图上  
  //CBitmap   *pOldBit=MemDC.SelectObject(&MemBitmap);  
  // 
  ////先用背景色将位图清除干净，这里我用的是白色作为背景  
  ////你也可以用自己应该用的颜色  
  //MemDC.FillSolidRect(0,0,nWidth,nHeight,RGB(255,255,255));  
  // 
  ////绘图  
  //MemDC.MoveTo(……);  
  //MemDC.LineTo(……);  
  // 
  ////将内存中的图拷贝到屏幕上进行显示  
  //pDC->BitBlt(0,0,nWidth,nHeight,&MemDC,0,0,SRCCOPY);  
  // 
  ////绘图完成后的清理  
  //MemBitmap.DeleteObject();  
  //MemDC.DeleteDC();  
   
}

void CReduceMeasurementDlg::OnStnClickedStcTitle()
{
	// TODO: 在此添加控件通知处理程序代码


	intExitFlag++;
	if(intExitFlag>9) this->OnOK();
}
void CReduceMeasurementDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	double data=0.0;
	CString str;
//	CWnd   *pWnd   =  GetDlgItem(IDC_STC_IMAGE);  
//RECT   rect;  
//					pWnd->GetClientRect(&rect);  
//					CDC   *pControlDC   =   pWnd->GetDC();  

	switch(nIDEvent)
	{
	case 3:
		API_GPIO_Output(0,0);
		Sleep(1);
		API_GPIO_Output(1,0);
		break;
	case 1:
	//	//OnPaint();
	//	//intExitFlag=0;

	//	//FlagCount++;
	//	//if(FlagCount==200)
	//	//{
	//	//	//FlagCount=0;
	//	//	m_DIR=m_DIR!=0?0:1;
	//	//	API_GPIO_Output(1, m_DIR);
	//	//	m_Array.RemoveAll();  //删除所有元素	
	//	//}
	//	//else if(FlagCount>200&&FlagCount<400)
	//	//{
	//		//data=GetValue();
	//		//m_Array.Add(data);
	//		//double x,y;
	//		
	//		//CListBox *plist=(CListBox*)GetDlgItem(IDC_LIST1);
	//		//CString str;
	//		//str.Format(L"%lf",data);
	//		//plist->AddString((LPCTSTR)str); 
	//		//CWnd   *pWnd   =   ReduceMeasurementDlg->GetDlgItem(IDC_STC_IMAGE);     

	//		//pWnd->Invalidate();  
	//		//pWnd->UpdateWindow();
	//	//	Invalidate();
	//	//}
	//	//else if(FlagCount==400)
	//	//{
	//	//		API_GPIO_Output(2, 0);
	//	//		FlagCount=0;
	//	//		KillTimer(1);
	//	//}
	//		BYTE buf[]={0x02,0x43,0xB0,0x01,0x03,0xF2};
	//BYTE *revbuf=NULL;
	//DWORD revlen=0;
	//DWORD errlen=0;
	//double data=0.0;
	//double temp=0.0;
	//DWORD dwRet;
	//UINT8 Level;
	//CString ss;
	//CString str;
	//int CW=0;
	//bool DropFlag=false;

	//	if(RunFlag)
	//	{
	//		API_GPIO_Input(2,&Level);//读停止信号
	//		if(Level==0)
	//		{       
	//			RunFlag=false;
	//			API_GPIO_Output(2, 0);
	//			CW=0;
	//			str.Format(L"采集数量：%d\n",FlagCount);
	//			TRACE(_T("%s"),str);
	//			FlagCount=0;
	//			Invalidate();
	//			//str="";
	//			//for(int i=0;i<(int)m_Array.GetSize();i++)
	//			//{
	//			//	ss.Format(_T("%d:%f;\n"),i,m_Array.GetAt(i));
	//			//	TRACE(_T("%s"),ss);
	//			//	//str.Append(ss);
	//			//}
	//			//str1.append (str2);   //将str2连接到str1后
	//			//TRACE(_T("%s"),str);
	//			TRACE0( "停止信号……\n"  );

	//		}
	//		API_GPIO_Input(1,&Level);//读方向信号
	//		if(Level==0)
	//		{
	//			revbuf=m_pSerial->Command((BYTE*)buf,sizeof(buf),&revlen);
	//			Sleep(100);
	//			//CW=CW!=0?0:1;
	//			CW=1;
	//			//API_GPIO_Output(1,CW);
	//			//API_GPIO_Output(1,1);
	//			FlagCount=0;
	//			m_Array.RemoveAll();  //删除所有元素
	//			//hDlg->Invalidate();
	//			TRACE0( "反向信号……\n"  );
	//		}
	//		if(CW==1)
	//		{
	//			API_GPIO_Input(0,&Level);//读采集信号
	//			if(Level==0 && DropFlag==true)
	//			{
	//				DropFlag=false;
	//				//data=rand()%50;

	//				//TRACE0( "Start Dump of MyClass members:\n"  );
	//				//hDlg->FlagCount++;
	//				FlagCount++;
	//				revbuf=m_pSerial->Command((BYTE*)buf,sizeof(buf),&revlen);
	//				if(revlen==6)
	//				{
	//				if(revbuf[0] == 0x02&&revbuf[1] == 0x06&&revbuf[4] == 0x03)
	//					data=(revbuf[2]*256+revbuf[3])/100.0;
	//					if(data>327.67) data=data-655.35;
	//					temp=data;
	//				}
	//				else
	//				{
	//					data=temp;
	//				}
	//				////data=hDlg->GetValue();
	//				////if(CW==1)
	//				////{
	//				//
	//				////str.Format(L"采集序号：%d\n",hDlg->FlagCount);
	//				////TRACE(_T("%s"),str);
	//				m_Array.Add(data);
	//				//hDlg->Invalidate();
	//				//}
	//				//double x,y;
	//				//CListBox *plist=(CListBox*)GetDlgItem(IDC_LIST1);
	//				//CString str;
	//				//str.Format(L"%lf",data);
	//				//plist->AddString((LPCTSTR)str); 
	//				//CWnd   *pWnd   =   ReduceMeasurementDlg->GetDlgItem(IDC_STC_IMAGE);     

	//				//pWnd->Invalidate();  
	//				//pWnd->UpdateWindow();
	//				//hDlg->Invalidate();
	//			}
	//			else if(Level==1)
	//			{
	//				DropFlag=true;
	//			}
	//		}
	//	}
		break;
	case 2:
		RunFlag=0;
		API_GPIO_Output(0,0);
		Sleep(1);
		API_GPIO_Output(1,1);
		KillTimer(2);
		((CButton*)GetDlgItem(IDC_BUT_TEST))->EnableWindow(true);
		((CButton*)GetDlgItem(IDC_BUT_SAVE))->EnableWindow(true);

		
		//GetStandardPos(&m_Array,&s1,&s2,&s3);
		if(theApp.AppFirstSectionStart!=0&&theApp.AppSecondSectionFinish!=0)
		{
			//str.Format(L"标准值：%s\n测量值：%s\n采集数量：%d\n发送数量：%d\n",theApp.AppStandardVal,theApp.AppMeasurementVal ,FlagCount,m_SendCout);
			//GetDlgItem(IDC_STC_CONTENT)->SetWindowTextW(str);
			//TRACE(_T("%s"),str);
			//Invalidate();

			int s1,s2,s3,s4;
			double v1,v2,v3,v4;
			//GetStandardPos(&m_Array,theApp.AppFirstSectionStart,theApp.AppSecondSectionFinish,&s1,&s2,&v1,&v2);
			//GetStandardPos(&m_Array,255,550,&s3,&s4,&s1,&s2,&v1,&v2);
			GetStandardPos(&m_Array,theApp.AppFirstSectionStart,theApp.AppFirstSectionFinish ,theApp.AppSecondSectionStart,theApp.AppSecondSectionFinish,&s1,&s2,&v1,&v2);
			theApp.AppFirstSectionPosition=s1;
			theApp.AppSecondSectionPosition=s2;
			theApp.AppPosStart= theApp.AppFirstSectionStart ;
			theApp.AppPosFinish=theApp.AppSecondSectionFinish ; 
			//theApp.AppPosStart= s3;
			//theApp.AppPosFinish=s4; 

			//v3=fabs(v2-v1)*theApp.AppZoomY;
			v3=fabs(v2-v1)*cos(theApp.AppAngleVal*pi/180.0);
			wchar_t   *stopstring;
			//v4=wcstod(theApp.AppStandardVal,&stopstring)*theApp.AppZoomY;
			v4=wcstod(theApp.AppStandardVal,&stopstring);
			theApp.AppMeasurementVal.Format(L"%.3f",v3);
			theApp.AppDifferenceVal.Format(L"%.3f",v3-v4);

			str.Format(L"标准值：%s\n测量值：%s\n标准差：%s\n第一点：%d\n第二点：%d\n第一点值：%.3f\n第二点值：%.3f\n采集数量：%d\n发送数量：%d\n",theApp.AppStandardVal,theApp.AppMeasurementVal,theApp.AppDifferenceVal,s1,s2,v1,v2 ,FlagCount,m_SendCout);
			GetDlgItem(IDC_STC_CONTENT)->SetWindowTextW(str);
			TRACE(_T("%s"),str);
			Invalidate();
		}
		else
		{
			str.Format(L"标准值：%s\n采集数量：%d\n发送数量：%d\n",theApp.AppStandardVal,FlagCount,m_SendCout);
			GetDlgItem(IDC_STC_CONTENT)->SetWindowTextW(str);
			TRACE(_T("%s"),str);
			Invalidate();
		}
		break;
	//default:
	//	break;
	}
	CDialog::OnTimer(nIDEvent);
}

void CReduceMeasurementDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	

	CDialog::OnClose();
}

double CReduceMeasurementDlg::GetValue(void)
{
	long revlen=0;
	short rev=0;

	BYTE* bufdata=NULL;
	//将接收的缓冲区拷贝到pRecvBuf种
	DWORD dwStart = GetTickCount();
	DWORD dwEnd = 0;
	

	BYTE buf[]={0x02,0x43,0xB0,0x01,0x03,0xF2};
	m_pSerial->WriteSyncPort((BYTE*)buf,sizeof(buf));
	//CopyMemory(buf,pRecvBuf,pRecvLen);
	do
	{
		if(pRecvLen>0)
		{
			bufdata = new BYTE[pRecvLen];
			CopyMemory(bufdata,pRecvBuf,pRecvLen);
			if(pRecvLen>0)
				rev=bufdata[2]*256+bufdata[3];
			if(rev>32767) rev=rev-65535;
			return rev/100.0;
		}
	 dwEnd = GetTickCount();
	 
	} while((dwEnd - dwStart) <= 1000); 
	
	return 0.0;

	//BYTE *revbuf=NULL;
	//revbuf=m_pSerial->ReadSyncPort(&revlen);
	//if(revlen>0)
	//	rev=revbuf[2]*256+revbuf[3];
	//return rev/100.0;
}
/*
void CReduceMeasurementDlg::OnBnClickedButTest()
{
	// TODO: 在此添加控件通知处理程序代码
	//m_Array.RemoveAll();  //删除所有元素
	//m_DIR=0;
	//API_GPIO_Output(1,m_DIR);
	//SetTimer(1,100,NULL);
	//API_GPIO_Output(2, 1);
	//iNUM=0;
	CReduceMessageDlg dlg;
	//m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 在此放置处理何时用
		//  “确定”来关闭对话框的代码
		((CButton*)GetDlgItem(IDC_BUT_TEST))->EnableWindow(false);
		TRACE0( "开始信号……\n"  );
		//RunFlag=1;
		//m_dwRunFlag=1;
		theApp.AppTempVal=0.0;
		RunFlag=2;
		ExcFlag=2;
		FlagCount=0;
		m_SendCout=0;
		//SetTimer(2,15000,NULL);
		API_GPIO_Output(1,1);
		Sleep(1);
		API_GPIO_Output(0,0);
		theApp.AppPosStart=0;
		theApp.AppPosFinish=0; 
		m_Array.RemoveAll();  //删除所有元素

		//API_GPIO_Output(2, 1);
		//m_hThread = CreateThread(NULL,0,ThreadFunc,this,0,&m_dwThreadID);
	}
}
*/

void CReduceMeasurementDlg::OnBnClickedButTest()
{
	// TODO: 在此添加控件通知处理程序代码
	//m_Array.RemoveAll();  //删除所有元素
	//m_DIR=0;
	//API_GPIO_Output(1,m_DIR);
	//SetTimer(1,100,NULL);
	//API_GPIO_Output(2, 1);
	//iNUM=0;
	CReduceMessageDlg dlg;
	//m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 在此放置处理何时用
		//  “确定”来关闭对话框的代码
		((CButton*)GetDlgItem(IDC_BUT_TEST))->EnableWindow(false);
		TRACE0( "开始信号……\n"  );
		//RunFlag=1;
		//m_dwRunFlag=1;
		theApp.AppTempVal=0.0;
		RunFlag=2;
		FlagCount=0;
		m_SendCout=0;
		SetTimer(2,15000,NULL);
		API_GPIO_Output(0,1);
		Sleep(1);
		API_GPIO_Output(1,0);
		theApp.AppPosStart=0;
		theApp.AppPosFinish=0; 
		m_Array.RemoveAll();  //删除所有元素

		//API_GPIO_Output(2, 1);
		//m_hThread = CreateThread(NULL,0,ThreadFunc,this,0,&m_dwThreadID);
	}
}

void CReduceMeasurementDlg::OnBnClickedButExtended()
{
	// TODO: 在此添加控件通知处理程序代码
	CTestDlg dlg;
	int Num;
	CString str;
	CString  m_Temp=_T("");

	//m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		Num=m_Array.GetCount();
		for(int i=0;i<Num;i++)
		{
		str.Format(_T("%lf"),m_Array.GetAt(i)); 
		m_Temp+=str+_T("\r\n");//+_T(";");
		}
		
		FileWrite(theApp.AppFilePath,m_Temp);
	}

	//int Num;
	//CString str;
	//CString  m_Temp=_T("");
	//SYSTEMTIME st;
	//CString strDate,strTime;
	//GetLocalTime(&st);
	//strDate.Format(_T("%4d%02d%02d%02d%02d%02d"),st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond);
	//
	//Num=m_Array.GetCount();
	//for(int i=0;i<Num;i++)
	//{
	//str.Format(_T("%lf"),m_Array.GetAt(i)); 
	//m_Temp+=str+_T("\r\n");//+_T(";");
	//}

	////FileWrite(_T("\\ResidentFlash2\\GUI\\") +strDate +_T(".txt"),m_Temp);
	//
	//FileWrite(ReturnPath() +strDate +_T(".txt"),m_Temp);
	////FileWrite(_T("\\硬盘\\") +strDate +_T(".txt"),m_Temp);


}

int CReduceMeasurementDlg::FileWrite(CString filename, CString content)
{
		  	HANDLE hFile = INVALID_HANDLE_VALUE;	// 文件句柄 
	// 创建一个文件或打开一个文件
	hFile = CreateFile(filename, GENERIC_READ | GENERIC_WRITE, 0, 
					   NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		//AfxMessageBox(_T("创建文件失败!"));
		return -1;
	}

	DWORD len,actlen;
	char *pcharbuff;

	//len = content.GetLength();							// 取得输入字符串的长度
	len=CStringA(content).GetLength();

	pcharbuff = new char[len];
	LPTSTR pStr = content.GetBuffer(len);					// 取得输入字符串的缓冲区指针

	// 将 Unicode 的 CString 转换为 char 型
	WideCharToMultiByte(CP_ACP, 0, pStr, len, pcharbuff, len, NULL, NULL);
	content.ReleaseBuffer();

	BOOL ret = SetFilePointer(hFile, 0, NULL, FILE_END);	// 移动文件指针到文件开头
	if (ret == 0xFFFFFFFF)
	{
		//AfxMessageBox(_T("将文件指针移至文件开头失败!"));
		delete[] pcharbuff;
		return -2;	
	}

	ret = WriteFile(hFile, pcharbuff, len, &actlen, NULL);	// 将数据写入文件中 
	
	if (pcharbuff != NULL) 
		delete[] pcharbuff;

	CloseHandle(hFile);
	if (ret == TRUE)
		return 0;
		//AfxMessageBox(_T("写文件成功!"));	
	else
		return -3;
		//AfxMessageBox(_T("写文件失败!"));
	return 0;
}

int CReduceMeasurementDlg::FileRead(CString filename,CString* content)
{
	int lRet;
	HANDLE hFile = INVALID_HANDLE_VALUE;	// 文件句柄 
	if(::GetFileAttributes(filename)==0xFFFFFFFF)
	{
	//::AfxMessageBox(_T("文件不存在"));
		return -5;
	}
	// 创建一个文件或打开一个文件
	hFile = CreateFile(filename, GENERIC_READ | GENERIC_WRITE, 0, 
					   NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		//AfxMessageBox(_T("打开文件失败!"));
		return -1;
	}

	DWORD filelen,actlen;
	char *pcharbuff;
	
	filelen = GetFileSize(hFile, NULL);							// 获取文件大小
	if (filelen == 0xFFFFFFFF)
	{
		//AfxMessageBox(_T("获取文件大小失败!"));
		return -2;	
	}

	BOOL ret = SetFilePointer(hFile, 0, NULL, FILE_BEGIN);		// 移动文件指针到文件开头
	if (ret == 0xFFFFFFFF)
	{
		//AfxMessageBox(_T("将文件指针移至文件开头失败!"));
		return -3;	
	}

	pcharbuff = new char[filelen];
	ret = ReadFile(hFile, pcharbuff, filelen, &actlen, NULL);	// 从文件中读出数据 
	if (ret == TRUE)
	{
		LPTSTR pStr = content->GetBuffer(filelen);	
		// 将字节转化为 Unicode 字符串
		MultiByteToWideChar(CP_ACP, 0, pcharbuff, filelen, pStr, filelen);
		content->ReleaseBuffer();
		//AfxMessageBox(_T("读文件成功!"));	
		lRet=0;
	}
	else
	{
		//AfxMessageBox(_T("读文件失败!"));	
		lRet=-4;
	}

	if (pcharbuff != NULL) 
		delete[] pcharbuff;

	CloseHandle(hFile);
	return lRet;
}

void CReduceMeasurementDlg::Split(CString source, CStringArray& dest, CString division)
{
    dest.RemoveAll();
    int pos = 0;
    int pre_pos = 0;
    while( -1 != pos ){
        pre_pos = pos;
        pos = source.Find(division,(pos));
		if(pos<0)
			dest.Add(source.Mid(pre_pos));
		else
		{
			dest.Add(source.Mid(pre_pos,(pos-pre_pos)));
			pos++;
		}
		
    }
}

CString CReduceMeasurementDlg::ReturnPath(void)
{
	  CString    sPath;   
	  GetModuleFileName(NULL,sPath.GetBufferSetLength(MAX_PATH+1),MAX_PATH);   
	  sPath.ReleaseBuffer();   
	  int    nPos;   
	  nPos=sPath.ReverseFind('\\');   
	  sPath=sPath.Left(nPos+1);   
	  return    sPath;   
}

void CReduceMeasurementDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	API_GPIO_Output(0, 0);
	API_GPIO_Output(1, 0);
	API_GPIO_Output(2, 0);
	API_GPIO_Output(3, 0);

//绘图完成后的清理
MemBitmap.DeleteObject(); 
MemDC.SelectObject(pOldBit);
MemDC.DeleteDC();

	//KillTimer(1);

	//关闭串口
	if (m_pSerial != NULL)
	{
		//关闭串口
		m_pSerial->ClosePort();

		//释放串口对象
		delete m_pSerial;
		m_pSerial = NULL;
		//AfxMessageBox(L"串口关闭成功");
	}
	if (WaitForSingleObject(m_hThread,400) == WAIT_TIMEOUT)
	{
		TerminateThread(m_hThread,0);
	}
	m_hThread = NULL;
}

double CReduceMeasurementDlg::GetMaxValue(CArray <double> *data, int dstart, int dfinish)
{
	int i,j;// 循环变量  
    double dTemp;  
	if(dstart<0||dfinish<0) return 0.0;
	int dlen=(int)data->GetSize();
	double *sdata=new double(dfinish-dstart);
	if(dfinish==-1) dfinish=dlen;
	for(i=0;i<dfinish-dstart;i++)
	{
		sdata[i]=data->GetAt(dstart+i);
		//TRACE(_T("%f"),sdata[i]);
	}
	
    //TRACE(_T("\n"));
      
    // 用冒泡法对数组进行排序  
    for (j = 0; j < dfinish-dstart ; j ++)  
    {  
        for (i = 0; i < dfinish-dstart - j; i ++)  
        {  
			if (sdata[i] < sdata[i+1])  
            {  
                // 互换  
				dTemp = sdata[i];  
				sdata[i]=sdata[i+1];
				sdata[i+1]=dTemp;

				//for(int p=0;p<dfinish-dstart;p++)
				//{
				//	TRACE(_T("%f"),sdata[p]);
				//}
				//TRACE(_T("\n"));

            }  
        }  
    } 
	dTemp=sdata[0];
	delete []sdata;
	return dTemp;
}

//int CReduceMeasurementDlg::GetStandardPos(CArray <double> * data, int * start, int *middle,int * finish)
int CReduceMeasurementDlg::GetStandardPos(CArray <double> * data, int * start,int *  finish)
{
	int i,j,n=0;// 循环变量  
	int sPos[10];
	int sCount=0;
	int rstart=0,rfinish=0;
	int qstart=0,qfinish=0;
	int tstart=0,tfinish=0;
    double dValue,*dTemp;
	int dlen=(int)data->GetSize();
	dTemp=new double[dlen];
	//dValue=new double(dlen);
	for(i=0;i<dlen-1;i++)
	{
		dTemp[i]=data->GetAt(i);
	}
	//取得图形终点
	for(i=dlen-4;i>=3;i--)
	{
		if(dTemp[i-2]>dTemp[i-1]&&(dTemp[i-1]-dTemp[i])>0.1&&(dTemp[i+1]-dTemp[i])>0.1&&dTemp[i+1]<dTemp[i+2])
		{
				rfinish=i;
				break;
		}
	}
    *finish =rfinish;

	//取得图形起点
	for(i=3;i<rfinish-4;i++)
	{
		//if(fabs(dTemp[i-2]-(-327.67))<0.001 &&fabs(dTemp[i-1]-(-327.67))<0.001 &&fabs(dTemp[i]-(-327.67))<0.001&&fabs(dTemp[i+1]-(-327.67))<0.001&&fabs(dTemp[i+2]-(-327.67))<0.001)
		if(dTemp[i-2]<dTemp[i-1]&&dTemp[i-1]<dTemp[i]&&fabs(dTemp[i+1]-dTemp[i])>2.0&&dTemp[i+1]<dTemp[i+2])
		{
			rstart=i+3;
			break;
		}
	}

	if(rstart==0)
	{
		for(i=3;i<rfinish-4;i++)
		{
			//if(fabs(dTemp[i-2]-(-327.67))<0.001 &&fabs(dTemp[i-1]-(-327.67))<0.001 &&fabs(dTemp[i]-(-327.67))<0.001&&fabs(dTemp[i+1]-(-327.67))<0.001&&fabs(dTemp[i+2]-(-327.67))<0.001)
			if(dTemp[i-2]==(-327.67) &&dTemp[i-1]==(-327.67) &&dTemp[i]==(-327.67)&&dTemp[i+1]==(-327.67)&&dTemp[i+2]==(-327.67))
			{
				rstart=i+3;
			}
			else if(dTemp[i-2]==0 &&dTemp[i-1]==0 &&dTemp[i]==0&&dTemp[i+1]==0&&dTemp[i+2]==0)
			{
				rstart=i+3;
			}
		}
	}
	*start=rstart;

	dValue=0;

	//CString content;
	//for(int i=rstart;i<rfinish;i++)
	//{
	//content.Format(_T("%.3f\r\n"), m_Array.GetAt(i));
	// 
	//FileWrite(ReturnPath() +_T("2015[1].txt"),content);
	//}
	//
	//double minVal=data->GetAt(rstart);
	//double maxVal=data->GetAt(rstart);
	//for(i=rstart;i<rfinish;i++)
	//{
	//	if(minVal>data->GetAt(i)) minVal=data->GetAt(i);
	//	if(maxVal<data->GetAt(i)) maxVal=data->GetAt(i);
	//}
	//
	//dTemp[rfinish-1]=minVal;

	//for(i=rstart;i<rfinish;i++)
	//{
	//}

	/*for(i=dlen-2;i>=1;i--)
	{
		dTemp[i]=(data->GetAt(i+1)-data->GetAt(i-1))/2.0;
		TRACE(_T("%f "),dTemp[i]);
	}*/
	
	//for(i=dlen-4;i>=3;i--)
	//{
	//	dValue[i]=(dTemp[i-2]+dTemp[i-1]+dTemp[i]+dTemp[i+1]+dTemp[i+2])/5;
	//	TRACE(_T("%f "),dTemp[i]);
	//}
	
	
	TRACE(_T("\n"));
	//for(i=dlen-4;i>=3;i--)
	sCount=0;
	for(i=rfinish-4;i>=rstart+4;i--)
	{
		if(dTemp[i-2]-dTemp[i-1]>0.015&&dTemp[i-1]-dTemp[i]>0.015&&dTemp[i+1]-dTemp[i]>0.015&&dTemp[i+2]-dTemp[i+1]>0.015)
		//if(dTemp[i-2]>dTemp[i-1]&&dTemp[i-1]>dTemp[i]&&dTemp[i]<dTemp[i+1]&&dTemp[i+1]<dTemp[i+2])
		//if(fabs(dTemp[i]-dTemp[i-1])>0.05)
		{
			TRACE(_T("%d "),i);
			sPos[sCount]=i;
			sCount++;
		}
	}
	switch(sCount)
	{
	case 1:
		qstart=rstart;
		qfinish=sPos[0];
		tstart=qfinish+1;
		tfinish=rfinish;
		break;
	case 2:
		
		break;
	case 3:

		break;
	}

		/*else if(n==2)
		{
			if(dTemp[i-2]<dTemp[i-1]&&(dTemp[i-1]-dTemp[i])<0.1&&dTemp[i+1]-dTemp[i]<0.1&&dTemp[i+1]>dTemp[i+2])
			{
				*start=i;
				TRACE(_T("%d "),i);
				n=3;
			}
		}*/
		//dTemp=data->GetAt(i);
		//if(dTemp!=-327.67)
		//{
		//	if(dTemp<dValue) dValue=dTemp					
		//}
		//dTemp=(data->GetAt(i)-data->GetAt(i-2))/2.0;
		

	
	 delete []dTemp; 
	return 0;
}

void CReduceMeasurementDlg::OnBnClickedCheck1()
{
	// TODO: 在此添加控件通知处理程序代码
	if(BST_CHECKED==((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck()) 
	{
		CW=1;
	}
	else
	{
		CW=0;
	}
}

void CReduceMeasurementDlg::OnBnClickedCheck2()
{
	// TODO: 在此添加控件通知处理程序代码
	if(BST_CHECKED==((CButton*)GetDlgItem(IDC_CHECK2))->GetCheck()) 
	{
		theApp.AppStoragePath =ReturnPath();
	}
	else
	{
		theApp.AppStoragePath =_T("\\硬盘\\");
	}
	
}
/*
void CReduceMeasurementDlg::OnBnClickedButLeft()
{
	// TODO: 在此添加控件通知处理程序代码
	API_GPIO_Output(0,1);
	Sleep(1);
	API_GPIO_Output(1,0);
	SetTimer(3,400,NULL);
}

void CReduceMeasurementDlg::OnBnClickedButRight()
{
	// TODO: 在此添加控件通知处理程序代码
	API_GPIO_Output(0,0);
	Sleep(1);
	API_GPIO_Output(1,1);
	SetTimer(3,400,NULL);
}
*/
BOOL CReduceMeasurementDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	int buID;
	buID= GetWindowLong(pMsg->hwnd,GWL_ID);//由窗口句柄获得ID号，GetWindowLong为获得窗口的ID号。
	//CWnd* pWnd=WindowFromPoint(pMsg->pt); //获得指定点句柄
	//buID=pWnd->GetDlgCtrlID();//获得该句柄的ID号。
	if(pMsg->message==WM_LBUTTONDOWN) 
	{     
		if(buID==IDC_BUT_LEFT) //按下
		{  
			//在这里添加单击按下事件的程序
			API_GPIO_Output(0,1);
			Sleep(1);
			API_GPIO_Output(1,0);
		}
		else if(buID==IDC_BUT_RIGHT)
		{
			API_GPIO_Output(0,0);
			Sleep(1);
			API_GPIO_Output(1,1);
		}
	}
	if(pMsg->message==WM_LBUTTONUP) 
	{ 
		if(buID==IDC_BUT_LEFT)
		{  
			//在这里添加单击松开事件的程序
			API_GPIO_Output(0,0);
			Sleep(1);
			API_GPIO_Output(1,0);
		} 
		else if(buID==IDC_BUT_RIGHT)
		{
			API_GPIO_Output(0,0);
			Sleep(1);
			API_GPIO_Output(1,0);
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CReduceMeasurementDlg::OnBnClickedButOpen()
{
	// TODO: 在此添加控件通知处理程序代码
	SipShowIM(SIPF_OFF); 

	CString FileFlt = _T("data file(*.txt)|*.txt");
    FileFlt += _T("||");
    CFileDialog FileDlg(TRUE,NULL,NULL,OFN_HIDEREADONLY,FileFlt,this,0);
    //FileDlg.m_ofn.lpstrInitialDir =L"\\ResidentFlash2\\GUI";//ReturnPath();
    FileDlg.m_ofn.lpstrTitle = L"选择数据...";
    INT_PTR nResponse = FileDlg.DoModal();
    if(nResponse == IDCANCEL)
        return;
	//filePath = FileDlg.GetPathName();   // filePath即为所打开的文件的路径  
	//CString strFileName=GetOpenFileName(_T("数据文件,*.txt"));

	double val,vTmp;
	CString content;
	CStringArray dest;
	int Count;
	wchar_t   *stopstring;

	CString strFilePath=FileDlg.GetPathName();
	CString strFileName=FileDlg.GetFileName(); 
	content.Empty();
	dest.RemoveAll();

	m_Array.RemoveAll();  //删除所有元素
	//CString strFileName=_T("2015_1_4_CN-6118AB.txt");
	//FileRead(ReturnPath() +strFileName,&content);
	FileRead(strFilePath,&content);
	//Split(theApp.AppParameterContent,souce,_T("}"));
	Split(content,dest,_T("\n"));
	Count = dest.GetSize();
	for(int i=0;i<Count-1;i++)
	{
		val=wcstod(dest[i],&stopstring);
		if(i>0)
		{
			val=(val==-327.67?vTmp:val);
		}
		vTmp=val;
		m_Array.Add(val);
	}
	
	int first,last;
	//strFileName.MakeLower();
	first=strFileName.Find(_T('_'))+1; 
	if(first>0)
	{
		last=strFileName.Find(_T('_'),first); 

		CString sMeasurementType = strFileName.Mid(first,last-first);
		
		first=strFileName.Find(_T("_"),last)+1; 
		last=strFileName.Find(_T('_'),first); 
		CString sMeasurementPos = strFileName.Mid(first,last-first);
		
		first=strFileName.Find(_T("_"),last)+1; 
		last=strFileName.Find(_T('.'),first); 
		CString sFigureNumber = strFileName.Mid(first,last-first);
		int lMeasurementTypeIndex=wcstol(sMeasurementType,&stopstring,0);
		int lMeasurementPosIndex=wcstol(sMeasurementPos,&stopstring,0);

		CStringArray soucest,destst,deststr;
		soucest.RemoveAll();
		destst.RemoveAll();
		deststr.RemoveAll();

		Split(theApp.AppStandardContent,soucest,_T("}"));
		int Num=soucest.GetSize();
		int nNum;
		for(int m=0;m<Num-1;m++)
		{
			if(soucest[m].Find(sFigureNumber)>0)
			{
				Split(soucest[m],destst,_T(";"));
				Split(destst[lMeasurementTypeIndex+1],deststr,_T(","));
				theApp.AppStandardVal=deststr[lMeasurementPosIndex*2+1];
				break;
			}
		}

		CStringArray souce,dest;
		CStringArray possouce,posmiddle,posdest;
		souce.RemoveAll();
		dest.RemoveAll();
		possouce.RemoveAll();
		posmiddle.RemoveAll();
		posdest.RemoveAll();

		Split(theApp.AppPositionContent,possouce,_T("}"));
		CString strTemp;
		CString strValue;
		int Count = possouce.GetSize();
		int nCount;
		for(int i=0;i<Count;i++)
		{	
			if(possouce[i].Find(sFigureNumber)>0)
			{
				Split(possouce[i],posmiddle,_T(";"));
				Split(posmiddle[lMeasurementTypeIndex+1],posdest,_T(","));
				TRACE(_T("%s\n"),posmiddle[theApp.AppMeasurementTypeIndex+1]);
				nCount = posdest.GetSize();
	
				wchar_t   *stopstring;
				theApp.AppFirstSectionStart=(int)wcstol(posdest[lMeasurementPosIndex*4],&stopstring,10);
				theApp.AppFirstSectionFinish=(int)wcstol(posdest[lMeasurementPosIndex*4+1],&stopstring,10);
				theApp.AppSecondSectionStart=(int)wcstol(posdest[lMeasurementPosIndex*4+2],&stopstring,10);
				theApp.AppSecondSectionFinish=(int)wcstol(posdest[lMeasurementPosIndex*4+3],&stopstring,10);
				break;			
			}
		}

		if(theApp.AppFirstSectionStart!=0&&theApp.AppSecondSectionFinish!=0)
		{
			CString str;
			int s1,s2,s3,s4;
			double v1,v2,v3,v4;
			//GetStandardPos(&m_Array,theApp.AppFirstSectionStart,theApp.AppSecondSectionFinish,&s1,&s2,&v1,&v2);
			//GetStandardPos(&m_Array,theApp.AppFirstSectionFinish,theApp.AppSecondSectionFinish,&s3,&s4,&s1,&s2,&v1,&v2);
			GetStandardPos(&m_Array,theApp.AppFirstSectionStart,theApp.AppFirstSectionFinish ,theApp.AppSecondSectionStart,theApp.AppSecondSectionFinish,&s1,&s2,&v1,&v2);
			theApp.AppFirstSectionPosition=s1;
			theApp.AppSecondSectionPosition=s2;
			theApp.AppPosStart= theApp.AppFirstSectionStart ;
			theApp.AppPosFinish=theApp.AppSecondSectionFinish ; 
			//theApp.AppPosStart= s3;
			//theApp.AppPosFinish=s4; 

			//v3=fabs(v2-v1);
			v3=fabs(v2-v1)*cos(theApp.AppAngleVal*pi/180.0);
			wchar_t   *stopstring;
			v4=wcstod(theApp.AppStandardVal,&stopstring);
			theApp.AppMeasurementVal.Format(L"%.3f",v3);
			theApp.AppDifferenceVal.Format(L"%.3f",v3-v4);

			//str.Format(L"标准值：%s\n测量值：%s\n第一点：%d\n第二点：%d\n第一点值：%.3f\n第二点值：%.3f\n采集数量：%d\n发送数量：%d\n",theApp.AppStandardVal,theApp.AppMeasurementVal,s1,s2,v1,v2,FlagCount,m_SendCout);
			str.Format(L"标准值：%s\n测量值：%s\n测量差：%s\n第一点：%d\n第二点：%d\n第一点值：%.3f\n第二点值：%.3f\n采集数量：%d\n发送数量：%d\n",theApp.AppStandardVal,theApp.AppMeasurementVal,theApp.AppDifferenceVal,s1,s2,v1,v2,FlagCount,m_SendCout);
			GetDlgItem(IDC_STC_CONTENT)->SetWindowTextW(str);
			TRACE(_T("%s"),str);
			Invalidate();
		}

		//CStringArray soucest;
		//CStringArray destst;
		//CString str;
		//int pos=0;
		//Split(theApp.AppPositionContent,souce,_T("}"));
		//Split(theApp.AppStandardContent,soucest,_T("}"));
		//CString strTemp;
		//CString strValue;
		//int Count = souce.GetSize();
		//int nCount;
		//int Num=soucest.GetSize();
		//int nNum;
		//for(int i=0;i<Count;i++)
		//{
		//	Split(souce[i],dest,_T(","));
		//}
	}

}

BOOL CReduceMeasurementDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	/*
	// TODO: 在此添加专用代码和/或调用基类
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
	*/
	return CDialog::OnCommand(wParam, lParam);
}
