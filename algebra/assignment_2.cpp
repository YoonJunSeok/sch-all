#include <iostream>
#include <vector>
#define R 't';
using namespace std;
typedef vector<vector<double>> matrix;
typedef vector<double> row;
int cnt = 3;
int s[16] = { 1,2,4,7,0,3,5,8,0,0,6,9,0,0,0,10 };
double eigenVector[4][4] = { {1,1,1,1}, {1,1,1,1}, {1,1,1,1}, {1,1,1,1} };
row ramda;
matrix systemA(4, row(4, 0));
matrix AMinusRamdaIdentity(4, row(4,0));
// for echelon
int non_zero(row& r) {
	for (int i = 0; i < r.size(); i++) {
		if (r[i] != 0) return i;
		else continue;
	}
	return -1;
}
void matrixPrint(matrix& mat) {
	cout << "-----------------\n";
	for (int i = 0; i < mat.size(); i++) {
		for (auto& iter : mat[i]) {
			cout << iter << " ";
		}
		cout << "\n";
	}
}
void operOne(matrix& mat, int i, double k) {
	for (auto& iter : mat[i]) {
		iter = round(k * iter * 10) / 10;
	}
}
void operTwo(matrix& mat, int n, double k, int j) {
	for (int i = 0; i < mat[0].size(); ++i) {
		mat[j][i] += round(k * mat[n][i] * 10) / 10;
	}
}
void echelon(matrix& mat) {
	for (int i = 0; i < mat.size() - 1; i++) {
		int j = non_zero(mat[i]);
		if (j == -1) continue;
		else {
			for (int k = 0; k < mat.size() - 1 - i; k++) {
				operTwo(mat, i, (-1) * mat[1 + i + k][j] / mat[i][j], 1 + i + k);
			}
		}
	}
}
void rowEchelon(matrix& mat) {
	echelon(mat);
	for (int i = 0; i < mat.size(); i++) {
		int j = non_zero(mat[i]);
		if (j == -1) continue;
		else {
			operOne(mat, i, 1 / mat[i][j]);
		}
	}
	for (int i = mat.size() - 1; i >= 0; i--) {
		int j = non_zero(mat[i]);
		if (j == -1) continue;
		else {
			for (int k = 0; k < i; k++) {
				operTwo(mat, i, (-1) * mat[i - 1 - k][j], i - 1 - k);
			}
		}
	}
}
// calculator
void setSystem() {
	int cnt = 0;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			systemA[i][j] = s[cnt];
			cnt++;
		}
	}
}
void setRamda() {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (i == j) {
				ramda.push_back(systemA[i][j]);
			}
		}
	}
}
void setAMinusRamdaIdentity(int ramda) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (i == j) {
				int eigenValue = systemA[i][j] - ramda;
				AMinusRamdaIdentity[i][j] = eigenValue;
			}
			else AMinusRamdaIdentity[i][j] = systemA[i][j];
		}
	}
}
void calEigenVector(matrix& mat) {
	// 위치와 그 위치에 해당하는 값.
	vector<pair<int, double>> equation;
	for (int i = 3; i >= 0; i--) {
		for (int j = 0; j < 4; j++) {
			// Xj 값이 있는지 확인.
			if (mat[i][j] != 0) {
				// 행에 해당하는 값 저장.
				equation.push_back(make_pair(j, mat[i][j]));
			}
		}
		// 행의 값이 모두 0이라면 그 행번째 x의 값은 1이다.
		// 실제로는 임의의 실수가 모두 해당될 수 있지만 계산의 편의성을 위한다.
		if (equation.size() == 0) {
			eigenVector[cnt][i] = 1;
		}
		// 행에 0이 아닌 값이 하나라면 그에 해당하는 x값은 0일수밖에 없다.
		else if (equation.size() == 1) {
			eigenVector[cnt][equation[0].first] = 0;
		}
		else if (equation.size() == 2) {
			// reduced row echelon form에서 1인 곳.
			if (equation[0].second == 1) {
				eigenVector[cnt][equation[0].first] = equation[1].second * -1;
				eigenVector[cnt][equation[1].first] = 1;
			}
		}
		equation.clear();
	}
	cnt--;
	if (cnt == -1) {
		for (int i = 3; i >= 0; i--) {
			for (int j = 0; j < 4; j++)
				cout << eigenVector[i][j] << " ";
			cout << "\n";
		}
		cout << "\n";
	}
}
// print
void printAMinus() {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++)
			cout << AMinusRamdaIdentity[i][j] << " ";
		cout << "\n";
	}
	cout << "\n";
}
int main() {
	setSystem();
	// 사용자로부터 하나의 벡터와 알고싶은 지수승을 입력받는다.
	double input[4];
	for (int i = 0; i < 4; i++) {
		double data; cin >> data;
		input[i] = data;
	}
	int k; cin >> k;
	setRamda();
	for (int i = 0; i < ramda.size(); i++) {
		// 1은 임의의 실수값을 의미한다.
		setAMinusRamdaIdentity(ramda[i]);
		rowEchelon(AMinusRamdaIdentity);
		printAMinus();
		calEigenVector(AMinusRamdaIdentity);
	}
}
