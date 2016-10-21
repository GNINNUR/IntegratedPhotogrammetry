#ifndef _LIDAR_GEOMETRY_DATA_TYPE_DEFINE_H__
#define _LIDAR_GEOMETRY_DATA_TYPE_DEFINE_H__

#include "orsBase\orsTypedef.h"

//��������
struct orsPOSParam
{
	int stripID;		//������,��0��ʼ		
	double time;		//time stamp
	orsPOINT3D coord;	//lat, lon, h ԭʼ����Ĭ��ΪWGS84��,��γ�ȡ������
	double r, p, h;		//radian ԭʼPOS����Ĭ��Ϊ��������ϵ�µ�ת��
	double R[9];

	float padding;     //���������λ
};

//ԭʼ����ŵ㼸����Ϣ
struct LidGeo_RawPoint
{
	// 	double Xs, Ys, Zs;
	// 	double R[9];
	orsPOSParam eoParam; //POS����
	double range;
	double sAngle;
	double X, Y, Z;	//�ŵ����ά����
};

//��ʱ��ĵ��¼
struct LidPt_XYZT
{
	double X, Y, Z;
	double t;
};

#ifndef _LidPt_SurvInfo_Def_
#define _LidPt_SurvInfo_Def_
typedef struct LidPt_SurvInfo  //����㶨λ������Ϣ
{
	int stripID;    //��������������ţ�������ص��㷨��Ҫ
	double x;		//(WGS84/radians)
	double y;		//(WGS84/radians)
	double z;
	double time;	//time stamp
	orsPOSParam POS_Info;	//pos��Ϣ
	double vecRange[3]; //ɨ�豾������ϵ�µ�б�࣬ɨ�������ĳ˻�

} LidPt_SurvInfo;
#endif


//ϵͳ���Ӳ�������
enum orsLidSysParam_TYPE
{
	LidGeo_rph=0,			//������׼��ƫ��
	LidGeo_rphxyz,			//��׼��ƫ��+ƫ�ķ���
	LidGeo_rph_appXYZ,		//ƫ��+���Ӳ������Ը���
	LidGeo_rphxyz_sa0,		//ƫ��+ƫ�ķ���+ɨ��ǹ������
	LidGeo_rphxyz_sa0_sac,	//ƫ��+ƫ�ķ���+ɨ��ǹ������+ɨ������Ը���
	LidGeo_rph_sa0,			//ƫ��+ɨ������
	LidGeo_rph_drange,		//ƫ��+������
	LidGeo_rph_drange_sa0	//ƫ��+������+ɨ��ǹ������
};

//POSϵͳ���  һ��Ư��ģ��
struct  POS_Param
{
	//	oriCalibParam calib_param;  //Lidarϵͳ���
	double *gps_drift;		//GPS���  6N
	double *ins_drift;		//INS���  6N

	int		stripNum;		//������
	double  time0;			//�ο�ʱ��
};

//�ϸ�ģ��
//ԭʼϵͳ������������õģ�
struct  orsLidSysParam
{
	double lever_arm[3];	//ƫ�ķ���
	double boresight_angle[3];	//ƫ��
	double sa0;				//ɨ������
	double sac;				//ɨ������Ը���ϵ��
	double drange;			//������

	double semiXYZ[3];		//ϵͳƽ����, �п��Ƶ�ʱ��

	POS_Param  posParam;
};

//Ressl model parameters
struct orsLidResslParam 
{
//	double		m_rotZ;				//������Z�����ת��
//  ÿ������������
	double		rotMatZ[9];		//������Z�����ת����
	orsPOINT3D  stripCenter;		//�����������꣨�ο��㣩

	//unknowns��ϵͳδ֪�����������޹�
	double		ax, ay, az;
	double		aroll;
	double		ayaw;
};

//Yuan model parameters
struct orsLidYuanParam 
{
	//	double		m_rotZ;				//������Z�����ת��
	//  ÿ������������
	double		rotMatZ[9];		//������Z�����ת����
	orsPOINT3D  stripCenter;		//�����������꣨�ο��㣩

	//unknowns��ϵͳδ֪�����������޹�
	double		ax, ay, az;
	double		kappa;
	double		omega;
};

//Jing model parameters
struct orsLidJingParam 
{
	//	double		m_rotZ;				//������Z�����ת��
	//  ÿ������������
	double		rotMatZ[9];		//������Z�����ת����
	orsPOINT3D  stripCenter;		//�����������꣨�ο��㣩  ZΪƽ���и�

	//unknowns��ϵͳδ֪�����������޹�
	double		phi, omega, kappa;
	
};

interface orsITrajectory;

struct orsLidTrajAidParam
{
	orsITrajectory *pTraj;		//trajectory

	double lever_arm[3];	//ƫ�ķ���
	double boresight_angle[3];	//ƫ��

	double drift[6];		//Ư�Ʋ���
};


#endif