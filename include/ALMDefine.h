//ALM�������ݽṹ����

#ifndef _ALM_PROJECT_BASE_DEFINE_H_
#define _ALM_PROJECT_BASE_DEFINE_H_


#include "Afxtempl.h"

//////////////////////////////////////////////////////////////////////////
#include <math.h>
#include <io.h>
#include <stdlib.h>
#include <stdio.h>

#include <string>
#pragma   warning(disable:4786)
#include <vector>
#pragma   warning(disable:4251)
#include <algorithm> 
#include <functional>  
using namespace::std;

#ifndef PATHLEN
#define PATHLEN  512
#endif

#ifndef NAMELEN
#define NAMELEN  128
#endif

#ifndef IDLEN
#define IDLEN    16
#endif

#ifndef PI
#define PI       3.1415926535897932384626433832795
#endif

#ifndef EI
#define EI 2.718281828459045f
#endif

#ifndef NONEIDX
#define NONEIDX  -999
#endif

#ifndef DEMNoDataValue
#define DEMNoDataValue -99999
#endif

#ifndef _StripInfo
#define _StripInfo
typedef struct tagStripInfo  
{
	CArray<CString, CString&>	ImageList;
	CArray<CString, CString&>	LasList;

	tagStripInfo();
	~tagStripInfo()
	{
		ImageList.RemoveAll();
		LasList.RemoveAll();
	}
}StripInfo,*PStripInfo;
#else
#pragma message("StripInfo�Ѿ�����!" )
#endif


#ifndef _BoundingBox2D
#define _BoundingBox2D
typedef struct tagBoundingBox2D 
{
	double	XMax;
	double	XMin;
	double	YMax;
	double	YMin;
	tagBoundingBox2D()
	{
		XMax=0.0;
		XMin=0.0;
		YMax=0.0;
		YMin=0.0;
	}
}BoundingBox2D,*PBoundingBox2D;
#else
#pragma message("BoundingBox2D�Ѿ�����!" )
#endif

#ifndef _BoundingBox3D
#define _BoundingBox3D
typedef struct tagBoundingBox3D    //��ά��Χ��
{
	double	XMax;
	double	XMin;
	double	YMax;
	double	YMin;
	double	ZMax;
	double	ZMin;
	tagBoundingBox3D()
	{
		XMax=0.0;
		XMin=0.0;
		YMax=0.0;
		YMin=0.0;
		ZMax=0.0;
		ZMin=0.0;
	}
}BoundingBox3D,*PBoundingBox3D;
#else
#pragma message("BoundingBox3D�Ѿ�����!" )
#endif

#ifndef _PT2D
#define _PT2D
typedef struct tagPT2D        //��ά��ṹ
{
	double   dfX;
	double   dfY;
}PT2D,*PPT2D;
#else
#pragma message("PT2D�Ѿ�����!" )
#endif

#ifndef _PT3D
#define _PT3D
typedef struct tagPT3D       //��ά��ṹ
{
	double   dfX;
	double   dfY;
	double   dfZ;
}PT3D,*PPT3D;
#else
#pragma message("PT3D�Ѿ�����!" )
#endif

#ifndef _PT4D
#define _PT4D
typedef struct tagPT4D        //��ά��ṹ
{
	double   dfX;
	double   dfY;
	double   dfZ;
	double   dfAlph;
}PT4D,*PPT4D;
#else
#pragma message("PT4D�Ѿ�����!" )
#endif

#ifndef _MARKCAM
#define _MARKCAM
typedef struct tagMARKCAM      //��־�Ľṹ
{
	double dfX;
	double dfY;
	int    nMarkNO;	
}MARKCAM,*PMAKCAM;
#else
#pragma message("MARKCAM�Ѿ�����!" )
#endif

#ifndef _IMGPARA
#define _IMGPARA
typedef struct tagIMGPARA       //Ӱ��·��
{	
	int  nIndex;	            //Ӱ������
	char chImgPath[PATHLEN];    //Ӱ��ȫ·��
}IMGPARA,*PIMGPARA;
#else
#pragma message("IMGPARA�Ѿ�����!" )
#endif

