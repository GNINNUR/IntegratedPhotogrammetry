
// PhotoOrientation.h : PhotoOrientation Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������

#include "orsBase\orsUtil.h"

orsIPlatform *getPlatform();

// CPhotoOrientationApp:
// �йش����ʵ�֣������ PhotoOrientation.cpp
//

class CPhotoOrientationApp : public CWinAppEx
{
public:
	CPhotoOrientationApp();
	~CPhotoOrientationApp();


// ��д
public:
	virtual BOOL InitInstance();

// ʵ��
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CPhotoOrientationApp theApp;
