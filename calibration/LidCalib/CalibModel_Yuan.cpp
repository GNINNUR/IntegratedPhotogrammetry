#include "StdAfx.h"
#include "LidCalib/CalibModel_Yuan.h"
#include "orsLidarGeometry/orsILidarGeoModel.h"
#include "orsBase/orsIPlatform.h"

#include "F:\OpenRS\IntegratedPhotogrammetry\external\levmar-2.5\levmar.h"

#include "CalibUtility/cp_IO.h"

//�۲�ֵ
struct myObs
{
	int nStrip;				//������
	orsILidarGeoModel  **pLidarModels;
	int nPoints;			//�ܵ���
	int nMisalign;			//���ò�������
//	CalibParam_Type type;	//��У��������
	LidPt_SurvInfo *pLidPoints;//caution������ֵ���������в��ܸı䣬���ҽ������ǶԳ�ʼ����ֵ��

	double *pPDC;		//x, y, z�԰��ò�����ƫ����
	int ndim;			//ά��  xp, yp, zp

	int nTiePlane;		//����ƽ����
	long *pPlaneLut;	//����ƽ���Ӧ�Ĳ��ұ�

	int nTiePts;			//�������ӵ���		
	int nTieGCP;		//���ӵ��еĿ��Ƶ���
	int nObs;			//�۲ⷽ����(��ƽ��㣬�̵߳�֮��)
	long *pTiePtsLut;		//���ӵ�Ĳ��ұ�
	LidMC_VP *pLidVP;	//�����
	TiePoint_Adj_mode TiePt_mode;

//	double tx, ty, tz;  //ƫ�ķ���

	double *L;          //������Ĺ۲�ֵ
	double *pWeight;	//�������Ȩ��
	double *hx, *hxx;	//Ϊ���׼���Ŀռ�

	orsLidYuanParam *pOriParam;	//ϵͳ����ֵ

	/////////////////////////////////
	////////////������///////////////
	int nPlanes;			//����ƽ������
	double *plane_rms;		//���ÿ������ƽ��
	int *pIndividualPlaneLut;    //����ƽ��ĵ��Ʋ��ұ�
};


//p:δ֪���� y����ľ���ֵ��mδ֪��������n���̸���
//2013.1.4  �Զ����ݼ�У�������ͼ���������
void distance_PointToPlane_Yuan(double *p, double *y, int m, int n, void *data)
{
	register int i, j;
	struct myObs *dptr;
	LidPt_SurvInfo *pLidPoints;
	double      *pMisalign;
	int nPoints, nTiePlane;
	double a, b, c, d, s;
	double xp, yp, zp;
	double *pPlane;
	long *pPlaneLut;
	int nStrip;
	orsLidYuanParam *pOriParam=NULL;
	orsILidarGeoModel **pLidModels=NULL;

	dptr=(struct myObs *)data; 
	pLidPoints=dptr->pLidPoints;
	pMisalign=p;
	nPoints=dptr->nPoints;
	nTiePlane=dptr->nTiePlane;

	pPlane=p+dptr->nMisalign;
	pPlaneLut=dptr->pPlaneLut;

	nStrip = dptr->nStrip;
	pOriParam = dptr->pOriParam;
	pLidModels = dptr->pLidarModels;

	//����ϵͳδ֪��
	for(i=0; i<nStrip; i++)
	{
		pOriParam[i].ax = p[0];
		pOriParam[i].ay = p[1];
		pOriParam[i].az = p[2];
		pOriParam[i].kappa = p[3];
		pOriParam[i].omega = p[4];
		pLidModels[i]->setSysParam((void*)(&pOriParam[i]));
	}

	//���¼��㼤�������
// 	for(i=0; i<nPoints; i++)
// 	{
// 		int sID = pLidPoints[i].stripID;
// 		pLidModels[sID]->cal_Laserfootprint(&pLidPoints[i], 1);
// 	}
	

	for(i=0; i<nTiePlane; i++)
	{//����ƽ��
		a=pPlane[i*4];
		b=pPlane[i*4+1];
		c=pPlane[i*4+2];
		d=pPlane[i*4+3];
		s=1.0/sqrt(a*a+b*b+c*c);

		for(j=pPlaneLut[i]; j<pPlaneLut[i+1]; j++)
		{//����ƽ���ڵĵ�
			int sID = pLidPoints[j].stripID;
			orsPOINT3D orgPts, dstPts;
			orgPts.X = pLidPoints[j].x;
			orgPts.Y = pLidPoints[j].y;
			orgPts.Z = pLidPoints[j].z;

			pLidModels[sID]->cal_Laserfootprint(&orgPts, &dstPts, 1);

			xp=dstPts.X;
			yp=dstPts.Y;
			zp=dstPts.Z;

			y[j]=(a*xp+b*yp+c*zp+d)*s;
		}
	}

	if(dptr->plane_rms && dptr->pIndividualPlaneLut)
	{//ͳ��ÿ��ƽ���rms
//		memset(dptr->plane_rms, 0, sizeof(double)*dptr->nPlanes);
		
		for(i=0; i<dptr->nPlanes; i++)
		{//����ƽ��
			double rms=0;
			for(j=dptr->pIndividualPlaneLut[i]; j<dptr->pIndividualPlaneLut[i+1]; j++)
			{//����ƽ���ڵĵ�
				rms += y[j]*y[j];
				
			}
			dptr->plane_rms[i]=sqrt(rms/(dptr->pIndividualPlaneLut[i+1]-dptr->pIndividualPlaneLut[i]));
		}

	}
}

//��δ֪��ƫ����
void jac_dis_PointToPlane_Yuan(double *p, double *jac, int m, int n, void *data)
{
	register int i, j;
	struct myObs *dptr;
	LidPt_SurvInfo *pLidPoints;
	double      *pMisalign;
	int nPoints, nTiePlane;
	double a, b, c, d, s;
	double a2, b2, c2, s3;
	double xp, yp, zp;
	double *pPlane;
	long *pPlaneLut;
	int pnp, ndim;
	//	double *pPDC;
	double *pdX, *pdY, *pdZ;
	int ncol;

	int nStrip;
	orsLidYuanParam *pOriParam=NULL;
	orsILidarGeoModel **pLidModels=NULL;

	double  pPDC[15];	//x, y, z, 5 unknowns


	dptr=(struct myObs *)data;
	pLidPoints=dptr->pLidPoints;
	pMisalign=p;
	nPoints=dptr->nPoints;
	nTiePlane=dptr->nTiePlane;

	pPlane=p+dptr->nMisalign;
	pPlaneLut=dptr->pPlaneLut;
	pnp=dptr->nMisalign;
	ndim=dptr->ndim;
	//	pPDC=dptr->pPDC;

	nStrip = dptr->nStrip;
	pOriParam = dptr->pOriParam;
	pLidModels = dptr->pLidarModels;

	//����ϵͳδ֪��
	for(i=0; i<nStrip; i++)
	{
		pOriParam[i].ax = p[0];
		pOriParam[i].ay = p[1];
		pOriParam[i].az = p[2];
		pOriParam[i].kappa= p[3];
		pOriParam[i].omega = p[4];
		pLidModels[i]->setSysParam((void*)(&pOriParam[i]));
	}

	//���¼��㼤�������
// 	for(i=0; i<nPoints; i++)
// 	{
// 		int sID = pLidPoints[i].stripID;
// 		pLidModels[sID]->cal_Laserfootprint(&pLidPoints[i], 1);
// 	}

	//�õ�ǰ�ĵ��������ƫ�������߼��ϲ���Ҫ���¼��������
	ncol=pnp+4*nTiePlane;
	memset(jac, 0, sizeof(double)*nPoints*ncol);
	//���� B, C, L
	for(i=0; i<nTiePlane; i++)
	{//����ƽ��
		a=pPlane[i*4];
		b=pPlane[i*4+1];
		c=pPlane[i*4+2];
		d=pPlane[i*4+3];

		a2=a*a;
		b2=b*b;
		c2=c*c;

		s=1.0/sqrt(a*a+b*b+c*c);
		s3=s*s*s;

		for(j=pPlaneLut[i]; j<pPlaneLut[i+1]; j++)
		{//����ƽ���ڵĵ�
			int sID = pLidPoints[j].stripID;

			orsPOINT3D orgPts, dstPts;
			orgPts.X = pLidPoints[j].x;
			orgPts.Y = pLidPoints[j].y;
			orgPts.Z = pLidPoints[j].z;

			pLidModels[sID]->cal_Laserfootprint(&orgPts, &dstPts, 1);

			xp=dstPts.X;
			yp=dstPts.Y;
			zp=dstPts.Z;


			pLidModels[sID]->calPDC_misalign((void*)&dstPts, 1, &pOriParam[sID], pPDC);

			pdX=pPDC;
			pdY=pdX+pnp;
			pdZ=pdY+pnp;


			//��
			jac[j*ncol]  =s*(a*pdX[0]+b*pdY[0]+c*pdZ[0]);
			jac[j*ncol+1]=s*(a*pdX[1]+b*pdY[1]+c*pdZ[1]);
			jac[j*ncol+2]=s*(a*pdX[2]+b*pdY[2]+c*pdZ[2]);
			jac[j*ncol+3]=s*(a*pdX[3]+b*pdY[3]+c*pdZ[3]);
			jac[j*ncol+4]=s*(a*pdX[4]+b*pdY[4]+c*pdZ[4]);
			//jac[j*ncol+5]=s*(a*pdX[5]+b*pdY[5]+c*pdZ[5]);

			//ƽ��
			jac[j*ncol+pnp+i*4]  =(xp*(b2+c2)-a*(b*yp+c*zp+d))*s3;
			jac[j*ncol+pnp+i*4+1]=(yp*(a2+c2)-b*(a*xp+c*zp+d))*s3;
			jac[j*ncol+pnp+i*4+2]=(zp*(a2+b2)-c*(a*xp+b*yp+d))*s3;
			jac[j*ncol+pnp+i*4+3]=s;
		}
	}
}

