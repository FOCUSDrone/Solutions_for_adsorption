#pragma once
#ifdef __cplusplus 
extern "C" {
#endif 
#include <stdbool.h>
	typedef struct {
		unsigned int row;
		unsigned int column;
		float** data;
	}matrix;

	typedef struct {
		matrix** matrixArray;
		unsigned int nowNum;
		unsigned int maxNum;
	}matrixGroup;

	typedef struct {
		matrixGroup oriMatrixGroup;
		matrixGroup proMatrixGroup;
		/*matrix (*matrixCreate) (struct matrixManager*, unsigned int, unsigned, float*);
		void (*matrixDeleteOne) (matrix*);
		void (*matrixoriAppend) (struct matrixManager*, matrix*);
		void (*matrixProAppend) (struct matrixManager*, matrix*);
		void (*matrixProErase) (struct matrixManager*);
		void (*matrixAllErase) (struct matrixManager*);
		void (*matrixPrintf) (struct matrixManager*);
		matrix (*matrixAdd)(struct matrixManager*, matrix*, matrix*);
		matrix(*matrixSubstract)(struct matrixManager*, matrix*, matrix*);
		matrix(*matrixDotProduct)(struct matrixManager*, matrix*, matrix*);
		matrix(*matrixNumProduct)(struct matrixManager*, matrix*, float);
		matrix(*matrixProduct)(struct matrixManager*, matrix*, matrix*);
		matrix(*matrixTranspose) (struct matrixManager*, matrix*);
		matrix (*matrixInverse) (struct matrixManager* , matrix*);*/
	}matrixManager;
	matrixManager matrixManagerCreate();
	matrix* matrixCreate(matrixManager*, unsigned int, unsigned int, float*, bool);
	matrix* identicalMatrixCreate(matrixManager*, unsigned int);
	void matrixDeleteOne(matrix*);
	void matrixOriAppend(matrixManager*, matrix*);
	void matrixProAppend(matrixManager*, matrix*);
	void matrixProErase(matrixManager*);
	void matrixAllErase(matrixManager*);
	void matrixPrintf(matrixManager*);
	matrix* matrixAdd(matrixManager*, matrix*, matrix*, matrix*);
	matrix* matrixSubstract(matrixManager*, matrix*, matrix*, matrix*);
	matrix* matrixDotProduct(matrixManager*, matrix*, matrix*, matrix*);
	matrix* matrixNumProduct(matrixManager*, matrix*, float, matrix*);
	matrix* matrixProduct(matrixManager*, matrix*, matrix*, matrix*);
	matrix* matrixTranspose(matrixManager*, matrix*, matrix*);
	matrix* matrixInverse(matrixManager*, matrix*, matrix*);
	matrix* matrixCombineProduct(matrixManager*, matrix*, matrix*, matrix*);

#define mAdd(x,y) matrixAdd(this->_matrixManager, x, y, NULL)
#define mSub(x,y) matrixSubstract(this->_matrixManager, x, y, NULL)
#define mProd(x,y) matrixProduct(this->_matrixManager, x, y, NULL)
#define mDotProd(xy) matrixDotProduct(this->_matrixManager, x, y, NULL)
#define mNumProd(x,y) matrixNumProduct(this->_matrixManager, x, y, NULL)
#define mTrans(x) matrixTranspose(this->_matrixManager, x, NULL)
#define mInv(x) matrixInverse(this->_matrixManager, x, NULL)
#define mComProd(x,y) matrixCombineProduct(this->_matrixManager, x, y, NULL)

#define SmAdd(x,y,s) matrixAdd(this->_matrixManager, x, y, s)
#define SmSub(x,y,s) matrixSubstract(this->_matrixManager, x, y, s)
#define SmProd(x,y,s) matrixProduct(this->_matrixManager, x, y, s)
#define SmDotProd(x,y,s) matrixDotProduct(this->_matrixManager, x, y, s)
#define SmNumProd(x,y,s) matrixNumProduct(this->_matrixManager, x, y, s)
#define SmTrans(x,s) matrixTranspose(this->_matrixManager, x, s)
#define SmInv(x,s) matrixInverse(this->_matrixManager, x, s)
#define SmComProd(x,y,s) matrixCombineProduct(this->_matrixManager, x, y, s)

#ifdef __cplusplus 
}
#endif 