#ifndef _STRIPPARA
#define _STRIPPARA
typedef struct tagSTRIPPARA            //��������
{
	int             nStripImgNum;      //����Ӱ����
	int             nIsReversal;       //�Ƿ���ת180��
	vector<IMGPARA> vImgPara;          //������Ӱ���б�
}STRIPPARA,*PSTRIPPARA;
#else
#pragma message("STRIPPARA�Ѿ�����!" )
#endif

#ifndef _IMGLIST
#define _IMGLIST
typedef struct tagIMGLIST              //��Ӱ�񺽴���Ϣ
{
	int nImgID;                        //Ӱ������(��) (��1��ʼ) 
//    char    chImgPath[PATHLEN];      //Ӱ��ȫ·��
//	char    chImgName[PATHLEN];        //Ӱ����
	CString strImgName;                //Ӱ����
	int     nStripID;                  //�ߺ�(������,��1��ʼ)
	int 	nPhoID;                    //Ƭ��(Ӱ���ں����ڵ�ID,��1��ʼ)  
	int    nIsReversal;                //�Ƿ���ת180��

}IMGLIST,*PIMGLIST; 
#endif

#ifndef _LIDARINFO
#define _LIDARINFO
typedef struct tagLIDARINFO              //LAS������Ϣ
{
//	char    chLasName[PATHLEN];        //Ӱ����
	CString strLidarName;                //Ӱ����
	int     nStripID;                  //�ߺ�(������,��1��ʼ)
	tagBoundingBox3D  dfBox3D;         //������Χ

	int    nFlag;                      //��ʶ
}LIDARINFO,*PLIDARINFO; 
#endif

// #ifndef _STRIPLIST
// #define _STRIPLIST
// typedef struct tagSTRIPLIST
// {   	
// 	int      camRev;
// 	int      imgNum;
// 	PIMGLIST pImgLst;	
// }STRIPLIST,*PSTRIPLIST; 
// #endif
// 
// #ifndef _OBVPARA
// #define _OBVPARA
// typedef struct tagOBVPARA
// {   	
// 	unsigned int  nID;	
// 	int           phSN;
// 	int           flag;
// 	double        mx;
// 	double        my;
// 	double        px;
// 	double        py;	
// }OBVPARA,*POBVPARA; 
// #endif
// 
// #ifndef _TIEPARA
// #define _TIEPARA
// typedef struct tagTIEPARA
// {
// 	int starIdx;
// 	int endIdx;
// }TIEPARA,*PTIEPARA; 
// #endif


// struct PHOTOINFO
// {
// 	char name[512];	//Ӱ����
// 	int row;		//Ӱ���к�
// 	int col;		//Ӱ���к�
// 	BOOL sel;		//�Ƿ�ѡ��
// 	BOOL rotate;	//�Ƿ���Ҫ��ת
// 
// 	PHOTOINFO()
// 	{
// 		memset(&name,0,512*sizeof(char));
// 		row=col=-1;
// 		sel=false;
// 		rotate=false;
// 	}
// 
// 	PHOTOINFO(char Name[512],int Row,int Col,BOOL Sel=false,BOOL Rotate=false)
// 	{
// 		strcpy(name,Name);
// 		row=Row;
// 		col=Col;
// 		sel=Sel;
// 		rotate=Rotate;
// 	}
// 
// 	PHOTOINFO(CString Name,int Row,int Col,BOOL Sel=false,BOOL Rotate=false)
// 	{
// 		strcpy(name,Name);
// 		row=Row;
// 		col=Col;
// 		sel=Sel;
// 		rotate=Rotate;
// 	}
// };
#ifndef _CAMPARA
#define _CAMPARA
typedef struct tagCAMPARA         //������������ݽṹ
{
	double dfU0,dfV0;             //����(����)
	double dfF;			          //����(����)

	int nWidth,nHeight;           //�������(����)
	double dfPixSize;			  //��Ԫ��С
	double dfWidth,dfHeight;      //�������(����)

//	int unit,origin;		      //����ϵ����		unit ��λ: 0,����; 1,����		
	                              //origin ԭ��:0,���½�; 1,���ĵ�

	int nDistortionNum;		      //����ģ�� 3,3��������ģ��; 4,4��������ģ�� 
	double dfA0,dfA1,dfR;		  //3���������
	double dfK1,dfK2,dfP1,dfP2;   //4���������
	double dfK3,dfB1,dfB2;        //Lecia�������

	int nAttrib;                  //�������

	tagCAMPARA()
	{
		dfU0=dfV0=dfF=0.0;
		nWidth=nHeight=0;
		dfPixSize=0.0;
		dfWidth=dfHeight=0.0;
		nDistortionNum=4;
		dfK1=dfK2=dfP1=dfP2=0.0;
// 		unit=1;
// 		origin=1;
		dfA0=dfA1=dfR=0.0;
		dfK3=dfB1=dfB2=0.0;

		nAttrib=1111;
	}

	tagCAMPARA(double F,double U0,double V0, double dfW=0.0,double dfH=0.0, double PS=0.0,
		int W=0,int H=0, double K1=0.0,double K2=0.0,double K3=0.0,double P1=0.0,
		double P2=0.0,double B1=0.0,double B2=0.0)
	{
		dfF=F;		
		dfU0=U0;
		dfV0=V0;
		dfWidth=dfW;
		dfHeight=dfH;
		
		dfPixSize=PS;
		nWidth=W;
		nHeight=H;
		dfK1=K1;
		dfK2=K2;
		dfP1=P1;
		dfP2=P2;
		
		dfK3=K3;
		dfB1=B1;
		dfB2=B2;
	}
	void operator = (const tagCAMPARA &camera)
	{
		nDistortionNum=camera.nDistortionNum;
		dfF=camera.dfF;
		dfU0=camera.dfU0;
		dfV0=camera.dfV0;
		dfK1=camera.dfK1;
		dfK2=camera.dfK2;
		dfK3=camera.dfK3;
		dfP1=camera.dfP1;
		dfP2=camera.dfP2;
		dfB1=camera.dfB1;
		dfB2=camera.dfB2;
		dfA0=camera.dfA0;
		dfA1=camera.dfA1;
		dfR=camera.dfR;
		dfPixSize=camera.dfPixSize;
		dfWidth=camera.dfWidth;
		dfHeight=camera.dfHeight;
	}
}CAMPARA,*PCAMPARA;
#else
#pragma message("CAMPARA�Ѿ�����!" )
#endif

