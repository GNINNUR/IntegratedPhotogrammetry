#ifndef _LID_BASE_FUNCTIONS_
#define _LID_BASE_FUNCTIONS_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/********************************************************************************************************************
//			create in 2008.9.18		by Tony	
//	ͨ�û��������⣺
//			1.�ļ�������ɾ��������
//			2.�ļ��д�����ɾ��
//			3.������������
//			4.���̿ռ��ѯ
********************************************************************************************************************/

#include "PointBaseDefinitons.h"

#ifdef _lidBASELIB_
#define  _BaseLib_  __declspec(dllexport)
#else
#define  _BaseLib_  __declspec(dllimport)	
#endif



typedef struct
{
//	char clib[128];	//���ֿ�Ŀ¼
	char sys[256];	//ϵͳ��Ŀ¼			program Ŀ¼ �����ò���
	char cur[256];	//��ǰ����Ŀ¼		��ǰ���ݴ���Ĺ���Ŀ¼
	char temp[256];	//��ʱ����Ŀ¼��������ʹ�ã�
//	char slib[128];	//ϵͳ��Ŀ¼
}LIDENV;

const	char	SYS_FILE_NAME[] = "sysEnv.txt";		//ϵͳ����������¼

//�ж�ָ���ļ��Ƿ����
bool	_BaseLib_	_IsFileExist(const char *filename);
//ָ��Ŀ¼�Ƿ����
bool	_BaseLib_	_IsDirectoryExist(const char *sCurDir);
//�ж��ļ����Ƿ����
//bool    _BaseLib_	_IsFolderExist(const char *sCurDir);
//ɾ��ָ��Ŀ¼�Լ�Ŀ¼�µ�ȫ���ļ�
bool	_BaseLib_	_DeleteDirectory(char *pszDir);
//ȡ��ǰ����ִ��Ŀ¼
bool	_BaseLib_	_GetProgramDirectory(CString *pszDir);	
//ȡִ�г��򻷾���Ϣ
bool	_BaseLib_	_GetEnv(LIDENV	*pEnv);
//����ִ�г��򻷾�		
//return		true/false		���óɹ�/ʧ�ܻ�ȡ������
bool	_BaseLib_	_SetEnv();
//��ȡָ������ʣ��ռ�
//	lpDirectoryName:  "C:" or "C:\\temp" or "C:\\temp\\"
//	dwFreeDiskSpace:  **MB
bool	_BaseLib_	_GetDiskFreeSpace(LPCTSTR lpDirectoryName, DWORD *dwFreeDiskSpace);
//ȡ��ǰϵͳ��ʾ�ֱ���
void	_BaseLib_	_GetDisplayResolution(int &cx, int &cy);

//��ȫ·���ļ�������ȡ�ļ���(�������ļ���׺)
CString	_BaseLib_   _ExtractFileName(CString strFilePathName);
//��ȡ�ļ���׺
CString	_BaseLib_	_ExtractFileSuffix(CString strFileName);
//��ȫ·������ȡ�����ļ�����������׺��
CString _BaseLib_   _ExtractFileFullName(CString strFilePathName);
//���ļ�������ȡ�ļ�Ŀ¼
CString _BaseLib_   _ExtractFileDir(CString strFilePathName);

//ѡ���ļ��жԻ���	true/false: ѡ��/δѡ��	�Ի������ƣ�ѡ��·���������ھ��
bool	_BaseLib_	_SelectFolderDialog(char* pszDlgTitle, CString *strFolder, HWND hParent=NULL);

//################################  ������ѧ������  ################################################
//�жϵ��Ƿ��ھ��η�Χ��		true/false
bool _BaseLib_		_PointInRect(double x, double y, D_RECT rect);





#ifndef _lidBASELIB_
#ifdef _DEBUG
#pragma comment(lib,"lidBasLibD.lib")
#else
#pragma comment(lib,"lidBasLib.lib")
#endif
#endif

#endif