//��ַ�ʽ��
void jac_dis_PointToPlane_Yuan_diff(double *p, double *jac, int m, int n, void *data)
{
	register int i, j;
	struct myObs *dptr;
	LidPt_SurvInfo *pLidPoints;
	double      *pMisalign;
	int nPoints, nTiePlane;
	// 	double a, b, c, d, s;
	// 	double a2, b2, c2, s3;
	// 	double xp, yp, zp;
	// 	double *pPlane;
	// 	long *pPlaneLut;
	int pnp, ndim;
	double *pPDC;
	double *pdX, *pdY, *pdZ;
	int ncol;	//δ֪������
	double *hx, *hxx;

	dptr=(struct myObs *)data;
	pLidPoints=dptr->pLidPoints;
	pMisalign=p;
	nPoints=dptr->nPoints;
	nTiePlane=dptr->nTiePlane;

	// 	pPlane=p+dptr->nMisalign;
	// 	pPlaneLut=dptr->pPlaneLut;
	pnp=dptr->nMisalign;
	ndim=dptr->ndim;
	pPDC=dptr->pPDC;
	hx = dptr->hx;
	hxx = dptr->hxx;

	ncol=pnp+4*nTiePlane;


	//���㵱ǰδ֪�������µķ��̹۲�ֵ
	distance_PointToPlane_Yuan(p, hx, ncol, nPoints, data);

	double d, tmp;
	double delta=1E-06;
	for(j=0; j<ncol; ++j)
	{
		/* determine d=max(1E-04*|p[j]|, delta), see HZ */
		d=(1E-04)*p[j]; // force evaluation
		d=fabs(d);
		if(d<delta)
			d=delta;

		tmp=p[j];
		p[j]+=d;

		distance_PointToPlane_Yuan(p, hxx, ncol, nPoints, data);

		p[j]=tmp; /* restore */

		d=(1.0)/d; /* invert so that divisions can be carried out faster as multiplications */
		for(i=0; i<nPoints; ++i)
		{
			jac[i*ncol+j]=(hxx[i]-hx[i])*d;
		}
	}
}

void LidStatDis_PointToPlane_Yuan(LidMC_Plane *pLidPatch, int nPatch, orsILidarGeoModel **pLidModels, int nStrip,
	double *Lidparam, bool bInit, orsLidYuanParam *pOriParam, double *rms, int nTies, double *pDis)
{
	double *X=NULL;
	LidPt_SurvInfo *pLidPoints=NULL;	/* �����۲�ֵ	*/
	long *pPlaneLut=NULL;	//ÿ��ƽ��������Ĳ��ұ�

	double *pPlane=NULL;	//����Ҫ����ռ�
	int i, j, k;

	int nCalibParam=5;    //5����ģ��
	
	X = new double[nCalibParam + 4*nTies];

	pPlaneLut=new long[nTies+1];

	if(bInit)
	{
		for(i=0; i<nCalibParam+4*nTies; i++)
		{
			X[i]=Lidparam[i];
		}
	}
	else
	{
		for(i=0; i<nCalibParam; i++)
		{
			X[i]=Lidparam[i];
		}
	}

	pPlane=X+nCalibParam;

	//����ϵͳδ֪��
// 	for(i=0; i<nStrip; i++)
// 	{
// 		pOriParam[i].ax = X[0];
// 		pOriParam[i].ay = X[1];
// 		pOriParam[i].az = X[2];
// 		pOriParam[i].aroll = X[3];
// 		pOriParam[i].ayaw = X[4];
// 		pLidModels[i]->setSysParam((void*)(&pOriParam[i]));
// 	}

	//��Ҫ��֤����������水tieID˳����֯
	int maxPtNum=0;
	int nLidPoints=0;
	for(i=0, j=-1; i<nPatch; i++)
	{
// 		//���¼��㼤�������
// 		int sID = pLidPatch[i].sID;
// 		pLidModels[sID]->cal_Laserfootprint(&(*(pLidPatch[i].pLidPoints))[0], pLidPatch[i].pLidPoints->size());

		if(j==pLidPatch[i].tieID)
		{
			if(maxPtNum<pLidPatch[i].pLidPoints->size())
			{//ȡ��������������Ϊ��ʼƽ�����
				maxPtNum=pLidPatch[i].pLidPoints->size();

				if(!bInit)
				{
					pPlane[j*4]  =pLidPatch[i].s1;
					pPlane[j*4+1]=pLidPatch[i].s2;
					pPlane[j*4+2]=pLidPatch[i].s3;
					pPlane[j*4+3]=pLidPatch[i].s4;
				}
			}

			pPlaneLut[j+1]+=pLidPatch[i].pLidPoints->size();
			nLidPoints+=pLidPatch[i].pLidPoints->size();
		}
		else
		{
			j=pLidPatch[i].tieID;
			maxPtNum=pLidPatch[i].pLidPoints->size();

			if(!bInit)
			{
				pPlane[j*4]  =pLidPatch[i].s1;
				pPlane[j*4+1]=pLidPatch[i].s2;
				pPlane[j*4+2]=pLidPatch[i].s3;
				pPlane[j*4+3]=pLidPatch[i].s4;
			}

			pPlaneLut[j+1]=pLidPatch[i].pLidPoints->size()+nLidPoints;
			nLidPoints+=pLidPatch[i].pLidPoints->size();
		}

	}
	pPlaneLut[0]=0;

	pLidPoints=new LidPt_SurvInfo[nLidPoints];
	k=0;
	for(i=0; i<nPatch; i++)
	{
		std::vector<LidPt_SurvInfo> *pVec=pLidPatch[i].pLidPoints;
		for(j=0; j<pVec->size(); j++)
		{
			pLidPoints[k++]=(*pVec)[j];
		}
	}

	orsILidarGeoModel **pLidarModels=NULL;
	pLidarModels = new orsILidarGeoModel* [nStrip];

	for(i=0; i<nStrip; i++)
	{
		pLidarModels[i] = ORS_CREATE_OBJECT(orsILidarGeoModel, /*ORS_LIDAR_GEOMETRYMODEL_Yuan*/ORS_LIDAR_GEOMETRYMODEL_YUAN);

		pLidarModels[i]->setSysParam((void*)(&pOriParam[i]));
	}
	


	double *dis=NULL;
	dis=new double[nLidPoints];

	struct myObs data;
	data.nMisalign=nCalibParam;
	data.nTiePlane=nTies;
	data.nPoints=nLidPoints;
	data.pLidPoints=pLidPoints;
	data.pPlaneLut=pPlaneLut;
//	data.type = param_type;
	data.pOriParam = pOriParam;
	data.pLidarModels = pLidarModels;
	data.nStrip = nStrip;

	data.pIndividualPlaneLut=NULL;
	data.plane_rms=NULL;


	//����������
	distance_PointToPlane_Yuan(X, dis, nCalibParam+4*nTies, nLidPoints, &data);

	if(pDis)
		memcpy(pDis, dis, sizeof(double)*nLidPoints);

	for(i=0; i<nTies; i++)
	{//����ƽ��
		rms[i]=0;
		for(j=pPlaneLut[i]; j<pPlaneLut[i+1]; j++)
		{//����ƽ���ڵĵ�
			rms[i]+=dis[j]*dis[j];
		}
		rms[i]/=(pPlaneLut[i+1]-pPlaneLut[i]);
		rms[i]=sqrt(rms[i]);
		//		fprintf(fp, "%d num:%d rms:%.6f\n", i, pPlaneLut[i+1]-pPlaneLut[i], rms[i]);
	}

	if(X)	delete[] X; X=NULL;
	if(pLidPoints) delete[] pLidPoints;	pLidPoints=NULL;
	if(pPlaneLut)  delete[] pPlaneLut;	pPlaneLut=NULL;
	if(dis)		delete[] dis;	dis=NULL;
	if(pLidarModels)	delete[] pLidarModels;	pLidarModels=NULL;
}