#ifndef _OUTPARA
#define _OUTPARA
typedef struct tagOUTPARA		//Ӱ���ⷽλԪ�ص����ݽṹ
{
	char chImgName[PATHLEN];    //Ӱ����
	double dfXs,dfYs,dfZs;		//��Ӱ���������GPS����
	double dfPhi,dfOmg,dfKapa;  //������Ԫ�� (phi-omg-kapaת��ϵͳ)
	double dfGPSTime;           //GPSʱ��(��)
	int nFlag;                  //��־

	tagOUTPARA()
	{
		dfXs=dfYs=dfZs=0.0;
		dfPhi=dfOmg=dfKapa=0.0;
		dfGPSTime=0;
		nFlag=0;
		chImgName[0]='\0';
	}
	tagOUTPARA(double XS,double YS,double ZS,
		double FAI,double OMG,double KAP,double GPST=0,int Flag=0)
	{
		dfXs=XS;
		dfYs=YS;
		dfZs=ZS;
		dfPhi=FAI;
		dfOmg=OMG;
		dfKapa=KAP;
		dfGPSTime=GPST;
		nFlag=Flag;
	}
}OUTPARA,*POUTPARA;
#else
#pragma message("OUTPARA�Ѿ�����!" )
#endif

#ifndef _IMGPOINT
#define _IMGPOINT
typedef struct tagIMGPOINT		        //������ݽṹ
{
	long lID;				//Ӱ���ĵ��
	int nImgID;				//Ӱ���λ�ڵ�Ӱ���
	double dfX,dfY;  		//��������

	tagIMGPOINT()
	{
		lID=-1;
		nImgID=-1;
		dfX=dfY=0.0;
	}
	tagIMGPOINT(long Id,int Imgid,double X,double Y)
	{
		lID=Id;
		nImgID=Imgid;
		dfX=X;
		dfY=Y;
	}
}IMGPOINT,*PIMGPOINT;
#else
#pragma message("IMGPOINT�Ѿ�����!" )
#endif

