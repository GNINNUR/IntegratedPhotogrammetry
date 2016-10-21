#ifndef _IPLR_BASE_INFORMATION_DEFINITION_H__
#define _IPLR_BASE_INFORMATION_DEFINITION_H__

//#include <vector>
//#include <string>
//using namespace std;

#include "ALMDefine.h"

//�������
enum iphCameraType
{
	//0-7��DPSһ��
	metricSCAN = 0,	// ����ɨ��Ӱ���п�꣬��Ҫ�ڶ��򣬰������ս���������������
	metricDC = 1,	// ��������������޿�꣬�м�У����, ����UltraCamD, DMC, ��������
	nonMETRIC = 2,	// ������������޼�У����������������ͨ�������
	ADS40 = 3,		// ������ɨ
	SPOT4 = 4,		// ������ɨ
	SPOT5 = 5,		// ������ɨ
	IKONOS = 6,		// ������ɨ
	QUCIKBIRD = 7,	// ������ɨ
	FISHEYE	= 8		// �������
};

//���굥λ
enum iphUnit
{
	Unit_Meter = 0,		//��λ��
	Unit_Degree360 = 1,	//��λ��(360)
	Unit_Degree400 = 2,	//��λ��(400)
	Unit_Radian = 3		//��λ����
};

//ת��ϵͳ
enum iphRotateSys
{
	RotateSys_YXZ = 0,	
	RotateSys_XYZ = 1
};



//���(�ڲ���+�ⷽλԪ��)
class iphCamera
{
public:
	bool	m_bInor;			//�Ƿ�����ڲ�����ʼ��
	bool	m_bExor;			//�Ƿ�����ⷽλԪ�س�ʼ��
	iphCameraType	m_Ctype;	//�������

	double 	m_x0, m_y0;	//principal point  (mm)
	double 	m_f;		//focal length     (mm)
	
	// lens distortion, for film and DC
	double m_k1, m_k2, m_k3;
	double m_p1, m_p2;

	//elements of exterior orientation
	double m_Xs, m_Ys, m_Zs;
	double m_phi, m_omega, m_kappa;

	char	m_HorizontalDatum[_MAX_PATH];
	iphUnit		m_HorizontalUnit;
	char	m_VerticalDatum[_MAX_PATH];
	iphUnit		m_VerticalUnit;

	iphRotateSys	m_RotateSys;
	iphUnit		m_AngleUnit;
	
	double m_R[9];		//rotate matrix
	double	m_GPSTime;

public:
	iphCamera()
	{
		m_bInor=false;
		m_bExor=false;
		m_Ctype = metricDC;

		m_x0=0;	m_y0=0;
		m_f=0;
		m_k1=m_k2=m_k3=0;
		m_p1=m_p2=0;
		
		m_Xs=m_Ys=m_Zs=0;
		m_phi=m_omega=m_kappa=0;
		m_R[0]=m_R[1]=m_R[2]=0;
		m_R[3]=m_R[4]=m_R[5]=0;
		m_R[6]=m_R[7]=m_R[8]=0;
		m_GPSTime=0;
	};

	~iphCamera()
	{
		
	};
	
	//�����ڶ������
	void SetInorParameter(double x0, double y0, double f, double k1, double k2, double k3, double p1, double p2);
	//�����ⷽλԪ��
	void SetExorParameter(double xs, double ys, double zs, double phi, double omega, double kappa);
	//��������ϵͳ
	void SetCoordinateSystem(char *HorizontalDatum, char *VerticalDatum, iphUnit HorizontalUnit, iphUnit VerticalUnit);
	//����ת��ϵͳ
	void SetRotateSystem(iphRotateSys RotateSys, iphUnit AngleUnit);

};

//typedef std::vector<std::string> iphNameList;

typedef struct
{
	int nImgID;                        //Ӱ������(��) (��1��ʼ) 
	CString strImgName;                //Ӱ����
	int     nStripID;                  //�ߺ�(������,��1��ʼ)
	int 	nPhoID;                    //Ƭ��(Ӱ���ں����ڵ�ID,��1��ʼ)  
	int    nIsReversal;                //�Ƿ���ת180��

	iphCamera		camera;

} iphImgINFO;

typedef CArray<iphImgINFO, iphImgINFO&>	iphImgList;


//������Ϣ
// class iphBlockInfo
// {
// public:
// 	char name[_MAX_PATH];			//������
// 	iphImgList	imgList;			//Ӱ���б�
// 	iphNameList	lidarNameList;		//las���б�
// 
// public:
// 	iphBlockInfo()
// 	{
// 		name[0]=0;
// 	};
// 
// 	~iphBlockInfo()
// 	{
// 		imgList.clear();
// 		lidarNameList.clear();
// 	};
// 
// 	void reinitialize()
// 	{
// 		name[0]=0;
// 		imgList.clear();
// 		lidarNameList.clear();
// 	};
// 
// 	iphBlockInfo& operator=(const iphBlockInfo& __x)
// 	{
// 		strncpy(name, __x.name, sizeof(char)*_MAX_PATH);
// 		imgList=__x.imgList;
// 		lidarNameList=__x.lidarNameList;
// 
// 		return	*this;
// 	};
// };


#endif 
