#ifndef _LIDAR_STRIP_ADJUSTMENT_INCLUDE_H__
#define _LIDAR_STRIP_ADJUSTMENT_INCLUDE_H__
//����ƽ���е�lidarƽ���


#include "AlignPrj.h"

#include "orsBase/orsIPlatform.h"
#include "orsLidarGeometry\orsITrajectory.h"
#include "LidStripAdj\LidAdj_def.h"
#include "CalibBasedef.h"
#include "Align_def.h"

using namespace std;

#ifdef LIDADJLIB_EXPORTS
#define  _LIDADJ_DLL_  __declspec(dllexport)
#else
#define  _LIDADJ_DLL_  __declspec(dllimport)	
#endif

class  _LIDADJ_DLL_ CLidAdjustment
{
public:
	CLidAdjustment(orsIPlatform *platform);
	~CLidAdjustment();

	//���ò���
	//2014.12.4 ������Ϣ�����ӵ�Ҫ�ֿ��洢  todo
	void setAlignPrj(CAlignPrj *pAlg);

	//
	bool loadTiePoints(const char* pszFileName, int minOverlap=2);

	//����ƽ��
	int Lidar_StripAdjustment(char *pszSAResultFile, orsLidSA_TYPE SA_type);


private:



private:
	CAlignPrj *m_pAlg;
	ref_ptr<orsITrajectory>	  m_traj;	//�����켣

	CArray<TieObjChain*, TieObjChain*>   m_TieList;
	int		m_nprojs;   //�������ϵ�ͬ���۲�ֵ����
public:
	double	*m_covar;	//δ֪����Э������
	int		m_nUnknown;		//δ֪������
	double  m_rms;		//��λȨ�����۲�ֵ��
	double  *m_corcoef;	//δ֪��֮������ϵ��
};


#endif