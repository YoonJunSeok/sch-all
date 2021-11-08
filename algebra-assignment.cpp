// Make the square matrix into an upper triangle form
// input n*n matrix
// using three operation
/*
Matrix that succeded in changing to upper triangle.
size : 3
1 -1 1 4 2 1 9 3 1
*/
#include<iostream>
#include<vector>
using namespace std;
int errorRowInCheck, errorColInCheck;
vector<long long> saveRow;
// Interchanging any two equations.
int gcd(int a, int b) {
	int c;
	while (b != 0) {
		c = a % b;
		a = b;
		b = c;
	}
	return a;
}
int lcm(int a, int b) {
	return a * b / gcd(a, b);
}
void operationOne(vector<vector<long long>> &matrix, int matrixNSize, int rowOne, int rowTwo) {
	for (int col = 0; col < matrixNSize; col++) {
		double tmp = matrix[rowOne][col];
		matrix[rowOne][col] = matrix[rowTwo][col];
		matrix[rowTwo][col] = tmp;
	}
};
// Multiplying any equation by a nonzero constant.
void operationTwo(vector<long long> &matrix, int matrixNSize, int multipleNum) {
	for (int col = 0; col < matrixNSize; col++) {
		matrix[col] *= multipleNum;
	}
}
// Adding a multiple of one equation to another.
void operationThree(vector<long long> &matrix, vector<long long> &matrixPlus, int matrixNSize) {
	for (int col = 0; col < matrixNSize; col++) {
		matrix[col] += matrixPlus[col];
	}
}
// Check upper triangle
bool checkUpperTriangle(vector<vector<long long>> &matrix, int matrixNSize) {
	for (int row = 0; row < matrixNSize; row++) {
		for (int col = 0; col < row; col++) {
			if (matrix[row][col] != 0)
				return false;
		}
	}
	return true;
}
// Print matrix
void printMatrix(vector<vector<long long>> matrix, int matrixSize) {
	for (int i = 0; i < matrixSize; i++) {
		for (int j = 0; j < matrixSize; j++)
			cout << matrix[i][j] << " ";
		cout << "\n";
	}
}
void saveMatrixOriginalRow(vector<long long>& matrix) {
	saveRow = matrix;
}
void setMatrixOriginalRow(vector<long long>& matrix) {
	matrix = saveRow;
}
bool checkDifferentSign(int a, int b) {
	if ((a > 0 && b < 0) || (a < 0 && b > 0)) return true;
	else return false;
}
int main() {
	int matrixNSize;
	int inputNum;
	cin >> matrixNSize;
	vector<vector<long long>> matrix(matrixNSize, vector<long long>(matrixNSize, 0));
	for (int row = 0; row < matrixNSize; row++) {
		for (int col = 0; col < matrixNSize; col++) {
			cin >> inputNum;
			matrix[row][col] = inputNum;
		}
	}
	saveMatrixOriginalRow(matrix[0]);
	for (int row = 1; row < matrixNSize; row++) {
		for (int col = 0; col < row; col++) {
			if (matrix[row][col] != 0) {
				// 비교 row를 변경하면 원래 row로 되돌려줄 row 저장.
				saveMatrixOriginalRow(matrix[col]);
				int LCM = lcm(abs(matrix[row][col]), abs(matrix[col][col]));
				// 비교 row
				bool differentSign = checkDifferentSign(matrix[row][col], matrix[col][col]);
				if (differentSign) {
					operationTwo(matrix[col], matrixNSize, (LCM / (abs(matrix[col][col]))));
				}
				else {
					operationTwo(matrix[col], matrixNSize, (LCM / (abs(matrix[col][col]) * -1)));
				}
				// 변경 row
				operationTwo(matrix[row], matrixNSize, LCM / (abs(matrix[row][col])));
				operationThree(matrix[row], matrix[col], matrixNSize);
				setMatrixOriginalRow(matrix[col]);
			}
		}
	}
	printMatrix(matrix, matrixNSize);
	if(checkUpperTriangle(matrix, matrixNSize)) cout << "Success\n";
	else cout << "Fail\n";
}
