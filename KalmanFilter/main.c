#include <stdio.h>
#include <malloc.h>
#include "matrix.h"
#include "KalmanFilter.h"
int main()
{
	matrixManager matMan = matrixManagerCreate();
	float pDataA[9] = { 1.5,3.6,3.6,6.7 ,4,5,7,8,9,};
	float pDataB[9] = { 1, 4.4, 3, 4.4, 2, 2, 3, 2, 7.4 };
	float pDataC[9] = { 1, 0, 0, 0 ,3, 0, 0, 0, 5.3};
	float pDataD[9] = { 0.2, 0.5, 0, 0.5, 1, 0, 0, 0, 3 };
	float pDataE[9] = { 0.4,5,2,0.5,2,4.4,4,2,6.7 };
	float pDataF[3] = { 0.8,0.5 ,6 };
	KalmanFilter* kalman_filter = KalmanFilterCreate(pDataA, pDataB, pDataC, pDataD, pDataE, pDataF, 3);
	matrix* A = matrixCreate(kalman_filter->_matrixManager, 3, 1, NULL, 1);
	KalmanFilterProcced(kalman_filter, pDataE, 3);
	A=KalmanFilterProcced(kalman_filter, pDataF, 3);
	printf("\r\n*********************\r\n");
	printf("filtered data:\r\n");
	for (int i = 0; i < 3; i++)
		printf("%f\r\n", A->data[i][0]);
	printf("*********************\r\n");
	KalmanFilterDestroy(kalman_filter);
	return 0;
}