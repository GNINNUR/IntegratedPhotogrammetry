
#if !defined(_Ors_Trajectory_trj_H_INCLUDED_)
#define _Ors_Trajectory_trj_H_INCLUDED_


#include "orsSRS/orsIEllipsoid.h"
#include "orsLidarGeometry/orsITrajectory.h"
#include "orsLidarGeometry/orsIPOS.h"

/*
//	�����ļ����壺��TerraScan User��s Guide�� pp162
*/
//The structure of trj file header
typedef struct {
	char Recog[8] ; // TSCANTRJ
	ors_int32 Version ; // File version 20010715
	ors_int32 HdrSize ; // sizeof(TrajHdr)
	ors_int32 PosCnt ; // Number of position records
	ors_int32 PosSize ; // Size of position records
	char Desc[79] ; // Description
	ors_byte Quality ; // Quality for whole trajectory (1-5)
	double BegTime ; // First time stamp
	double EndTime ; // Last time stamp
	ors_int32 OrigNbr ; // Original number (before any splitting)
	ors_int32 Number ; // Flightline number (in laser points)
	char VrtVideo[400] ; // Vertical facing video
	double VrtBeg ; // Start time of VrtVideo[]
	double VrtEnd ; // End time of VrtVideo[]
	char FwdVideo[400] ; // Forward facing video
	double FwdBeg ; // Start time of FwdVideo[]
	double FwdEnd ; // End time of FwdVideo[]
} TrajHdr ;

//The structure of the trajectory position records is:
typedef struct {
	double Time ; // Time stamp (seconds in some system)

	//lat, lon, h
	double x;	//Position	�뵼��ʱѡ���ͶӰ�й�  (��γ��ʱΪradian)
	double y;
	double z;
	double heading ; // Heading (deg)
	double roll ; // Roll (deg)
	double pitch ; // Pitch (deg)
	ors_int32 Quality ; // Quality tag value (0-5)
	ors_int32 Mark ; // Run time flag
} TrajPos ;	

class orsXTrajectory_trj:  public orsITrajectory, public orsObjectBase 
{
private:
	orsArray<TrajPos> m_rawTraj;
//	orsArray<TrajPos> m_userTraj;//�û�����ϵ�µĺ���
	TrajHdr	m_TrjHeader;

// 	orsImageGeometryCoordSysTYPE  m_rawCoordType;
// 	orsImageGeometryCoordSysTYPE  m_userCoordType;
// 
// 	ref_ptr<orsISpatialReference>  m_orgCoordSys;
// 	ref_ptr<orsISpatialReference>  m_dstCoordSys;
// 	ref_ptr<orsITangentPlane>	m_pTangentPlane; 
// 
// 	ref_ptr<orsICoordinateTransform> m_coordTransform;	//forͶӰת��
// 
// 	double m_Anchor_lat0, m_Anchor_lon0, m_Anchor_h0;
// 	double m_CentralMeridian;	//unit: radian
// 	double m_RRaw2User[9];

	ref_ptr<orsIPOS>	m_posTrans;

	bool	m_bTransform;

private:
	bool read_trjfile(const char *filename);
	void findTimeSlice(double t, int &sIdx, int &eIdx);

	void interpolationXYZ_Lagrange(double t, double t0, double t1, double v0[3], double v1[3], double v[3]);
	void interpolationRotate_Lagrange(double t, double t0, double t1, double R0[9], double R1[9], double R[9]);
//	void interpolationRotate_Lagrange_dpsMatrix(double t, double t0, double t1, double v0[3], double v1[3], double v[3]);

// 	void coordinate_transform(double pos_from[3], double pos_to[3]);
// 	void attitude_transform(double R_from[9], double lat, double lon, double R_to[9]);

public:
	orsXTrajectory_trj( bool bForRegister );
	virtual ~orsXTrajectory_trj();

	virtual bool Initialize( const orsChar *imageFileName );
	virtual bool Initialize(std::vector<orsPOSParam> *posVec) {return false;};

	virtual int	GetOrder() const { return 1; };

// 	virtual void GetXsYsZs( double t, double *Xs, double *Ys, double *Zs );
// 	virtual void GetRotateMatrix( double t, double *R );

	virtual void Interpolate_linear(double t, orsPOINT3D *gps_pos, double *R);

	virtual void Interpolate_linear(orsPOSParam *pos_t, orsPOSParam *pos0, orsPOSParam *pos1){assert(false);};

