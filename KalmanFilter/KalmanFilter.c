#include "KalmanFilter.h"
#include "malloc.h"
#include "matrix.h"
#include <stdbool.h>


/***************************************************************
	Brief:
		初始化卡尔曼滤波器，输入数据以一维数组形式输入
	Parameters:
		_preditMatrix:预测矩阵
		_measureMatrix:测量矩阵		
		_covPredictMatrix:预测矩阵误差的协方差矩阵（实对称矩阵）
		_covMeasureMatrix:测量矩阵误差的协方差矩阵
		initPostLastCovErrorMatrix:初始化一个得数误差的协方差矩阵
		initpostPredictData:初始化一个预测的值
		dim:数据维度（如dim=3时3*3矩阵应输入float[9],数据向量输入float[3])）
	Return:
		Matrix*
***************************************************************/

KalmanFilter* KalmanFilterCreate(float* _preditMatrix, float* _measureMatrix, float* _covPredictMatrix,
								float* _covMeasureMatrix, float* initPostLastCovErrorMatrix, float* initPostPredictData, unsigned int dim)
{
	static matrixManager _matrixManager;
	_matrixManager = matrixManagerCreate();
	matrix* identicalMatrix;	//I
	matrix* nowPredictData_;	//X(k)
	matrix* nowPredictData;		//X(k)~
	matrix* lastPredictData;	//X(k-1)
	matrix* measureData;		//Z(k)
	matrix* predictMatrix;		//A
	matrix* measureMatrix;		//H
	matrix* covPredictMatrix;	//Q
	matrix* covMeasureMatrix;	//R
	matrix* nowCovErrorMatrix_;	//P(k)~
	matrix* nowCovErrorMatrix;	//P(k)
	matrix* lastCovErrorMatrix_;//P(k-1)~暂未使用
	matrix* lastCovErrorMatrix;	//P(k-1)
	matrix* gainMatrix;			//K

	identicalMatrix = identicalMatrixCreate(&_matrixManager, dim);
	nowPredictData_ = matrixCreate(&_matrixManager, dim, 1, NULL, 1);
	nowPredictData = matrixCreate(&_matrixManager, dim, 1, NULL, 1);
	lastPredictData = matrixCreate(&_matrixManager, dim, 1, initPostPredictData, 1);
	measureData = matrixCreate(&_matrixManager, dim, 1, NULL, 1);
	predictMatrix = matrixCreate(&_matrixManager, dim, dim, _preditMatrix, 1);
	measureMatrix = matrixCreate(&_matrixManager, dim, dim, _measureMatrix, 1);
	covPredictMatrix = matrixCreate(&_matrixManager, dim, dim, NULL, 1);
	covMeasureMatrix = matrixCreate(&_matrixManager, dim, dim, NULL, 1);
	nowCovErrorMatrix_ = matrixCreate(&_matrixManager, dim, dim, NULL, 1);
	nowCovErrorMatrix = matrixCreate(&_matrixManager, dim, dim, NULL, 1);
	lastCovErrorMatrix_ = matrixCreate(&_matrixManager, dim, dim, NULL, 1);
	lastCovErrorMatrix = matrixCreate(&_matrixManager, dim, dim, initPostLastCovErrorMatrix, 1);
	gainMatrix = matrixCreate(&_matrixManager, dim, dim, NULL, 1);
	static KalmanFilter ret;
	ret.dim = dim,
	ret.nowPredictData_ = nowPredictData_;
	ret.nowPredictData = nowPredictData;
	ret.lastPredictData = lastPredictData;
	ret.measureData = measureData;
	ret.predictMatrix = predictMatrix;
	ret.measureMatrix = measureMatrix;
	ret.covPredictMatrix = covPredictMatrix;
	ret.covMeasureMatrix = covMeasureMatrix;
	ret.nowCovErrorMatrix_ = nowCovErrorMatrix_;
	ret.nowCovErrorMatrix = nowCovErrorMatrix;
	ret.lastCovErrorMatrix_ = lastCovErrorMatrix_;
	ret.lastCovErrorMatrix = lastCovErrorMatrix;
	ret.gainMatrix = gainMatrix;
	ret.identicalMatrix = identicalMatrix;
	ret._matrixManager = &_matrixManager;
	return &ret;
}

/***************************************************************
	Brief:
		更新测量数据，每次KalmanFilterProcced()自动调用
	Parameters:
		this:Create创建的一个卡尔曼滤波器结构体
		data:传进来的一个n个长度的一维数组
		dim:数组维度
	Return:
		Matrix*
***************************************************************/

static void measureDataUpdate(KalmanFilter* this, float* data, unsigned int dim)
{
	this->measureData = matrixCreate(this->_matrixManager, dim, 1, data, 0);
}

/***************************************************************
	Brief:
		进行滤波，可以定义一个n*1矩阵接收数据向量，也可以通过this->nowPredictData->data[x][0]
		这条路径获取滤波后的数据
	Parameters:
		this:Create创建的一个卡尔曼滤波器结构体
		data:传进来的一个n个长度的一维数组
		dim:数组维度
	Return:
		Matrix*
***************************************************************/
matrix* KalmanFilterProcced(KalmanFilter* this, float* data, unsigned int dim)
{
	measureDataUpdate(this, data, dim);
	
	this->nowPredictData_ = SmProd(this->predictMatrix, this->lastPredictData, this->nowPredictData_);//X(k)~=A*X(k-1)~
	this->nowCovErrorMatrix_ = SmAdd(mComProd(this->predictMatrix, this->lastCovErrorMatrix), this->covPredictMatrix, this->nowCovErrorMatrix_);//P(k)~=A*P(k-1)A^T+Q

	this->gainMatrix = SmProd(mProd(this->nowCovErrorMatrix_, mTrans(this->measureMatrix)),
		mInv(mAdd(mComProd(this->measureMatrix, this->nowCovErrorMatrix_), this->covMeasureMatrix)) , this->gainMatrix);//K=(P(k)~*H^T)*(H*P(k)^-1*H^T+R)

	this->nowPredictData = SmAdd(this->nowPredictData_, mProd(this->gainMatrix, mSub(this->measureData, mProd(this->measureMatrix, this->nowPredictData_))) , this->nowPredictData);//X(k)=X(k)~+K*(Z(k)-H*X(k)~)

	this->nowCovErrorMatrix = SmProd(mSub(this->identicalMatrix, mProd(this->gainMatrix, this->measureMatrix)), this->nowCovErrorMatrix_, this->nowCovErrorMatrix);//P(k)=(I-K*H)*P(k)~

	matrixPrintf(this->_matrixManager);/*打印矩阵信息*/
	matrixProErase(this->_matrixManager);/*清理过程中产生的矩阵*/
	matrixPrintf(this->_matrixManager);/*打印清理过程中产生的矩阵后的矩阵信息*/

	return this->nowPredictData;
}

/***************************************************************
	Brief:
		销毁滤波器，在主函数结束前务必调用，销毁动态内存，销毁完后再次调用要再次Create一个KalmanFilter
	Parameters:
		data:传进来的一个n个长度的一维数组
		dim:数组维度
	Return:
		Matrix*
***************************************************************/
void KalmanFilterDestroy(KalmanFilter* this)
{
	matrixAllErase(this->_matrixManager);
	matrixPrintf(this->_matrixManager); /*打印清空后所有矩阵的矩阵的信息*/
}





