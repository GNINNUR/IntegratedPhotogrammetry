#ifndef _GENERAL_POINT_INDEX_H__
#define _GENERAL_POINT_INDEX_H__

#include <vector>
#include "tin\tinClass.h"
#include "orsBase\orsString.h"
//#include "ATN_def.h"
#include "orsPointCloud\orsIPointCloudService.h"
#include "orsPointCloud\orsIPointCloudReader.h"
#include "CalibBasedef.h"


#ifdef GENPOINTINDEX_EXPORTS
#define  _GenPointIdx_Lib_  __declspec(dllexport)
#else
#define  _GenPointIdx_Lib_  __declspec(dllimport)	
#endif



class _GenPointIdx_Lib_ CKDTPtIndex
{
public:
	CKDTPtIndex(orsIPlatform *platform);
	~CKDTPtIndex();

	bool Open(const char *pLasFileName);	

	ors_uint32 get_point_contentMask();

	//����Բ������Ͱ뾶��������
	void GetPoints(double x0, double y0, double radius, std::vector<LidPt_SurvInfo> *ptDataVec); 

	//��ȡ������ڵĵ�
	void GetPoints(orsPOINT3D *pPolygon, int ptNum, std::vector<LidPt_SurvInfo> *ptDataVec);

	//��ȡ�����
	void GetNNPoint(orsPOINT3D *pt, double radius, std::vector<LidPt_SurvInfo> *ptDataVec, double *NN_dis);

	//��ȡ�����ڵ������ζ���,ͬʱ�ڲ�pt��zֵ�������������
	locateresult GetTriangleVertex(orsPOINT3D *pt, double radius, std::vector<LidPt_SurvInfo> *ptDataVec, double *rmse);

	//	double interpolateByTIN(double x, double y, double radius);

	bool QueryNNPoint(double x, double y, LidPt_SurvInfo *pt);

protected:
	void Release();
	
//	ref_ptr<orsIPointCloudService>  m_pointCloudService;
	orsIPointCloudReader*  m_reader;


private:
	CTINClass *m_pTin;

};


#endif