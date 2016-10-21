#include "stdafx.h"
#include "ATNPointIndex.h"
#include "orsBase/orsMemoryPool.h"
#include "math.h"
#include "assert.h"
//#include "lidbase.h"
#include "MyLine.h"

#include "orsBase/orsIPlatform.h"
#include "orsPointCloud/orsIPointCloudWriter.h"
#include "orsPointCloud/orsIPointCloudService.h"

orsIPlatform *g_platform;

orsIPlatform* getPlatform()
{
	return g_platform;
}

ORS_GET_POINT_CLOUD_SERVICE_IMPL();


//#include "CalibBasedef.h"
//#include "math.h"

inline double sqr(double value)
{
	return value * value;
};

#include <sys/stat.h>
bool _IsFileExist(const char *filePath)
{
	if( NULL == filePath )
		return false;


	struct stat stFileInfo;
	bool blnReturn;
	int intStat;

	// Attempt to get the file attributes
	intStat = stat( filePath, &stFileInfo);

	if(intStat == 0) {
		// We were able to get the file attributes
		// so the file obviously exists.
		//		_getPlatform()->logPrint( ORS_LOG_DEBUG, "\tFile %s exists\n", filePath );

		blnReturn = true;
	} else {
		// We were not able to get the file attributes.
		// This may mean that we don't have permission to
		// access the folder which contains this file. If you
		// need to do that level of checking, lookup the
		// return values of stat which will give you
		// more details on why stat failed.

		//		_getPlatform()->logPrint( ORS_LOG_DEBUG, "\tFile %s does not exist\n", filePath );

		blnReturn = false;
	}

	return(blnReturn); 
}

CATNPtIndex::CATNPtIndex(orsIPlatform *platform)
{
	g_platform = platform;

	m_pBlockLut=NULL;
	m_pLidBlock=NULL;

	m_pTin=0;
	m_pTin = new CTINClass( "pczAenVQ" );

	m_reader = ORS_CREATE_OBJECT(orsIPointCloudReader, 
			ORS_INTERFACE_POINTCLOUDSOURCE_READER_ATN);
}

CATNPtIndex::~CATNPtIndex()
{
	Release();
	if(m_pTin)
	{
		delete m_pTin;
		m_pTin=0;
	}
}

void CATNPtIndex::Release()
{
	if(m_pBlockLut)
	{
		delete m_pBlockLut;
		m_pBlockLut=NULL;
	}
	if(m_pLidBlock)
	{
		delete m_pLidBlock;
		m_pLidBlock=NULL;
	}
	m_reader->close();

//	m_strBlkName.Empty();
	m_gridRow=0;
	m_gridCol=0;
	m_gridNum=0;
	m_blkNum=0;
}

static orsMemoryPool<ATNPt_Block>	block_buffer;
static orsMemoryPool<ATNPoint>	point_buffer;
static __int64  block_offset = 227; //�����ļ�ͷ

static void chunk_ATN_point(LidBlock* cell, const ATNPoint* pos)
{
	assert(cell->ptNum);
	ATNPt_Block  *pATN_Block=NULL;
	ATNPoint	*pItem=NULL;
	
	
	pATN_Block=block_buffer.PoolAlloc();
	pItem=point_buffer.PoolAlloc();
	pATN_Block->pItem=pItem;
	memcpy(pItem, pos, sizeof(ATNPoint));
	
	
	pATN_Block->buffer_next=cell->pData;
	cell->pData=pATN_Block;
	
	cell->ptNum--;
	
}

