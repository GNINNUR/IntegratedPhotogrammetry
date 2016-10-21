// PolygonFilling.h: interface for the CPolygonFilling class.
//	Create in 2008.11.29	����������
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_POLYGONFILLING_H__C8ECBF7D_6164_4A11_83FF_37A89598508F__INCLUDED_)
#define AFX_POLYGONFILLING_H__C8ECBF7D_6164_4A11_83FF_37A89598508F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "lidBase.h"
//#include "BaseDataType.h "
#include "PointBaseDefinitons.h"

//ɨ����X��Y����
struct	ScanLine
{
	double  start;	//���
	double	end;	//�յ�
};

// struct POINT2D	{
// 	double x, y;
// };

class _BaseLib_ CPolygonFilling  
{
public:
	CPolygonFilling(POINT2D *polygon=NULL, int VertexNum=0);
	virtual ~CPolygonFilling();

	//������Ҫ���Ķ���Σ���֤����˳������
	void	SetPolygon(POINT2D *polygon, int VertexNum);
	//��X��������		2008.12.5  debug unsolved, (��������)
	bool	XAxisFilling(double	y0, ScanLine **pLine, int &num);
	//��X�������Σ�������Ƭ���򻯼���  ���뱣֤���õĶ����Ϊ����
	bool	XAxisFillingRect(double	y0, ScanLine *pLine);
	
	void	GetPolyBoundary(double &XMax, double &XMin, double &YMax, double &YMin);
//	void	GetPolyBoundary(D_RECT &rect);
	
	bool	PointInPolygen(double x, double y);

private:
	void	CalBoundingBox();
	bool	Insert(double x1,double y1,double x2,double y2,double x3,double y3);
	bool	Online(double x1,double y1,double x2,double y2,double x3,double y3);

private:
	POINT2D		*m_polygon;
	int			m_VertexNum;
	//����ΰ�Χ��
	double		m_Xmax;
	double		m_Xmin;
	double		m_Ymax;
	double		m_Ymin;
};

#endif // !defined(AFX_POLYGONFILLING_H__C8ECBF7D_6164_4A11_83FF_37A89598508F__INCLUDED_)
