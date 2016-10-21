/*
===============================================================================

  FILE:  PointBaseDefinitions.h
  
  CONTENTS:
		Point Processing���õ��Ļ������ݽṹ����

  PROGRAMMERS:
			Tony Zhang
    
  
  COPYRIGHT:
  
    copyright (C) 2007  zhangjing_whu@126.com
    

  
  CHANGE HISTORY:
				Created  in  2008.4.16	By  T.Zhang
   
  
===============================================================================
*/

#ifndef __POINT_BASE_DEFINITONS__
#define __POINT_BASE_DEFINITONS__


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ALMDefine.h"

#pragma pack(1) // ����1�ֽڷ�ʽ���ж���

//LiDAR�����ṹ		28Bytes
//������ʵ�ʵ�������
struct	LIDAR_POINT	
{
	double	X;//X��������(X������ڣ�XҪ����XScaleFactor����XOffset)
	double	Y;//Y��������(Y������ڣ�YҪ����YScaleFactor����YOffset)
	double	Z;//Z��������(Z������ڣ�ZҪ����ZScaleFactor����ZOffset)
	unsigned short	 Intensity;//����ǿ��ֵ
	unsigned char    Pulse;	//������Ϣ������ز���3bits(0,1,2)���ܷ�����3bits(3,4,5)��ɨ�跽��1bits(6)�����߱�Ե1bits(7)
	unsigned char	 Classification;//�����
};

//��չ��LIDAR_POINT�ṹ�����GPSʱ��
struct	LIDAR_POINT1
{
	double	X;//X��������(X������ڣ�XҪ����XScaleFactor����XOffset)
	double	Y;//Y��������(Y������ڣ�YҪ����YScaleFactor����YOffset)
	double	Z;//Z��������(Z������ڣ�ZҪ����ZScaleFactor����ZOffset)
	unsigned short	 Intensity;//����ǿ��ֵ
	unsigned char    Pulse;	//������Ϣ������ز���3bits(0,1,2)���ܷ�����3bits(3,4,5)��ɨ�跽��1bits(6)�����߱�Ե1bits(7)
	unsigned char	 Classification;//�����
	double		GPSTime;
};

//������ģ������		16Bytes
typedef struct  
{
	long	X;//Xģ������
	long	Y;//Yģ������
	long	Z;//Zģ������
	unsigned short	 Intensity;//����ǿ��ֵ
	unsigned char    Pulse;	//������Ϣ������ز���3bits(0,1,2)���ܷ�����3bits(3,4,5)��ɨ�跽��1bits(6)�����߱�Ե1bits(7)
	unsigned char	 Classification;//�����
} RAW_POINT;

//���õ�ſ��Ի�ȡLIDAR_POINT�ṹ��δ������ԭʼ��Ϣ
typedef struct
{
	DWORD			PntNO;			//���		
	LIDAR_POINT		PntInfo;		//����Ϣ

}POINT_RECORD;

struct	L_DOT
{
	long	x;
	long	y;
};

struct	D_DOT 
{
	double	x;
	double	y;
};

struct	L_3DOT
{
	long	x;
	long	y;
	long	z;
};


#ifndef	_D_RECT
#define _D_RECT
struct D_RECT
{
	double	XMax;
	double	XMin;
	double	YMax;
	double	YMin;
}; 
#endif

#ifndef _POINT2D_Def_
#define _POINT2D_Def_

struct POINT2D	{
	double x, y;
};

#endif

#ifndef _POINT3D_Def_
#define _POINT3D_Def_

struct POINT3D	{
	double X, Y, Z;
};

#endif

/*
typedef struct 
{
	double	XMax;
	double	XMin;
	double	YMax;
	double	YMin;
}D_RECT;*/

//typedef	BoundingBox2D	D_RECT;

typedef BoundingBox3D	D_CUBE;
/*
#ifndef _D_CUBE
#define _D_CUBE
typedef struct 
{
	double	XMax;
	double	XMin;
	double	YMax;
	double	YMin;
	double  ZMax;
	double	ZMin;
}D_CUBE;
#endif*/


typedef float F_POINT[3];
typedef long L_POINT[3];


#ifndef _LID_COLOR
#define _LID_COLOR
typedef unsigned char color[3];
#else
#pragma message("color�Ѿ�����!" )
#endif

#ifndef _LID_VEC
#define _LID_VEC
typedef float vec[3];
#else
#pragma message("vec�Ѿ�����!" )
#endif

typedef float point[3];
typedef float quat[4];



/*********************************************************
		y
		|
		|____x			OpenGL coordinate
	    /
	   /
	  z
**********************************************************/
struct F_NORMALVEC 
{
	float	x;
	float	y;
	float	z;
};

//��ֵ����
#define MAX_DOUBLE(x)		( x=1.7E+308 )
#define MIN_DOUBLE(x)		( x=-1.7E+308 )	
#define MAX_FLOAT(x)		( x=3.3e33f )
#define MIN_FLOAT(x)		( x=-3.3e33f )
#define MAX_LONG(x)			( x=2147483647 )	
#define MIN_LONG(x)			( x=-2147483648 )
#define MAX_USHORT(x)		( x=65535 )
#define MIN_USHORT(x)		( x=0 )


//����ļ�����
enum  expFileTYPE 
{
	typeTif,			//tif��ʽ
	typeDem				//VirtuoZo dem��ʽ
};

//��ֵ��ʽ
enum Resample_Mode
{
	GridMaxValue	=0,		//���������ֵ
	GridMinValue	=1,		//��������Сֵ
	GridMeanValue	=2,		//������ƽ��ֵ
	GridIDWValue	=3,		//�����ھ��뵼����Ȩֵ
	TINLinear		=4		//TIN���Բ�ֵ
};

#define		VZ_DEM_NodataValue		-99999
#define		Tif_DEM_NodataValue		-9999999



#pragma pack() // ȡ��1�ֽڶ��뷽ʽ
#endif