static void write_chunked_points(LidBlock* cell, orsIPointCloudWriter *writer)
{
	assert(cell->ptNum==0);

	ATNPt_Block  *pBlock, *b;
	ATNPoint	 *pt;

	pBlock=cell->pData;
	while(pBlock)
	{
		pt=pBlock->pItem;
		
		double xyz[3];
		xyz[0]=pt->x;
		xyz[1]=pt->y;
		xyz[2]=pt->z;

		writer->write_point(xyz, pt->intensity, pt->return_number, pt->number_of_returns_of_given_pulse,
			0, 0, pt->classification, 1, 1, 1, pt->gpstime, &(pt->obsInfo));
//		cell->fileOffset--;
		
		b=pBlock;
		pBlock=pBlock->buffer_next;
		point_buffer.PoolDealloc(pt);
		block_buffer.PoolDealloc(b);
	}
//	assert(cell->fileOffset==0);
	
	cell->ptNum=cell->fileOffset;
	cell->fileOffset=block_offset;
	block_offset+=cell->ptNum*ATN_RawPoint_Length;
}
//˵���������ļ�  *.idx    �ֿ��ļ�  *_blk.las
//�����ļ���ʽ: 
/*		char pBlkFileName[256];	���Ʒֿ��ļ���
		D_RECT	boundary;		���ư�Χ��
		double  gridSize;		�����С
		double  gridRow;		����
		double  gridCol;		����
		int  gridNum;			����������Ӧ���ұ���
		long *pBlockLut;		���ұ�
		int  blkNum;			����
		LidBlock *pLidBlock;	�ֿ�����
  */