	virtual bool SetRayCoordType( orsImageGeometryCoordSysTYPE coordType ) 
	{
//		m_rawCoordType = coordType;
		m_posTrans->SetRayCoordType(coordType);
		return true;
	}
	virtual orsImageGeometryCoordSysTYPE GetRayCoordType()const
	{
		return ORS_igctUnknown;
	}

	virtual bool SetUserCoordType( orsImageGeometryCoordSysTYPE coordType )
	{
// 		m_userCoordType = coordType;
// 
// 		if(m_userCoordType != m_rawCoordType)
		m_posTrans->SetUserCoordType(coordType);

		m_bTransform = true;

		return true;
	}

	virtual orsImageGeometryCoordSysTYPE GetUserCoordType() const
	{
		return ORS_igctUnknown;
	}

	//��������ϵԭ�㣨��ƽ���ͶӰ����ϵ��
	virtual void SetTangentPlane_AnchorPoint(double lat0, double long0, double h0);
	virtual void SetCentralMeridian(double meridian) { m_posTrans->SetCentralMeridian(meridian); };

	// �������굽�û������ת��
	virtual bool RayCoord2UserCoord( double X, double Y, double Z,  double *xUser, double *yUser, double *zUser )const;

	// �û����굽���������ת��
	virtual bool UserCoord2RayCoord( double xUser, double yUser, double zUser,  double *X, double *Y, double *Z ) const;

	// �����﷽����ϵͳ������ɹ�����ģ��ʵ������ϵ��ת��
	virtual bool SetSRS( const char *hcsWkt, const char *vcsWkt );

	virtual orsISpatialReference *GetSpatialReference();


	virtual void Project( double X, double Y, double Z, double *x, double *y ){ assert(false); };
	virtual void IntersectWithZ( double x, double y, double Z, double *X, double *Y ){ assert(false); };
	virtual bool RayVector( double xi, double yi, double Z, double *a, double *b, double *c ){return false;};
	virtual bool GetImagingRay( double xi, double yi, orsRAY *image_ray){return false;};
	virtual bool RayVector_3D( double X, double Y, double Z, double *a, double *b, double *c ){return false;};
	virtual void GetLinearMatrix( double x, double y, orsMatrixD &mA, orsVectorD &vL){};
	virtual void PartialDerivative_XYZ( double xg, double yg, double zg, orsPD_PIXEL_XYZ *pd ){};
	virtual void SpaceIntersectMatrix( double xg, double yg, double zg, orsMatrixD &mA, orsVectorD &vL){};

	virtual bool IntersectWithXY( double xi, double yi, double X, double Y, double *Z ){return false;};
	virtual double IntersectWithLine( double xi, double yi, const orsLINE3D &line ){return 0.0;};
	virtual void SetIntersectPlane( const orsPLANE *plane ) {};
	virtual bool IntersectWithPlane( double xi, double yi, orsPOINT3D *point ){return false;};
	virtual bool PartialDerivative_Plane( double xi, double yi, orsPD_XYZ_PLANE *pd, orsPOINT3D *point ){return false;};
	virtual bool IsVertical( double xi[2], double yi[2], double distTh ){return false;};

	// ȡ�����߳����ϵĵ���������
	virtual float  GetPixelGSD( double Z = 0) {return 0;};
	virtual float  GetResolution_X( double Z=0 ) {return 0;};
	virtual float  GetResolution_Y( double Z=0 ) {return 0;};

	// ȡ�����̵߳ĸ̷ֱ߳���
	virtual float  GetResolution_Z( double Z = 0 ) {return 0;};

	// ȡ�̷߳�Χ
	virtual double GetMeanZ() const {return 0;};
	virtual double GetMinZ() const {return 0;};
	virtual double GetMaxZ() const {return 0;};

	// ���и߶�
	virtual double GetFlyHeight() const {return 0;};
	
public:

	//
	// �Ѿ�����ĳ���ģ��������ļ����ļ���Ϊ��ʱ����ԭʼ�ļ�
	// ��ʱ��������
	virtual bool Export( const orsChar *fileName = NULL ) {return false;};

public:
	ORS_OBJECT_IMP1( orsXTrajectory_trj, orsITrajectory, "trj", "terrascan trj file" );
};


//gDPS_DLL_API orsXStereoModel_refImg *LoadStereoModel( const char *modelName );

#endif
