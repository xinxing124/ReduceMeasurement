// ReduceMessageDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ReduceMeasurement.h"
#include "ReduceMessageDlg.h"


// CReduceMessageDlg 对话框

IMPLEMENT_DYNAMIC(CReduceMessageDlg, CDialog)

CReduceMessageDlg::CReduceMessageDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CReduceMessageDlg::IDD, pParent)
		, m_Temp(_T(""))
{

}

CReduceMessageDlg::~CReduceMessageDlg()
{
}

void CReduceMessageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CReduceMessageDlg, CDialog)
#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
	ON_WM_SIZE()
#endif
	ON_BN_CLICKED(IDOK, &CReduceMessageDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CReduceMessageDlg::OnBnClickedCancel)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_CBN_SELCHANGE(IDC_COMBO2, &CReduceMessageDlg::OnCbnSelchangeCombo2)
	ON_CBN_DROPDOWN(IDC_COMBO2, &CReduceMessageDlg::OnCbnDropdownCombo2)
	ON_CBN_SELCHANGE(IDC_COMBO3, &CReduceMessageDlg::OnCbnSelchangeCombo3)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CReduceMessageDlg::OnCbnSelchangeCombo1)
	ON_CBN_DROPDOWN(IDC_COMBO3, &CReduceMessageDlg::OnCbnDropdownCombo3)
	ON_CBN_SELCHANGE(IDC_COMBO4, &CReduceMessageDlg::OnCbnSelchangeCombo4)
	ON_CBN_DROPDOWN(IDC_COMBO4, &CReduceMessageDlg::OnCbnDropdownCombo4)
	ON_CBN_CLOSEUP(IDC_COMBO4, &CReduceMessageDlg::OnCbnCloseupCombo4)
END_MESSAGE_MAP()

BOOL CReduceMessageDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
int ret=0;
	CString strTmp;

	
	//ret=FileRead(_T("\\ResidentFlash2\\GUI\\xishu.txt"),&strTmp);

	// TODO:  在此添加额外的初始化
	SYSTEMTIME st;
	CString strDate,strTime;
	GetLocalTime(&st);
	strDate.Format(_T("%4d%02d%02d"),st.wYear,st.wMonth,st.wDay);
	//strTime.Format("%2d:%2d:2d", st.wHour,st.wMinute,st.wSecond);
	/*
	//if(::GetFileAttributes(_T("\\ResidentFlash2\\GUI\\") +strDate +_T(".txt")) == 0xFFFFFFFF){
	if(::GetFileAttributes(_T("\\硬盘\\") +strDate +_T(".txt")) == 0xFFFFFFFF){
		existfile=false;
		m_Temp=_T("站名;轨枕号;类型;道岔号;测量值\r\n");
	}
	else{
		existfile=true;
		//FileRead(_T("\\ResidentFlash2\\GUI\\") +strDate +_T(".txt"),&m_Temp);
		m_Temp=_T("");
		FileRead(_T("\\硬盘\\") +strDate +_T(".txt"),&m_Temp);
	}
	*/
	/*
	CWnd   *pEdit   =   GetDlgItem(IDC_COMBO1);
	CFont   *pfont   =   pEdit->GetFont();
	LOGFONT   logfont;
	pfont->GetLogFont(&logfont);
	logfont.lfHeight   =   -12;
	m_font.CreateFontIndirect(&logfont);
	pEdit->SetFont(&m_font);
	
	
	GetDlgItem(IDC_STATIC01)->SetFont(&m_font);
	GetDlgItem(IDC_STATIC02)->SetFont(&m_font);
	GetDlgItem(IDC_STATIC03)->SetFont(&m_font);
	GetDlgItem(IDC_STATIC04)->SetFont(&m_font);
	
	GetDlgItem(IDOK)->SetFont(&m_font);
	GetDlgItem(IDCANCEL)->SetFont(&m_font);
	*/
	//IDC_STATIC01

	//theApp.FileContent
	//theApp.FileContent.
	CStringArray souce;
	CStringArray dest;
	Split(theApp.AppParameterContent,souce,_T("}"));
//AfxExtractSubString
	CString strTemp;
	((CComboBox*)GetDlgItem(IDC_COMBO1))->ResetContent();//消除现有所有内容
	

int Count = souce.GetSize();
int nCount;
//MessageBox(dest[0], _T(""), MB_OK);
//strTemp.Format(_T("%d"),nCount);
//MessageBox(strTemp, _T(""), MB_OK);

