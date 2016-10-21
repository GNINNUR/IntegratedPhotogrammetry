#ifndef _LIDAR_STRIP_ADJUSTMENT_POS_DRIFT_MODEL_INCLUDE_H__
#define _LIDAR_STRIP_ADJUSTMENT_POS_DRIFT_MODEL_INCLUDE_H__

#include "CalibBasedef.h"
#include "LidAdj_def.h"

interface orsILidarGeoModel;

//posƯ�Ƶ�����ƽ��ģ��
	int  LSA_POSDrift(
		const char *pszResultFile,		/* ƽ�����ļ� */
		const int nkeyPt,	 /* ��������� */
		const int ngcp,		/* number of points (starting from the 1st) whose parameters should not be modified.	
							* All B_ij (see below) with i<ncon are assumed to be zero
							���Ƶ���ڹ۲�ֵ����ʼ����*/
		LidAdj_keyPt *pLidTies,	//				 char *vmask,			/* visibility mask: vmask[i, j]=1 if point i visible in image j, 0 otherwise. nxm */
		double *pX,				/* unknowns */
		int nParams,            //number of unknowns   
		unsigned long param_type,       /* ����ƽ������ */
		/*oriCalibParam oriParam*/
		orsLidSysParam oriParam,
		double *refTime,         /* �������Ĳο�ʱ������ */
		int nStrip,               /*  ������ */
		orsILidarGeoModel *pLidGeo
		);



#endif