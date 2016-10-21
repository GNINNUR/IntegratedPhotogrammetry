// PolygonFilling.cpp: implementation of the CPolygonFilling class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "lidBasLib.h"
#include "PolygonFilling.h"
#include "math.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define eps 1.0e-5

CPolygonFilling::CPolygonFilling(POINT2D *polygon, int VertexNum)
{
	m_polygon=polygon;
	m_VertexNum=VertexNum;
	CalBoundingBox();
}

CPolygonFilling::~CPolygonFilling()
{
	m_polygon=NULL;
}

void CPolygonFilling::SetPolygon(POINT2D *polygon, int VertexNum)
{
	m_polygon=polygon;
	m_VertexNum=VertexNum;
	CalBoundingBox();
}

//��������
static int compareCoordinate( const void *mt1, const void *mt2 )
{
	double t = (double *)mt1 - (double *)mt2;
	if( t < 0 )
		return -1;
	else if( t > 0 )
		return 1;
	return 0;
}

//����ָ����y����������		�ܹ�����͹�򰼶����
//		y0			����е�y����
//		pLine		�ڲ�����ռ䣬ɨ���зֶ�
//		num			ɨ������
//		return		true/false		��/���� ���
bool CPolygonFilling::XAxisFilling(double y0, ScanLine **pLine, int &num)
{
	bool	bReturn=true;
	int	i, j;
	double	*pIntersect=NULL;
	int		IntersectNum;

	if(y0<m_Ymin || y0>m_Ymax)
	{
		bReturn = false;
		goto	ERR_END;
	}
	if((pIntersect=new double[m_VertexNum])==NULL)
	{
		bReturn=false;
		goto	ERR_END;
	}
	IntersectNum=0;
	for(i=0; i<m_VertexNum; i++)
	{
		j=(i+1)%m_VertexNum;
		if((y0-m_polygon[i].y)*(y0-m_polygon[j].y)<=0)
		{//�н���
			if((m_polygon[i].y-m_polygon[j].y)==0)
			{//��������
				pIntersect[IntersectNum++]=m_polygon[i].x;
				pIntersect[IntersectNum++]=m_polygon[j].x;
			}
			else
			{//һ������
				pIntersect[IntersectNum++]=((m_polygon[i].x-m_polygon[j].x)*(y0-m_polygon[i].y)
											+(m_polygon[i].y-m_polygon[j].y)*m_polygon[i].x)
											/(m_polygon[i].y-m_polygon[j].y);
			}
		}
	}
	if(IntersectNum==0)
	{
		bReturn=false;
		num=0;
		goto	ERR_END;
	}
	//��������
	//��X������������
	qsort(pIntersect, IntersectNum, sizeof(double), compareCoordinate);

	if(IntersectNum%2==1)
	{
		TRACE("����������!\n");
		bReturn=false;
		goto	ERR_END;
	}

	num=IntersectNum/2;
	if((*pLine=new ScanLine[num])==NULL)
	{
		bReturn=false;
		goto	ERR_END;
	}
	
	for(i=0, j=0; i<IntersectNum; i++)
	{
		(*pLine)[j].start=pIntersect[i++];
		(*pLine)[j].end=pIntersect[i];
	}
	

ERR_END:
	if(pIntersect)
	{
		delete[] pIntersect;
		pIntersect=NULL;
	}
	return bReturn;
}