bool CATNPtIndex::CreateQTIdx(const char *pszOrgFileName, const char *pszIdxFileName, double gridSize)
{
//	CATNReader reader;
//	CATNWriter writer;
//	LASheader *header;
	double xmax, ymax, xmin, ymin;
	int colNum, rowNum;
	int iCol, iRow, idx;
	int i, j;
	LidBlock  *pLidBlock=NULL;
	ATNPoint atnPt;
	double x, y;
	long *pBlockLut=NULL;	//�ֿ�λ�ò��ұ�
	orsString strBlkName;

	strBlkName=pszIdxFileName;
	i=strBlkName.reverseFind('.');
	strBlkName=strBlkName.left(i);
	strBlkName+=_T("_blk.las");
	
	ref_ptr<orsIPointCloudReader> reader = ORS_CREATE_OBJECT(orsIPointCloudReader, ORS_INTERFACE_POINTCLOUDSOURCE_READER_ATN);

// 	if( getPointCloudlService() )
// 		reader = getPointCloudlService()->openPointFileForRead( pszOrgFileName );

	if( NULL == reader.get() )
	{
		getPlatform()->logPrint( ORS_LOG_INFO, "Can not create atn.reader" );
		return false;
	}

	if(!reader->open(pszOrgFileName))
	{
		getPlatform()->logPrint( ORS_LOG_INFO, "Can not open %s", pszOrgFileName );
		return false;
	}
//	reader.read_header(&header);

//	header = (LASheader*)reader->get_header();
	orsPOINT3D bbmax, bbmin;
	reader->get_boundary(&bbmax, &bbmin);

	xmax=bbmax.X;	xmin=bbmin.X;
	ymax=bbmax.Y;	ymin=bbmin.Y;

// 	while(reader.read_point(&atnPt))
// 	{
// 		x=atnPt.x;
// 		y=atnPt.y;
// 
// 		if(x<xmin) xmin=x;
// 		if(x>xmax) xmax=x;
// 		if(y<ymin) ymin=y;
// 		if(y>ymax) ymax=y;
// 	}
// 	reader.reopen();
	
	colNum=(int)ceil((xmax-xmin)/gridSize);
	rowNum=(int)ceil((ymax-ymin)/gridSize);
	
	pLidBlock=new LidBlock[colNum*rowNum];
	memset(pLidBlock, 0, sizeof(LidBlock)*colNum*rowNum);
	for(i=0; i<rowNum; i++)
	{
		for(j=0; j<colNum; j++)
		{
			pLidBlock[i*colNum+j].idx=i*colNum+j;
			pLidBlock[i*colNum+j].boundbox.XMin=xmin+j*gridSize;
			pLidBlock[i*colNum+j].boundbox.XMax=xmin+(j+1)*gridSize;
			pLidBlock[i*colNum+j].boundbox.YMax=ymax-i*gridSize;
			pLidBlock[i*colNum+j].boundbox.YMin=ymax-(i+1)*gridSize;
		}
	}
	
// 	D_RECT bb_box;	
// 	bb_box.XMin=1.7e+308;	bb_box.YMin=1.7e+308;
// 	bb_box.XMax=-1.7e+308;	bb_box.YMax=-1.7e+308;

	double xyz[3];
	while(reader->read_point(xyz))
	{//ͳ��ÿһ�ֿ�ĵ���
		x=xyz[0];
		y=xyz[1];
		iCol=(int)floor((x-xmin)/gridSize);
		iRow=(int)floor((ymax-y)/gridSize);
		
		idx=iRow*colNum+iCol;
		pLidBlock[idx].ptNum++;

// 		if(x<bb_box.XMin)	bb_box.XMin=x;
// 		if(x>bb_box.XMax)	bb_box.XMax=x;
// 		if(y<bb_box.YMin)	bb_box.YMin=y;
// 		if(y>bb_box.YMax)	bb_box.YMax=y;
	}
	
	pBlockLut=new long[rowNum*colNum];
	for(i=0; i<rowNum; i++)
	{
		for(j=0; j<colNum; j++)
		{
			idx=i*colNum+j;
			if(pLidBlock[idx].ptNum==0)
				pBlockLut[idx]=-1;
			else
				pBlockLut[idx]=0;
		}
	}
	
	ref_ptr<orsIPointCloudWriter> writer = ORS_CREATE_OBJECT(orsIPointCloudWriter, ORS_INTERFACE_POINTCLOUDSOURCE_WRITER_ATN);

// 	if( getPointCloudlService() )
// 		writer = getPointCloudlService()->openPointFileForWrite( strBlkName );

	if( NULL == writer.get() )
	{
		getPlatform()->logPrint( ORS_LOG_INFO, "Can not create atn.writer" );
		return false;
	}

	double scaleFac[3];
	reader->get_scale_factor(scaleFac);

	if(!writer->open(strBlkName, lasPOINT_ATN, scaleFac[0], scaleFac[1], scaleFac[2]))
	{
		getPlatform()->logPrint( ORS_LOG_INFO, "Can not open %s", strBlkName );
		return false;
	}
//	writer.write_header(&header);
	reader->reopen();
//	block_offset=header->offset_to_point_data;

	//д����ǰ����ƫ��ֵ
	block_offset = 227;
	while (reader->read_point(xyz))
	{//�ֿ�д���� 
		x=xyz[0];
		y=xyz[1];

		iCol=(int)floor((x-xmin)/gridSize);
		iRow=(int)floor((ymax-y)/gridSize);
		
		idx=iRow*colNum+iCol;
		
		atnPt.x = xyz[0];
		atnPt.y = xyz[1];
		atnPt.z = xyz[2];
		atnPt.intensity = reader->get_intensity();
		atnPt.return_number = reader->get_return_number();
		atnPt.number_of_returns_of_given_pulse = reader->get_number_of_returns();
		atnPt.classification = reader->get_classification();
		atnPt.gpstime = reader->get_gpstime();

//		orsPointObservedInfo obs1;
		reader->get_point_observed_info(&(atnPt.obsInfo));

// 		atnPt.latitude = obs1.pos.lat;	
// 		atnPt.longitude = obs1.pos.lon;
// 		atnPt.altitude = obs1.pos.h;
// 		atnPt.roll = obs1.roll;		
// 		atnPt.pitch = obs1.pitch;
// 		atnPt.heading = obs1.heading;
// 		atnPt.scanAngle = obs1.scanAngle;
// 		atnPt.range = obs1.range;

		chunk_ATN_point(&(pLidBlock[idx]), &atnPt);
		pLidBlock[idx].fileOffset++;	//��ʱ�洢����
		
		if(pLidBlock[idx].ptNum==0)
		{//write chunk points
			write_chunked_points(&(pLidBlock[idx]), writer.get());
		}
		
	}
	writer->close();
	
	FILE *fp=NULL;
	D_RECT boundary;
	int gridNum, blkNum;
	fp=fopen(pszIdxFileName, "wb");
	assert(fp);
	if(fp==NULL)
		return false;
		
	fwrite(strBlkName.c_str(), sizeof(char), 256, fp);
	boundary.XMax=xmax;		boundary.XMin=xmin;
	boundary.YMax=ymax;		boundary.YMin=ymin;

	fwrite(&boundary, sizeof(boundary), 1, fp);
	fwrite(&gridSize, sizeof(gridSize), 1, fp);
	fwrite(&rowNum, sizeof(rowNum), 1, fp);
	fwrite(&colNum, sizeof(colNum), 1, fp);
	gridNum=rowNum*colNum;
	fwrite(&gridNum, sizeof(gridNum), 1, fp);

	blkNum=0;
//	int diff=0, maxdiff=0;
	for(i=0; i<gridNum; i++)
	{
// 		diff=i-blkNum;
// 		if(diff>=maxdiff)
// 			maxdiff=diff;
// 		else
// 		{
// 			TRACE("error i=%d, blkNum=%d\n", i, blkNum);
// 		}
		if(pBlockLut[i]==0)
		{
			pBlockLut[i]=blkNum;
			blkNum++;
		}
	}
//	int pos;
//	int err;
//	pos=ftell(fp);
//	err=GetLastError();
	int n=fwrite(pBlockLut, sizeof(long), gridNum, fp);
//	err=GetLastError();
//	pos=ftell(fp);
//	err=GetLastError();
	
#ifdef _DEBUG
// 	for(i=0, j=0; i<gridNum; i++)
// 	{
// 		if(pLidBlock[i].ptNum>0)
// 			j++;
// 	}
// 	
// 	assert(blkNum==j);

#endif

	fwrite(&blkNum, sizeof(blkNum), 1, fp);
	for(i=0; i<gridNum; i++)
	{
		if(pLidBlock[i].ptNum>0)
		{
			fwrite(&(pLidBlock[i]), sizeof(pLidBlock[i]), 1, fp);
		}
	}
	fclose(fp);

	if(pLidBlock)
	{
		delete pLidBlock;
		pLidBlock=NULL;
	}
	
	if(pBlockLut)
	{
		delete pBlockLut;
		pBlockLut=NULL;
	}
	
	return true;
}

