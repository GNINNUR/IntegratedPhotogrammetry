#ifndef _CIRCLE_DETECTION_H_ZJ_2010_08_11_
#define _CIRCLE_DETECTION_H_ZJ_2010_08_11_

/*************************************************************************
//�Ӹ������ݵ��м���Բ��λ��  zj
//  1.ˮƽԲ�ļ��			_2d
//  2.б����Բ�ļ��	ƽ��ģ�ͻ�û�и����
**************************************************************************/

#ifdef _LIDCALIB_
#define  _lidCalib_Dll_  __declspec(dllexport)
#else
#define  _lidCalib_Dll_  __declspec(dllimport)	
#endif

// #ifndef _LIDCALIB_
// #ifdef _DEBUG
// #pragma comment(lib,"LidCalib.lib")
// #else
// #pragma comment(lib,"LidCalib.lib")
// #endif
// #endif

#include "\OpenRS\desktop\src\orsPointCloud\lastools\lasdefinitions.h"
#include "Geometry.h"

//#include "lastools/lasreader.h"

//���ˮƽ���ϵ�Բ��
void _lidCalib_Dll_ DetectCircleCenter_2d(POINT2D *pData, int ptNum, double radius, POINT2D &center);

bool _lidCalib_Dll_ simulate_onecircle(int &ptNum, double radius, POINT2D *pData);

//�������ͬ��Բ��Բ��
void _lidCalib_Dll_ DetectConcentric_2d(POINT2D *pData1, int ptNum1, double radius1, 
						 POINT2D *pData2, int ptNum2, double radius2, POINT2D &center);


//ת���ɾ���
bool ConvertToMatrix(LASpointXYZI *pData, int ptNum);

//ת����Ӱ�񱣴�
//bool _lidCalib_Dll_ ConvertToImage(char *pszName, LASpointXYZI *pData, int ptNum, double radius, double gridsize, int style=0);

#endif