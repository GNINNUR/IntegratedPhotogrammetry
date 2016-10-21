//  ��ת�Ǻ���ת�������
#ifndef _Calib_GEOMETRY_H_zj_20110813
#define _Calib_GEOMETRY_H_zj_20110813

#include "CalibBasedef.h"

#ifdef _CALIB_UTILITY_
#define  _CalibUtility_Dll_  __declspec(dllexport)
#else
#define  _CalibUtility_Dll_  __declspec(dllimport)	
#endif

// #ifndef _CALIB_UTILITY_
// #ifdef _DEBUG
// #pragma comment(lib,"CalibUtilityD.lib")
// #else
// #pragma comment(lib,"CalibUtility.lib")
// #endif
// #endif

#ifndef _dVector_ANGLE_Def_
#define _dVector_ANGLE_Def_
//
// �����Է���ǵ�ƫ����
//
struct dVector_ANGLE 	{
	double da_alpha, da_beta;
	double db_alpha, db_beta;
	double dc_beta;
};
#endif


/************************************ RotateMat.c ****************************
	function :	set up the rotation matrix
	input 	 :	a,b,c--------- angle array
	output	 :	*r--------- rotation matrix

				                 | 0 -c -b |      | 0 -k -f |
	R=(I+S)*transpose(I-S)   S== | c  0 -a |/2 == | k  0 -w |/2
				                 | b  a  0 |      | f  w  0 |

****************************************************************************/
void _CalibUtility_Dll_ RotateMat_abc(double a,double b,double c, double *r);
_CalibUtility_Dll_ double R2abc(double *R, double *ao, double *bo, double *co);

//
// ������ʽ��phi, omega, kappa��﷨, ��ʽ����ת����
//
_CalibUtility_Dll_ void RotateMat_fwk(double phi, double omega, double kapa, double *r);
_CalibUtility_Dll_ void R2fwk(double *R, double *phi, double *omega, double *kapa);

//
// ����ŷ����omega, phi, kappa��﷨, ��ʽ����ת����
//
_CalibUtility_Dll_ void RotateMat_wfk(double omega, double phi, double kapa, double *r );
_CalibUtility_Dll_ void R2wfk(double *R, double *phi, double *omega,  double *kapa);


/********************************************************************************
L1-----P-----L2
       |
       P1
********************************************************************************/
_CalibUtility_Dll_ double Perpendicular( double x1, double y1, double x2, double y2, double px, double py, double *x, double *y );

_CalibUtility_Dll_ double P3ArcPara(POINT3D pts[3], double *xc,double *yc,double ang[3]);
_CalibUtility_Dll_ double P3ArcPara(POINT2D pts[3], double *xc,double *yc,double ang[3]);

_CalibUtility_Dll_ double TanP2ArcPara(double a,double b, POINT3D pts[2], double *xc,double *yc,double ang[2]);


//////////////////////////////////////////////////////////////////////////
//����ϵ ---> ����ϵ   (radian)
_CalibUtility_Dll_ void RotateMat_roll(double roll, double *r);
_CalibUtility_Dll_ void RotateMat_pitch(double pitch, double *r);
_CalibUtility_Dll_ void RotateMat_heading(double heading, double *r);
_CalibUtility_Dll_ void RotateMat_rph(double roll, double pitch, double heading, double *r);

//////////////////////////////////////////////////////////////////////////
//��ƽ�������ֱ������ϵ֮�����ת����   (radian)
//TPlane���ñ�������ʽ��WGS84����ֱ������
_CalibUtility_Dll_ void RotateMat_NED2ECEF(double lat0, double lon0, double *R);
_CalibUtility_Dll_ void RotateMat_ENU2ECEF(double lat0, double lon0, double *R);
//void RotateMat_TPlane2WGS84_NED(double lat0, double lon0, double *R);

//�ع�����ϵ����ƽ������ϵ����ת����TPlane���ö�������ʽ
_CalibUtility_Dll_ void RotateMat_ECEF2NED(double lat0, double lon0, double *R);
_CalibUtility_Dll_ void RotateMat_ECEF2ENU(double lat0, double lon0, double *R);

//void RotateMat_WGS842TPlane_ENU1(double lat0, double lon0, double *R);

//void RotateMat_WGS842TPlane_ENU2(double lat0, double lon0, double *R);


#endif