CString sTmp;
int pos=0;
for(int i=0;i<Count-1;i++)
{

//MessageBox(NULL, souce[i], _T(""), MB_OK);
	Split(souce[i],dest,_T(","));
	nCount = dest.GetSize();
	pos=dest[0].FindOneOf(_T("{"));
	if(pos==-1) pos=1;
	((CComboBox*)GetDlgItem(IDC_COMBO1))->AddString(dest[0].Mid(pos+1));
	//
	if(theApp.AppComboIndex_01!=-1)
	{
		((CComboBox*)GetDlgItem(IDC_COMBO1))->SetCurSel(theApp.AppComboIndex_01);
		OnCbnDropdownCombo2();
		if(theApp.AppComboIndex_02!=-1)
			((CComboBox*)GetDlgItem(IDC_COMBO2))->SetCurSel(theApp.AppComboIndex_02);
		((CComboBox*)GetDlgItem(IDC_COMBO3))->SetCurSel(theApp.AppComboIndex_03);
		OnCbnDropdownCombo4();
		if(theApp.AppComboIndex_04!=-1)
		((CComboBox*)GetDlgItem(IDC_COMBO4))->SetCurSel(theApp.AppComboIndex_04);
	}
	//for(int j=0;j<nCount;j++)
	//{
		//MessageBox(dest[j].Mid(1), _T(""), MB_OK);
	//((CComboBox*)GetDlgItem(IDC_COMBO1))->AddString(dest[j].Mid(1));
	//	((CComboBox*)GetDlgItem(IDC_COMBO1))->AddString(dest[j]);
	//}
//strTemp.Format(_T("%d"),nCount);
//MessageBox(strTemp, _T(""), MB_OK);

	//((CComboBox*)GetDlgItem(IDC_COMBO2))->AddString(souce[0]);

}
//strTemp.Mid(1);

/*
	如： //控件内容初始化
 CString strTemp;
 ((CComboBox*)GetDlgItem(IDC_COMBO1))->ResetContent();//消除现有所有内容
 for(int i=1;i<=100;i++)
 {
  strTemp.Format("%d",i);
  ((CComboBox*)GetDlgItem(IDC_COMBO_CF))->AddString(strTemp);
 }
3，下拉的时候添加
如： CString strTemp;
 int iCount=((CComboBox*)GetDlgItem(IDC_COMBO_CF))->GetCount();//取得目前已经有的行数
 if(iCount<1)//防止重复多次添加
 {
  ((CComboBox*)GetDlgItem(IDC_COMBO_CF))->ResetContent();
  for(int i=1;i<=100;i++)
  {
   strTemp.Format("%d",i);
   ((CComboBox*)GetDlgItem(IDC_COMBO_CF))->AddString(strTemp);
  }
 }
*/
	((CButton*)GetDlgItem(IDC_BUT_SAVE))->EnableWindow(false);
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

// CReduceMessageDlg 消息处理程序

void CReduceMessageDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	SYSTEMTIME st;
	CString strDate,strTime;
	GetLocalTime(&st);
	strDate.Format(_T("%4d%02d%02d"),st.wYear,st.wMonth,st.wDay);
	
	//选择记录
	theApp.AppComboIndex_01=((CComboBox*)GetDlgItem(IDC_COMBO1))->GetCurSel();
	theApp.AppComboIndex_02=((CComboBox*)GetDlgItem(IDC_COMBO2))->GetCurSel();
	theApp.AppComboIndex_03=((CComboBox*)GetDlgItem(IDC_COMBO3))->GetCurSel();
	theApp.AppComboIndex_04=((CComboBox*)GetDlgItem(IDC_COMBO4))->GetCurSel();

	//CString strTemp_01,strTemp_02,strTemp_03,strTemp_04,strTemp_05,strTemp_06;
	//((CComboBox*)GetDlgItem(IDC_COMBO1))->GetWindowTextW(strTemp_01);
	//((CComboBox*)GetDlgItem(IDC_COMBO2))->GetWindowTextW(strTemp_02);
	//((CComboBox*)GetDlgItem(IDC_COMBO3))->GetWindowTextW(strTemp_03);
	//((CComboBox*)GetDlgItem(IDC_COMBO4))->GetWindowTextW(strTemp_04);
	//strTemp_05.Format(_T("%.3f"),theApp.Val);
	//theApp.AppFilePath= _T("\\硬盘\\") +strDate +_T(".txt");
	//theApp.AppFilePath= theApp.AppStoragePath +strDate +_T(".txt");
	
	//if(::GetFileAttributes(theApp.AppFilePath) == 0xFFFFFFFF){	
	//	m_Temp=_T("位置标识\t道岔编号\t测量位置(m)\t标准值(mm)\t测量值(mm)\t差值(mm)\r\n");
	//}
	//else{
	//	m_Temp=_T("");
	//}
	////		位置标识			道岔编号			测量位置			标准值				测量值				差值
	//m_Temp+=strTemp_01+_T("\t")+strTemp_02+_T("\t")+strTemp_03+_T("\t")+strTemp_04+_T("\t")+strTemp_05+_T("\t")+strTemp_06+_T("\r\n");
	////FileWrite(_T("\\ResidentFlash2\\GUI\\") +strDate +_T(".txt"),m_Temp);
	//FileWrite(theApp.AppFilePath,m_Temp);

	CString str;
	//str.Format(L"标准值：%s\n测量值：%s\n",theApp.AppStandardVal,theApp.AppMeasurementVal);
	str.Format(L"标准值：%s\n测量值：0.0\n",theApp.AppStandardVal);
	this->GetParent()->GetDlgItem(IDC_STC_CONTENT)->SetWindowTextW(str);
	OnOK();
}

void CReduceMessageDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}

int CReduceMessageDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	return 0;
}

void CReduceMessageDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
}

void CReduceMessageDlg::OnCbnSelchangeCombo2()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CReduceMessageDlg::OnCbnDropdownCombo2()
{
	// TODO: 在此添加控件通知处理程序代码
		CStringArray souce;
	CStringArray dest;
	Split(theApp.AppParameterContent,souce,_T("}"));

	CString strTemp;
	

int Count = souce.GetSize();
int nCount;

for(int i=0;i<Count;i++)
{
	Split(souce[i],dest,_T(","));
	nCount = dest.GetSize();

	if(i==((CComboBox*)GetDlgItem(IDC_COMBO1))->GetCurSel())
	{
		((CComboBox*)GetDlgItem(IDC_COMBO2))->ResetContent();//消除现有所有内容
		for(int j=1;j<nCount;j+=2)
		{
		//((CComboBox*)GetDlgItem(IDC_COMBO1))->AddString(dest[j].Mid(1));
			((CComboBox*)GetDlgItem(IDC_COMBO2))->AddString(dest[j]);
		}
	}

}

}

//写入文件
int CReduceMessageDlg::FileWrite(CString filename=_T(""),CString content=_T(""))
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
}

int CReduceMessageDlg::FileRead(CString filename,CString* content)
{
	int lRet;
	HANDLE hFile = INVALID_HANDLE_VALUE;	// 文件句柄 
	if(::GetFileAttributes(filename) == 0xFFFFFFFF)
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

void CReduceMessageDlg::Split(CString source, CStringArray& dest, CString division)
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
void CReduceMessageDlg::OnCbnSelchangeCombo3()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CReduceMessageDlg::OnCbnSelchangeCombo1()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CReduceMessageDlg::OnCbnDropdownCombo3()
{
	// TODO: 在此添加控件通知处理程序代码

}

void CReduceMessageDlg::OnCbnSelchangeCombo4()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CReduceMessageDlg::OnCbnDropdownCombo4()
{
	// TODO: 在此添加控件通知处理程序代码
	CStringArray souce;
	CStringArray dest;
	CStringArray soucest;
	CStringArray destst;
	CString str;
	int pos=0;

	try{
	Split(theApp.AppParameterContent,souce,_T("}"));
	Split(theApp.AppStandardContent,soucest,_T("}"));
	CString strTemp;
	CString strValue;
	int Count = souce.GetSize();
	int nCount;
	int Num=soucest.GetSize();
	int nNum;
	for(int i=0;i<Count;i++)
	{
		Split(souce[i],dest,_T(","));
		nCount = dest.GetSize();

		if(i==((CComboBox*)GetDlgItem(IDC_COMBO1))->GetCurSel())
		{
			pos=dest[0].FindOneOf(_T("{"));
			if(pos==-1) pos=1;
			TRACE(_T("%s\n"),dest[0].Mid(pos+1));
			theApp.AppIdentification= dest[0].Mid(pos+1);
			for(int j=0;j<nCount;j++)
			{
				if(j==((CComboBox*)GetDlgItem(IDC_COMBO2))->GetCurSel())
				{
					TRACE(_T("%s\n"),dest[j*2+1]);
					theApp.AppTurnoutNumber= dest[j*2+1];
					for(int k=0;k<2;k++)
					{
						if(k==((CComboBox*)GetDlgItem(IDC_COMBO3))->GetCurSel())
						{
							((CComboBox*)GetDlgItem(IDC_COMBO3))->GetLBText(k,strValue);
							TRACE(_T("%s\n"),strValue);
							theApp.AppMeasurementType=strValue;
							theApp.AppMeasurementTypeIndex=k;
							for(int m=0;m<Num-1;m++)
							{
								TRACE(_T("Souce:%s\n"),soucest[m]);
								TRACE(_T("Dest:%s\n"),dest[j*2+2]);
								theApp.AppFigureNumber=dest[j*2+2];
								if(soucest[m].Find(dest[j*2+2])>0)
								{
									Split(soucest[m],destst,_T(";"));
									Split(destst[k+1],deststr,_T(","));
									nNum = deststr.GetSize();
									((CComboBox*)GetDlgItem(IDC_COMBO4))->ResetContent();//消除现有所有内容
									for(int n=0;n<nNum;n+=2)
									{
										((CComboBox*)GetDlgItem(IDC_COMBO4))->AddString(deststr[n]);

									}

								}
							}
							break;
						}
					}	
				}
			}
		}

	}
	}
	catch(...)
	{

	}
}

void CReduceMessageDlg::OnCbnCloseupCombo4()
{
	int index;

	CStringArray possouce;
	CStringArray possoucest;
	CStringArray possoucett;
	CString str;
	int pos=0;
	int indexPos=0;

	//	CReduceMeasurementApp();
	//CString AppFileContent;
	//CString AppStandardContent;
	//CString AppParameterContent;
	//CString AppIdentification;
	//CString AppTurnoutNumber;
	//CString AppMeasurementPos;
	//CString AppStandardVal;
	//CString AppMeasurementVal;
	//CString AppDifferenceVal;
	try{
	if(((CComboBox*)GetDlgItem(IDC_COMBO4))->GetCurSel()>-1)
	{	index=((CComboBox*)GetDlgItem(IDC_COMBO4))->GetCurSel();
		theApp.AppMeasurementPosIndex=index;
		theApp.AppMeasurementPos=deststr[index*2];
		TRACE(_T("%s\n"),deststr[index*2]);
		theApp.AppStandardVal=deststr[index*2+1];
		TRACE(_T("%s\n"),deststr[index*2+1]);

		Split(theApp.AppPositionContent,possouce,_T("}"));
		CString strTemp;
		CString strValue;
		int Count = possouce.GetSize();
		int nCount;
		for(int i=0;i<Count;i++)
		{	
			TRACE(_T("%s\n"),possouce[i]);
			TRACE(_T("%s\n"),theApp.AppFigureNumber);
			if(possouce[i].Find(theApp.AppFigureNumber)>0)
			{
				Split(possouce[i],possoucest,_T(";"));
				Split(possoucest[theApp.AppMeasurementTypeIndex+1],possoucett,_T(","));
				TRACE(_T("%s\n"),possoucest[theApp.AppMeasurementTypeIndex+1]);
				nCount = possoucett.GetSize();
				if(nCount%4==0)
				{
					for(int j=theApp.AppMeasurementPosIndex*4;j<nCount;j+=4)
					{
						TRACE(_T("#$# %s #$# %s #$# %s #$# %s #$#\n"),possoucett[j],possoucett[j+1],possoucett[j+2],possoucett[j+3]);
						wchar_t   *stopstring;
						theApp.AppFirstSectionStart=(int)wcstol(possoucett[j],&stopstring,10);
						theApp.AppFirstSectionFinish=(int)wcstol(possoucett[j+1],&stopstring,10);
						theApp.AppSecondSectionStart=(int)wcstol(possoucett[j+2],&stopstring,10);
						theApp.AppSecondSectionFinish=(int)wcstol(possoucett[j+3],&stopstring,10);
						break;
					}
				}
				else
				{
					theApp.AppFirstSectionStart=0;
					theApp.AppFirstSectionFinish=0;
					theApp.AppSecondSectionStart=0;
					theApp.AppSecondSectionFinish=0;
				}
			}
		}
	}
	}
	catch(...)
	{

	}
	// TODO: 在此添加控件通知处理程序代码
	TRACE0( "……\n"  );
}
