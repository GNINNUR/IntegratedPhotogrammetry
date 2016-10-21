#ifndef _INTEGRATED_AT_MANAGER_INCLUDE_H__
#define _INTEGRATED_AT_MANAGER_INCLUDE_H__

#include "AlignPrj.h"
#include "Calib_Camera.h"
#include "CombineAdjustment\CATTStruct_def.h"
#include <vector>
#include <map>

#include "orsBase/orsIPlatform.h"

#include "CombineAdjustment/orsAlgCameraCalib.h"
#include "CombineAdjustment/orsAlgImageBA.h"

using namespace std;

#ifdef COMBINEADJUSTMENT_EXPORTS
#define  _CADJ_DLL_  __declspec(dllexport)
#else
#define  _CADJ_DLL_  __declspec(dllimport)	
#endif

//first key: ��ƬID(�����������), second key: Ӱ���б��еı��
typedef std::map<int, int> myLUT;


class  _CADJ_DLL_ CIntATManager
{
public:
	CIntATManager(orsIPlatform *platform);
	virtual ~CIntATManager();

	//���ý�����Ŀ¼
	void setOutputDir(const char *pszDir);

	//�����������
	void setCameraParam(double x0, double y0, double f, double imgWid, double imgHei, double pixWid, double pixHei,
		lensDistortion lensDist, double camera_mount_angle[3]=NULL);

	//���ò�����Ϣ���Զ���ȡӰ���ʼ�ⷽλԪ��
	void setAlignPrj(CAlignPrj *pAlg);

	//����ͬ���۲�ֵ
	bool loadTiePoints(const char* pszFileName);

	//��ȡvisualSFM��nvm�ļ�
	bool loadnvmFile(const char* pszFileName);

	//��ȡ���Ƶ��ļ�
	bool loadPATBconFile(const char *pFileName);

	//��ȡGPS����
	bool loadGPSEOFile(const char *pFileName);

	//��ȡPATBƽ����ori�ļ�
	bool loadoriFile(const char *pFileName);
	//��ȡƽ�������ӵ�����
	bool loadadjFile(const char *pFileName);

    //����GPS���������Ƭ��ƫ��
	void calImageAngleElement_byGPS();

	bool loadPATBimFile(const char *pFileName);

	//��lidar��������ȡ����ê��
	std::vector<orsPOSParam>* get_OrientedArchorsFromLIDAR(double interval);

	//���������
	bool output_OrientedArchors(const char *pFileName);

	//ȡ������
	int getStripNum();
	//ȡ������
	int getSurvAreaNum();
	//ȡ��Ƭ����
	int getSumofImages();
	//ȡӰ��������Ƭ��(�����)
	std::string getImageName(int imgId);

	//����Ӱ����ȡ�����ţ������ڷ���-1
	int getImageID(const char *imgName);

	//��ȡӰ���ⶨ�����
	//POS_IntepolatedFromLIDAR: ��POS�켣����ȡ��Ƭ��������
	//POS_Raw: ֱ��ʹ�ó�ʼ������aop�ļ�����ȡ��
	bool extractImgOrParam(double *pImgExor, orsCamSysExtraParam_TYPE epType, CamPOS_TYPE posType);
	//t_i, t0_i
	bool extractImgExposureTime(double *timeArr);

	//��ȡGPS�۲���Ϣ
	bool extractGPSInfo(GPSinfo *pGPS);

//	bool extractImgOrDirectly(double *pImgExor);
	/////////////////////////////////////////////////////////////////
	//        1.�����װ���궨                                   //
	//���ð�װ��������Ӱ���ⷽλԪ��
	void cameraMountingError_correction(double *pImgExor, orsCamSysExtraParam *param);

	//������ò����궨
	bool camera_calibration_BA(char *pszCorrespFile, orsCamSysExtraParam *param, orsCamSysExtraParam_TYPE epType, CamPOS_TYPE posType);

	//ǰ��-�󽻷�
	bool camera_calibration_IP(char *pszCorrespFile, orsCamSysExtraParam *param, orsCamSysExtraParam_TYPE epType, CamPOS_TYPE posType);

	/////////////////////////////////////////////////////////////////
	//                  2.Ӱ�������ƽ��                           //
	//���������ƽ���6���ⷽλԪ��
	//GPS�����Ĺ�����ƽ��
	//POS�����Ĺ�����ƽ��
	bool imageBA(/*char *pszCorrespFile, */char *pszSBAResultFile, orsImageBA_TYPE BA_type);


protected:
	
	//�������ӵ�����
	void cal_tiePtCoord(std::vector<ATT_tiePoint> *pTiesVec, double *pExor, orsCamSysExtraParam *calib_param, double *p3Ds);

	//�빤���е���Ƭ����Ӧ
	//pExor: ��Ƭ�ⷽλԪ��
	//calib_param: �����װ����
	bool output_imgExor(const char *pszFileName, double *pExor, orsCamSysExtraParam *calib_param);

private:
//	ATT_CameroRoateMODE	m_rMode;	//�����תģʽ

	double m_camera_mount_angle[3]; //�����װ�ǣ�radian��
	
	_iphCamera	*m_pCamera;
	CAlignPrj	*m_AlgPrj;
	std::vector<std::string>	 m_imgNameVec;
	std::vector<ATT_StripInfo>   m_blockInfo;


	myLUT   *m_imgID_LUT;	//��ƬID�Ĳ��ұ�

	std::vector<orsPOSParam>	m_OrAVec;	//����ê��
	std::vector<ATT_tiePoint>	m_tiePts;	//���ӵ�

	std::vector<ATT_GCP>		m_GCP;		//���Ƶ�

	int		m_nprojs;//��������۲�ֵ������һ�����ӵ��Ӧ�����㣩

	std::string  m_outputDir;

	POINT3D		m_center;
};


#endif