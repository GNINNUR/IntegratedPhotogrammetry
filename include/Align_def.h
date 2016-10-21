#ifndef _Alignment_Define_h_zj_2010_10_06_
#define _Alignment_Define_h_zj_2010_10_06_

//#include "vld.h"

#include "Afxtempl.h"
#include "LidBaseDef.h"
//#include "orsBase/orsString.h"
#include "orsBase/orsTypedef.h"


//#pragma pack(8) 

#define sFactor 1000L		//��������
//#define sFlag	10000000L	//�����1000��������ÿ�������1000��Ӱ��

inline int getStripID(int ID)
{
	return (ID /*- sFlag*/) / sFactor;
};

inline int getPhotoID(int ID)
{
	return (ID /*- sFlag*/) % sFactor;
};

inline int getUniqueID(int stripID, int photoID)
{
	return /*sFlag+*/stripID*sFactor+photoID;
};

#ifndef _Lid_Line_Def_
#define _Lid_Line_Def_
typedef struct Align_LidLine		//LiDARɨ������
{	
	int LineID;				//(��1��ʼ)
	CString GridName;		//���ƶ�Ӧ�ĸ����ļ���
	CString LasName;		//��׼��las�ļ���
//	CString AtnName;        //Atn�ļ���
	CString TrjName;		//trj�ļ���
	double  averFH;			//ƽ������
} Align_LidLine;
#endif

#ifndef _Align_Image_Def_
#define _Align_Image_Def_
typedef struct Align_Image				//��Ӱ�񺽴���Ϣ
{
	int nImgID;                        //Ӱ������(��) (��0001��ʼ) sFlag+nStripID*1000+imgID 
	CString ImgName;					//Ӱ����
// 	int     nStripID;                  //�ߺ�(������,��1��ʼ)
// 	int 	nPhoID;                    //Ƭ��(Ӱ���ں����ڵ�ID,��1��ʼ)  
	int    nIsReversal;                //�Ƿ���ת180��
	double timestamp;				   //��Ƭ��ʱ�� 2014.12.23 add
}Align_Image;
#endif

//#ifndef _Tie_Object_Def_
//#define _Tie_Object_Def_
typedef enum 
{
    ST_LiDAR=0,	//3D����
	ST_Image=1	//2D����
} Source_Type;

typedef enum
{
    TO_POINT=0, TO_LINE=1, TO_PATCH=2
} TieObj_Type; 

//tie point type
#ifndef Tiepoint_Type
#define Tiepoint_Type
#define  TP_type_undefine		0
#define  TP_type_vertical		1		//�̵߳�
#define  TP_type_horizon		2		//ƽ���
#define  TP_type_hor_ver		3		//ƽ�ߵ�
#endif

class TieObject
{
public:
	TieObject(){};
	virtual ~TieObject(){};


public:
	int sourceID;	//��������ID (Ӱ��/LiDAR)   ��1��ʼ
	Source_Type sourceType;	//lidar, image
	int	tieID;		//add 2010.10.13 by zj ֧�ַ�����ҡ��ڶ���ͼ���ڵĵ���׷�ٻ���   ��1��ʼ
	TieObj_Type objType;		//���ͣ�POINT/LINE/PATCH
};

class TiePoint : public TieObject
{
public:
	TiePoint() 
	{ 
		tpType=TP_type_undefine; 
	};

	char tpType;
	orsPOINT2D pt2D;
	orsPOINT3D pt3D;
};

class TieLine : public TieObject
{
public:
	TieLine() { pt2D=0; pt3D=0; };
	virtual ~TieLine() 
	{ 
		if(pt2D)	delete pt2D; pt2D=0; 
		if(pt3D)	delete pt3D; pt3D=0;
	};

	int ptNum;
	orsPOINT2D *pt2D;
	orsPOINT3D *pt3D;
};

class TiePatch : public TieObject
{
public:

	TiePatch() { pt2D=0; pt3D=0; };
	virtual ~TiePatch() 
	{ 
		if(pt2D) delete pt2D; pt2D=0; 
		if(pt3D) delete pt3D; pt3D=0;
	};

	int ptNum;
	orsPOINT2D *pt2D;
	orsPOINT3D *pt3D;
};

// typedef struct TieObject
// {
// 	int sourceID;			//��������ID
// 	char type;				//���ͣ�POINT/LINE/PATCH
// 	union 	
// 	{
// 		POINT2D tiePoint;	//������	
// 		POINT2D *tieLine;
// 		POINT2D *tiePatch;
// 	}obj;
// }TieObject;
//#endif

//��Ƭ���ӵ�(�ߡ���)����
class TieObjChain
{
public:
	int TieID;				//���ӵ�ID ��1��ʼ
	TieObj_Type type;
	bool bDel;
	int  objNum;
	CArray<TieObject*, TieObject*> *pChain;
	
	bool bGCP;
	orsPOINT3D gcp;

	TieObjChain() 
	{ 
		TieID=-1;
		type=TO_POINT;
		objNum=0;
		pChain=NULL; 
		bDel=false;
		bGCP=false;
	}

	virtual ~TieObjChain()
	{
		if(pChain)
		{
			TieObject *pObj;
			for(int i=0; i<pChain->GetSize(); i++)
			{
				pObj=pChain->GetAt(i);
				delete pObj;
			}
			pChain->RemoveAll();

			delete pChain;
		}
		pChain=NULL; 
		bDel=false;
		bGCP=false;
	}

};

//������Ƶ�
// typedef struct TieObj_GCP
// {
// 	POINT3D gcp;
// 	int tieObjID;
// }TieObj_GCP;

enum ORS_LINKMSG	{
	ORS_LM_LINK,
		ORS_LM_MOUSEMOVE,
		ORS_LM_ObjType,				//�������������
		ORS_LM_TOMeasure_Exit,		//������ֹ
		ORS_LM_RegistViewer_Exit,
		ORS_LM_Measure_Info			//����������Ϣ
};

struct linkMSG 
{
	UINT id;   //��Ϣ����
	
	int sourceID;	//��������ID (Ӱ��/LiDAR)
	Source_Type sourceType;	//lidar, image
	int	tieID;		//add 2010.10.13 by zj ֧�ַ�����ҡ��ڶ���ͼ���ڵĵ���׷�ٻ��� 
	TieObj_Type objType;		//���ͣ�POINT/LINE/PATCH

	
	int ptNum;	//�����ܵ���
	int ptID;	//��ǰ���(��0��ʼ)
	orsPOINT2D pt2D;	//�������
	orsPOINT3D pt3D;	//�﷽����

	double zoom;
	UINT nWindows;
	HWND hLinkWindows[32];
	UINT bWindowLinkOn[32];
};

// static int linkMSGLen(int ptNum)
// {
// 	int len;
// 	len=sizeof(UINT)+sizeof(TieObj_Type)+sizeof(int)+sizeof(POINT2D)*ptNum+sizeof(int)+sizeof(double)
// 		+sizeof(UINT)+sizeof(HWND)*32+sizeof(UINT)*32;
// 
// 	return len;
// }

//���outlier
#define keypoint_outlier	0x02
#define keypoint_inlier	0x03

struct Align_Keypoint
{
	UINT ID;
	orsPOINT2D pt2D;		//��������
	orsPOINT3D pt3D;		//�﷽����
	char flag;
};

// struct Keypoint3D
// {
// 	ors_uint32 ID;
// 	double x, y, z;		//�﷽����
// 	ors_byte flag;
// };


//#pragma pack()
#endif