//ֻ����۲�ֵ
void cal_Obs_ConjugatePts_Yuan(double *p, double *y, int m, int n, void *data)
{
	register int i, j, k;
	struct myObs *dptr;
	LidMC_VP *pLidVP;
	double      *pMisalign/*, *pSrcMisalign*/;
	int nMisalign;
//	int nVP;
//	double a, b, c, d, s;
//	double xp, yp, zp;
//	double *pPlane;
	long *pLut;
	LidPt_SurvInfo *pLidPoints;
	int nLidPt;
	//POINT3D *ptDst, *ptSrc, *pTmp;
	//double xSrc[3], ySrc[3], zSrc[3], xDst[3], yDst[3], zDst[3];
	double eX, eY, eZ;
	int nTies, nTieGCP, nObs;
	double *L;
	double dXs, dYs, dZs;
	double *pWeight;
	orsILidarGeoModel **pLidModels;


	dptr=(struct myObs *)data;
	nMisalign=dptr->nMisalign;
	pLidVP=dptr->pLidVP;
	pMisalign=p;
//	pSrcMisalign=dptr->SrcParam;
//	nVP=dptr->nVP;
	pLidPoints=dptr->pLidPoints;	
	pLut=dptr->pTiePtsLut;
//	ptDst=dptr->ptDst;
//	ptSrc=dptr->ptSrc;
	nTies=dptr->nTiePts;
	nTieGCP=dptr->nTieGCP;
	nObs=dptr->nObs;
	L=dptr->L;
	pWeight = dptr->pWeight;
	pLidModels = dptr->pLidarModels;

	int nStrips = dptr->nStrip;
	int nPoints = dptr->nPoints;

	orsLidYuanParam *pOriParam;
	pOriParam = dptr->pOriParam;
	pLidModels = dptr->pLidarModels;

	//����ϵͳδ֪��
	for(i=0; i<nStrips; i++)
	{
		pOriParam[i].ax = p[0];
		pOriParam[i].ay = p[1];
		pOriParam[i].az = p[2];
		pOriParam[i].kappa = p[3];
		pOriParam[i].omega = p[4];
		pLidModels[i]->setSysParam((void*)(&pOriParam[i]));
	}

	//���¼��㼤�������
	for(i=0, j=0; i<nPoints; i++)
	{
		int sID = pLidPoints[i].stripID;
//		pLidModels[sID]->cal_Laserfootprint(&pLidPoints[i], 1);
		
		orsPOINT3D orgPts, dstPts;
		orgPts.X = pLidPoints[i].x;
		orgPts.Y = pLidPoints[i].y;
		orgPts.Z = pLidPoints[i].z;

		pLidModels[sID]->cal_Laserfootprint(&orgPts, &dstPts, 1);

		pLidVP[i].vx = dstPts.X;
		pLidVP[i].vy = dstPts.Y;
		pLidVP[i].vz = dstPts.Z;

		//���¹۲�ֵ
		if(dptr->TiePt_mode == LidAdj_TP_UseHorVer)
		{
			if(pLidVP[i].VP_type == TP_type_hor_ver)
			{
				pLidVP[i].vx *= pWeight[j];	y[j]=pLidVP[i].vx; j++;
				pLidVP[i].vy *= pWeight[j];	y[j]=pLidVP[i].vy; j++;
				pLidVP[i].vz *= pWeight[j];	y[j]=pLidVP[i].vz; j++;
			}
			else if(pLidVP[i].VP_type == TP_type_horizon)
			{
				pLidVP[i].vx *= pWeight[j];	y[j]=pLidVP[i].vx; j++;
				pLidVP[i].vy *= pWeight[j];	y[j]=pLidVP[i].vy; j++;
			}
			else if(pLidVP[i].VP_type == TP_type_vertical)
			{
				pLidVP[i].vz *= pWeight[j];	y[j]=pLidVP[i].vz; j++;
			}
		}
		else if(dptr->TiePt_mode == LidAdj_TP_UseHor)
		{
			if(pLidVP[i].VP_type == TP_type_hor_ver)
			{
				pLidVP[i].vx *= pWeight[j];	y[j]=pLidVP[i].vx; j++;
				pLidVP[i].vy *= pWeight[j];	y[j]=pLidVP[i].vy; j++;
			}
			else if(pLidVP[i].VP_type == TP_type_horizon)
			{
				pLidVP[i].vx *= pWeight[j];	y[j]=pLidVP[i].vx; j++;
				pLidVP[i].vy *= pWeight[j];	y[j]=pLidVP[i].vy; j++;
			}
		}
		else if(dptr->TiePt_mode == LidAdj_TP_UseVer)
		{
			if(pLidVP[i].VP_type == TP_type_hor_ver)
			{
				pLidVP[i].vz *= pWeight[j];	y[j]=pLidVP[i].vz; j++;
			}
			else if(pLidVP[i].VP_type == TP_type_vertical)
			{
				pLidVP[i].vz *= pWeight[j];	y[j]=pLidVP[i].vz; j++;
			}
		}
	}

	//���¼������ӵ������ֵ
	//���Ƶ㱣�ֲ���
	if(L)
	{
		for(i=nTieGCP; i<nTies; i++)
		{
			eX=0; eY=0;	eZ=0;
			for(j=pLut[i]; j<pLut[i+1]; j++)
			{
				eX += pLidVP[j].vx * pWeight[j];
				eY += pLidVP[j].vy * pWeight[j];
				eZ += pLidVP[j].vz * pWeight[j];
			}
			int nLinks=pLut[i+1]-pLut[i];
			eX/=nLinks;
			eY/=nLinks;
			eZ/=nLinks;

			k=pLut[i];	//���ӵ�ĵ�һ��
			if(dptr->TiePt_mode == LidAdj_TP_UseHorVer)
			{
				if(pLidVP[k].VP_type == TP_type_hor_ver)
				{
					for(j=pLut[i]; j<pLut[i+1]; j++)
					{
						*L=eX;		L++;	
						*L=eY;		L++;	
						*L=eZ;		L++;	
					}
				}
				else if(pLidVP[k].VP_type == TP_type_horizon)
				{
					for(j=pLut[i]; j<pLut[i+1]; j++)
					{
						*L=eX;		L++;	
						*L=eY;		L++;	
					}
				}
				else if(pLidVP[k].VP_type == TP_type_vertical)
				{
					for(j=pLut[i]; j<pLut[i+1]; j++)
					{
						*L=eZ;		L++;	
					}
				}
			}
			else if(dptr->TiePt_mode == LidAdj_TP_UseHor)
			{
				if(pLidVP[k].VP_type == TP_type_hor_ver)
				{
					for(j=pLut[i]; j<pLut[i+1]; j++)
					{
						*L=eX;		L++;	
						*L=eY;		L++;	
					}
				}
				else if(pLidVP[k].VP_type == TP_type_horizon)
				{
					for(j=pLut[i]; j<pLut[i+1]; j++)
					{
						*L=eX;		L++;	
						*L=eY;		L++;	
					}
				}
			}
			else if(dptr->TiePt_mode == LidAdj_TP_UseVer)
			{
				if(pLidVP[k].VP_type == TP_type_hor_ver)
				{
					for(j=pLut[i]; j<pLut[i+1]; j++)
					{
						*L=eZ;		L++;	
					}
				}
				else if(pLidVP[k].VP_type == TP_type_vertical)
				{
					for(j=pLut[i]; j<pLut[i+1]; j++)
					{
						*L=eZ;		L++;	
					}
				}
			}
		}
	}
}

void jac_ConjugatePts_Yuan(double *p, double *jac, int m, int n, void *data)
{
	register int i, j;
	double tmp;
	register double d;
	double delta=1E-06;
//	double *hx, *hxx;
	struct myObs *dptr;
//	double nVP;
//	double *pLut;

	int nStrip;
	orsLidYuanParam *pOriParam=NULL;
	orsILidarGeoModel **pLidModels=NULL;

	double  pPDC[15];	//x, y, z, 5 unknowns
	double *pdx, *pdy, *pdz;

	dptr=(struct myObs *)data;

	LidPt_SurvInfo *pLidPoints=dptr->pLidPoints;
	int nPoints=dptr->nPoints;
	int nTiePts=dptr->nTiePts;
	int nTieGCP = dptr->nTieGCP;
	int nObs = dptr->nObs;
	int nMisalign = dptr->nMisalign;
	long *pLut = dptr->pTiePtsLut;
	LidMC_VP *pLidVP = dptr->pLidVP;

	nStrip = dptr->nStrip;
	pOriParam = dptr->pOriParam;
	pLidModels = dptr->pLidarModels;

	//����ϵͳδ֪��
	for(i=0; i<nStrip; i++)
	{
		pOriParam[i].ax = p[0];
		pOriParam[i].ay = p[1];
		pOriParam[i].az = p[2];
		pOriParam[i].kappa= p[3];
		pOriParam[i].omega = p[4];
		pLidModels[i]->setSysParam((void*)(&pOriParam[i]));
	}

	//���¼��㼤�������
// 	for(i=0; i<nPoints; i++)
// 	{
// 		int sID = pLidPoints[i].stripID;
// 		pLidModels[sID]->cal_Laserfootprint(&pLidPoints[i], 1);
// 	}

	//�õ�ǰ�ĵ��������ƫ�������߼��ϲ���Ҫ���¼��������
	memset(jac, 0, sizeof(double)*nObs*nMisalign);
	
	//���� B, C, L
	int iObs=0;
	for(i=0; i<nTiePts; i++)
	{//����ƽ��
		
		int k=pLut[i];	//���ӵ�ĵ�һ��

		for(j=pLut[i]; j<pLut[i+1]; j++)
		{//����ƽ���ڵĵ�
			int sID = pLidPoints[j].stripID;

			orsPOINT3D orgPts, dstPts;
			orgPts.X = pLidPoints[j].x;
			orgPts.Y = pLidPoints[j].y;
			orgPts.Z = pLidPoints[j].z;

			pLidModels[sID]->cal_Laserfootprint(&orgPts, &dstPts, 1);

			pLidModels[sID]->calPDC_misalign((void*)&dstPts, 1, &pOriParam[sID], pPDC);

			pdx = jac + iObs*nMisalign;
			pdy = pdx + nMisalign;
			pdz = pdy + nMisalign;

			if(dptr->TiePt_mode == LidAdj_TP_UseHorVer)
			{
				if(pLidVP[k].VP_type == TP_type_hor_ver)
				{
					memcpy(pdx, pPDC, sizeof(double)*3*nMisalign);
					iObs += 3;
				}
				else if(pLidVP[k].VP_type == TP_type_horizon)
				{
					memcpy(pdx, pPDC, sizeof(double)*2*nMisalign);
					iObs += 2;
				}
				else if(pLidVP[k].VP_type == TP_type_vertical)
				{
					memcpy(pdx, pPDC+2*nMisalign, sizeof(double)*nMisalign);
					iObs += 1;
				}
			}
			else if(dptr->TiePt_mode == LidAdj_TP_UseHor)
			{
				if(pLidVP[k].VP_type == TP_type_hor_ver)
				{
					memcpy(pdx, pPDC, sizeof(double)*2*nMisalign);
					iObs += 2;
				}
				else if(pLidVP[k].VP_type == TP_type_horizon)
				{
					memcpy(pdx, pPDC, sizeof(double)*2*nMisalign);
					iObs += 2;
				}
			}
			else if(dptr->TiePt_mode == LidAdj_TP_UseVer)
			{
				if(pLidVP[k].VP_type == TP_type_hor_ver)
				{
					memcpy(pdx, pPDC+2*nMisalign, sizeof(double)*nMisalign);
					iObs += 1;
				}
				else if(pLidVP[k].VP_type == TP_type_vertical)
				{
					memcpy(pdx, pPDC+2*nMisalign, sizeof(double)*nMisalign);
					iObs += 1;
				}
			}
		}
	}
}

