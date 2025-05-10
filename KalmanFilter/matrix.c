#include <stdio.h>
#include "matrix.h"
#include <malloc.h>
#include <string.h>
#include <stdbool.h>

int count, _count, countRow, countCol;

void matrixOriAppend(matrixManager* this, matrix* _matrix)
{
	if (this->oriMatrixGroup.nowNum == this->oriMatrixGroup.maxNum)
	{
		this->oriMatrixGroup.maxNum += 20;
		matrix** p = (matrix**)realloc(
			this->oriMatrixGroup.matrixArray,
			this->oriMatrixGroup.maxNum * sizeof(matrix*));
		if (p == NULL)
			return;
		this->oriMatrixGroup.matrixArray = p;
	}
	this->oriMatrixGroup.matrixArray[this->oriMatrixGroup.nowNum]
		= _matrix;
	this->oriMatrixGroup.nowNum += 1;
}

void matrixProAppend(matrixManager* this, matrix* _matrix)
{

	if (this->proMatrixGroup.nowNum == this->proMatrixGroup.maxNum)
	{
		this->proMatrixGroup.maxNum += 40;
		matrix** p =
			(matrix**)realloc(this->proMatrixGroup.matrixArray
				, this->proMatrixGroup.maxNum * sizeof(matrix*));
		if (p == NULL)
			return;
		this->proMatrixGroup.matrixArray = p;
	}
	this->proMatrixGroup.matrixArray[this->proMatrixGroup.nowNum]
		= _matrix;
	this->proMatrixGroup.nowNum += 1;
}

static void matrixDeleteOne(matrix* _matrix)
{
	//if (_matrix == NULL||_matrix->data==NULL)
	//	return;
	if (_matrix == NULL || _matrix->data != NULL)
		return;
	for (count = 0; count < _matrix->row; count++)
	{
		if (_matrix->data[count] != NULL)
			free(_matrix->data[count]);
		_matrix->data[count] = NULL;
	}
	if (_matrix->data != NULL)
		free(_matrix->data);
	_matrix->data = NULL;
	if (_matrix != NULL)
		free(_matrix);
	_matrix = NULL;
}

void matrixProErase(matrixManager* this)
{
	for (count = 0; count < this->proMatrixGroup.nowNum; count++)
	{
		matrixDeleteOne(this->proMatrixGroup.matrixArray[count]);
	}
	this->proMatrixGroup.nowNum = 0;
	this->proMatrixGroup.maxNum = 20;
}

void matrixAllErase(matrixManager* this)
{
	for (count = 0; count < this->proMatrixGroup.nowNum; count++)
	{
		if (this->proMatrixGroup.matrixArray[count] != NULL)
			matrixDeleteOne(this->proMatrixGroup.matrixArray[count]);
	}
	this->proMatrixGroup.nowNum = 0;
	this->proMatrixGroup.maxNum = 40;
	this->proMatrixGroup.matrixArray = (matrix**)malloc(sizeof(matrix*) * 40);
	for (count = 0; count < this->oriMatrixGroup.nowNum; count++)
	{
		if (this->oriMatrixGroup.matrixArray[count] != NULL)
			matrixDeleteOne(this->oriMatrixGroup.matrixArray[count]);
	}
	this->oriMatrixGroup.matrixArray = (matrix**)malloc(sizeof(matrix*) * 20);
	this->oriMatrixGroup.nowNum = 0;
	this->oriMatrixGroup.maxNum = 20;
}

matrix* matrixCreate(matrixManager* this, unsigned int row, unsigned int column, float* data, bool ifOri)
{
	matrix* ret = (matrix*)malloc(sizeof(matrix));
	//matrix ret = { row, column, NULL};
	if (ret == NULL)
	{
		printf("Malloc Error!\r\n");
		return NULL;
	}
	ret->column = column;
	ret->row = row;
	ret->data = NULL;
	ret->data = (float**)malloc(sizeof(float*) * row);
	if (ret->data == NULL)
	{
		printf("Create Matrix Fail!");
		return ret;
	}
	for (count = 0; count < row; count++)
	{
		ret->data[count] = (float*)malloc(sizeof(float) * column);
		if (ret->data[count] == NULL)
		{
			printf("Create Matrix Fail!");
			return ret;
		}
		if (data != NULL)
			memcpy(ret->data[count], data + count * column, sizeof(float) * column);
		else
			memset(ret->data[count], 0, sizeof(float) * column);
	}
	if (ifOri)
		matrixOriAppend(this, ret);
	else
		matrixProAppend(this, ret);
	return ret;
}

