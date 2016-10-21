//ƽ��Լ����lidar����ƽ�ͬʱ����lidarϵͳ����ƽ�����
#ifndef _Plane_Constraint_Model_h_zj_2010_10_23_
#define _Plane_Constraint_Model_h_zj_2010_10_23_


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


//ƽ�����  ax+by+cz+d=0
struct PlaneParam
{
	double a, b, c, d;
};



//ransac���ƽ�����,������ƽ���ڵĵ㣬Ĭ�ϰ�1.5��rms���ֲ�
int PlaneFitting_ransac(LidMC_Plane *pLidPatch);

//����ָ��������������ƽ���ƽ�����
int _lidCalib_Dll_ lidPlaneFitting(LidMC_Plane *pLidPlane, int nPlane, oriCalibParam oriParam, double *Lidparam, CalibParam_Type type);

//����ransac���ƽ�棬���㷨����
//int _lidCalib_Dll_ lidPlaneFitting(LidMC_Plane *pLidPlane, int nPlane);


//2013.1.4  �Զ����ݼ�У�������ͼ���������
void distance_PointToPlane_new(double *p, double *y, int m, int n, void *data);

void _lidCalib_Dll_ LidPlane_ransac(LidMC_Plane *pLidPlane, int nPlane);

//ƽ���, ƽ��������ǰLiDARϵͳ��������ʼ��������ϵͳ������ͣ��������rms��ͬ��������
void _lidCalib_Dll_ LidStatDis_PointToPlane(LidMC_Plane *pLidPatch, int nPatch, double *Lidparam, bool bInit, oriCalibParam oriParam,
											CalibParam_Type param_type,double *rms, int nTies, double *pDis=NULL);

class _lidCalib_Dll_ CPlaneCalib : public CLidarSelfCalibration
{
public:
     CPlaneCalib();
 	 virtual ~CPlaneCalib();
// 	 void setResultName(char *pName);


	 //����ƫ�ĽǺ�ƫ�ķ���, ƽ��Լ��
	 int LidMC_Misalign_plane(
		 const int nPlane,	/* number of planes */
		 LidMC_Plane *pLidPlane,
		 double *pX,			/* initial parameter vector p0: (a1, ..., am, b1, ..., bn).
								* aj are the image j parameters, bi are the i-th point parameters,
								* size m*cnp + n*pnp
								δ֪����ƫ�ķ����Ͱ��ý�    
								pX=[Xt, Yt, Zt, r, p, h]^T ����δ֪����ֵ��*/
								const int pnp			/* number of parameters δ֪������*/
								//double *pObs,			/*�۲�ֵ(����۲�ֵ����Ȩƽ��ֵ)*/
								//double errTh2			//(������ֹ����,�﷽�������);
								);

	 // ƽ��Լ��
	 //2013.1.5  ��ָ����ϵͳ������ͽ��м�У��ȷ��δ֪������
	 int LidMC_Misalign_plane(
		 const char *pszResultFile,		/* ƽ�����ļ� */
		 const int nPlane,	/* number of planes */
		 LidMC_Plane *pLidPlane,
		 double *pX,			/* initial parameter vector p0: (a1, ..., am, b1, ..., bn).
								* aj are the image j parameters, bi are the i-th point parameters,
								* size m*cnp + n*pnp
								δ֪�������ý�    
								pX=[Xt, Yt, Zt, r, p, h]^T ����δ֪����ֵ��*/
		oriCalibParam oriParam,	//ϵͳ����ֵ
		const CalibParam_Type type	//�������	
								);

	 //zֵԼ��
	 int LidMC_Misalign_plane_zconstraint(
		 const int nVP,
		 LidMC_VP *pLidVP,
		 const int nPlane,	/* number of planes */
		 LidMC_Plane *pLidPlane,
		 double *pX,			/* initial parameter vector p0: (a1, ..., am, b1, ..., bn).
								* aj are the image j parameters, bi are the i-th point parameters,
								* size m*cnp + n*pnp
								δ֪����ƫ�ķ����Ͱ��ý�    
								pX=[Xt, Yt, Zt, r, p, h]^T ����δ֪����ֵ��*/
								const int pnp			/* number of parameters δ֪������*/
								//double *pObs,			/*�۲�ֵ(����۲�ֵ����Ȩƽ��ֵ)*/
								//double errTh2			//(������ֹ����,�﷽�������);
								);


	 //ƽ��ƽ��ȼ��
	 int LidMC_PlaneAdj_QA(
		 const int nPlane,	/* number of planes */
		 LidMC_Plane *pLidPlane,
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