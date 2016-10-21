#ifndef _ORS_LIDARGEOMETRY_MODEL_RESSL_INCLUDE_H_
#define _ORS_LIDARGEOMETRY_MODEL_RESSL_INCLUDE_H_

#include "orsLidarGeometry/orsILidarGeoModel.h"
#include "orsSRS/orsIEllipsoid.h"
#include "orsSRS\orsISpatialReference.h"
#include "orsMath\orsIMatrixService.h"

class orsXLidarGeoModel_Ressl:  public orsILidarGeoModel, public orsObjectBase
{
private:
	//�ֲ�����ϵ����ΪXYZ -->  ������
//	double		m_rotZ;				//������Z�����ת��
	double		m_rotMatZ[9];		//������Z�����ת����
//	double		m_rotMatZ_Trans[9]; //ת��
	orsPOINT3D  m_stripCenter;		//�����������꣨�ο��㣩

	//unknowns
	double		m_unknowns_ax, m_unknowns_ay, m_unknowns_az;
	double		m_unknowns_aroll;
	double		m_unknowns_ayaw;

	orsLidResslParam	*m_extParam;

private:
	//���ڸ�������ı���
	double		m_XVec[3];
	double		m_YVec[3];
//	int			m_vecLen;

private:
	ref_ptr<orsITangentPlane>	m_pTangentPlane; 

	ref_ptr<orsISpatialReference>  m_orgCoordSys;
	ref_ptr<orsISpatialReference>  m_dstCoordSys;
	ref_ptr<orsICoordinateTransform> m_coordTransform;	//forͶӰת��

	//	ref_ptr<orsISpatialReference> m_pSRS;

	ref_ptr<orsIEllipsoid>	m_wgs84;
	ref_ptr<orsIMatrixService> m_matrixService;

	
	//	orsLidSysExtraParam_TYPE	m_type;
	double m_leverarm[3];	//ƫ�ķ���
	double m_sa0, m_sac;	//ɨ�����ɨ������Ը���
	double m_semiXYZ[3];
	double m_drange;		//������
	double m_Rboresight[9];

public:
	orsXLidarGeoModel_Ressl(bool bForRegister);
	virtual ~orsXLidarGeoModel_Ressl();

	virtual void setSysParam(void *extParam);

	//���㼤��ŵ�����
	inline virtual void cal_Laserfootprint(LidGeo_RawPoint *pdata, ors_int32 ptNum)
	{
		assert(0);
	};

	inline virtual void cal_Laserfootprint(orsPOINT3D *gps_pos, double Rn[9], double range, double sAngle, 
		double *X, double *Y, double *Z)
	{
		assert(0);
	};

	inline virtual void cal_Laserfootprint(LidPt_SurvInfo *pts, int num);

	inline virtual void cal_Laserfootprint(orsPOINT3D *oriPts, orsPOINT3D *dstPts, int num);

// 	inline virtual void cal_Laserfootprint(LidPt_XYZT *oriPts, LidPt_XYZT *dstPts, int num)
// 	{
// 		assert(0);
// 	};

	//����ɨ��б��
	inline virtual void cal_RangeVec(LidPt_SurvInfo *pts, int Num)
	{
		assert(0);
	};


	inline virtual void estimate_range_scanAngle(double *rangeVec, double *range, double *sAngle)
	{
		assert(0);
	};

	//����ƫ����
	inline virtual void calPDC_misalign(void *data, ors_int32 ptNum, 
		/*orsLidSysExtraParam_TYPE type, */void *extParam, double *pPDC);

	virtual bool setSRS(const char *hcsWkt, const char *vcsWkt);

public:
	ORS_OBJECT_IMP1( orsXLidarGeoModel_Ressl, orsILidarGeoModel, "Ressl", "Ressl" );
};


#endif