void matrixPrintf(matrixManager* this)
{
	printf("ori:\r\n");
	printf("%d\r\n", this->oriMatrixGroup.nowNum);
	printf("%d\r\n", this->oriMatrixGroup.maxNum);
	for (count = 0; count < this->oriMatrixGroup.nowNum; count++)
	{
		for (countRow = 0; countRow < this->oriMatrixGroup.matrixArray[count]->row; countRow++)
		{
			for (countCol = 0; countCol < this->oriMatrixGroup.matrixArray[count]->column; countCol++)
			{
				//if (this->oriMatrixGroup.matrixArray[count]!= NULL && this->oriMatrixGroup.matrixArray[count]->data != NULL)
				printf("%f  ", this->oriMatrixGroup.matrixArray[count]->data[countRow][countCol]);
			}
			printf("\r\n");
		}
		printf("------------------------------------------\r\n");
	}
	printf("pro:\r\n");
	printf("%d\r\n", this->proMatrixGroup.nowNum);
	printf("%d\r\n", this->proMatrixGroup.maxNum);
	for (count = 0; count < this->proMatrixGroup.nowNum; count++)
	{
		for (countRow = 0; countRow < this->proMatrixGroup.matrixArray[count]->row; countRow++)
		{
			for (countCol = 0; countCol < this->proMatrixGroup.matrixArray[count]->column; countCol++)
			{
				//if (this->proMatrixGroup.matrixArray[count]!= NULL && this->proMatrixGroup.matrixArray[count]->data!=NULL)
				printf("%f  ", this->proMatrixGroup.matrixArray[count]->data[countRow][countCol]);
			}
			printf("\r\n");
		}
		printf("------------------------------------------\r\n");
	}
}

matrix* identicalMatrixCreate(matrixManager* this, unsigned int dim)
{
	matrix* ret = matrixCreate(this, dim, dim, NULL, 1);
	for (count = 0; count < dim; count++)
		ret->data[count][count] = 1;
	return ret;
}

matrix* matrixCopy(matrixManager* this, matrix* _matrix, bool ifOri)
{
	//static matrix ret;
	matrix* ret = (matrix*)malloc(sizeof(matrix));
	if (ret == NULL)
	{
		printf("Malloc Error!\r\n");
		return NULL;
	}
	ret->column = _matrix->column;
	ret->row = _matrix->row;
	ret->data = NULL;
	ret->data = (float**)malloc(sizeof(float*) * _matrix->row);
	if (ret->data == NULL)
	{
		printf("Copy Matrix Fail!");
		return ret;
	}
	for (count = 0; count < _matrix->row; count++)
	{
		ret->data[count] = (float*)malloc(sizeof(float) * _matrix->column);
		if (ret->data[count] == NULL)
		{
			printf("Copy Matrix Fail!");
			return ret;
		}
		if (_matrix->data != NULL)
			memcpy(ret->data[count], _matrix->data[count], sizeof(float) * _matrix->column);
		else
			memset(ret->data[count], 0, sizeof(float) * _matrix->column);
	}
	if (ifOri)
		matrixOriAppend(this, ret);
	else
		matrixProAppend(this, ret);
	return ret;
}

matrix* matrixAdd(matrixManager* this, matrix* matrixA, matrix* matrixB, matrix* saveMatrix)
{
	if (matrixA->column != matrixB->column || matrixA->row != matrixB->row)
	{
		printf("Matrices Size Match Error! Your linear algebra is a piece of shit!\r\n");
		return matrixA;
	}
	//static matrix ret;
	matrix* ret;
	if (saveMatrix == NULL)
	{
		ret = matrixCreate(this, matrixA->row, matrixA->column, NULL, 0);
	}
	else if ((saveMatrix->column != matrixA->column || saveMatrix->row != matrixA->row))
	{
		printf("Save Error! Matrix Size Not Match!");
		ret = matrixCreate(this, matrixA->row, matrixA->column, NULL, 0);
	}
	else {
		ret = saveMatrix;
	}
	for (countRow = 0; countRow < matrixA->row; countRow++)
		for (countCol = 0; countCol < matrixA->column; countCol++)
			ret->data[countRow][countCol] = matrixA->data[countRow][countCol] + matrixB->data[countRow][countCol];
	return ret;
}

matrix* matrixSubstract(matrixManager* this, matrix* matrixA, matrix* matrixB, matrix* saveMatrix)
{
	if (matrixA->column != matrixB->column || matrixA->row != matrixB->row)
	{
		printf("Matrices Size Match Error! Your linear algebra is a piece of shit!\r\n");
		return matrixA;
	}
	//static matrix ret;
	matrix* ret;
	if (saveMatrix == NULL)
	{
		ret = matrixCreate(this, matrixA->row, matrixA->column, NULL, 0);
	}
	else if ((saveMatrix->column != matrixA->column || saveMatrix->row != matrixA->row))
	{
		printf("Save Error! Matrix Size Not Match!");
		ret = matrixCreate(this, matrixA->row, matrixA->column, NULL, 0);
	}
	else {
		ret = saveMatrix;
	}
	for (countRow = 0; countRow < matrixA->row; countRow++)
		for (countCol = 0; countCol < matrixA->column; countCol++)
			ret->data[countRow][countCol] = matrixA->data[countRow][countCol] - matrixB->data[countRow][countCol];
	return ret;
}

