#include <iostream>
#include <vector>
#include <algorithm>
#define MAX_SIZE 10001
using namespace std;
vector<pair<int, int>> adj[MAX_SIZE];
vector<int> vertexNumber;
class PriorityQueue {
public:
	// vertexNumber와 weight 보유.

	pair<int, int>* minHeap;
	int heapSize;

	PriorityQueue() {
		this->minHeap = new pair<int, int>[MAX_SIZE];
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
		// 새로운 간선은 맨 마지막에 입력.
		++heapSize;
		minHeap[heapSize].first = info.first;
		minHeap[heapSize].second = info.second;
		bool sameVertex = false;
		for (int i = 1; i < heapSize; i++) {
			// 새로 들어온 간선과 기존에 있는 간선이 겹치는지 확인.
			if (minHeap[i].first == minHeap[heapSize].first) {
				sameVertex = true;
				// 새로운 정점의 가중치가 기존 정점의 가중치보다 작은 경우. 기존 정점과 교체.
				if (minHeap[i].second > minHeap[heapSize].second) {
					swap(i, heapSize);
					minHeap[heapSize].first = 0;
					minHeap[heapSize].second = 0;
					heapSize--;
					updateUp(i);
				}
				// 새로운 정점의 가중치가 기존 정점의 가중치보다 크거나 같은 경우. 새로운 정점 삭제.
				else {
					minHeap[heapSize].first = 0;
					minHeap[heapSize].second = 0;
					heapSize--;
				}
				break;
			}
		}
		if (!sameVertex)
			updateUp(heapSize);
	}

	void pop() {
		if (heapSize != 0) {
			// 최소와 최대의 자리를 바꾼다.
			swap(1, heapSize);
			// 최소를 가져왔으므로 최소의 자리를 삭제한다.
			minHeap[heapSize].first = 0;
			minHeap[heapSize].second = 0;
			heapSize--;
			updateDown();
		}
	}

	// 루트에 있는 정점의 올바른 자리 탐색.
	// pop 하여 높은 weight의 정점이 루트에 갔을 때 사용.
	void updateDown() {
		int cur = 1;
		int leftChild = cur * 2;
		int rightChild = cur * 2 + 1;
		while (1) {
			// 자식이 2명일 때.
			if (rightChild <= heapSize) {
				// 현재 정점의 가중치가 모든 자식들의 가중치보다 작은 경우. break
				if (minHeap[cur].second < minHeap[leftChild].second && minHeap[cur].second < minHeap[rightChild].second) break;
				// 현재 정점의 가중치가 왼쪽 자식보다 작고 오른쪽 자식보다 큰 경우.
				else if (minHeap[cur].second < minHeap[leftChild].second && minHeap[cur].second > minHeap[rightChild].second) {
					swap(cur, rightChild);
					cur = rightChild;
				}
				// 현재 정점의 가중치가 왼쪽 자식보다 크고 오른쪽 자식보다 작은 경우.
				else if (minHeap[cur].second > minHeap[leftChild].second && minHeap[cur].second < minHeap[rightChild].second) {
					swap(cur, leftChild);
					cur = leftChild;
				}
				// 현재 정점의 가중치가 모든 자식들의 가중치보다 큰 경우.
				else {
					// 왼쪽 자식의 가중치가 오른쪽 자식의 가중치보다 작은 경우.
					if (minHeap[leftChild].second < minHeap[rightChild].second) {
						swap(cur, leftChild);
						cur = leftChild;
					}
					// 왼쪽 자식의 가중치가 오른쪽 자식의 가중치보다 큰 경우.
					else if (minHeap[leftChild].second > minHeap[rightChild].second) {
						swap(cur, rightChild);
						cur = rightChild;
					}
					// 왼쪽 자식의 가중치가 오른쪽 자식의 가중치와 같은 경우.
					else {
						// 왼쪽 자식의 정점이 오른쪽 자식의 정점보다 작은 경우.
						if (minHeap[leftChild].first < minHeap[rightChild].first) {
							swap(cur, leftChild);
							cur = leftChild;
						}
						// 왼쪽 자식의 정점이 오른쪽 자식의 정점보다 큰 경우.
						else if (minHeap[leftChild].first > minHeap[rightChild].first) {
							swap(cur, rightChild);
							cur = rightChild;
						}
						// 자식들의 정점이 같은 경우는 push 할 때 이미 처리했기에 나올 수 없다.
					}
				}
				leftChild = cur * 2;
				rightChild = cur * 2 + 1;
			}
			// 자식이 1명일 때.
			else if (leftChild <= heapSize) {
				// 현재 정점의 가중치가 자식의 가중치보다 작은 경우. break
				if (minHeap[cur].second < minHeap[leftChild].second) break;
				// 현재 정점의 가중치가 자식의 가중치보다 큰 경우
				else if (minHeap[cur].second > minHeap[leftChild].second) {
					swap(cur, leftChild);
					cur = leftChild;
				}
				// 현재 정점의 가중치가 자식의 가중치와 같은 경우. 정점 우선순위 비교.
				else {
					// 현재 정점이 자식의 정점보다 작은 경우. break
					if (minHeap[cur].first < minHeap[leftChild].first) break;
					// 현재 정점이 자식의 정점보다 큰 경우.
					else if (minHeap[cur].first > minHeap[leftChild].first) {
						swap(cur, leftChild);
						cur = leftChild;
					}
					// 현재 정점과 자식의 정점이 같은 경우는 push에서 예외처리했기에 나올 수 없다.
				}
				leftChild = cur * 2;
			}
			// 정점이 없거나 
			else break;
		}
	}

