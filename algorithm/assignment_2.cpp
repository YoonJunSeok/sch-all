#include <iostream>
#include <vector>
#include <algorithm>
#define MAX_SIZE 10000
using namespace std;
// 인접 연결 리스트.
// vertex[vertexNumber]은 연결되어있는 간선과 그 간선의 가중치를 pair로 가지고 있다.
vector<pair<int, int>> vertex[MAX_SIZE];
// vector number 저장
vector<int> vertexNumber;
class PriorityQueue {
public: 
	// vertexNumber와 weight 보유.
	pair<int, int> minHeap[MAX_SIZE];
	int heapSize;

	PriorityQueue() {
		pair<int, int> p = { 0,0 };
		fill(minHeap, minHeap + MAX_SIZE, p);
		heapSize = 0;
	}

	void swap(int x, int y) {
		int tmpVertex = minHeap[x].first;
		int tmpWeight = minHeap[x].second;
		minHeap[x].first = minHeap[y].first;
		minHeap[x].second = minHeap[y].second;
		minHeap[y].first = tmpVertex;
		minHeap[y].second = tmpWeight;
	}

	void push(pair<int, int> info) {
		// store vertex number
		++heapSize;
		minHeap[heapSize].first = info.first;
		// stroe edge weight
		minHeap[heapSize].second = info.second;
		for (int i = 1; i < heapSize; i++) {
			// same vertex, different weight
			if (minHeap[i].first == minHeap[heapSize].first) {
				if (minHeap[i].second > minHeap[heapSize].second) {
					swap(i, heapSize);
					minHeap[heapSize].first = 0;
					minHeap[heapSize].second = 0;
					heapSize--;
				}
				else {
					minHeap[heapSize].first = 0;
					minHeap[heapSize].second = 0;
					heapSize--;
				}
			}
		}
		for (int i = heapSize; i > 1; i /= 2) {
			if (minHeap[i / 2].second > minHeap[i].second)
				swap(i / 2, i);
			else if (minHeap[i/2].second < minHeap[i].second)
				break;
			else if (minHeap[i/2].second == minHeap[i].second) {
				// same weight, small vertex over big vertex
				if (minHeap[i/2].first > minHeap[i].first) {
					swap(i/2, i);
				}
			}
		}
		update();
	}

	void pop() {
		if (heapSize != 0) {
			// 최소와 최대의 자리를 바꾼다.
			swap(1, heapSize);
			// 최소를 가져왔으므로 최소의 자리를 삭제한다.
			minHeap[heapSize].first = 0;
			minHeap[heapSize].second = 0;
			heapSize--;
			update();
		}
	}

	void update() {
		// 최대의 자리를 min heap에 맞게 변경한다.
		for (int i = 1; i * 2 <= heapSize; i++) {
			if (minHeap[i * 2 + 1].first != 0) {
				if (minHeap[i].second < minHeap[i * 2].second && minHeap[i].second < minHeap[i * 2 + 1].second) break;
				else if (minHeap[i * 2].second < minHeap[i * 2 + 1].second) {
					swap(i, i * 2);
					i = i * 2;
				}
				else if (minHeap[i * 2].second > minHeap[i * 2 + 1].second) {
					swap(i, i * 2 + 1);
					i = i * 2 + 1;
				}
				else if (minHeap[i * 2].second == minHeap[i * 2 + 1].second) {
					if (minHeap[i * 2].first > minHeap[i * 2 + 1].first)
						swap(i, i * 2 + 1);
					else swap(i, i * 2);
				}
			}
		}
	}

	bool empty() {
		if (heapSize == 0) return true;
		else return false;
	}

	pair<int,int> top() {
		return minHeap[1];
	}
};
void prim(PriorityQueue pq, int startVertex) {
	bool check[MAX_SIZE] = { false, };
	// start vertex is tree
	// if cehck[vertexNumber] = false then fringe.
	check[startVertex] = true;
	int sumMinWeight = 0;
	// base case
	for (int i = 0; i < vertex[startVertex].size(); i++) {
		pq.push(make_pair(vertex[startVertex][i].first, vertex[startVertex][i].second));
		check[vertex[startVertex][i].first] = false;
	}
	vertexNumber.push_back(startVertex);

	while (!pq.empty()) {
		pair<int, int> p = pq.top();
		pq.pop();
		if (!check[p.first]) {
			check[p.first] = true;
			sumMinWeight += p.second;

			for (int j = 0; j < vertex[p.first].size(); j++) {
				if (!check[vertex[p.first][j].first])
					pq.push(make_pair(vertex[p.first][j].first, vertex[p.first][j].second));
			}

			// 최소의 weight 값과 vertex number 값을 가져온다.
			vertexNumber.push_back(p.first);
		}
	}
	cout << sumMinWeight << " ";
	for (int i = 0; i < vertexNumber.size(); i++)
		cout << vertexNumber[i] << " ";
	cout << "\n";
}
int main() {
	int vertexSize, edgeSize, questionCnt;
	cin >> vertexSize >> edgeSize >> questionCnt;
	// store vertex, edge information in adjacency list 
	for (int i = 0; i < edgeSize; i++) {
		int source, destination, weight;
		cin >> source >> destination >> weight;
		// source에 해당하는 vertex는 weight을 가지고 있는 간선을 가지고 destinationo과 연결되어 있다.
		vertex[source].push_back(make_pair(destination, weight));
		vertex[destination].push_back(make_pair(source, weight));
	}
	for (int i = 0; i < questionCnt; i++) {
		PriorityQueue pq;
		vertexNumber.clear();
		char cmd; cin >> cmd;
		if (cmd == 'P') {
			int startVertex; cin >> startVertex;
			prim(pq, startVertex);
		}
	}
}
