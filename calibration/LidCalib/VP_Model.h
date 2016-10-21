#ifndef _Virtual_Point_Model_H_zj_2010_09_17_
#define _Virtual_Point_Model_H_zj_2010_09_17_

//////////////////////////////////////////////////////////////////////////
//�������ӵ�ģ��

#include "CalibBasedef.h"
#include <vector>

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

using namespace std;


// #ifdef __cplusplus
// extern "C" {
// #endif

/*
	������������
	VP:  Virtual Point ����㣬�޵������
	VTP: Virtual True Point  �е�����Ƶ�
	NN: �����ģ��
	Tri: ������ģ��
*/

enum VP_mode 
{ 
	VP_TRIANGLE,  //�����β�ֵ
	VP_IDW			//���뷴�Ȳ�ֵ
};


void cal_VPObs(double *p, double *y, int m, int n, void *data);

//����ָ����������VP���
void _lidCalib_Dll_ LidStatDis_VP(LidMC_VP *pLidVP, int nVP, int ngcp, double *pX, oriCalibParam oriParam, 
								  CalibParam_Type  param_type, double *pdx, double *pdy, double *pdz, double *rms);

double _lidCalib_Dll_ TriangleInterpolate( std::vector<ATNPoint> *pLidPoints, double x, double y );

class _lidCalib_Dll_ CVCPCalib : public CLidarSelfCalibration
{
public:
	CVCPCalib();
	virtual ~CVCPCalib();

	//����ƫ�ĽǺ�ƫ�ķ���, ���������(΢С��Ƭ)��Ӧ����
	//���Ƶ���Ҫ����������ʼ��
int  LidMC_Misalign_VP_Triangle_xyzrph_semiXYZ(
	const int nVP,	        /* number of Virtual points */
	const int ngcp,		/* number of points (starting from the 1st) whose parameters should not be modified.	
						* All B_ij (see below) with i<ncon are assumed to be zero
						���Ƶ���ڹ۲�ֵ����ʼ����*/
	LidMC_VP *pLidPatch,

						//				 char *vmask,			/* visibility mask: vmask[i, j]=1 if point i visible in image j, 0 otherwise. nxm */
	double *pX,			/* initial parameter vector p0: (a1, ..., am, b1, ..., bn).
											* aj are the image j parameters, bi are the i-th point parameters,
											* size m*cnp + n*pnp
											δ֪����ƫ�ķ����Ͱ��ý�    
											pX=[Xt, Yt, Zt, r, p, h]^T ����δ֪����ֵ��*/
	const int pnp=6			/* number of parameters δ֪������*/
	//				 double *pObs,			/*�۲�ֵ(����۲�ֵ����Ȩƽ��ֵ)*/
	// 				 double errTh2			//(������ֹ����,�﷽�������);
	//				VP_mode mode        //�����ļ��㷽ʽ
	);

	//ֻ�����ƽ�������GPSϵͳ����
	//2013.11.22 �Թ۲�ֵ��X,Y,Z����������м�Ȩ
	int  LidMC_Misalign_VP_Triangle(
		const char *pszResultFile,		/* ƽ�����ļ� */
		const int nVP,	        /* number of Virtual points */
		const int ngcp,		/* number of points (starting from the 1st) whose parameters should not be modified.	
							* All B_ij (see below) with i<ncon are assumed to be zero
							���Ƶ���ڹ۲�ֵ����ʼ����*/
		LidMC_VP *pLidPatch,

							//				 char *vmask,			/* visibility mask: vmask[i, j]=1 if point i visible in image j, 0 otherwise. nxm */
		double *pX,			/* initial parameter vector p0: (a1, ..., am, b1, ..., bn).
												* aj are the image j parameters, bi are the i-th point parameters,
												* size m*cnp + n*pnp
												δ֪����ƫ�ķ����Ͱ��ý�    
												pX=[Xt, Yt, Zt, r, p, h]^T ����δ֪����ֵ��*/
		//				const int pnp,			/* number of parameters δ֪������*/
		oriCalibParam oriParam,
		CalibParam_Type  param_type				/* ��У�������� */
									);

	//����6����װ����, ����ڶ�Ӧ, �ڲ�����GPSϵͳ����
	int  LidMC_Misalign_VTP_NN_xyzrph_semiXYZ(
		const int nPoint,	        /* number of Virtual points */
		ATNPoint *pLidPoint,
		POINT3D *pGCP,
		double *pX,			
		const int pnp=6			/* number of parameters δ֪������*/
		//				 double *pObs,			/*�۲�ֵ(����۲�ֵ����Ȩƽ��ֵ)*/
		// 				 double errTh2			//(������ֹ����,�﷽�������);
		//				VP_mode mode        //�����ļ��㷽ʽ
		);

	//ֻ����3���Ƕ�, ����ڶ�Ӧ, �ڲ�����GPSϵͳ����
	int  LidMC_Misalign_VTP_NN_rph_semiXYZ(
		const int nPoint,	        /* number of Virtual points */
		ATNPoint *pLidPoint,
		POINT3D *pGCP,
		double *pX,			
		const int pnp=6			/* number of parameters δ֪������*/
		//				 double *pObs,			/*�۲�ֵ(����۲�ֵ����Ȩƽ��ֵ)*/
		// 				 double errTh2			//(������ֹ����,�﷽�������);
		//				VP_mode mode        //�����ļ��㷽ʽ
		);

	//ƽ��ȼ��
	int  LidMC_VPAdj_QA(
		const int nVP,
		const int ngcp,
		LidMC_VP *pLidVP,
		double *pInitX,			/* δ֪����ֵ*/
		double *pAdjX,			/* δ֪��ƽ��ֵ*/
		const int pnp,			/* number of parameters δ֪������*/
		LiDMC_PARAMETER paramtype	/* ��������*/
		//double *pObs,			/*�۲�ֵ(����۲�ֵ����Ȩƽ��ֵ)*/
		//double errTh2			//(������ֹ����,�﷽�������);
		);

public:
	double	*m_covar;	//δ֪����Э������
	int		m_pn;		//δ֪������
	double  m_rms;		//��λȨ�����۲�ֵ��
	double  *m_corcoef;	//δ֪��֮������ϵ��
};


#endif