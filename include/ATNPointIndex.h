//ATN�ļ��ķֿ���������߲��Ҷ�λ��Ч��
#ifndef _ATN_Point_Index_h_zj_2010_09_27_
#define _ATN_Point_Index_h_zj_2010_09_27_

#include <vector>
#include "tin\tinClass.h"
#include "orsBase\orsString.h"
#include "ATN_def.h"
#include "orsPointCloud\orsIPointCloudReader.h"
#include "CalibBasedef.h"


#ifdef POINTINDEX_EXPORTS
#define  _PointIdx_Lib_  __declspec(dllexport)
#else
#define  _PointIdx_Lib_  __declspec(dllimport)	
#endif

// #ifndef _LIDCALIB_
// #ifdef _DEBUG
// #pragma comment(lib,"LidCalib.lib")
// #else
// #pragma comment(lib,"LidCalib.lib")
// #endif
// #endif

//using namespace std;

class _PointIdx_Lib_ CATNPtIndex
{
public:
	CATNPtIndex(orsIPlatform *platform);
	~CATNPtIndex();
	
	//�Ե����ļ������ֿ����������ٲ���
	bool CreateQTIdx(const char *pszOrgFileName, const char *pszIdxFileName, double gridSize=5.0);
	
	bool OpenIdx(const char *pszIdxFileName);
	bool Open(const char *pATNFileName);	//����ATN�ļ����������ļ�

	//����Բ������Ͱ뾶��������
	void GetPoints(double x0, double y0, double radius, std::vector<ATNPoint> *ptDataVec); 
	//��ȡ������ڵĵ�
	void GetPoints(POINT3D *pPolygon, int ptNum, std::vector<ATNPoint> *ptDataVec);

	void GetPoints(POINT3D *pPolygon, int ptNum, std::vector<LidPt_SurvInfo> *ptDataVec);

	//��ȡ�����
	void GetNNPoint(POINT3D *pt, double radius, std::vector<ATNPoint> *ptDataVec, double *NN_dis);

	//��ȡ�����ڵ������ζ���,ͬʱ�ڲ�pt��zֵ�������������
	locateresult GetTriangleVertex(POINT3D *pt, double radius, std::vector<ATNPoint> *ptDataVec, double *rmse);

//	double interpolateByTIN(double x, double y, double radius);

	bool QueryNNPoint(double x, double y, ATNPoint *pt);
	
protected:
	void Release();
	//����������ȡһ��
	ATNPoint* GetOneBlock(double x, double y, int &num);

	ref_ptr<orsIPointCloudReader>  m_reader;
		   
private:
	CTINClass *m_pTin;


private:
	orsString m_strBlkName;	//�ֿ��ļ���
	D_RECT	m_boundary;		//�߽�
	double  m_gridSize;		//�����С
	int     m_gridRow;		//��������
	int		m_gridCol;		//��������
	int		m_gridNum;		//����������Ӧ���ұ���(�����п��ܴ��ڿ�����)
	long   *m_pBlockLut;	//�ֿ���ұ�
	int		m_blkNum;		//����(�������ݵ�������)

	LidBlock *m_pLidBlock;	//�ֿ�����
};


#endif