//����ָ����y����������		����
//		y0			����е�y����
//		pLine		ɨ���зֶ�
//		return		true/false		��/���� ���
bool CPolygonFilling::XAxisFillingRect(double y0, ScanLine *pLine)
{
	bool	bReturn=true;
	int	i, j;
	double	pIntersect[4];
	int		IntersectNum;
	
	if(y0<m_Ymin || y0>m_Ymax)
	{
		bReturn = false;
		goto	ERR_END;
	}
	
	IntersectNum=0;
	for(i=0; i<m_VertexNum; i++)
	{
		j=(i+1)%m_VertexNum;
		if((y0-m_polygon[i].y)*(y0-m_polygon[j].y)<=0)
		{//�н���
			if((m_polygon[i].y-m_polygon[j].y)==0)
			{//��������
				pIntersect[IntersectNum++]=m_polygon[i].x;
				pIntersect[IntersectNum++]=m_polygon[j].x;
			}
			else
			{//һ������
				pIntersect[IntersectNum++]=((m_polygon[i].x-m_polygon[j].x)*(y0-m_polygon[i].y)
					+(m_polygon[i].y-m_polygon[j].y)*m_polygon[i].x)
					/(m_polygon[i].y-m_polygon[j].y);
			}
		}
	}
	if(IntersectNum==0)
	{
		bReturn=false;
		goto	ERR_END;
	}
	//��������
	//��X������������

	if(IntersectNum!=2)
	{
		TRACE("����������!\n");
		bReturn=false;
		goto	ERR_END;
	}
	
	if(pIntersect[0]<=pIntersect[1])
	{
		pLine->start=pIntersect[0];
		pLine->end=pIntersect[1];
	}
	else
	{
		pLine->start=pIntersect[1];
		pLine->end=pIntersect[0];
	}
	
	
ERR_END:
	
	return bReturn;
}

void CPolygonFilling::CalBoundingBox()
{
	if(m_polygon==NULL)
		return;

	m_Xmax=-1.7e+308;
	m_Ymax=-1.7e+308;
	m_Xmin=1.7e+308;
	m_Ymin=1.7e+308;

	for(int i=0; i<m_VertexNum; i++)
	{
		if(m_Xmax<m_polygon[i].x)
			m_Xmax=m_polygon[i].x;
		if(m_Xmin>m_polygon[i].x)
			m_Xmin=m_polygon[i].x;
		if(m_Ymax<m_polygon[i].y)
			m_Ymax=m_polygon[i].y;
		if(m_Ymin>m_polygon[i].y)
			m_Ymin=m_polygon[i].y;
	}
}

void CPolygonFilling::GetPolyBoundary(double &XMax, double &XMin, double &YMax, double &YMin)
{
	XMax=m_Xmax;
	XMin=m_Xmin;
	YMax=m_Ymax;
	YMin=m_Ymin;
}

// void CPolygonFilling::GetPolyBoundary(D_RECT &rect)
// {
// 	rect.XMax=m_Xmax;
// 	rect.XMin=m_Xmin;
// 	rect.YMax=m_Ymax;
// 	rect.YMin=m_Ymin;
// }

bool CPolygonFilling::PointInPolygen(double x, double y)
{
	int  counter;
	int j;
	//    *pIn=0;
//	POINT2D *p1;
	POINT2D *p2, *p3;
	
	
//	p1=&(point[i]);
	
	counter=0;
	p2=&(m_polygon[0]);
	for(j=1; j<=m_VertexNum; j++)
	{
		
		p3=&(m_polygon[j%m_VertexNum]);
		
		if(Insert(x,y,p2->x,p2->y,p3->x,p3->y)) 
			counter++;
		p2=p3;
	}
	
	if(counter%2==1)
	{
		return	true;
	}
	else
	{
		return	false;
	}
}

///�ж���p1Ϊ���㣬y=p1.y���������߶�p2p3��û�н���
bool CPolygonFilling::Insert(double x1,double y1,double x2,double y2,double x3,double y3)
{
	double xinters;
	bool inserts;
	
	inserts=false;
	if(Online(x1,y1,x2,y2,x3,y3))  //�ж�p1,p2,p3�����Ƿ���(p1��p2��p3֮��)
		inserts = true;
	
	if(y1>min(y2,y3))
	{
		if(y1<=max(y2,y3)) 
		{
			if(x1<=max(x2,x3))
			{
				if(y2!=y3) 
				{
					xinters=(y1-y2)*(x3-x2)/(y3-y2)+x2;//����y=p1.y��ֱ�����߶�p2p3�Ľ��������
					if(fabs(x2-x3)<eps || x1<=xinters)
						inserts=true;
				}
			}
		}
	}
	return	inserts;
}

////�ж�p2,p1,p3�����Ƿ���
bool CPolygonFilling::Online(double x1,double y1,double x2,double y2,double x3,double y3)
{
	
	if(x1>=min(x2,x3) && x1<=max(x2,x3) && y1>=min(y2,y3) && y1<=max(y2,y3))
	{
		if(fabs((x2-x1)*(y3-y1)-(x3-x1)*(y2-y1))<=eps)
            return true;
    }
    return false;
}