bool CATNPtIndex::Open(const char *pATNFileName)
{
	orsString strAtnName, strIdxName;
	int pos;

	strAtnName=pATNFileName;

	pos=strAtnName.reverseFind('.');
	strIdxName=strAtnName.left(pos);
	strIdxName+=".idx";
	
	if(_IsFileExist(strIdxName))
	{
		return OpenIdx(strIdxName);
	}
	else if(_IsFileExist(pATNFileName))
	{//�Զ���������
		CreateQTIdx(pATNFileName, strIdxName);
		return OpenIdx(strIdxName);
	}
	else
		return false;
}


bool CATNPtIndex::OpenIdx(const char *pszIdxFileName)
{
	FILE *fp=NULL;
	char pName[256];
	fp=fopen(pszIdxFileName, "rb");
	if(fp==NULL)
		return false;

	Release();

	fread(pName, sizeof(char), 256, fp);
	m_strBlkName=pName;
	

	int pos;
	m_strBlkName=pszIdxFileName;			//��Ϊ���·��   zj  2011.8.29
	pos=m_strBlkName.reverseFind('.');
	m_strBlkName=m_strBlkName.left(pos);
	m_strBlkName+="_blk.las";


	fread(&m_boundary, sizeof(m_boundary), 1, fp);
	fread(&m_gridSize, sizeof(m_gridSize), 1, fp);
	fread(&m_gridRow, sizeof(m_gridRow), 1, fp);
	fread(&m_gridCol, sizeof(m_gridCol), 1, fp);
	fread(&m_gridNum, sizeof(m_gridNum), 1, fp);
	
	m_pBlockLut=new long[m_gridNum];
	fread(m_pBlockLut, sizeof(long), m_gridNum, fp);
	fread(&m_blkNum, sizeof(m_blkNum), 1, fp);

	m_pLidBlock=new LidBlock[m_blkNum];
	fread(m_pLidBlock, sizeof(LidBlock), m_blkNum, fp);

	fclose(fp);	fp=NULL;

// 	if( getPointCloudlService() )
// 		m_reader = getPointCloudlService()->openPointFileForRead( m_strBlkName );

	if(m_reader.get())
		m_reader->open(m_strBlkName);

	if( NULL == m_reader.get() )
	{
		getPlatform()->logPrint( ORS_LOG_INFO, "Can not open %s", m_strBlkName );
		return false;
	}

	return true;
}