#ifndef _CONTROLPOINT
#define _CONTROLPOINT
typedef struct tagCONTROLPOINT		 //�ռ���Ƶ����ݽṹ
{
	unsigned int nName;              //����
	double dfX,dfY,dfZ;			     //�ռ�����
	char chName[IDLEN];			     //���Ƶ���
	int nAttrib;			//0 һ��㣻1 ƽ�߿��Ƶ㣻2 ���㣻
	                        //3 ƽ����Ƶ㣻4 �߳̿��Ƶ�
	int nGroup;	            //������Ϣ
	
	tagCONTROLPOINT()
	{
		nName=999000;
		chName[0]='\0';
		dfX=dfY=dfZ=0.0;
		nAttrib=0;
		nGroup=0;
	}
}CONTROLPOINT,*PCONTROLPOINT;
#else
#pragma message("CONTROLPOINT�Ѿ�����!" )
#endif

#ifndef _PCONTROLPOINT
#define _PCONTROLPOINT
typedef struct tagPHOTOCONTROLPT		//Ӱ����Ƶ����ݽṹ
{
	char chName[IDLEN];		//���Ƶ���
	int nPhotoID;			//����Ӱ���
	double dfx,dfy;			//ƽ������

	tagPHOTOCONTROLPT()
	{
		chName[0]='\0';
		nPhotoID=-1;
		dfx=dfy=0.0;
	}
}PHOTOCONTROLPT,*PPHOTOCONTROLPT;
#else
#pragma message("PCONTROLPOINT�Ѿ�����!" )
#endif

#ifndef _SPACEPOINT
#define _SPACEPOINT
typedef struct tagSPACEPOINT		//�ռ�����ݽṹ
{
	long lID;				        //�ռ��ĵ��
	double dfX,dfY,dfZ;			    //�ռ������

	tagSPACEPOINT()
	{
		lID=-1;
		dfX=dfY=dfZ=0.0;
	}
	tagSPACEPOINT(long Id,double x,double y,double z)
	{
		lID=Id;
		dfX=x;
		dfY=y;
		dfZ=z;
	} 
}SPACEPOINT,*PSPACEPOINT;
#else
#pragma message("SPACEPOINT�Ѿ�����!" )
#endif

// #ifndef _POINT2D_Def_
// #define _POINT2D_Def_
// typedef struct tagPOINT2D		//��ά�����ݽṹ
// {
// 	double x,y;
// }PPOINT2D;
// #else
// #pragma message("POINT_2D�Ѿ�����!" )
// #endif

// #ifndef _LINE2D
// #define _LINE2D
// typedef struct tagLINE2D		//��άֱ�����ݽṹ
// {
// 	POINT2D PTbegin;        	//���
// 	POINT2D PTend;	            //�յ�
// 
// 	tagLINE2D()
// 	{
// 		memset(&PTbegin, 0, sizeof(POINT2D));
// //		memset(&PTend, 0, sizeof(POINT2D));
// 	};
// 	tagLINE2D(POINT2D b,POINT2D e)
// 	{
// 		PTbegin.x=b.x;	PTbegin.y=b.y;
// 		PTend.x=e.x;	PTend.y=e.y;
// 	};
// 	tagLINE2D(double x1,double y1,double x2,double y2)
// 	{
// 		PTbegin.x=x1, PTbegin.y=y1;
// 		PTend.x=x2,	PTend.y=y2;
// 	};
// }LINE2D,*PLINE2D;
// #else
// #pragma message("LINE2D�Ѿ�����!" )
// #endif

// #ifndef _POLYGON2D
// #define _POLYGON2D
// typedef struct tagPOLYGON2D
// {
// 	int nVNum;			       //������
// 	int nFlag;			       //��������Ա��
// 	tagPOINT2D *pVertex;	   //��������
// 	tagPOLYGON2D *pNext;	   //��һ�������
// 
// 	tagPOLYGON2D()
// 	{
// 		nVNum=0;
// 		nFlag=0;
// 		pVertex=NULL;
// 		pNext=NULL;
// 	};
// 
// 	~tagPOLYGON2D()
// 	{
// 		if (pVertex)
// 		{
// 			delete pVertex;
// 			pVertex=NULL;
// 		}
// 		if (pNext)
// 		{
// 			delete pNext;
// 			pNext=NULL;
// 		}
// 	};
// }POLYGON2D,*PPOLYGON2D;
// #else
// #pragma message("POLYGON2D�Ѿ�����!" )
// #endif