	// 해당 정점부터 루트까지 올바른 자리 탐색.
	// push 하여 낮은 weight이 마지막에 존재할 때 사용.
	void updateUp(int cur) {
		int parent = cur / 2;
		while (parent >= 1) {
			// 일반적인 경우.
			// 현재 weight이 부모 weight보다 작은 경우.
			if (minHeap[cur].second < minHeap[parent].second) {
				swap(cur, parent);
				cur = parent;
				parent = cur / 2;
			}
			// 특별한 경우.
			// 현재 weight과 부모 weight이 같은 경우.
			else if (minHeap[cur].second == minHeap[parent].second) {
				//정점의 우선순위가 현재 정점이 높은 경우.
				if (minHeap[cur].first < minHeap[parent].first) {
					swap(cur, parent);
					cur = parent;
					parent = cur / 2;
				}
				//정점의 우선순위가 현재 정점이 낮거나 같은 경우.
				// 낮은 경우는 교체할 필요가 없고
				// 가중치와 정점이 모두 같은 경우는 push하면서 이미 고려한 경우이므로 나올 수 없다.
				else break;
			}
			// 현재 weight이 부모 weight 보다 큰 경우. break
			else break;
		}
	}

	bool empty() {
		if (heapSize == 0) return true;
		else return false;
	}

	pair<int, int> top() {
		return minHeap[1];
	}
};
void prim(PriorityQueue pq, int startVertex) {
	bool check[MAX_SIZE] = { false, };
	check[startVertex] = true;
	int sumMinWeight = 0;
	for (int i = 0; i < adj[startVertex].size(); i++) {
		pq.push(make_pair(adj[startVertex][i].first, adj[startVertex][i].second));
	}
	vertexNumber.push_back(startVertex);

	while (!pq.empty()) {
		pair<int, int> p = pq.top();
		pq.pop();
		if (!check[p.first]) {
			check[p.first] = true;
			sumMinWeight += p.second;

			for (int j = 0; j < adj[p.first].size(); j++) {
				if (!check[adj[p.first][j].first])
					pq.push(make_pair(adj[p.first][j].first, adj[p.first][j].second));
			}

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
		adj[source].push_back(make_pair(destination, weight));
		adj[destination].push_back(make_pair(source, weight));
	}
	for (int i = 0; i < questionCnt; i++) {
		PriorityQueue pq;
		vertexNumber.clear();
		char cmd; cin >> cmd;
		if (cmd == 'P') {
			int startVertex; cin >> startVertex;
			prim(pq, startVertex);
		}
		delete pq.minHeap;
	}
}