void CATNPtIndex::GetPoints(double x0, double y0, double radius, std::vector<ATNPoint> *ptDataVec)
{
	int iRowS, iColS, iRowE, iColE, idx, blkID;
	int i, j, k;
//	double xmin, xmax, ymin, ymax;
	D_RECT  bb_circle;
	LidBlock *pBlk=NULL;
	ATNPoint  point;
	double r2, dis2;

//	xmin=m_boundary.XMin;	xmax=m_boundary.XMax;
//	ymin=m_boundary.YMin;	ymax=m_boundary.YMax;

	ptDataVec->clear();
	
	//ȡԲ����Ӿ���
	bb_circle.XMin=x0-radius;
	bb_circle.XMax=x0+radius;
	bb_circle.YMax=y0+radius;
	bb_circle.YMin=y0-radius;


	iColS=(int)floor((bb_circle.XMin-m_boundary.XMin)/m_gridSize);
	iColE=(int)floor((bb_circle.XMax-m_boundary.XMin)/m_gridSize);
	iRowS=(int)floor((m_boundary.YMax-bb_circle.YMax)/m_gridSize);
	iRowE=(int)floor((m_boundary.YMax-bb_circle.YMin)/m_gridSize);

	if(iColS<0)
		iColS=0;
	if(iColE>m_gridCol-1)
		iColE=m_gridCol-1;
	if(iRowS<0)
		iRowS=0;
	if(iRowE>m_gridRow-1)
		iRowE=m_gridRow-1;

	r2=radius*radius;
	for(i=iRowS; i<=iRowE; i++)
	{
		for(j=iColS; j<=iColE; j++)
		{
			idx=i*m_gridCol+j;
			blkID=m_pBlockLut[idx];
			if(blkID<0)   //�����ݵ�
				continue;

			double coord[3];
			pBlk=m_pLidBlock+blkID;
			m_reader->set_readpos(pBlk->fileOffset);
			for(k=0; k<pBlk->ptNum; k++)
			{
				m_reader->read_point(coord);
				
				dis2=(coord[0]-x0)*(coord[0]-x0)+(coord[1]-y0)*(coord[1]-y0);
				if(dis2<r2)
				{
					point.x = coord[0];
					point.y = coord[1];
					point.z = coord[2];
					point.classification = m_reader->get_classification();
					point.intensity = m_reader->get_intensity();
					point.gpstime = m_reader->get_gpstime();
					m_reader->get_point_observed_info( &(point.obsInfo) );

					ptDataVec->push_back(point);
				}
			}
		}
	}

}

locateresult CATNPtIndex::GetTriangleVertex(POINT3D *pt, double radius, std::vector<ATNPoint> *ptDataVec, double *rmse)
{
	locateresult result;

	std::vector<ATNPoint> candidate;
	int i, j;
	ATNPoint point;

	ptDataVec->clear();

	GetPoints(pt->X, pt->Y, radius, &candidate);
	if(candidate.size()<3)
	{
		result=OUTSIDE;
		return	 result;
	}

	m_pTin->BeginAddPoints();
	for(i=0; i<candidate.size(); i++)
	{
		point=candidate[i];
		m_pTin->AddPoint(point.x, point.y, i);
	}

	m_pTin->EndAddPoints();
	m_pTin->FastConstruct();

	triEDGE searchtri;
	triPOINT *triPt[3];   
	
	result=m_pTin->LocatePoint(pt->X, pt->Y, searchtri);

	if(result==OUTSIDE) return	 result;

	triPt[0]=m_pTin->org(searchtri);
	triPt[1]=m_pTin->dest(searchtri);
	triPt[2]=m_pTin->apex(searchtri);

	for(j=0; j<3; j++)
	{
		i=triPt[j]->attr;
		point=candidate[i];
		ptDataVec->push_back(point);
	}

	//�������ڲ�zֵ
	double dx1, dy1, dx21, dx31, dy21, dy31, dz21, dz31;
	ATNPoint p1, p2, p3;

	p1 = (*ptDataVec)[0];
	p2 = (*ptDataVec)[1];
	p3 = (*ptDataVec)[2];

	dx1 = p1.x - pt->X;	dy1 = p1.y - pt->Y;
	dx21 = p2.x - p1.x;	dy21 = p2.y - p1.y;	dz21 = p2.z - p1.z;	
	dx31 = p3.x - p1.x;	dy31 = p3.y - p1.y;	dz31 = p3.z - p1.z;

	pt->Z = p1.z + ( dx1*( dy21*dz31 - dy31*dz21) - dy1*( dx21*dz31 - dx31*dz21) )
		/ ( dx21*dy31 - dx31*dy21 );

	//���������
	double var=0;
	for(i=0; i<candidate.size(); i++)
	{
		point=candidate[i];
		var+=(point.z-pt->Z)*(point.z-pt->Z);
	}
	
	*rmse = sqrt(var)/candidate.size();
	return result;
}	