// #ifndef _POINT3D
// #define _POINT3D
// typedef struct tagPOINT3D		//��ά�����ݽṹ
// {
// 	double X,Y,Z;
// 
// 	tagPOINT3D()
// 	{
// 		X=Y=Z=0.0;
// 	}
// 	tagPOINT3D(double x,double y,double z)
// 	{
// 		X=x;
// 		Y=y;
// 		Z=z;
// 	}
// 	tagPOINT3D(SPACEPOINT sp)
// 	{
// 		X=sp.dfX;
// 		Y=sp.dfY;
// 		Z=sp.dfZ;
// 	}
// }POINT3D,*PPOINT3D;
// #else
// #pragma message("POINT_3D�Ѿ�����!" )
// #endif

#ifndef _RECTBOX
#define _RECTBOX
typedef struct tagRECTBOX       //���νṹ
{
	float fl,fr,ft,fb;          //��߽� �ұ߽� �ϱ߽� �±߽�
	tagRECTBOX()
	{
		fl=fr=ft=fb=0.0;
	}
	tagRECTBOX(float L,float R,float T,float B)
	{
		fl=L;
		fr=R;
		ft=T;
		fb=B;
	}
	float Width()
	{
		return fr-fl;
	}
	float Height()
	{
		return ft-fb;
	}
}RECTBOX,*PRECTBOX;
#else
#pragma message("RECTBOX�Ѿ�����!" )
#endif

#ifndef _ATIMAGE
#define _ATIMAGE
typedef struct tagATIMAGE		//����Ӱ���
{
	int nImgID;		            //Ӱ���
	double dfx,dfy;		        //x,y����
	tagATIMAGE *pNext;	        //��һ���ַ

	tagATIMAGE()
	{
		nImgID=0;
		dfx=dfy=0.0;
		pNext=NULL;
	}

	tagATIMAGE(int Id,double X,double Y,tagATIMAGE *Next=NULL)
	{
		nImgID=Id;
		dfx=X;
		dfy=Y;
		pNext=Next;
	}

	~tagATIMAGE()
	{
		if (pNext!=NULL)
		{
			delete pNext;
			pNext=NULL;
 		}
	}
}ATIMAGE,*PATIMAGE;
#else
#pragma message("ATIMAGE�Ѿ�����!" )
#endif

// #ifndef _ATPOINT
// #define _ATPOINT
// typedef struct tagATPOINT		//���������������ݽṹ
// {
// 	POINT3D spPoint;	       //�ռ������
// 	int nImgPtNum;			   //Ӱ�������
// 	ATIMAGE *pImgPt; 		   //Ӱ������������Ӧ��Ӱ��Ƭ��,�����ӵ���һӰ���
// 
// 	tagATPOINT()
// 	{
// 		spPoint = SPACEPOINT();
// 		nImgPtNum=0;
// 		pImgPt=NULL;
// 	}
// 	tagATPOINT(POINT3D sp,int ipnum,ATIMAGE *imgp)
// 	{
// 		spPoint=sp;
// 		nImgPtNum=ipnum;
// 
// 		ATIMAGE *curpt=pImgPt=new ATIMAGE;
// 		ATIMAGE *curp=imgp;
// 
// 		for (int i=0;i<nImgPtNum;i++)
// 		{
// 			memcpy(curpt,curp,sizeof(ATIMAGE));
// 			curp=curp->pNext;
// 			curpt->pNext=new ATIMAGE;
// 			curpt=curpt->pNext;
// 		}
// 		delete curpt;
// 	}
// 
// 	~tagATPOINT()
// 	{
// 		if (pImgPt!=NULL) 
// 		{
// 			delete pImgPt;
// 			pImgPt=NULL;
// 		}
// 	}
// }ATPOINT,*PATPOINT;
// #else
// #pragma message("ATPOINT�Ѿ�����!" )
// #endif

// #ifndef _ATLINK
// #define _ATLINK
// typedef struct tagATLINK		//������������
// {
// 	long lID;			//���
// 	ATPOINT *pPoint;		//���ݵ�ַ
// 
// 	tagATLINK()
// 	{
// 		lID=0;
// 		pPoint=NULL;
// 	}
// 
// 	~tagATLINK()
// 	{
// 		if (pPoint!=NULL)
// 		{
// 			delete pPoint;
// 			pPoint=NULL;
// 		}
// 	}
// }ATLINK,*PATLINK;
// #else
// #pragma message("ATLINK�Ѿ�����!" )
// #endif

