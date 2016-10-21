#ifndef _COMBINE_BUNDLE_ADJUSTMENT_DEFINE_INCLUDE_
#define _COMBINE_BUNDLE_ADJUSTMENT_DEFINE_INCLUDE_

#include "Calib_Camera.h"
#include "CATTStruct_def.h"

//���ϵͳ���Ӳ�������
enum orsCamSysExtraParam_TYPE
{
	CamGeo_rph=0,			//������׼��ƫ��
	CamGeo_rphxyz,			//��׼��ƫ��+ƫ�ķ���

};

//���ϵͳ������������õģ�
struct  orsCamSysExtraParam
{
	double lever_arm[3];		//ƫ�ķ���
	double boresight_angle[3];	//��׼��ƫ��
};

enum CamPOS_TYPE
{
	POS_Raw=0,	//ֱ��ʹ��ԭʼPOS
	POS_IntepolatedFromLIDAR	//�ӵ��ƺ����в�ֵ��POS
};

/* pointers to additional data, used for computed image projections and their jacobians */
struct imgCalib_globs{
	double *pUnknowns;	//δ֪������ ��У����+�﷽����
	double *pImgpts;	//�������۲�ֵ
	//	double *p3Ds;	//�﷽������
	double *pImgExor;	//��Ƭ�ⷽλԪ��
	char *mask;	//���ϡ�����
	int nVFrame;	//�������Ƭ��(�������Ƭ)����ڰ��������˵
	int nimg, ntie, nproj; //ʵ����Ƭ�������ӵ������񷽵���
	int cnp, pnp, mnp; /* cnp��Ƭδ֪��������pnp�﷽��δ֪������, mnp�񷽵���������� */
	int ngcp;	//���Ƶ���
	int nfixedImg;	//�̶�Ƭ��

	orsCamSysExtraParam_TYPE   calibType;
	orsCamSysExtraParam		   *calibParam;
	_iphCamera  *camInfo;
	FILE *fplog;

	double *hx, *hxx; //���ڼ���jac
};

enum orsImageBA_TYPE
{
	ImageBA_classic=0,				//��ͳ����
	ImageBA_classic_W,				//ѡȨ���������дֲλ		
	ImageBA_GPS_supported,			//GPS����
	ImageBA_POS_supported			//POS����
};

//ԭʼPOS��Ϣ
struct POSinfo
{
	int survID;		//��0��ʼ
	double Xgps, Ygps, Zgps;
	double phi, omega, kappa;
	double gpstime;
	double t0;	//ʱ���׼��
};

struct GPSinfo
{
	int survID;		//��0��ʼ
	double XYZ[3];
	double gpstime;
	double t0;
};

#define		ATT_GS_Undo				0			//�����ֲ�
#define		ATT_GS_IterWght_LI		1			//ѡȨ���� ������ʷ���


/* pointers to additional data, used for computed image projections and their jacobians */
struct imgBA_globs{
	double *pUnknowns;	//δ֪������ ��У����+�﷽����
	double *pObs;	//�۲�ֵ���ʼΪ������꣬(����Ӱ�����۲�ֵ���ں���)
	double *pAdjObs; //ƽ���Ĺ۲�ֵ��������󲹳�
	//	double *p3Ds;	//�﷽������
	//	double *pImgExor;	//��Ƭ�ⷽλԪ��
	char *mask;	//���ϡ�����
	//	int nVFrame;	//�������Ƭ��(�������Ƭ)����ڰ��������˵
	int nimg, ntie, nproj; //ʵ����Ƭ�������ӵ������񷽵���
	int cnp, pnp, mnp; /* cnp��Ƭδ֪��������pnp�﷽��δ֪������, mnp�񷽵���������� */
	int ngcp;	//���Ƶ���
	int nfixedImg;	//�̶�Ƭ��

	//GPS��������
	int nGPSoffset;	//GPSƫ�ķ�������
	int nGPSdrift;	//GPSƯ�Ʋ�������
//	double GPSt0;	//�ο�ʱ��
//	double *pTimes;	//��Ƭ�ع�ʱ��
//	double *pGpsObs; //GPS�۲�ֵ
	GPSinfo *pGpsObs; //GPS�۲�ֵ
	int nSurv;       //�����������ʱ��λ�ȡ��Ӱ��

	/////////////////������Ϣ////////////////////
	_iphCamera  *camInfo;  //�����Ϣ
	orsImageBA_TYPE	BA_type; //��������
	FILE *fplog;       //״̬���
	const char *pszImgPtResidual;	//ƽ�������в��ļ���
	double *hx, *hxx; //���ڼ���jac����ʱ�洢�ռ�

	POSinfo *posObvs;	//pos�۲���Ϣ
	std::vector<ATT_GCP> gcp;	//���Ƶ���Ϣ

	double *sigma0;		//�۲�ֵ�����鵥λȨ�����
	int		nObsCls;		//�۲�ֵ�������
	double  confidence;	//���ֲ������ˮƽ��0.003��

	int     gross_snooping;	//�ֲ�̽�ⷽ��
	double	raw_rms;		//ƽ��ǰ�Ĺ۲�ֵ�����
	double	adj_rms;		//ƽ���Ĺ۲�ֵ�����

	int  lens_selfCalib_param_num;		//��ͷ�Լ�У��������
};


#endif