void CATNPtIndex::GetNNPoint(POINT3D *pt, double radius, std::vector<ATNPoint> *ptDataVec, double *NN_dis)
{
	std::vector<ATNPoint> candidate;
	int i, j;
	ATNPoint point;

	ptDataVec->clear();

	GetPoints(pt->X, pt->Y, radius, &candidate);
	if(candidate.size()<1)
	{//�����뾶̫С
		*NN_dis = radius+1.0;
		return;
	}

	double minDis=radius+1;
	double dis;
	int ID;
	for(i=0; i<candidate.size(); i++)
	{
		dis=sqrt(sqr(candidate[i].x-pt->X)+sqr(candidate[i].y-pt->Y));
		if(dis<minDis)
		{
			minDis = dis;
			ID = i;
		}	
	}

	ptDataVec->push_back(candidate[ID]);
	
	*NN_dis = minDis;
	return;
}

// double CATNPtIndex::interpolateByTIN(double x, double y, double radius)
// {
// 
// }

//�ж�xy�����Ƿ����
static bool IsEqualXY(D_3DOT p1, D_3DOT p2)
{
	if(fabs(p1.x-p2.x)>1e-6)
		return	false;
	if(fabs(p1.y-p2.y)>1e-6)
		return	false;
	
	return	true;
}

static bool IsPtInPolygon(double x, double y, POINT3D *pPolygon, int ptNum, D_RECT *pRect)
{
	D_3DOT	pt, xpt, ipt;
	D_3DOT	p1, p2;
	CMyGLLine	LineSeg3d, xLineSeg3d;
	int		i;
//	double xmax, ymax, xmin, ymin;
//	int ptNum;

//	ptNum=pPolygon->size();

    // ѭ����ȡ����
	pt.x=x;
	pt.y=y;
    pt.z = 0.0;
    xpt = pt;
    xpt.x = pRect->XMax + 10.0;
    //xpt.x = 1.7e208;
    LineSeg3d.SetLine(pt,xpt);
	
	
    int nCount = 0;
	p1.x = pPolygon[0].X;
	p1.y = pPolygon[0].Y;
	p1.z = 0.0;
    for (i=1; i <= ptNum; i++)
    {
        p2.x = pPolygon[i%ptNum].X;
		p2.y = pPolygon[i%ptNum].Y;
        p2.z = 0.0;
        // ����������붥����ȣ�ֱ�ӷ���
		/*
        if (IsEqualXY(pt,p1))
		{
		return true;
	}*/
		
        xLineSeg3d.SetLine(p1, p2);
        // �����������ĳһ�����ϣ�ֱ�ӷ���
        if (xLineSeg3d.IsOnLine(pt))
        {
			return true;
        }
        // ��������߶��뽻����ڣ����뽻���
        if (LineSeg3d.IsIntersectWith(xLineSeg3d, ipt))
        {
            // �����������Ϊ���㣬�ж�����Ķ˵�����һ��
            if (IsEqualXY(ipt,p1))
            {
                if (p2.y > ipt.y)
                    nCount++;
            }
            else if(IsEqualXY(ipt,p2))
            {
                if (p1.y > ipt.y)
                    nCount++;
            }
            else
                nCount++;
        }
        p1 = p2;
    }
	
	if ((nCount % 2) == 0)
        return false; // ������Ϊż��������������
    else
        return true; // ������Ϊ��������������
}

