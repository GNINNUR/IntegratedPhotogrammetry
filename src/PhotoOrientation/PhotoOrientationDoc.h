
// PhotoOrientationDoc.h : CPhotoOrientationDoc ��Ľӿ�
//


#pragma once
#include "IPhBaseDef.h"
#include <vector>
using namespace std;

class CPhotoOrientationDoc : public CDocument
{
protected: // �������л�����
	CPhotoOrientationDoc();
	DECLARE_DYNCREATE(CPhotoOrientationDoc)

// ����
public:
	

private:
	vector <POSData_ALS50 > m_vPosData;
	iphCamera						m_camera;

	vector<string> m_ImgNameList;	
	vector<string> m_keyNameList;

// ����
public:

// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// ʵ��
public:
	virtual ~CPhotoOrientationDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnImportrawpos();
	afx_msg void OnExportiop();
	afx_msg void OnExportaop();
	afx_msg void OnUpdateExportiop(CCmdUI *pCmdUI);
	afx_msg void OnUpdateExportaop(CCmdUI *pCmdUI);
	afx_msg void OnTplane();
	afx_msg void OnRoughorientation();
};