CLidCalib_Yuan::CLidCalib_Yuan()
{
	m_covar = NULL;
	m_rms = 0;
	m_corcoef = NULL;
}

CLidCalib_Yuan::~CLidCalib_Yuan()
{
	if(m_covar)
		delete m_covar;
	m_covar = NULL;

	if(m_corcoef)
		delete m_corcoef;
	m_corcoef = NULL;
}

int  CLidCalib_Yuan::LidMC_Calib_Coplanar(
		const char *pszResultFile,		/* ƽ�����ļ� */
		const int nCorrsp,	        /* number of Correspondent Objects */
		const int ngcp,		/* number of points (starting from the 1st) whose parameters should not be modified.	
							* All B_ij (see below) with i<ncon are assumed to be zero
							���Ƶ���ڹ۲�ֵ����ʼ����*/
		LidMC_Obj *pLidPatch,
//		TiePoint_Adj_mode TiePt_mode,
							//				 char *vmask,			/* visibility mask: vmask[i, j]=1 if point i visible in image j, 0 otherwise. nxm */
		double *pX,			/* initial parameter vector p0: (a1, ..., am, b1, ..., bn).
												* aj are the image j parameters, bi are the i-th point parameters,
												* size m*cnp + n*pnp
												δ֪����ƫ�ķ����Ͱ��ý�    
												pX=[Xt, Yt, Zt, r, p, h]^T ����δ֪����ֵ��*/
		//				const int pnp,			/* number of parameters δ֪������*/
		orsLidYuanParam *pOriParam,           /*   �������� */
		int		nStrips                       /*   ������     */
//		unsigned long	param_type				/* ��У�������� */
		)
{
	int i, j, k;
	int ndim/*, npd*/;
	double *C=NULL;	 
	double *B=NULL, *L=NULL;
	
	double *pPlane=NULL;		//��ʼƽ�����
	double *X=NULL;		//��ʼ�궨����
	
//	double a, b, c, d;	//ƽ�����  ax+by+cz+d=0
//	double a2, b2, c2;
	
	int nTies;
	int nLidPoints;		/* ���� */ 
	int nParam;			/* ƽ��������� */
	
	LidPt_SurvInfo *pLidPoints=NULL;	/* �����۲�ֵ	*/
	//	double  *pPlane=NULL;		/* ƽ����� */
	long *pPlaneLut=NULL;	//ÿ��ƽ��������Ĳ��ұ�
	
	double *dX=NULL;		//�궨�����ĸ�����
	double *dPlane=NULL;	//ƽ������ĸ�����
	double *pPDC=NULL;
//	double *pdX, *pdY, *pdZ;
//	double s, s3;
//	double xp, yp, zp;


	LidMC_Plane *pLidPlane = (LidMC_Plane*)pLidPatch;
	int nPlane = nCorrsp;
	
	//ͳ������������, tieID������������
	int tieID=-1;
	nTies=0;
	for(i=0; i<nPlane; i++)
	{
		if(tieID!=pLidPlane[i].tieID)
		{
			nTies++;
			tieID=pLidPlane[i].tieID;
		}
	}
//	nTies+=1;
	
	nParam=nTies*4;	//ƽ���һ�㷽��
// 	pPlane=new double[nParam];
 	pPlaneLut=new long[nTies+1];


// 	dX=new double[pnp];
// 	dPlane=new double[nParam];

	int nCalibParam = 5;     //five unknowns
//	int nMisalign=3;
	X = new double[nCalibParam + 4*nTies];
 	for(i=0; i<nCalibParam; i++)
 	{
		X[i]=pX[i];
	}
	pPlane = X+nCalibParam;
	
	//��Ҫ��֤����������水tieID˳����֯
	int maxPtNum=0;
	nLidPoints=0;
	for(i=0, j=-1; i<nPlane; i++)
	{
		if(j==pLidPlane[i].tieID)
		{
			if(maxPtNum<pLidPlane[i].pLidPoints->size())
			{//ȡ��������������Ϊ��ʼƽ�����
				maxPtNum=pLidPlane[i].pLidPoints->size();

				pPlane[j*4]  =pLidPlane[i].s1;
				pPlane[j*4+1]=pLidPlane[i].s2;
				pPlane[j*4+2]=pLidPlane[i].s3;
				pPlane[j*4+3]=pLidPlane[i].s4;
			}
// 			else
// 			{
// 
// 			}

			pPlaneLut[j+1]+=pLidPlane[i].pLidPoints->size();
			nLidPoints+=pLidPlane[i].pLidPoints->size();
		}
		else
		{
			j=pLidPlane[i].tieID;
			maxPtNum=pLidPlane[i].pLidPoints->size();
			
			pPlane[j*4]  =pLidPlane[i].s1;
			pPlane[j*4+1]=pLidPlane[i].s2;
			pPlane[j*4+2]=pLidPlane[i].s3;
			pPlane[j*4+3]=pLidPlane[i].s4;
			
			pPlaneLut[j+1]=pLidPlane[i].pLidPoints->size()+nLidPoints;
			nLidPoints+=pLidPlane[i].pLidPoints->size();
		}

	}
	pPlaneLut[0]=0;
	
	pLidPoints=new LidPt_SurvInfo[nLidPoints];
	k=0;
	for(i=0; i<nPlane; i++)
	{
		std::vector<LidPt_SurvInfo> *pVec=pLidPlane[i].pLidPoints;
		for(j=0; j<pVec->size(); j++)
		{
//			(*pVec)[j].stripID = pLidPlane[i].sID;    //���������������

			pLidPoints[k++]=(*pVec)[j];
		}
	}
	
	ndim=3;		//x,y,z
//	npd=6;		//dtx, dty, dtz, dr, dp, dh
	
 	pPDC=new double[nLidPoints*(nCalibParam + 4*nTies)];
// 	B=new double[nLidPoints*npd];
// 	C=new double[nLidPoints*4*nTies];	//δ����ѹ���洢
 	L=new double[nLidPoints];
	double *hx=new double[nLidPoints];
	double *hxx=new double[nLidPoints];

	for(i=0; i<nLidPoints; i++)
	{
		L[i]=0;
	}
	
	double opts[LM_OPTS_SZ], info[LM_INFO_SZ];
	opts[0]=LM_INIT_MU; opts[1]=1E-15; opts[2]=1E-15; opts[3]=1E-20; opts[4]= LM_DIFF_DELTA;

	orsILidarGeoModel **pLidarModel=NULL;

	pLidarModel = new orsILidarGeoModel*[nStrips];
	for(i=0; i<nStrips; i++)
	{
		pLidarModel[i] = ORS_CREATE_OBJECT(orsILidarGeoModel, /*ORS_LIDAR_GEOMETRYMODEL_Yuan*/ORS_LIDAR_GEOMETRYMODEL_YUAN);
		
		pLidarModel[i]->setSysParam((void*)(&pOriParam[i]));
	}

	struct myObs data;
	data.nStrip = nStrips;
	data.pLidarModels = pLidarModel;

	data.nMisalign=nCalibParam;
	data.nTiePlane=nTies;
	data.nPoints=nLidPoints;
	data.pLidPoints=pLidPoints;
	data.pPlaneLut=pPlaneLut;
	data.pPDC=pPDC;
	data.ndim=3;
//	data.type = type;
	data.hx = hx;
	data.hxx = hxx;
	data.pOriParam = pOriParam;
	

	double *plane_rms=NULL;
	int *pIndividualPlaneLut=NULL;

	data.nPlanes = nPlane;
	plane_rms=new double[nPlane];
	pIndividualPlaneLut=new int[nPlane+1];

	data.plane_rms = plane_rms;
	data.pIndividualPlaneLut = pIndividualPlaneLut;

	pIndividualPlaneLut[0]=0;
	for(i=0, j=1; i<nPlane; i++, j++)
	{
		pIndividualPlaneLut[j] = pIndividualPlaneLut[j-1]+pLidPlane[i].pLidPoints->size();
	}

/*
#ifdef _DEBUG
	FILE *fp=NULL;
	fp=fopen("G:/D_experiment/adjustment/3param/plane_adjust_3p", "wt");

	double *dis=NULL;
	dis=new double[nLidPoints];
	double *rms=NULL;
	rms=new double[nTies];

	//ͳ�Ƴ�ʼ������ľ��뷽��
	distance_PointToPlane_rph(X, dis, nMisalign+4*nTies, nLidPoints, &data);
	fprintf(fp, "init plane rms\n");
	for(i=0; i<nTies; i++)
	{//����ƽ��
		rms[i]=0;
		for(j=pPlaneLut[i]; j<pPlaneLut[i+1]; j++)
		{//����ƽ���ڵĵ�
			rms[i]+=dis[j]*dis[j];
		}
		rms[i]/=(pPlaneLut[i+1]-pPlaneLut[i]);
		rms[i]=sqrt(rms[i]);
		fprintf(fp, "%d num:%d rms:%.6f\n", i, pPlaneLut[i+1]-pPlaneLut[i], rms[i]);
	}
#endif*/

	double *rms = new double[nTies]; 
	LidStatDis_PointToPlane_Yuan(pLidPlane, nPlane, pLidarModel, nStrips, X, false, pOriParam, rms, nTies, NULL);


	FILE *fp = fopen(pszResultFile, "wt");

	fprintf(fp, "## calibration method: Coplanar Constraint\n");
//	fprintf(fp, "## calib param type: %d\n", type);
	fprintf(fp, "## init calib param\n");
	for(i=0; i<nCalibParam; i++)
	{
		fprintf(fp, "%.9lf\n", X[i]);
	}

	fprintf(fp, "total points number: %d\n", nLidPoints);
	fprintf(fp, "before adjustment\n");
	fprintf(fp, "init plane rms.\n");
	for(i=0; i<nTies; i++)
	{//����ƽ��
		int ptNum=0;
		for(j=0; j<nPlane; j++)
		{
			if(pLidPlane[j].tieID==i)
				ptNum+=pLidPlane[j].pLidPoints->size();
		}
		fprintf(fp, "%d num:%d rms:%.6f\n", i, ptNum, rms[i]);
	}

	if(m_covar)	delete m_covar;

	m_pn = nCalibParam+nTies*4;
	m_covar = new double[m_pn*m_pn];

	int ret=dlevmar_der(distance_PointToPlane_Yuan, jac_dis_PointToPlane_Yuan, X, L, nCalibParam+nTies*4, nLidPoints, 1000, 
						opts, info, NULL, m_covar, &data);


	//����ƽ�淽�̺���ܵõ���ȷ�ĵ������
// 	for(i=0, j=-1; i<nPlane; i++)
// 	{
// 		j=pLidPlane[i].tieID;
// 		
// 		pLidPlane[i].s1 = pPlane[j*4];
// 		pLidPlane[i].s2 = pPlane[j*4+1];
// 		pLidPlane[i].s3	= pPlane[j*4+2];
// 		pLidPlane[i].s4	= pPlane[j*4+3];
// 	}

	double *dis = new double[nLidPoints];

	LidStatDis_PointToPlane_Yuan(pLidPlane, nPlane, pLidarModel, nStrips, X, true, pOriParam, rms, nTies, dis);

	fprintf(fp, "after adjustment\n");

	fprintf(fp, "resolving plane rms\n");
	for(i=0; i<nTies; i++)
	{//����ƽ��
		int ptNum=0;
		for(j=0; j<nPlane; j++)
		{
			if(pLidPlane[j].tieID==i)
				ptNum+=pLidPlane[j].pLidPoints->size();
		}
		fprintf(fp, "%d num:%d rms:%.6f\n", i, ptNum, rms[i]);
	}

	fprintf(fp, "## resolving results\n");
	for(i=0; i<nCalibParam; i++)
	{
		fprintf(fp, "%.9lf\n", X[i]);
	}


	m_rms = sqrt(info[1]/(nLidPoints - nCalibParam+nTies*4));


	int pnp;
	pnp = nCalibParam + nTies*4;
	
	if(m_corcoef) delete m_corcoef;
	m_corcoef = new double[pnp*pnp];

	k=0;
	for(i=0;i<pnp; i++)
	{
		double Dxx = sqrt(m_covar[i*pnp+i]);

		for(j=0; j<pnp; j++)
		{
			double Dyy = sqrt(m_covar[j*pnp+j]);
			double Dxy = m_covar[i*pnp+j];

			double Dyx = m_covar[j*pnp+i];

			if(i>j)
				Dxy = Dyx;

//			assert(Dxy < 1.2*Dyx && Dxy > 0.8*Dyx);
			
			m_corcoef[k] = Dxy/Dxx/Dyy;
			k++;
		}
	}

	fprintf(fp, "####### RMS #########\n");
	fprintf(fp, "%.5lf\n", m_rms);

	fprintf(fp, "####### covariance of unknowns #########\n");
	fprintf(fp, "number of unknowns : %d\n", m_pn);
	for(i=0; i<m_pn; i++)
	{
		for(j=0; j<m_pn; j++)
		{	
			k = i*m_pn + j;
			fprintf(fp, " %e", m_covar[k]);
		}
		fprintf(fp, "\n");
	}

	fprintf(fp, "####### correlation coefficient of unknowns #######\n");
	k=0;
	for(i=0; i<pnp; i++)
	{
		for(j=0; j<pnp; j++)
		{
// 			if(i<pnp-1 && j>i)
// 			{
				fprintf(fp," %e", fabs(m_corcoef[i*pnp+j]));
// 				k++;
// 			}
// 			else
// 				fprintf(fp, " /");
		}
		fprintf(fp, "\n");
	}

	if(fp)	fclose(fp);	fp=NULL;
		
	//��У�������
	for(i=0; i<nCalibParam; i++)
	{
		pX[i] = X[i];
	}

	//ƽ���������
	for(i=0; i<nPlane; i++)
	{
		j=pLidPlane[i].tieID;

		pLidPlane[i].s1 = pPlane[j*4];
		pLidPlane[i].s2 = pPlane[j*4+1];
		pLidPlane[i].s3 = pPlane[j*4+2];
		pLidPlane[i].s4 = pPlane[j*4+3];

	}

	//�����������в���ڷ���
	orsString strTmp;
	orsString strPath = orsString::getDirFromPath(pszResultFile);
	orsString purename;
	strTmp = orsString::getPureFileName(pszResultFile);
	int pos = strTmp.reverseFind( '.');
	purename = strTmp.left(pos);

	char tmp[128];
	for(i=0; i<nTies; i++)
	{//����ƽ��

		orsString name = strPath + "\\";
		sprintf(tmp, "_plane_%d.txt", i);
		name += purename;
		name +=	tmp;

		FILE *fout = fopen(name, "wt");
		int ptNum=0;
		for(j=0; j<nPlane; j++)
		{
			if(pLidPlane[j].tieID==i)
				ptNum+=pLidPlane[j].pLidPoints->size();
		}
		fprintf(fout, "planeID %d, point num:%d, rms:%.6f\n", i, ptNum, rms[i]);
		for(j=pPlaneLut[i]; j<pPlaneLut[i+1]; j++)
		{//����ƽ���ڵĵ�
			fprintf(fout, "%5f\n", dis[j]);
		}

		fclose(fout);	
	}

	/////////////////////////////////////////////////////
	///////������У����/////////////////////////////////
	orsString cParamFile = strPath + "\\";
	cParamFile += purename;
	cParamFile += ".cp"; //calib param file


	pOriParam[0].ax = X[0]; pOriParam[0].ay = X[1]; pOriParam[0].az = X[2];
	pOriParam[0].kappa = X[3]; pOriParam[0].omega = X[4];
	write_CPfile(cParamFile, LCMT_Yuan, (void*)pOriParam, 1);

/*
#ifdef _DEBUG 
	 
	fprintf(fp, "## resolving results\n");
	fprintf(fp, "## misalign: r, p, h\n");
	fprintf(fp, "%.9lf\n", tx);
	fprintf(fp, "%.9lf\n", ty);
	fprintf(fp, "%.9lf\n", tz);
	fprintf(fp, "%.9lf\n", X[0]);
	fprintf(fp, "%.9lf\n", X[1]);
	fprintf(fp, "%.9lf\n", X[2]);

	pPlane=X+nMisalign;
	fprintf(fp, "## %d tie_plane param\n", nTies);
	for(i=0; i<nTies; i++)
	{
		fprintf(fp, "%.6lf\n", pPlane[i*4]);
		fprintf(fp, "%.6lf\n", pPlane[i*4+1]);
		fprintf(fp, "%.6lf\n", pPlane[i*4+2]);
		fprintf(fp, "%.6lf\n", pPlane[i*4+3]);
	}
	 	
	

	//����ÿ��������������
	distance_PointToPlane_rph(X, dis, nMisalign+4*nTies, nLidPoints, &data);

	fprintf(fp, "## tie_plane rms\n");
	
	for(i=0; i<nTies; i++)
	{//����ƽ��
		rms[i]=0;
		for(j=pPlaneLut[i]; j<pPlaneLut[i+1]; j++)
		{//����ƽ���ڵĵ�
			rms[i]+=dis[j]*dis[j];
		}
		rms[i]/=(pPlaneLut[i+1]-pPlaneLut[i]);
		rms[i]=sqrt(rms[i]);
		fprintf(fp, "%d num:%d rms:%.6f\n", i, pPlaneLut[i+1]-pPlaneLut[i], rms[i]);
	}

	
	if(fp)	fclose(fp);	fp=NULL;
#endif*/

//	if(dis)		delete[] dis;	dis=NULL;
//	if(rms)	delete[] rms;	rms=NULL;

	if(rms)			delete rms;		rms = NULL;
	if(pLidPoints)	delete pLidPoints;	pLidPoints=NULL;
	if(pPlaneLut)	delete pPlaneLut;		pPlaneLut=NULL;
	if(L)			delete L;			L=NULL;
	if(X)			delete X;			X=NULL;
	if(pPDC)		delete pPDC;		pPDC=NULL;
	if(hx)			delete hx;			hx=NULL;
	if(hxx)			delete hxx;			hxx=NULL;
	if(dis)			delete dis;			dis=NULL;
//	if(covar)		delete covar;		covar=NULL;

	if(pLidarModel)	delete pLidarModel;	pLidarModel=NULL;

	if(plane_rms)	delete plane_rms;	plane_rms=NULL;
	if(pIndividualPlaneLut)	delete pIndividualPlaneLut;	pIndividualPlaneLut=NULL;
	return 0;
}

