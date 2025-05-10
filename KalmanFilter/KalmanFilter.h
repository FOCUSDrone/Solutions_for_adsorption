#pragma once
#ifdef __cplusplus 
extern "C" {
#endif 
#include "matrix.h"
	typedef struct {//带"_"表先验
		unsigned int dim;
		matrix* nowPredictData_;
		matrix* nowPredictData;
		matrix* lastPredictData;
		matrix* measureData;
		matrix* predictMatrix;
		matrix* measureMatrix;
		matrix* covPredictMatrix;
		matrix* covMeasureMatrix;
		matrix* nowCovErrorMatrix_;
		matrix* nowCovErrorMatrix;
		matrix* lastCovErrorMatrix_;
		matrix* lastCovErrorMatrix;
		matrix* gainMatrix;
		matrix* identicalMatrix;
		matrixManager* _matrixManager;
	}KalmanFilter;
	KalmanFilter* KalmanFilterCreate(float* _preditMatrix, float* _measureMatrix, float* initPostLastCovErrorMatrix, float* _covPredictMatrix, float* _covMeasureMatrix, float* initpostPredictData, unsigned int dim);
	matrix* KalmanFilterProcced(KalmanFilter* this, float* data, unsigned int num);
	void KalmanFilterDestroy(KalmanFilter* this);


#ifdef __cplusplus 
}
#endif