matrix* matrixDotProduct(matrixManager* this, matrix* matrixA, matrix* matrixB, matrix* saveMatrix)
{
	if (matrixA->column != matrixB->column || matrixA->row != matrixB->row)
	{
		printf("Matrices Size Match Error! Your linear algebra is a piece of shit!\r\n");
		return matrixA;
	}
	//static matrix ret;
	matrix* ret;
	if (saveMatrix == NULL)
	{
		ret = matrixCreate(this, matrixA->row, matrixA->column, NULL, 0);
	}
	else if ((saveMatrix->column != matrixA->column || saveMatrix->row != matrixA->row))
	{
		printf("Save Error! Matrix Size Not Match!");
		ret = matrixCreate(this, matrixA->row, matrixA->column, NULL, 0);
	}
	else {
		ret = saveMatrix;
	}
	for (countRow = 0; countRow < matrixA->row; countRow++)
		for (countCol = 0; countCol < matrixA->column; countCol++)
			ret->data[countRow][countCol] = matrixA->data[countRow][countCol] * matrixB->data[countRow][countCol];
	return ret;
}

matrix* matrixNumProduct(matrixManager* this, matrix* matrixA, float num, matrix* saveMatrix)
{
	//static matrix ret;
	matrix* ret;
	if (saveMatrix == NULL)
	{
		ret = matrixCreate(this, matrixA->row, matrixA->column, NULL, 0);
	}
	else if ((saveMatrix->column != matrixA->column || saveMatrix->row != matrixA->row))
	{
		printf("Save Error! Matrix Size Not Match!");
		ret = matrixCreate(this, matrixA->row, matrixA->column, NULL, 0);
	}
	else {
		ret = saveMatrix;
	}
	for (countRow = 0; countRow < matrixA->row; countRow++)
		for (countCol = 0; countCol < matrixA->column; countCol++)
			ret->data[countRow][countCol] = matrixA->data[countRow][countCol] * num;
	return ret;
}

matrix* matrixProduct(matrixManager* this, matrix* matrixA, matrix* matrixB, matrix* saveMatrix)
{
	if (matrixA->column == 1 && matrixA->row == 1)
		return matrixNumProduct(this, matrixB, **matrixA->data, saveMatrix);
	else if (matrixB->column == 1 && matrixB->row == 1)
		return matrixNumProduct(this, matrixA, **matrixB->data, saveMatrix);
	else if (matrixA->column != matrixB->row)
	{
		printf("Matrices Size Match Error! Your linear algebra is a piece of shit!\r\n");
		return matrixA;
	}
	//static matrix ret;
	matrix* ret;
	if (saveMatrix == NULL)
	{
		ret = matrixCreate(this, matrixA->row, matrixB->column, NULL, 0);
	}
	else if ((saveMatrix->column != matrixB->column || saveMatrix->row != matrixA->row))
	{
		printf("Save Error! Matrix Size Not Match!");
		ret = matrixCreate(this, matrixA->row, matrixA->column, NULL, 0);
	}
	else {
		ret = saveMatrix;
	}
	for (countRow = 0; countRow < matrixA->row; countRow++)
		for (countCol = 0; countCol < matrixB->column; countCol++)
		{
			for (count = 0; count < matrixA->column; count++)
			{
				ret->data[countRow][countCol] += matrixA->data[countRow][count] * matrixB->data[count][countCol];
			}
		}
	return ret;
}

matrix* matrixTranspose(matrixManager* this, matrix* _matrix, matrix* saveMatrix)
{
	//static matrix ret;
	matrix* ret;
	if (saveMatrix == NULL)
	{
		ret = matrixCreate(this, _matrix->column, _matrix->row, NULL, 0);
	}
	else if ((saveMatrix->column != _matrix->row || saveMatrix->row != _matrix->column))
	{
		printf("Save Error! Matrix Size Not Match!");
		ret = matrixCreate(this, _matrix->column, _matrix->row, NULL, 0);
	}
	else {
		ret = saveMatrix;
	}
	for (countRow = 0; countRow < _matrix->column; countRow++)
		for (countCol = 0; countCol < _matrix->row; countCol++)
			ret->data[countRow][countCol] = _matrix->data[countCol][countRow];
	return ret;
}

