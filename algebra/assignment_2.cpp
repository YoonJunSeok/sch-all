#include <iostream>
#include <vector>
#define matrix vector<vector<double>>
#define row vector<double>
int systemA[16] = { 1,2,4,7,0,3,5,8,0,0,6,9,0,0,0,10 };
row ramda;
matrix AMinusRamdaIdentity[4];
matrix eigenVector[4];
using namespace std;
// for echelon
int non_zero(row& r) {
	for (int i = 0; i < r.size(); i++) {
		if (r[i] != 0) return i;
		else continue;
	}
	return -1;
}
void print(matrix& mat) {
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
		iter = k * iter;
	}
}
void operTwo(matrix& mat, int n, double k, int j) {
	for (int i = 0; i < mat[0].size(); ++i) {
		mat[j][i] += k * mat[n][i];
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
int main() {
	matrix input;
	int k;
	for (int i = 0; i < 4; i++) {
		row r;
		for (int j = 0; j < 4; j++) {
			double tmp;
			cin >> tmp;
			r.push_back(tmp);
		}
		input.push_back(r);
	}
	cin >> k;
	rowEchelon(input);
}
