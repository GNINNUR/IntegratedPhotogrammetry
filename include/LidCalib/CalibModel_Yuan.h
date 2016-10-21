#ifndef _LIDAR_CALIBRARION_YUAN_METHOD_ZJ_2016_03_29_H__
#define _LIDAR_CALIBRARION_YUAN_METHOD_ZJ_2016_03_29_H__


#include "CalibBasedef.h"
#include <vector>

#ifdef _LIDCALIB_
#define  _lidCalib_Dll_  __declspec(dllexport)
#else
#define  _lidCalib_Dll_  __declspec(dllimport)	
#endif


using namespace std;



class _lidCalib_Dll_ CLidCalib_Yuan : public CLidarSelfCalibration
{
public:
	CLidCalib_Yuan();
	virtual ~CLidCalib_Yuan();

	int  LidMC_Calib_Coplanar(
		const char *pszResultFile,		/* ƽ�����ļ� */
		const int nCorrsp,	        /* number of Correspondent Objects */
		const int ngcp,		/* number of points (starting from the 1st) whose parameters should not be modified.	
							* All B_ij (see below) with i<ncon are assumed to be zero
							���Ƶ���ڹ۲�ֵ����ʼ����
							��coplanar��������ʱ����*/
		LidMC_Obj *pLidPatch,
	
//		TiePoint_Adj_mode TiePt_mode,
							//				 char *vmask,			/* visibility mask: vmask[i, j]=1 if point i visible in image j, 0 otherwise. nxm */
		double *pX,			/* initial parameter vector p0: (a1, ..., am, b1, ..., bn).
												* aj are the image j parameters, bi are the i-th point parameters,
												* size m*cnp + n*pnp
												δ֪����ƫ�ķ����Ͱ��ý�    
												pX=[Xt, Yt, Zt, r, p, h]^T ����δ֪����ֵ��*/
		//				const int pnp,			/* number of parameters δ֪������*/
		orsLidYuanParam *pOriParam,            /*   ��������   */
		int		nStrips                        /*   ������     */
//		unsigned long	param_type				/* ��У�������� */
		);

	int  LidMC_Calib_ConjugatePoints (
		const char *pszResultFile,		/* ƽ�����ļ� */
		const int nConjugatePts,	    /* number of Correspondent Objects */
		const int ngcp,		/* number of points (starting from the 1st) whose parameters should not be modified.	
							* All B_ij (see below) with i<ncon are assumed to be zero
							���Ƶ���ڹ۲�ֵ����ʼ����
							pLidObjs�еĴ�0��ʼ��ngcp����Ϊ���Ƶ�*/
		LidMC_VP *pLidObjs,            /* conjugate points array */ 
		TiePoint_Adj_mode TiePt_mode,
			
							//				 char *vmask,			/* visibility mask: vmask[i, j]=1 if point i visible in image j, 0 otherwise. nxm */
		double *pX,			/* initial parameter vector p0: (a1, ..., am, b1, ..., bn).
												* aj are the image j parameters, bi are the i-th point parameters,
												* size m*cnp + n*pnp
												δ֪����ƫ�ķ����Ͱ��ý�    
												pX=[Xt, Yt, Zt, r, p, h]^T ����δ֪����ֵ��*/
		//				const int pnp,			/* number of parameters δ֪������*/
		orsLidYuanParam *pOriParam,            /*   ��������   */
		int		nStrips                        /*   ������     */
//		unsigned long	param_type				/* ��У�������� */
		);

	//ƽ��ȼ��
	int  LidMC_QA(
		const int nVP,
		const int ngcp,
		LidMC_VP *pLidVP,
		double *pInitX,			/* δ֪����ֵ*/
		double *pAdjX,			/* δ֪��ƽ��ֵ*/
		const int pnp,			/* number of parameters δ֪������*/
		Calib_Method  mType,
		orsLidYuanParam oriParam,
		unsigned long	param_type		/* ��������*/
		);

public:
	double	*m_covar;	//δ֪����Э������
	int		m_pn;		//δ֪������
	double  m_rms;		//��λȨ�����۲�ֵ��
	double  *m_corcoef;	//δ֪��֮������ϵ��
};




#endif