#ifndef _MODTRANSLATE
#define _MODTRANSLATE
typedef struct tagMODTRANSLATE		//ģ�ͱ任���������ݽṹ
{
	double dfScale;				    //����Scale
	double dfXm,dfYm,dfZm;			//ƽ��Offset
	double dfPhi,dfOmg,dfKapa;	    //��תRotation

	tagMODTRANSLATE()
	{
		dfScale=1.0;			
		dfXm=dfYm=dfZm=0.0;
		dfPhi=dfOmg=dfKapa=0.0;		
	}
	tagMODTRANSLATE(double s,double x,double y,double z,double f,double w,double k)
	{
		dfScale=s;
		dfXm=x;
		dfYm=y;
		dfZm=z;
		dfPhi=f;
		dfOmg=w;
		dfKapa=k;		
	}
}MODTRANSLATE,*PMODTRANSLATE;
#else
#pragma message("MODTRANSLATE�Ѿ�����!" )
#endif

// #ifndef _GRAYDATA
// #define _GRAYDATA
// typedef struct tagGRAYDATA  //�Ҷ�ͼ�������ݽṹ
// {
// 	BYTE *data;			//�Ҷ�ֵ���������µ�Ϊԭ��
// 	int w,h;			//�ҶȾ�����
// 	double xset,yset;	//�ҶȾ�������½���ԭӰ���е�����(�ں�������лָ��Ҷȵ���������)
// 
// 	tagGRAYDATA()
// 	{
// 		data=NULL;
// 		w=h=0;
// 		xset=yset=0;
// 	}
// 	tagGRAYDATA(int W,int H,double X=0,double Y=0)
// 	{
// 		w=W;
// 		h=H;
// 		xset=X;
// 		yset=Y;
// 		data=new BYTE[w*h];
// 
// 		memset(data, 0, w*h*sizeof(BYTE));
// 	}
// 	tagGRAYDATA(int W,int H,BYTE *Data,double X=0,double Y=0)
// 	{
// 		w=W;
// 		h=H;
// 		xset=X;
// 		yset=Y;
// 		data=new BYTE[w*h];
// 
// 		memcpy(data, Data, w*h*sizeof(BYTE));
// 	}
// 
// 	~tagGRAYDATA()
// 	{
// 		if (data!=NULL)
// 		{
// 			delete data;
// 			data=NULL;
// 		}
// 	}
// }GRAYDATA,*PGRAYDATA;
// #else
// #pragma message("GRAYDATA�Ѿ�����!" )
// #endif

#ifndef _MATCHPOINT
#define _MATCHPOINT
typedef struct tagMATCHPOINT		//һ��ƥ���Ľṹ
{
	long	lID;				    //���
	int		nLinkMark;		        //������
	int		nLPID,nRPID;		    //����Ӱ���Ӱ���
	double	dflx,dfly,dfrx,dfry;	//����Ӱ���ϵĵ������
	BOOL	bIsOld;			        //Ŀ�����Դ false ������ true ��ƥ���
	tagMATCHPOINT()
	{
		lID=-1;
		nLinkMark=-1;
		nLPID=nRPID=-1;
		dflx=dfly=dfrx=dfry=0.0;
		bIsOld=false;
	}
	
	tagMATCHPOINT(long ID,int Link,int Lpno,int Rpno,double Lx,double Ly,double Rx,double Ry,bool Oral)
	{
		lID=ID;
		nLinkMark=Link;
		nLPID=Lpno;
		nRPID=Rpno;
		dflx=Lx;
		dfly=Ly;
		dfrx=Rx;
		dfry=Ry;
		bIsOld=Oral;
	}
}MATCHPOINT,*PMATCHPOINT;
#else
#pragma message("MATCHPOINT�Ѿ�����!" )
#endif

