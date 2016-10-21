#ifndef  _Strip_Overlap_H_ZJ_2010_08_24_
#define _Strip_Overlap_H_ZJ_2010_08_24_

//////////////////////////////////////////
//����LiDAR�����ص���  zj
//////////////////////////////////////////////////////////////////////////

#include "lastools\lasreader.h"
#include "Geometry.h"
#include "AFXTEMPL.H"
#include "..\LidCalib\Trj_Def.h"
#include "LidBaseDef.h"
#include "MemoryPool.h"


#ifdef _LIDQC_
#define  _lidQC_Dll_  __declspec(dllexport)
#else
#define  _lidQC_Dll_  __declspec(dllimport)	
#endif

#ifndef _LIDQC_
#ifdef _DEBUG
#pragma comment(lib,"LidQCD.lib")
#else
#pragma comment(lib,"LidQC.lib")
#endif
#endif

//��άHull�ı߽�� (��άҲ�ɲ������ƵĴ�����)
struct Hull2D_Block
{
	int row;
	int col;
	bool *pBoundary;

	Hull2D_Block()
	{
		row=0;
		col=0;
		pBoundary=0;
	};

	~Hull2D_Block()
	{
		row=0;
		col=0;
		if(pBoundary)	delete pBoundary;
	}
};

struct Overlap_Block
{
	int row;
	int col;
	double x0;
	double y0;
	double dx;
	double dy;
	BYTE *pOverlap;

	Overlap_Block()
	{
		pOverlap=0;
	}
	~Overlap_Block()
	{
		if(pOverlap)
			delete pOverlap;

		pOverlap=0;
	}
};

#define NULL_Flag		 0x00	//00000000b
#define ProcData_Flag    0x01	//00000001b
#define RefData_Flag	 0x02	//00000010b
#define OverlapData_Flag 0x03	//00000011b

//�������͹��, ������Ҫ��ȫ�����ݵ����ڴ棬�ڴ��������ʱ��ʧ��
bool _lidQC_Dll_ CalLasBoundary(LASreader* lasreader, float concavity, CArray<POINT3D, POINT3D> *pBoundary);	

//���������ĵ����ڲο������е��ص���
bool _lidQC_Dll_ GetOverlapData(LASreader* pProcDataset, LASreader* pRefDataset, CArray<LASpointXYZI, LASpointXYZI> *pOverlapData);


//�������߲ü�,��Ҫ���й켣����,�ü����ֱ���޸ķ����
void _lidQC_Dll_ OverlapClip_Centreline(char *pszLasFile1, char *pszLasFile2, char *pszTrjFile1, char *pszTrjFile2);

//�ü��켣����,��������Ƶ�ʱ���¼�ü�
bool _lidQC_Dll_ CutTrajectory(char *pszLasFile, char *pszTrjFile, TrajPos **pTrjBuf, int &num);

//�жϵ��Ƿ��ڶ������
bool IsPtInPolygon(double Pnt[3], CArray<POINT3D, POINT3D> *pBoundary);


//�����ص������
//pOverlapErr:  x, y:ƽ������, z:�߳����, i:ǿ�����
//type:  0 �̲߳� / 1 ǿ�Ȳ�
void _lidQC_Dll_ QC_CalOverlapError(char* pProcFile, char* pRefFile, CMemoryPool<POINT3D, POINT3D&> *pOverlapErr, int type=0);

void _lidQC_Dll_ QC_CalOverlapError(char* pProcFile, char* pRefFile, char* pDiffFile, int type=0);


//����λ�ڵ��Ʊ߽�����ݿ�
void CalPointsBoundaryBlock(LASreader *pProcDataset, /*D_RECT bkRect, */double bkSize, Hull2D_Block *pHullBlock);

//���������������Ƶ��ص���
void _lidQC_Dll_ CalOverlapBlock(LASreader *pProcDataset, LASreader* pRefDataset, double bkSize, Overlap_Block *pOBlock);

//ɨ����
//void scanLine(int *pMatrix, );

#endif