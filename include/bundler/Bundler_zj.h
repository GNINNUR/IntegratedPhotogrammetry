#ifndef __bundler_extend_interface_by_zj_h__
#define __bundler_extend_interface_by_zj_h__


#include "BundlerApp.h"
#include "BundlerExport.h"
#include "AT_StructureDef.h"


#ifndef _camera_param
#define _camera_param
//����ڲ���
typedef struct camera_param
{
	double x0, y0; 
	double f;
	double k1, k2, k3;
	double p1, p2;
	double _b1, _b2;

	double dx, dy;		//���ش�С
	int		imgWid, imgHei;	//���ش�С
}camera_param;

#else
#pragma message("camera_param already defined!" )
#endif

#ifndef _camera_extorparam
#define _camera_extorparam
typedef struct extor_param
{
	double X, Y, Z;
	double f, w, k;

	double R[9];

}extor_param;
#endif

void calcImgProj(camera_param camInfo, double t[3], double R[9], double objpt[3], double imgpt[2]);

_Bunder_dll_ std::vector<KeypointMatch> LoadMatchTable(const char *filename, int i1, int i2);

class _Bunder_dll_ CBundler : public BundlerApp
{
public:
	CBundler();
	~CBundler();

	//����Ӱ���б�
	bool	SetImageNameList(const char*  imgList);		//�ļ���
	bool	SetImageNameList(vector<string>*  imgList); //�б�

	//�����������б�
	bool	SetKeyNameList(const char* keyList);		//�ļ���
	bool   SetKeyNameList(vector<string>* keyList); //�б�


	int		LoadImagePOSFile(const char*  pFileName);
	void  SetCameraInfo(camera_param camInfo);


	//����ƥ���ԣ�i1, i2Ϊ��Ӧ��Ӱ���  index is 0 based
	bool  SetMatchTable_xy(vector<KeypointMatch> *pMatches, int i1, int i2);


	/******�Զ�����*******/

	//һ����Լ�����ԣ�ɾ�����һ��ƥ����Լ�������ͬ��ƥ���
	void  ConsistencyConstraints(const char *initMatchFile, const char *pruneMatchFile);

	//�߶�Լ����⣬��Ԥ��֪����Ƭ�ĳ߶�ʱ��ɾ���߶Ȳ���ϴ��ƥ���
	void  ScaleConstraints(const char *initMatchFile, const char *pruneMatchFile/*, double scaleThresh*/);

	//����Լ����������Ƭ֮������ǹ̶�  todo 5.24
	void  OrientationConstraints(const char *initMatchFile, const char *pruneMatchFile/*, double scaleThresh*/);

	//����F������к���Լ��
	void  EpipolarConstraints(const char *initMatchFile, const char *pruneMatchFile, camera_param camInfo, int thresh=9);


	//�������úõ�ƥ����Խ��к���Լ��   �ȵ���SetMatchTable_xy()�������ʼƥ�������ý�ȥ
	void  EpipolarConstraints(camera_param camInfo, int thresh=9);

	//���õ�Ӧ������У������﷽��Ĺ������������Լ��
	void  HomographyConstraints(const char *initMatchFile, const char *pruneMatchFile, camera_param camInfo, int thresh=6);

	//������Զ������� 2����Ƭ
	void  RelativePose2Constraints();

	//��ƬԼ�� (����任)
	void  TripleConstraints_Affine(const char *tracksFile, const char *pruneMatchFile, camera_param camInfo, int reprojth=4);

	//��POS����Ƭ���
//	void  TripleConstraints_POS(const char *initMatchFile, const char *pruneMatchFile, camera_param camInfo, int reprojth=4);

	/*******************************************************/
	//***************�������ӵ�******************************/
	void  TrackTiePoints(const char *initMatchFile, const char *tiePFile/*, camera_param camInfo*/);

	void  GetMultiViewerTiePoints(const char *filename, const char *multiTiePtsName);

	void  RefineMultiViewerTiePoints(const char *filename, const char *multiTiePtsName);

//	void  ExportATImgPair();

	/******************************************************/                                                                     
	/******************************************************/
	/***********************����***************************/

	
private:
	//void  PruneMultiMatches();
	//����ƥ���ԣ�bLoadKeys�Ƿ���������ļ��ж�ȡ������
	bool  WriteMatchTable_xy(const char *filename, bool bLoadKeys=true);
	
	bool  LoadMatchTable_xy(const char *filename);
	

	//��׷�ٵõ������ӵ�д���ļ�
	bool  WriteTrackPoints(const char *filename);

	bool	ReadTrackPairs(const char *filename, AT_TiePoint *&pTiePts, int &tiePtsNum);
	bool  WriteTrackPairs(const char *filename, AT_TiePoint *pTiePts, int tiePtsNum);
	
	//����ת���
	void  Image2Photo(double xi, double yi, double& xp, double& yp);

	//���ת����
	void  Photo2Image(double xp, double yp, double& xi, double& yi);

	//��������������(ת��Ϊphoto����)
	void LoadKeys_TransPhotoCoord(bool descriptor = false);
	//ֱ�Ӽ���������(��������)
	void LoadKeys_PixelCoord(bool descriptor = false);

	void DoTrack();

	void intersection(AT_3DTiePoint *pData, int num, int lImgID, int rImgID);

	void projection(AT_3DTiePoint *pData, int num, AT_ImgPoint *pImgPts);

private:
	char m_pImgNameList[1024];
	char m_pKeyNameList[1024];

	// interior orientation
	double m_inorImage_a[3];	// pixel to photo
	double m_inorImage_b[3];

	double m_inorPixel_a[3];	// photo to pixel
	double m_inorPixel_b[3];

	camera_param  m_camInfo;
	extor_param	*m_pCamExor;
	int		m_imgNum;
	bool	m_bSetPOS;
};




#endif