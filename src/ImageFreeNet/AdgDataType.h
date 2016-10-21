/******************************************************************************
 * $Id: AdgDataType.h, v 1.0 date: 10/23/2006 ������ Exp$
 *
 * Project:  
 * Purpose:  declarations of AdgDataType
 * Author:   ������, wsjws@163.com
 *
 ******************************************************************************
 * Copyright (c) 2006, ������
 *
 *		δ�õ���Ȩ���𿽱�
 *
 ******************************************************************************
 *
 * $Log: AdgDataType.h,v $
 *
 * Revision 0.9  10/5/2007  ������
 *	��װ�仯�벻��Ĳ��ֶ��ļ��ṹ���ڴ�ṹ��������
 *
 * Revision 0.6  10/23/2006  ������
 * ����
 * Revision 0.5  8/23/2006  л����
 * New
 */

#ifndef _ADG_DATA_TYPE_H
#define  _ADG_DATA_TYPE_H

#include "clip.h"
//#include "DPSDataObject.h"
#include "ALMdefine.h"

//#define PI 3.1415926

#ifndef _POINT2D_Def_
#define _POINT2D_Def_

struct POINT2D	{
	double x, y;
};

#endif

struct	extorPARA	{	// *.eop, exterior orientation parameter
	double Xs, Ys, Zs;			//��λ��m
	double phi, omega, kappa;	//��λ������
	
	double R[9];		//ֻ����ת��������ʽ��һ�µ�
	double	GPSTime;
};

struct	inorPARA	{	// *.iop, interior orientation parameter
	double inorImage_a[3];	// pixel to image
	double inorImage_b[3];
	
	double inorPixel_a[3];	// image to pixel
	double inorPixel_b[3];

	double x0, y0, f;
	double	k1, k2, k3;
	double	p1, p2;

};

struct  overlapINFO {
	short imgIndex;		// -1 == NONE
	short overlap;
};

// ���ڱ���pos�ļ�����
struct imagePOS {
	int ImageID;
	double timeMark;
	double Xs,Ys,Zs;
	double phi, omega, kappa;
};


struct CEnvelope
{
public:
	double XMin,YMin,XMax,YMax;
	
	POINT2D MidPoint()
	{ 
		POINT2D pt2d;
		
		pt2d.x =(XMax+XMin)/2;
		pt2d.y = (YMax+YMin)/2;
		
		return pt2d;
	}
	
	double Width(){ return (XMax-XMin);}
	double Height(){ return(YMax-YMin);}
	bool   IsInRect(double x, double y)	{	return (x<XMax && x>XMin && y<YMax && y>YMin );	};
}; 



struct triPOINT;

enum	topoINDEX	{
	tiRIGHT = 0,
	tiTopRIGHT = 1,
	tiTop = 2,
	tiTopLEFT = 3,
	tiLEFT = 4,
	tiBottomLEFT = 5,
	tiBOTTOM = 6,
	tiBottomRIGHT = 7
};


// ���ڱ���Ӱ������
struct photoINFO	{
	CString		fileName;
    overlapINFO nbImages[8];
	extorPARA	extOr;
	inorPARA	intOr;

	triPOINT *tinPt;
	short	marker;		// �������
	BYTE	useless;	//���ܽ��ж��򣬲��ܼ�����������Ӱ��
};


// ������˳�� ?
struct  stripINFO
{
	short	fisrtImgIndex;	// ��һ��Ӱ�������������Ӱ��������˹�ϵȷ�� ��
	short	numOfImages;	// Ӱ����
	short	flyDirAngle;	// ���з����[0,360]
};


//��������Ϣ
struct blockINFO {
	// 
	char tag[40];			// "dps survey block data file: V1.0"

	CString prjName;		//path & file
	CString prjDir;

	short  flyOverlap;			// �����ص���
	short  stripOverlap;		// �����ص���

	float	imgGrdSize;			// Ӱ������С

	vector< stripINFO >	strips;		// �����棬ʵʱ�ؽ� 
	vector< photoINFO > images;		// ֻ����Ӱ����
};


#endif