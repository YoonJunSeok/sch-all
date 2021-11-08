#include <iostream>
#include <vector>
using namespace std;
int matrixNSize;
// operation 1
void changeRow(vector<vector<double>>& matrix, int row, int curRow) {
	vector<double> saveRow;
	saveRow = matrix[curRow];
	matrix[curRow] = matrix[row];
	matrix[row] = saveRow;
}
void checkHaveToChangeRow(vector<vector<double>>& matrix, int checkRow) {
	bool correct = false;
	for (int row = 0; row < matrixNSize; row++) {
		int zeroCnt = 0;
		for (int col = 0; col <= row; col++)
			if (matrix[row][col] == 0) zeroCnt++;
		if (zeroCnt == checkRow) {
			correct = true;
			changeRow(matrix, row, checkRow);
		}
		if (correct) break;
	}
}
// operation 2
void rowMultipledByNum(vector<double>& matrix, int multipleNum) {
	for (int col = 0; col < matrixNSize; col++) {
		matrix[col] *= multipleNum;
	}
}
// operation 3
void rowAddedRow(vector<double> &matrix, vector<double> &matrixPlus) {
	for (int col = 0; col < matrixNSize; col++) {
		matrix[col] += matrixPlus[col];
	}
}
// get gcd
int gcd(int a, int b) {
	int c;
	while (b != 0) {
		c = a % b;
		a = b;
		b = c;
	}
	return a;
}
// get lcm
int lcm(int a, int b) {
	return (a * b) / gcd(a, b);
}
// Check upper triangle
bool checkUpperTriangle(vector<vector<double>> &matrix) {
	for (int row = 0; row < matrixNSize; row++) {
		for (int col = 0; col < row; col++) {
			if (matrix[row][col] != 0)
				return false;
		}
	}
	return true;
}
bool checkDifferentSign(int a, int b) {
	if ((a > 0 && b < 0) || (a < 0 && b > 0)) return true;
	else return false;
}
// Print matrix
void printMatrix(vector<vector<double>> &matrix) {
	for (int i = 0; i < matrixNSize; i++) {
		for (int j = 0; j < matrixNSize; j++) {
			cout << fixed;
			cout.precision(10);
			cout << matrix[i][j] << " ";
		}
		cout << "\n";
	}
	cout << "\n";
}

int main() {
	bool upperTriangle = true;
	cin >> matrixNSize;
	vector<vector<double>> matrix(matrixNSize, vector<double>(matrixNSize, 0));
	for (int row = 0; row < matrixNSize; row++) {
		for (int col = 0; col < matrixNSize; col++) {
			double inputNum;  cin >> inputNum;
			matrix[row][col] = inputNum;
		}
	}
	for (int row = 1; row < matrixNSize; row++) {
		checkHaveToChangeRow(matrix, row);
		for (int col = 0; col < row; col++) {
			vector<double> save = matrix[col];
			int LCM = lcm(abs(matrix[row][col]), abs(matrix[col][col]));
			int GCD = gcd(abs(matrix[row][col]), abs(matrix[col][col]));
			rowMultipledByNum(matrix[row], LCM / matrix[row][col]);
			if (!checkDifferentSign(matrix[row][col], matrix[col][col]))
				LCM *= -1;
			rowMultipledByNum(matrix[col], LCM / matrix[col][col]);
			rowAddedRow(matrix[row], matrix[col]);
			matrix[col] = save;
		}
		printMatrix(matrix);
		if (checkUpperTriangle(matrix)) {
			upperTriangle = true;
			break;
		}
		else upperTriangle = false;
	}
	if (upperTriangle) printMatrix(matrix);
	else cout << "Can not chage upper triangle form" << "\n";
}
