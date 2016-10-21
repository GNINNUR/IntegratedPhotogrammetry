#ifndef _ORS_ALGORITHM_CAMERA_CALIBRATION_INCLUDE_
#define _ORS_ALGORITHM_CAMERA_CALIBRATION_INCLUDE_

#include "orsIBundler.h"
#include "Calib_Camera.h"
#include "CombineAdjustment/BA_def.h"

class orsAlgCameraCalib : public orsIBundler
{

public:
	//������ƽ��궨
	virtual int bundle( void *adata );

	//ֱ������sparseLM���
	virtual int bundle_SparseLM( void *adata );

	orsAlgCameraCalib();
	virtual ~orsAlgCameraCalib();

//ǰ��-�󽻱궨��
public:
	int calib_IP( void *adata );
	//����LM�����
	int calib_IP_LM( void *adata );
};


#endif