int  CLidCalib_Yuan::LidMC_Calib_ConjugatePoints(
		const char *pszResultFile,		/* ƽ�����ļ� */
		const int nConjugatePts,	        /* number of Correspondent Objects */
		const int ngcp,		/* number of points (starting from the 1st) whose parameters should not be modified.	
							* All B_ij (see below) with i<ncon are assumed to be zero
							���Ƶ���ڹ۲�ֵ����ʼ����
							pLidObjs�еĴ�0��ʼ��ngcp����Ϊ���Ƶ�*/
		LidMC_VP *pLidObjs,
		TiePoint_Adj_mode TiePt_mode,
		
		double *pX,			/* initial parameter vector p0: (a1, ..., am, b1, ..., bn).
												* aj are the image j parameters, bi are the i-th point parameters,
												* size m*cnp + n*pnp
												δ֪����ƫ�ķ����Ͱ��ý�    
												pX=[Xt, Yt, Zt, r, p, h]^T ����δ֪����ֵ��*/
		//				const int pnp,			/* number of parameters δ֪������*/
		orsLidYuanParam *pOriParam,           /*   �������� */
		int		nStrips                       /*   ������     */
//		unsigned long	param_type				/* ��У�������� */
		)
{
	double *L=NULL;
	double *X=NULL;
	double p_eL2;
	double init_p_eL2;
	int i, j, k;
	double *pA=NULL, *pAT=NULL;
	double  *pdX=NULL;	//δ֪��������
	POINT3D *pTiePt=NULL;
//	CAffineTransform	affineTrans;
//	LidMC_Patch *prePatch=NULL, *postPatch=NULL, *pTPatch=NULL;
	long *pLut=NULL;
	LidPt_SurvInfo *pATNPoints=NULL;
//	double *SrcParam;
//	POINT3D *ptSrc=NULL, *ptDst=NULL;
	double *hx=NULL, *hxx=NULL;
	double *pWeight=NULL;	//�۲�ֵȨ��
	
	//ͳ������������, tieID������������
	int tieID=-1;
	int nTies=0;  //��pLidObjs��Ϣ����ȡ�ж��ٸ����ӵ�
	int nObs=0;
	int /*nHorVer,*/ nHor, nVer;
	int nXDirection, nYDirection, nZDirection;
	double wXD, wYD, wZD;


	LidMC_VP *pLidVP = (LidMC_VP*)pLidObjs;
	
	/*nHorVer = */nHor = nVer = 0;
	for(i=0; i<nConjugatePts; i++)
	{//ͳ�����ӵ����
		if(tieID!=pLidVP[i].tieID)
		{
			nTies++;
			tieID=pLidVP[i].tieID;
		}

		if(TiePt_mode == LidAdj_TP_UseHorVer)
		{
			if(pLidVP[i].VP_type == TP_type_hor_ver /*&& Use_Hor_Ver*/)
			{
				nHor++;
				nVer++;
			}
			else if(pLidVP[i].VP_type == TP_type_horizon /*&& Use_Hor*/)
				nHor++;
			else if(pLidVP[i].VP_type == TP_type_vertical /*&& Use_Ver*/)
				nVer++;
		}
		else if(TiePt_mode == LidAdj_TP_UseHor)
		{
			if(pLidVP[i].VP_type == TP_type_hor_ver /*&& Use_Hor_Ver*/)
			{
				nHor++;
				
			}
			else if(pLidVP[i].VP_type == TP_type_horizon /*&& Use_Hor*/)
				nHor++;
		}
		else if(TiePt_mode == LidAdj_TP_UseVer)
		{
			if(pLidVP[i].VP_type == TP_type_hor_ver /*&& Use_Hor_Ver*/)
			{
				
				nVer++;
			}
			else if(pLidVP[i].VP_type == TP_type_vertical /*&& Use_Ver*/)
				nVer++;
		}
	}

	nObs = /*nHorVer*3 + */nHor*2 + nVer;
	
	nXDirection = /*nHorVer + */nHor;
	nYDirection = /*nHorVer + */nHor;
	nZDirection = /*nHorVer + */nVer;

	//��Ȩ���������������Ȩ
	//ֱ���޸Ĺ۲�ֵ���̣�Ȩ��������Σ�����Ȩֵȥƽ����
	if(nXDirection==0)
		wXD = 0;
	else
		wXD = sqrt(1.0/nXDirection);

	if(nYDirection==0)
		wYD = 0;
	else
		wYD = sqrt(1.0/nYDirection);

	if(nZDirection==0)
		wZD = 0;
	else
		wZD = sqrt(1.0/nZDirection);


	pATNPoints=new LidPt_SurvInfo[nConjugatePts];
	for(i=0; i<nConjugatePts; i++)
	{
		//��ȡ��ʵ�����
		pATNPoints[i].x=pLidVP[i].vx;
		pATNPoints[i].y=pLidVP[i].vy;
		pATNPoints[i].z=pLidVP[i].vz;
		pATNPoints[i].stripID = (*(pLidVP[i].pLidPoints))[0].stripID;
	}

	/////////////////////////////////////////////
	//////////////////��ȡб��//////////////////
	orsILidarGeoModel **pLidarModel=NULL;
	pLidarModel = new orsILidarGeoModel*[nStrips];
	for(i=0; i<nStrips; i++)
	{
		pLidarModel[i] = ORS_CREATE_OBJECT(orsILidarGeoModel,ORS_LIDAR_GEOMETRYMODEL_YUAN);

		pLidarModel[i]->setSysParam((void*)(&pOriParam[i]));
	}



	POINT3D *ptObs=NULL;
	ptObs=new POINT3D[nTies];	//���������ֵ (ȡƽ��)
	pLut=new long[nTies+1];

	int nTieGcp=0;   //���ӵ����ж��ٸ�gcp���ӵ�
	tieID=-1;
	for(i=0; i<ngcp; i++)
	{
		if(tieID!=pLidVP[i].tieID)
		{
			tieID=pLidVP[i].tieID;
			ptObs[tieID].X=pLidVP[i].eX;
			ptObs[tieID].Y=pLidVP[i].eY;
			ptObs[tieID].Z=pLidVP[i].eZ;

			nTieGcp++;
			pLut[tieID]=i;
		}
	}
	
	int nLinks=0;
	for(i=ngcp; i<nConjugatePts; i++)
	{
		if(tieID!=pLidVP[i].tieID)
		{
			tieID=pLidVP[i].tieID;
			pLut[tieID]=i;

			ptObs[tieID].X=pLidVP[i].vx;
			ptObs[tieID].Y=pLidVP[i].vy;
			ptObs[tieID].Z=pLidVP[i].vz;
		}
		else
		{
			ptObs[tieID].X+=pLidVP[i].vx;
			ptObs[tieID].Y+=pLidVP[i].vy;
			ptObs[tieID].Z+=pLidVP[i].vz;
		}
	}
	pLut[nTies]=nConjugatePts;

	for(i=nTieGcp; i<nTies; i++)
	{
		nLinks=pLut[i+1]-pLut[i];	//ÿ�����ӵ��Ӧ��Ƭ��
		//���ӵ�ȡƽ��ֵ
		ptObs[i].X/=nLinks;
		ptObs[i].Y/=nLinks;
		ptObs[i].Z/=nLinks;

		for(j=pLut[i]; j<pLut[i+1]; j++)
		{
			pLidVP[j].eX=ptObs[i].X;
			pLidVP[j].eY=ptObs[i].Y;
			pLidVP[j].eZ=ptObs[i].Z;
		}
	}

//	double *L=NULL;
	//��۲ⷽ�����
	L=new double[nObs];	//������
	hx=new double[nObs];
	hxx=new double[nObs];
	pWeight = new double[nObs];
	for(i=0; i<nObs; i++)
	{
		pWeight[i] = 1.0;
	}

	//��ȨЧ�����ã���ʱ�õ�Ȩ
	wXD = wYD = wZD = 1;

	for(i=0, j=0; i<nConjugatePts; i++)
	{
		if(TiePt_mode == LidAdj_TP_UseHorVer)
		{//ƽ���͸̵߳�
			if(pLidVP[i].VP_type == TP_type_hor_ver)
			{
				L[j]=pLidVP[i].eX * wXD;	pWeight[j] = wXD;	j++;
				L[j]=pLidVP[i].eY * wYD;	pWeight[j] = wYD;	j++;
				L[j]=pLidVP[i].eZ * wZD;	pWeight[j] = wZD;	j++;
			}
			else if(pLidVP[i].VP_type == TP_type_horizon)
			{
				L[j]=pLidVP[i].eX * wXD;	pWeight[j] = wXD;	j++;
				L[j]=pLidVP[i].eY * wYD;	pWeight[j] = wYD;	j++;
			}
			else if(pLidVP[i].VP_type == TP_type_vertical)
			{
				L[j]=pLidVP[i].eZ * wZD;	pWeight[j] = wXD;	j++;
			}
		}
		else if(TiePt_mode == LidAdj_TP_UseHor)
		{//ƽ���
			if(pLidVP[i].VP_type == TP_type_hor_ver)
			{
				L[j]=pLidVP[i].eX * wXD;	pWeight[j] = wXD;	j++;
				L[j]=pLidVP[i].eY * wYD;	pWeight[j] = wYD;	j++;
//				L[j]=pLidVP[i].eZ * wZD;	pWeight[j] = wZD;	j++;
			}
			else if(pLidVP[i].VP_type == TP_type_horizon)
			{
				L[j]=pLidVP[i].eX * wXD;	pWeight[j] = wXD;	j++;
				L[j]=pLidVP[i].eY * wYD;	pWeight[j] = wYD;	j++;
			}
		}
		else if(TiePt_mode == LidAdj_TP_UseVer)
		{//�̵߳�
			if(pLidVP[i].VP_type == TP_type_hor_ver)
			{
// 				L[j]=pLidVP[i].eX * wXD;	pWeight[j] = wXD;	j++;
// 				L[j]=pLidVP[i].eY * wYD;	pWeight[j] = wYD;	j++;
				L[j]=pLidVP[i].eZ * wZD;	pWeight[j] = wZD;	j++;
			}
			else if(pLidVP[i].VP_type == TP_type_vertical)
			{
				L[j]=pLidVP[i].eZ * wZD;	pWeight[j] = wXD;	j++;
			}
		}
	}

	int pnp=5;
// 	if(param_type & LidAdj_boreAngle)
// 		pnp += 3;
// 	if(param_type & LidAdj_leverArm)
// 		pnp += 3;
// 	if(param_type & LidAdj_GPS_drift)
// 		pnp += 6;
// 	if(param_type & LidAdj_INS_drift)
// 		pnp += 6;
	
	X=new double[pnp];
	memcpy(X, pX, sizeof(double)*pnp);
	

	//�뼤�����أ�һ��������Ӧ���������
//	ptSrc=new POINT3D[nVP*3];	
//	ptDst=new POINT3D[nVP*3];

	double opts[LM_OPTS_SZ], info[LM_INFO_SZ];
	opts[0]=LM_INIT_MU; opts[1]=1E-15; opts[2]=1E-15; opts[3]=1E-20; opts[4]= LM_DIFF_DELTA;

	struct myObs data;
	data.nMisalign=pnp;
	data.nTiePts=nTies;
	data.nObs=nObs;
	data.nTieGCP=nTieGcp;
// 	data.nVP=nVP;
 	data.pLidVP=pLidVP;
	data.TiePt_mode = TiePt_mode;
	data.pTiePtsLut=pLut;
	data.pLidPoints=pATNPoints;
	data.nPoints = nConjugatePts;
	data.L=L;
	//	data.SrcParam=SrcParam;
//	data.ptSrc=ptSrc;
//	data.ptDst=ptDst;
	data.hx=hx;
	data.hxx=hxx;
	data.pWeight = pWeight;

//	data.param_type=param_type;
// 	data.tx=pX[0];
// 	data.ty=pX[1];
// 	data.tz=pX[2];
// 	data.r=pX[3];
// 	data.p=pX[4];
// 	data.h=pX[5];
	data.pOriParam = pOriParam;
	data.pLidarModels = pLidarModel;
	data.nStrip = nStrips;


//#ifdef _DEBUG
	FILE *fp=NULL;
	fp=fopen(pszResultFile, "wt");
	if(fp==NULL)
	{
		assert(false);
		return -1;
	}

	fprintf(fp, "## calibration method: Corresponding Points\n");
	fprintf(fp, "## calibration model: Yuan's model\n");
	fprintf(fp, "TiePoint mode: %d\n", TiePt_mode);
	fprintf(fp, "## calib param type: ax, ay, az, kappa, omega\n");
	fprintf(fp, "## init calib param\n");
	for(i=0; i<pnp; i++)
	{
		fprintf(fp, "%.9lf\n", pX[i]);
	}

	fprintf(fp, "tiepoints: %d; total points=%d hor=%d ver=%d\n", nTies, nConjugatePts, /*nHorVer, */nHor, nVer);

	//���ֵ
	double *pdx=new double[nConjugatePts];
	double *pdy=new double[nConjugatePts];
	double *pdz=new double[nConjugatePts];
	double rms0, rms_x0, rms_y0, rms_z0, rms1, rms_x1, rms_y1, rms_z1;
	
//	data.L = NULL; //������L
	cal_Obs_ConjugatePts_Yuan(X, hx, pnp, nObs, &data);

	data.L = L;

	rms_x0 = rms_y0 = rms_z0 = 0.0;
	fprintf(fp, "before adjustment\n");
	/*nHorVer=0,*/ nHor=0, nVer=0;
	bool bjump;
	for(i=0, j=0; i<nConjugatePts;	i++)
	{
		bjump=true;
		if(TiePt_mode == LidAdj_TP_UseHorVer)
		{
			if(pLidVP[i].VP_type == TP_type_hor_ver)
			{
				pdx[i]=hx[j]-L[j];		j++;
				pdy[i]=hx[j]-L[j];		j++;
				pdz[i]=hx[j]-L[j];		j++;	

				nHor++;
				nVer++;
				bjump=false;
			}
			else if(pLidVP[i].VP_type == TP_type_horizon)
			{
				pdx[i]=hx[j]-L[j];		j++;
				pdy[i]=hx[j]-L[j];		j++;
				pdz[i]=0;	//ƽ��㣬�߳���0
				nHor++;
				bjump=false;
			}
			else if(pLidVP[i].VP_type == TP_type_vertical)
			{//�̵߳㣬ƽ����0
				pdx[i]=0;	
				pdy[i]=0;		
				pdz[i]=hx[j]-L[j];		j++;
				nVer++;
				bjump=false;
			}
		}
		else if(TiePt_mode == LidAdj_TP_UseHor)
		{
			if(pLidVP[i].VP_type == TP_type_hor_ver)
			{
				pdx[i]=hx[j]-L[j];		j++;
				pdy[i]=hx[j]-L[j];		j++;
				pdz[i]=0;			

				nHor++;
				bjump=false;
			}
			else if(pLidVP[i].VP_type & TP_type_horizon)
			{
				pdx[i]=hx[j]-L[j];		j++;
				pdy[i]=hx[j]-L[j];		j++;
				pdz[i]=0;	//ƽ��㣬�߳���0
				nHor++;
				bjump=false;
			}
		}
		else if(TiePt_mode == LidAdj_TP_UseVer)
		{
			if(pLidVP[i].VP_type & TP_type_hor_ver)
			{
				pdx[i]=0;		
				pdy[i]=0;		
				pdz[i]=hx[j]-L[j];		j++;	

				nVer++;
				bjump=false;
			}
			else if(pLidVP[i].VP_type & TP_type_vertical)
			{//�̵߳㣬ƽ����0
				pdx[i]=0;	
				pdy[i]=0;		
				pdz[i]=hx[j]-L[j];		j++;
				nVer++;
				bjump=false;
			}
		}
		
		if(!bjump)
		{
			fprintf(fp, "%d %d %.3f %.3f %.3f %.3f %.3f %.3f\n", pLidVP[i].tieID, i, pLidVP[i].vx,pLidVP[i].vy, pLidVP[i].vz, pdx[i], pdy[i], pdz[i]);

			rms_x0+=pdx[i]*pdx[i];
			rms_y0+=pdy[i]*pdy[i];
			rms_z0+=pdz[i]*pdz[i];
		}
	}
	rms0 = rms_x0+rms_y0+rms_z0;

	rms_x0=sqrt(rms_x0/(nHor));
	rms_y0=sqrt(rms_y0/(nHor));
	rms_z0=sqrt(rms_z0/(nVer));

	rms0 = sqrt(rms0/(2*nHor+nVer));
//	fprintf(fp, "####### RMS #######\n");
//	fprintf(fp, "%.5lf\n", rms);
//	fprintf(fp, "rms_x=%f rms_y=%f rms_z=%f\n", rms_x, rms_y, rms_z);
//#endif
	//�ⷽ��L=AX

	m_pn = pnp;
	if(m_covar)	delete m_covar;
	m_covar = new double[m_pn*m_pn];

	int ret=dlevmar_der(cal_Obs_ConjugatePts_Yuan,	//����L
								jac_ConjugatePts_Yuan, //����A
								X, L, pnp, nObs, 200, opts, info, NULL, m_covar, &data);

// 	int ret=dlevmar_dif(cal_VirtualPoint_VCP,	//����L
// 		X, L, pnp, nObs, 200, opts, info, NULL, m_covar, &data);

	m_rms = sqrt(info[1]/(nObs - pnp));

	if(m_corcoef) delete m_corcoef;
	m_corcoef = new double[pnp*pnp];

	k=0;
	for(i=0;i<pnp; i++)
	{
		double Dxx = sqrt(m_covar[i*pnp+i]);
		for(j=0; j<pnp; j++)
		{
			double Dyy = sqrt(m_covar[j*pnp+j]);
			double Dxy = m_covar[i*pnp+j];

			m_corcoef[k] = Dxy/Dxx/Dyy;
			k++;
		}
	}

//	int ret=dlevmar_der(distance_PointToPlane, jac_dis_PointToPlane, X, L, pnp, nVP, 1000, opts, info, NULL, NULL, &data);

//#ifdef _DEBUG
	cal_Obs_ConjugatePts_Yuan(X, hx, pnp, nObs, &data);
	rms_x1=rms_y1=rms_z1=0.0;
	fprintf(fp, "after adjustment\n");
	for(i=0, j=0; i<nConjugatePts;	i++)
	{
		bjump=true;
		if(TiePt_mode == LidAdj_TP_UseHorVer)
		{
			if(pLidVP[i].VP_type == TP_type_hor_ver)
			{
				pdx[i]=hx[j]-L[j];		j++;
				pdy[i]=hx[j]-L[j];		j++;
				pdz[i]=hx[j]-L[j];		j++;	

				nHor++;
				nVer++;
				bjump=false;
			}
			else if(pLidVP[i].VP_type == TP_type_horizon)
			{
				pdx[i]=hx[j]-L[j];		j++;
				pdy[i]=hx[j]-L[j];		j++;
				pdz[i]=0;	//ƽ��㣬�߳���0
				nHor++;
				bjump=false;
			}
			else if(pLidVP[i].VP_type == TP_type_vertical)
			{//�̵߳㣬ƽ����0
				pdx[i]=0;	
				pdy[i]=0;		
				pdz[i]=hx[j]-L[j];		j++;
				nVer++;
				bjump=false;
			}
		}
		else if(TiePt_mode == LidAdj_TP_UseHor)
		{
			if(pLidVP[i].VP_type == TP_type_hor_ver)
			{
				pdx[i]=hx[j]-L[j];		j++;
				pdy[i]=hx[j]-L[j];		j++;
				pdz[i]=0;			

				nHor++;
				bjump=false;
			}
			else if(pLidVP[i].VP_type & TP_type_horizon)
			{
				pdx[i]=hx[j]-L[j];		j++;
				pdy[i]=hx[j]-L[j];		j++;
				pdz[i]=0;	//ƽ��㣬�߳���0
				nHor++;
				bjump=false;
			}
		}
		else if(TiePt_mode == LidAdj_TP_UseVer)
		{
			if(pLidVP[i].VP_type & TP_type_hor_ver)
			{
				pdx[i]=0;		
				pdy[i]=0;		
				pdz[i]=hx[j]-L[j];		j++;	

				nVer++;
				bjump=false;
			}
			else if(pLidVP[i].VP_type & TP_type_vertical)
			{//�̵߳㣬ƽ����0
				pdx[i]=0;	
				pdy[i]=0;		
				pdz[i]=hx[j]-L[j];		j++;
				nVer++;
				bjump=false;
			}
		}

		if(!bjump)
		{
			fprintf(fp, "%d %d %.3f %.3f %.3f %.3f %.3f %.3f\n", pLidVP[i].tieID, i, 
				pLidVP[i].vx,pLidVP[i].vy, pLidVP[i].vz, pdx[i], pdy[i], pdz[i]);

			rms_x1+=pdx[i]*pdx[i];
			rms_y1+=pdy[i]*pdy[i];
			rms_z1+=pdz[i]*pdz[i];
		}
	}
	rms1 = rms_x1+rms_y1+rms_z1;
	rms_x1=sqrt(rms_x1/(nHor));
	rms_y1=sqrt(rms_y1/(nHor));
	rms_z1=sqrt(rms_z1/(nVer));
	rms1 = sqrt(rms1/(2*nHor+nVer));
	
	fprintf(fp, "## parameters estmation\n");
	for(i=0; i<pnp; i++)
	{
		fprintf(fp, "%.9lf\n", X[i]);
	}

	fprintf(fp, "####### RMS #######\n");
	fprintf(fp, "before adjustment:\n");
//	rms = rms_x0+rms_y0+rms_z0;
	
	fprintf(fp, "rms_x=%f rms_y=%f rms_z=%f\n", rms_x0, rms_y0, rms_z0);
	fprintf(fp, "%.5lf\n", rms0);

	fprintf(fp, "after adjustment:\n");
// 	rms = rms_x1+rms_y1+rms_z1;
// 	rms = sqrt(rms/(2*nHor+nVer));
	fprintf(fp, "rms_x=%f rms_y=%f rms_z=%f\n", rms_x1, rms_y1, rms_z1);
	fprintf(fp, "%.5lf\n", rms1);

	if(rms1 < m_rms)
	{
		;
	}
	else
	{
		;
	}
	//	rms = sqrt(rms/(2*nHor+nVer));
	//	fprintf(fp, "####### RMS #######\n");
	//	fprintf(fp, "%.5lf\n", rms);

	fprintf(fp, "####### covariance of unknowns #######\n");
	fprintf(fp, "number of unknowns : %d\n", m_pn);
	for(i=0; i<m_pn; i++)
	{
		for(j=0; j<m_pn; j++)
		{	
			k = i*m_pn + j;
			fprintf(fp, " %e", m_covar[k]);
		}
		fprintf(fp, "\n");
	}

	fprintf(fp, "####### correlation coefficient of unknowns #######\n");
	k=0;
	for(i=0; i<pnp; i++)
	{
		for(j=0; j<pnp; j++)
		{
// 			if(i<pnp-1 && j>i)
// 			{
				fprintf(fp," %e", fabs(m_corcoef[k]));
				k++;
// 			}
// 			else
// 				fprintf(fp, " -");
		}
		fprintf(fp, "\n");
	}
	if(fp)	fclose(fp);	fp=NULL;

	/////////////////////////////////////////////////////
	///////������У����/////////////////////////////////
	orsString strTmp;
	orsString strPath = orsString::getDirFromPath(pszResultFile);
	orsString purename;
	strTmp = orsString::getPureFileName(pszResultFile);
	int pos = strTmp.reverseFind( '.');
	purename = strTmp.left(pos);
	orsString cParamFile = strPath + "\\";
	cParamFile += purename;
	cParamFile += ".cp"; //calib param file

	pOriParam[0].ax = X[0]; pOriParam[0].ay = X[1]; pOriParam[0].az = X[2];
	pOriParam[0].kappa = X[3]; pOriParam[0].omega = X[4];
	write_CPfile(cParamFile, LCMT_Yuan, (void*)pOriParam, 1);

	if(pdx)		delete pdx;			pdx=NULL;
	if(pdy)		delete pdy;			pdy=NULL;
	if(pdz)		delete pdz;			pdz=NULL;
//#endif

	if(ptObs)		delete ptObs;		ptObs=NULL;
	if(pLut)		delete pLut;		pLut=NULL;
	if(L)			delete L;			L=NULL;
	if(X)			delete X;			X=NULL;
	if(pATNPoints)	delete pATNPoints;	pATNPoints=NULL;
//	if(SrcParam) delete SrcParam;	SrcParam=NULL;
// 	if(ptSrc)     delete ptSrc;		ptSrc=NULL;
// 	if(ptDst)		delete ptDst;		ptDst=NULL;
	if(hx)			delete hx;			hx=NULL;
	if(hxx)			delete hxx;			hxx=NULL;
	if(pWeight)		delete pWeight;		pWeight=NULL;

	if(pLidarModel)	delete pLidarModel;	pLidarModel=NULL;
	return ret;
}

int  CLidCalib_Yuan::LidMC_QA(
	const int nVP,
	const int ngcp,
	LidMC_VP *pLidVP,
	double *pInitX,			/* δ֪����ֵ*/
	double *pAdjX,			/* δ֪��ƽ��ֵ*/
	const int pnp,			/* number of parameters δ֪������*/
	Calib_Method  mType,
	orsLidYuanParam oriParam,
	unsigned long	param_type		/* ��������*/
	)
{
	return 0;
}