// #ifndef _SEEDPOINT
// #define _SEEDPOINT
// typedef struct tagSEEDPOINT
// {
// 	int row1,col1;	//��Ӱ��ĺ����ţ���Ƭ��
// 	double x1,y1;	//��Ӱ���ϵ�x��y����
// 
// 	int row2,col2;	//��Ӱ��ĺ����ţ���Ƭ��
// 	double x2,y2;	//��Ӱ���ϵ�x��y����
// 
// 	tagSEEDPOINT()
// 	{
// 		row1=col1=-1;
// 		x1=y1=0;
// 
// 		row2=col2=-1;
// 		x2=y2=0;	
// 	}
// 	tagSEEDPOINT(int R1,int C1,double X1,double Y1,int R2,int C2,double X2,double Y2)
// 	{
// 		row1=R1;
// 		col1=C1;
// 		x1=X1;
// 		y1=Y1;
// 
// 		row2=R2;
// 		col2=C2;
// 		x2=X2;
// 		y2=Y2;	
// 	}
// }SEEDPOINT,*PSEEDPOINT;
// #else
// #pragma message("SEEDPOINT�Ѿ�����!" )
// #endif
// 
// #ifndef _STRIPOFFSET
// #define _STRIPOFFSET
// typedef struct tagSTRIPOFFSET
// {
// 	int row;
// 	int offset;
// 
// 	tagSTRIPOFFSET()
// 	{
// 		row=0;
// 		offset=0;
// 	}
// }STRIPOFFSET,*PSTRIPOFFSET;
// #else
// #pragma message("STRIPOFFSET�Ѿ�����!" )
// #endif
// 
// #ifndef _IPADDRESS
// #define _IPADDRESS
// typedef struct tagIPADDRESS
// {
// 	int id1,id2,id3,id4;
// 
// 	tagIPADDRESS()
// 	{
// 		id1=id2=id3=id4=0;
// 	}
// 
// 	tagIPADDRESS(int Id1,int Id2,int Id3,int Id4)
// 	{
// 		id1=Id1;
// 		id2=Id2;
// 		id3=Id3;
// 		id4=Id4;
// 	}
// }IPADDRESS,*PIPADDRESS;
// #else
// #pragma message("IPADDRESS�Ѿ�����!" )
// #endif
// 
// //
// //Ϊ����β��ж���Ľṹ
// #ifndef _POLYVERTEX
// #define _POLYVERTEX
// typedef struct tagPOLYVERTEX  //˫������Ķ���ζ���ṹ
// {
// 	int idno; //�����id��id��Ϊ0����Ϊ��������
// 	double x,y;//���������
// 
// 	tagPOLYVERTEX *front; //ǰ��ڵ�ָ��
// 	tagPOLYVERTEX *after; //����ڵ�ָ��
// 
// 	//�ڸýڵ�Ϊ�½ڵ�ʱ�����½ṹ������
// 	tagPOLYVERTEX *goalfr; //ָ�򱻲ö���ε�ǰ��ڵ�ָ��
// 	tagPOLYVERTEX *goalaf; //ָ�򱻲ö���εĺ���ڵ�ָ��
// 
// 	tagPOLYVERTEX *cutfr; //ָ����ж���ε�ǰ��ڵ�ָ��
// 	tagPOLYVERTEX *cutaf; //ָ����ж���εĺ���ڵ�ָ��
// 
// 	tagPOLYVERTEX()
// 	{
// 		int idno=0;
// 		x=y=0;
// 		front=after=goalfr=goalaf=cutfr=cutaf=NULL;
// 	}
// 
// 	~tagPOLYVERTEX()
// 	{
// 		if (after&&after->idno) 
// 		{
// 			delete after;
// 			after=NULL;
// 		}
// 		front=after=goalfr=goalaf=cutfr=cutaf=NULL;
// 	}
// }POLYVERTEX,*PPOLYVERTEX;
// #else
// #pragma message("POLYVERTEX�Ѿ�����!" )
// #endif
// 
// #ifndef _SUPPOLYGON
// #define _SUPPOLYGON
// typedef struct tagSUPPOLYGON   //˫���������εĽṹ
// {
// 	int polyid;				//����α��
// 	int pnum;				//�����
// 	POLYVERTEX *pvertex;	//�׽��ָ��
// 	POLYVERTEX *endvert;	//ĩ���ָ��
// 
// 	tagSUPPOLYGON()
// 	{
// 		polyid=-1;
// 		pnum=0;
// 		pvertex=NULL;
// 		endvert=NULL;
// 	}
// 
// 	~tagSUPPOLYGON()
// 	{
// 		if (pvertex)
// 		{
// 			delete pvertex;
// 			pvertex=NULL;
// 		}
// 	}
// }SUPPOLYGON,*PSUPPOLYGON;
// #else
// #pragma message("SUPPOLYGON�Ѿ�����!" )
// #endif


#endif