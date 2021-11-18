#include<iostream>
#include<vector>
using namespace std;
int matrixNSize;
bool isOverflow = false;
// check overflow
bool checkOverflow(long long num1, long long num2) {
	long long result = num1 * num2;
	if ((result / num1) != num2) {
		isOverflow = true;
		return true;
	}
	else return false;
}
void operationOne(vector<vector<long long>> &matrix, int row1, int row2) {
	vector<long long> tmpMatrix = matrix[row1];
	matrix[row1] = matrix[row2];
	matrix[row2] = tmpMatrix;
}
void operationTwo(vector<long long>& matrix, long long num) {
	for (int col = 0; col < matrixNSize; col++) {
		if (matrix[col] != 0) {
			if (!checkOverflow(abs(matrix[col]), abs(num))) {
				matrix[col] *= num;
			}
		}
	}
}
void operationThree(vector<vector<long long>>& matrix, int row1, int row2) {
	for (int col = 0; col < matrixNSize; col++)
		matrix[row1][col] += matrix[row2][col];
}
// gcd & lcm
long long gcd(long long a, long long b) {
	long long c;
	while (b != 0) {
		c = a % b; a = b; b = c;
	}
	return a;
}
long long lcm(long long a, long long b) {
	if (!checkOverflow(a, b))
		return (a * b) / gcd(a, b);
	else return 1;
}
// 해당 행 전체에 해당하는 GCD를 구해 나눠준다.
void setNumMatrixDivideByGcd(vector<long long>& matrix, int nonZeroCol) {
	int GCD = matrix[nonZeroCol];
	for (int col = nonZeroCol+1; col < matrixNSize; col++)
		GCD = gcd(GCD, abs(matrix[col]));
	for (int col = nonZeroCol; col < matrixNSize; col++)
		matrix[col] /= GCD;
}
// upper triangle을 만족하는 형태로 만들어준다.
void setUpperTriangle(vector<vector<long long>>& matrix) {
	vector<int> alreadyRow(matrixNSize+1, 0);
	for (int row = 0; row < matrixNSize; row++) {
		int nonZeroCol = -1;
		int zeroCnt = 0;
		for (int col = 0; col < matrixNSize; col++) {
			if (matrix[row][col] == 0) zeroCnt++;
			else {
				if (row != 0) {
					nonZeroCol = col;
					break;
				}
			}
		}
		if (zeroCnt >= matrixNSize) zeroCnt = matrixNSize - 1;
		if (alreadyRow.at(zeroCnt) == 0) {
			operationOne(matrix, row, zeroCnt);
			alreadyRow.at(zeroCnt) = 1;
		}
		// 식을 간략화 하기 위해 GCD로 나눠준다.
		if (nonZeroCol != -1)
			setNumMatrixDivideByGcd(matrix[row], nonZeroCol);
	}
}

// Check upper triangle
bool checkUpperTriangle(vector<vector<long long>>& matrix) {
	for (int row = 0; row < matrixNSize; row++) {
		for (int col = 0; col < row; col++) {
			if (matrix[row][col] != 0)
				return false;
		}
	}
	return true;
}

// check sign
bool checkDifferentSign(long long a, long long b) {
	if ((a > 0 && b < 0) || (a < 0 && b > 0)) return true;
	else return false;
}
// print matrix
void printMatrix(vector<vector<long long>>& matrix) {
	cout << "\n";
	for (int i = 0; i < matrixNSize; i++) {
		for (int j = 0; j < matrixNSize; j++) {
			cout << matrix[i][j] << " ";
		}
		cout << "\n";
	}
	cout << "\n";
}
int main() {
	cin >> matrixNSize;
	vector<vector<long long>> matrix(matrixNSize, vector<long long>(matrixNSize, 0));
	for (int row = 0; row < matrixNSize; row++) {
		for (int col = 0; col < matrixNSize; col++) {
			long long inputNum;  cin >> inputNum;
			matrix[row][col] = inputNum;
		}
	}
	// matrix를 변형하면 호출.
	// 새로 입력을 받았으니 호출.
	setUpperTriangle(matrix);
	// matrix 모양에 따라 operation 2,3 수행
	// row 0인 경우에는 어느 모양이 나오든 상관없기에 스킵.
	for (int row = 1; row < matrixNSize; row++) {
		// upper triangle 모양이 나오게끔 해당하는 부분만 0으로 만든다.
		for (int col = 0; col < row; col++) {
			// 해당하는 부분이 이미 0인 경우 계산 스킵.
			if (matrix[row][col] != 0) {
				// 계산을 수행하면서 기존의 행의 값이 변경되면 안되므로 백업해준다.
				vector<long long> backupRow = matrix[col];
				// 0으로 만들기 위해 최소공배수 or 1로 만들어줘야한다.
				// 1로 만들기 위해서는 실수형태로 존재해야되는데 실수계산은 오차가 있을 수 있으니 최소공배수로 계산한다.
				// 최소공배수로 실행하면 값이 범위를 벗어날 수 있으므로 입력에 제한을 둔다.
				long long LCM = lcm(abs(matrix[col][col]), abs(matrix[row][col]));
				// 각 원소가 최소공배수가 되기 위해 곱해야되는 수.
				// 변경하는 row에 곱하는 값.
				long long rowMultiple = LCM / abs(matrix[row][col]);
				// 비교로 사용되는 row에 곱하는 값.
				long long colMultiple = LCM / abs(matrix[col][col]);
				// 두 원소의 합이 0이 되게하기 위해 부호를 비교해 알맞게 조정해준다.
				// 부호가 다르면 그대로 진행
				if (checkDifferentSign(matrix[col][col], matrix[row][col])) {
					operationTwo(matrix[row], rowMultiple);
					operationTwo(matrix[col], colMultiple);
					operationThree(matrix, row, col);
				}
				// 부호가 다르면 -1을 비교하는 행에 곱한다.
				else {
					operationTwo(matrix[col], -1);
					operationTwo(matrix[row], rowMultiple);
					operationTwo(matrix[col], colMultiple);
					operationThree(matrix, row, col);
				}
				// 원래 행으로 돌려놓는다.
				matrix[col] = backupRow;
				// 변경했으므로 upper triangle 형태로 만든다.
				setUpperTriangle(matrix);
			}
		}
		if (isOverflow) break;
	}
	if (isOverflow) 
		cout << "\n사용중인 자료형의 범위를 넘어간 계산입니다." << "\n";
	else {
		if (checkUpperTriangle(matrix)) {
			printMatrix(matrix);
		}
		else cout << "Fail" << "\n";
	}
}
