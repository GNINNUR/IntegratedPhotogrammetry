#ifndef _COMBINED_ATT_STRUCTURE_DEFINE_H__
#define _COMBINED_ATT_STRUCTURE_DEFINE_H__

#include "orsLidarGeometry\orsITrajectory.h"
#include <vector>
using namespace std;

//�����װʱ����תģʽ
enum ATT_CameroRoateMODE	{
	ATT_rmNONE = 0,				// ������ת			
	ATT_rmANTICLOCKWISE90,	// ��˳ʱ����ת90��
	ATT_rmCLOCKWISE90,		// ˳ʱ����ת90��
	ATT_rm180				// 180��
};


//����ŵ����ӵ�
struct ATT_LaserPoint
{
	int lidID;  //��1��ʼ
	double t;
//	double gpstime;
	double X, Y, Z;
//	orsPOSParam pos;
	double range;
	double sAngle;
};

//���
struct ATT_ImagePoint
{
	int imgID;		//������+��Ƭ�ţ���0001��ʼ
	double xi, yi;	//��������
	double xp, yp;
};
//��Ƭ
struct ATT_ImageEXOR
{
	int survID;				//����ID ע�⣺��0��ʼ˳����
	char imgName[64];
	int imgID;				//��Ƭ�ţ�������+Ӱ���
	double t0;				//�����ڵ�ʱ���׼������Ư��ֵ��ƽ��

	orsPOSParam eoParam;    //��Ƭ�ⷽλԪ��
	std::vector<orsPOSParam> orientedTraj;	//���������

	ATT_ImageEXOR()
	{

	}

	~ATT_ImageEXOR()
	{
		orientedTraj.clear();
	}
};

//����
struct ATT_StripInfo
{
	int stripID;							//��Align_LidLine�е�������ű���һ�£���1��ʼ
	std::string atnName;
	std::vector<ATT_ImageEXOR> imgEOVec;	//��GPSʱ������

	ATT_StripInfo()
	{
		atnName=_T("");
	}

	~ATT_StripInfo()
	{
		atnName.clear();
		imgEOVec.clear();
	}
};

#define		TiePtTYPE_TIE		 0L		//���ӵ�
#define		TiePtTYPE_GCP_XY	 1L		//ƽ����Ƶ�
#define		TiePtTYPE_GCP_Z		 2L		//�߳̿��Ƶ�
#define		TiePtTYPE_GCP_XYZ	 3L		//ƽ�߿��Ƶ�

// enum ATT_TiePtTYPE
// {
// 	TiePtTYPE_TIE = 0L,		//���ӵ�
// 	TiePtTYPE_GCP_XY = 1L,		//ƽ����Ƶ�
// 	TiePtTYPE_GCP_Z = 2L,		//�߳̿��Ƶ�
// 	TiePtTYPE_GCP_XYZ = 3L,		//ƽ�߿��Ƶ�
// };

struct ATT_tiePoint
{
	ors_int64 tieId;
	double X, Y, Z;	
	int tType;
	std::vector<ATT_LaserPoint> laspts;
	std::vector<ATT_ImagePoint> projs;

	ATT_tiePoint()
	{
		tType = TiePtTYPE_TIE;
	}
	~ATT_tiePoint()
	{
		laspts.clear();
		projs.clear();
	}
};

struct ATT_GCP
{
	ors_int64 tieId;
	double X, Y, Z;	
	int tType;
};

typedef struct tagLidImage_ATT_DATA
{
	ref_ptr<orsITrajectory>	pTraj;



}LidImage_ATT_DATA;


//��ֵ����
#define MAX_DOUBLE(x)		( x=1.7E+308 )
#define MIN_DOUBLE(x)		( x=-1.7E+308 )	
#define MAX_FLOAT(x)		( x=3.3e33f )
#define MIN_FLOAT(x)		( x=-3.3e33f )
#define MAX_LONG(x)			( x=2147483647 )	
#define MIN_LONG(x)			( x=-2147483648 )
#define MAX_USHORT(x)		( x=65535 )
#define MIN_USHORT(x)		( x=0 )

#endif