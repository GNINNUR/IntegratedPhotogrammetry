#ifndef _LIDAR_Calibration_Base_Define_H_zj_2010_09_21_
#define _LIDAR_Calibration_Base_Define_H_zj_2010_09_21_

//#include "LidBaseDef.h"
#include <vector>
//#include "ATN_def.h"
#include "orsBase\orsTypedef.h"
#include "orsLidarGeometry\LidGeo_def.h"


#ifndef _POINT2D_Def_
#define _POINT2D_Def_
// struct POINT2D	{
// 	double x, y;
// };
typedef orsPOINT2D POINT2D;
#endif

#ifndef _POINT3D_Def_
#define _POINT3D_Def_
// struct POINT3D	{
// 	double X, Y, Z;
// };
typedef orsPOINT3D POINT3D;
#endif

#ifndef _PLANE_Def_
#define _PLANE_Def_
struct	PLANE {
	double X0, Y0, Z0;
	float A, B, C;				// normal line����������
};
#endif


#ifndef _LINE3D_Def_
#define _LINE3D_Def_
struct	LINE3D	{
	double X0, Y0, Z0;
	float a, b, c;				// ����ʸ��
};
#endif


#ifndef PI
#define PI 3.1415926535897932384626433832795
#endif



//tie point type
#ifndef Tiepoint_Type
#define Tiepoint_Type
#define  TP_type_undefine		0
#define  TP_type_vertical		1		//�̵߳�
#define  TP_type_horizon		2		//ƽ���
#define  TP_type_hor_ver		3		//ƽ�ߵ�
#endif

//���ӵ�ƽ��ģʽ
enum TiePoint_Adj_mode
{
	LidAdj_TP_UseHorVer = 0,    //��ƽ��߳�
	LidAdj_TP_UseHor,			 //ֻ��ƽ��
	LidAdj_TP_UseVer            //ֻ�ø߳�
};

#define  OBJTYPE_Undefine	0
#define  OBJTYPE_VP			1
#define  OBJTYPE_Plane		2



class LidMC_Obj
{
public:
	virtual int getType()=0;

};

//�������ӵ�
class LidMC_VP : LidMC_Obj
{
public:

	int tieID;					//������ID, ��0��ʼ��˳�����
	double  vx, vy, vz;		//����۲�ֵ

	std::vector<LidPt_SurvInfo> *pLidPoints;	//ʵ�ʼ��������	
// 	void *pLidPoints; //ʵ�ʼ��������
// 	int num_LidPt;	  //����

	double  eX, eY, eZ;	//����ֵ (��ֵ)
	char	VP_type;		//0: undefined;  1: horizontal and vertical control point;   2: horizontal control point;   3: vertical control point
	
	char	reliability;		//0: ���ɿ���1: �ɿ�

	LidMC_VP()
	{
		pLidPoints=NULL;
		reliability=1;
	}

	~LidMC_VP()
	{
		if(pLidPoints)	delete pLidPoints;	pLidPoints=NULL;
	}

	virtual int getType() { return OBJTYPE_VP; };
};

//����Ƭ
class LidMC_Plane : LidMC_Obj
{
public:

	int tieID;								//������ID, ��0��ʼ��˳�����
	double  s1, s2, s3, s4;					//ƽ�淽�̲���
	std::vector<LidPt_SurvInfo> *pLidPoints;	//ʵ�ʼ��������, ������
//	int sID;								//���������������

// 	void *pLidPoints;
// 	int num_LidPt;


	LidMC_Plane()
	{
		pLidPoints=NULL;
	}

	~LidMC_Plane()
	{
		if(pLidPoints)	delete pLidPoints;	pLidPoints=NULL;
	}

	virtual int getType() { return OBJTYPE_Plane; };
};

//ƽ���δ֪��ģ��
typedef int LiDMC_PARAMETER;
static LiDMC_PARAMETER	LiDMC_rph=0;	//r, p, h
static LiDMC_PARAMETER	LiDMC_xyzrph=1;	//tx, ty, tz, r, p, h
static LiDMC_PARAMETER	LiDMC_rph_SemiXYZ=2;		//tx, ty, tz, r, p, h, semiXYZ
static LiDMC_PARAMETER	LiDMC_xyzrph_SemiXYZ=3;			//r, p, h, semiXYZ

//��� (��ƽ������)
typedef struct ImgPt
{
	int imgID;
	double x, y;
};

//Ӱ��ƥ���
typedef struct ImgMatchPt
{
	std::vector<ImgPt>	matchPt;		//֧�ֶ�Ƭ
	POINT3D		objPt;						//��������﷽����
};

enum Calib_Method
{
	Lid_Calib_VP_VCP=0,		//��Ӧ���VCP����
	Lid_Calib_VP_NN,		//��Ӧ���NN����
};

//����ƽ���������
#define LidAdj_NoneSysError					0L		//��ϵͳ����
#define LidAdj_boreAngle					1L		//ƫ�Ľǣ�3����
#define LidAdj_leverArm						2L		//ƫ�ķ�����3����
#define LidAdj_sa0							4L		//ɨ���sa0: 1����
#define LidAdj_sac							8L		//ɨ���sac: 1����
#define LidAdj_drange						16L		//������: 1����
#define LidAdj_appXYZ						32L		//����ƫ��: 3����
#define LidAdj_GPS_drift					64L		//GPSƯ�ƣ�6����
#define LidAdj_INS_drift					128L	//INSƯ�ƣ�6����




enum CalibParam_Type
{
	Calib_None = -1,
	Calib_rph=1,			//��У����ƫ�Ľ�
	Calib_rphxyz=2,			//��Уƫ�Ľ�+ƫ�ķ���
	Calib_rph_appXYZ=33,	//ƫ�Ľ�+���Ӳ������Ը���
	Calib_rphxyz_sa0=6,		//ƫ�ķ���+ƫ�Ľ�+ɨ��ǹ������
	Calib_rphxyz_sa0_sac=14,	//ƫ�ķ���+ƫ�Ľ�+ɨ��ǹ������+ɨ������Ը���
	Calib_rph_sa0=5,			//ƫ�Ľ�+ɨ������
	Calib_rph_drange=17,		//ƫ�Ľ�+������
	Calib_rph_drange_sa0=21	//ƫ�Ľ�+������+ɨ���
};

//ԭʼϵͳ������������õģ�
// struct  oriCalibParam
// {
// 	double lever_arm[3];	//ƫ�ķ���
// 	double misalign_angle[3];	//ƫ�Ľ�
// 	double sa0;				//ɨ������
// 	double sac;				//ɨ������Ը���ϵ��
// 	double semiXYZ[3];		//ϵͳƽ����
// 	double drange;			//������
// 
// 	POS_Param  posParam;
// };

//LIDAR�Լ�У�ӿ�
class CLidarSelfCalibration
{
public:
	CLidarSelfCalibration() { m_pResultName = 0; };
	virtual ~CLidarSelfCalibration() 
	{ 
		if(m_pResultName) 
			free(m_pResultName);
	};

	virtual void setResultName(char *pName) 
	{
		if (pName == 0)
		{
			fprintf(stderr, "ERROR: result_file_name = NULL not supported by CLidarSelfCalibration\n");
		}
		else
		{
			free (m_pResultName);
			m_pResultName = strdup(pName);
		}
	};

	char *m_pResultName;
};

#endif