void CATNPtIndex::GetPoints(POINT3D *pPolygon, int ptNum, std::vector<ATNPoint> *ptDataVec)
{
	int vertexNum;
	int iRowS, iColS, iRowE, iColE, idx, blkID;
	int i, j, k;
	//	double xmin, xmax, ymin, ymax;
	D_RECT  bb_box;
	LidBlock *pBlk=NULL;
	ATNPoint  point;
//	double r2, dis2;
	
	//	xmin=m_boundary.XMin;	xmax=m_boundary.XMax;
	//	ymin=m_boundary.YMin;	ymax=m_boundary.YMax;
	
	ptDataVec->clear();

	//ȡ����ε���Ӿ���
	vertexNum=ptNum;
	bb_box.XMin=1.7e+308;
	bb_box.XMax=-1.7e+308;
	bb_box.YMax=-1.7e+308;
	bb_box.YMin=1.7e+308;
	for(i=0; i<vertexNum; i++)
	{
		if(bb_box.XMin>pPolygon[i].X)
			bb_box.XMin=pPolygon[i].X;
		if(bb_box.XMax<pPolygon[i].X)
			bb_box.XMax=pPolygon[i].X;
		if(bb_box.YMin>pPolygon[i].Y)
			bb_box.YMin=pPolygon[i].Y;
		if(bb_box.YMax<pPolygon[i].Y)
			bb_box.YMax=pPolygon[i].Y;
	}
	
	
	iColS=(int)floor((bb_box.XMin-m_boundary.XMin)/m_gridSize);
	iColE=(int)floor((bb_box.XMax-m_boundary.XMin)/m_gridSize);
	iRowS=(int)floor((m_boundary.YMax-bb_box.YMax)/m_gridSize);
	iRowE=(int)floor((m_boundary.YMax-bb_box.YMin)/m_gridSize);
	
	//����ȡ
	for(i=iRowS; i<=iRowE; i++)
	{
		for(j=iColS; j<=iColE; j++)
		{
			idx=i*m_gridCol+j;
			blkID=m_pBlockLut[idx];
			if(blkID<0)   //�����ݵ�
				continue;

			pBlk=m_pLidBlock+blkID;
			m_reader->set_readpos(pBlk->fileOffset);
			for(k=0; k<pBlk->ptNum; k++)
			{
				double coord[3];
				m_reader->read_point(coord);

				// �����λ����С�������֮�⣬��϶����ڶ������
				if (( coord[0] > bb_box.XMax) ||
					( coord[0] < bb_box.XMin) ||
					( coord[1] > bb_box.YMax)||
					( coord[1] < bb_box.YMin))
					continue;

				point.x = coord[0];
				point.y = coord[1];
				point.z = coord[2];
				point.classification = m_reader->get_classification();
				point.intensity = m_reader->get_intensity();
				point.gpstime = m_reader->get_gpstime();
				m_reader->get_point_observed_info( &(point.obsInfo) );

				if(IsPtInPolygon(point.x, point.y, pPolygon, vertexNum, &bb_box))
				{
					ptDataVec->push_back(point);
				}

			}

		}
	}

}

bool CATNPtIndex::QueryNNPoint(double x, double y, ATNPoint *pt)
{
	//	bool bReturn=true;
	std::vector<ATNPoint> ptDataVec;

	GetPoints(x, y, 5.0, &ptDataVec);

	int num=ptDataVec.size();


	if(num==0)
		return false;

	ATNPoint nnpt;
	double min_dis2=1.7e+308;
	double dis2;
	for(int i=0; i<num; i++)
	{
		dis2=(ptDataVec[i].x-x)*(ptDataVec[i].x-x)+(ptDataVec[i].y-y)*(ptDataVec[i].y-y);
		if(dis2<min_dis2)
		{
			min_dis2=dis2;
			nnpt=ptDataVec[i];
		}
		
	}

	*pt=nnpt;
	return	true;
}

void CATNPtIndex::GetPoints(POINT3D *pPolygon, int ptNum, std::vector<LidPt_SurvInfo> *ptDataVec)
{
	std::vector<ATNPoint> DataVec;

	GetPoints(pPolygon, ptNum, &DataVec);

	for(int i=0; i<DataVec.size(); i++)
	{
		LidPt_SurvInfo point;
		point.x = DataVec[i].x;
		point.y = DataVec[i].y;
		point.z = DataVec[i].z;

		point.time = DataVec[i].gpstime;
		point.POS_Info.coord = DataVec[i].obsInfo.pos;
		point.POS_Info.r = DataVec[i].obsInfo.roll;
		point.POS_Info.p = DataVec[i].obsInfo.pitch;
		point.POS_Info.h = DataVec[i].obsInfo.heading;

		ptDataVec->push_back(point);
	}
}