matrix* matrixInverse(matrixManager* this, matrix* submatrix, matrix* saveMatrix)
{
	matrix* _matrix = matrixCopy(this, submatrix, 0);
	//	matrix _matrix = *matrixNumProduct(this, submatrix, 1);
	float operation = 0;
	if (_matrix->column != _matrix->row)
	{
		printf("Matrices Not Square! Your linear algebra is a piece of shit!\r\n");
		return _matrix;
	}
	//static matrix ret;
	matrix* ret;
	if (saveMatrix == NULL)
	{
		ret = matrixCreate(this, _matrix->row, _matrix->column, NULL, 0);
	}
	else if ((saveMatrix->row != _matrix->row || saveMatrix->column != _matrix->column))
	{
		printf("Save Error! Matrix Size Not Match!");
		ret = matrixCreate(this, _matrix->row, _matrix->column, NULL, 0);
	}
	else {
		ret = saveMatrix;
	}
	for (count = 0; count < _matrix->column; count++)
		ret->data[count][count] = 1;
	float* temp = (float*)malloc(sizeof(float) * _matrix->row);
	if (temp == NULL)
	{
		printf("Malloc Error\r\n");
		return _matrix;
	}
	countRow = 0, countCol = 0;
	while (countRow < _matrix->row)
	{
		if (_matrix->data[countRow][countCol] == 0)
		{
			countRow++;
			if (countRow >= _matrix->row)
				printf("Error: Matrix Singular!\r\n");
			continue;
		}
		if (countRow != countCol)//P矩阵正在做的事，counCol行代表希望该轮高斯消元主元所在行，countRow为实际主元所在行
		{
			memcpy(temp, _matrix->data[countRow], sizeof(float) * _matrix->column);
			memcpy(_matrix->data[countRow], _matrix->data[countCol], sizeof(float) * _matrix->column);
			memcpy(_matrix->data[countCol], temp, sizeof(float) * _matrix->column);
			memcpy(temp, ret->data[countRow], sizeof(float) * _matrix->column);//增广矩阵也作同样的变换
			memcpy(ret->data[countRow], ret->data[countCol], sizeof(float) * _matrix->column);
			memcpy(ret->data[countCol], temp, sizeof(float) * _matrix->column);
			countRow = countCol;
		}
		operation = _matrix->data[countRow][countCol];
		for (_count = _matrix->column - 1; _count >= 0; _count--)//主元变为1
		{
			ret->data[countRow][_count] /= operation;
			_matrix->data[countRow][_count] /= operation;
		}
		for (_count = countRow + 1; _count < _matrix->row; _count++)//L变换，共matrix.col-countCol行要变换
		{
			operation = _matrix->data[_count][countCol];
			for (count = _matrix->column - 1; count >= 0; count--)//共row-1行要相减
			{
				ret->data[_count][count] -= operation * ret->data[countRow][count];
				_matrix->data[_count][count] -= operation * _matrix->data[countRow][count];
			}
		}
		countRow++;
		countCol++;
	}
	if (temp != NULL)
		free(temp);//别忘了释放动态内存
	temp = NULL;
	for (countRow = _matrix->row - 1; countRow > 0; countRow--)//U变换,别理这坨屎山了，我写完看不懂了
	{
		for (_count = countRow - 1; _count >= 0; _count--)
		{
			operation = _matrix->data[_count][countRow];
			for (countCol = _matrix->column - 1; countCol >= 0; countCol--)
			{
				ret->data[_count][countCol] -= ret->data[countRow][countCol] * operation;
			}
		}
	}

	return ret;
}

matrix* matrixCombineProduct(matrixManager* this, matrix* matrixA, matrix* matrixB, matrix* saveMatrix)
{
	matrix* ret;
	matrix* temp;
	matrix* AT = matrixTranspose(this, matrixA, NULL);
	ret = matrixProduct(this, matrixA, matrixB, NULL);
	temp = matrixCopy(this, ret, 0);
	ret = matrixProduct(this, temp, AT, saveMatrix);
	return ret;
}


matrixManager matrixManagerCreate()
{
	matrixGroup oriMatrixGroup = {
		(matrix**)malloc(sizeof(matrix*) * 20),
		0,
		20
	};
	matrixGroup proMatrixGroup = {
		(matrix**)malloc(sizeof(matrix*) * 40),
		0,
		40
	};
	matrixManager ret = {
		oriMatrixGroup,
		proMatrixGroup,
		//matrixCreate,
		//matrixDeleteOne,
		//matrixoriAppend,
		//matrixProAppend,
		//matrixProErase,
		//matrixAllErase,
		//matrixPrintf,
		//matrixAdd,
		//matrixSubstract,
		//matrixDotProduct,
		//matrixNumProduct,
		//matrixProduct,
		//matrixTranspose,
		//matrixInverse
	};
	return ret;
}


