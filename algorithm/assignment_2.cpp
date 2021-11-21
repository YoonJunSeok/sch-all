#include <iostream>
#include <vector>
#include <algorithm>
#define MAX_SIZE 10001
using namespace std;
vector<pair<int, int>> adj[MAX_SIZE];
// min heap 기반 priority queue.
class PriorityQueue {
public:
	// vertexNumber와 weight로 구성된 edge를 가지고 있는 heap 배열 선언.
	pair<int, int>* minHeap;
	int heapSize;

	PriorityQueue() {
		this->minHeap = new pair<int, int>[MAX_SIZE];
		heapSize = 0;
	}

	~PriorityQueue() {
		delete[] minHeap;
	}

	// heap에 들어있는 정점들의 위치를 서로 바꾸기 위한 함수.
	void swap(int x, int y) {
		int tmpVertex = minHeap[x].first;
		int tmpWeight = minHeap[x].second;
		minHeap[x].first = minHeap[y].first;
		minHeap[x].second = minHeap[y].second;
		minHeap[y].first = tmpVertex;
		minHeap[y].second = tmpWeight;
	}

	// heap에 새로운 간선 정보를 넣기 위한 함수.
	void push(pair<int, int> info) {
		// 새로운 간선 정보는 맨 마지막에 입력.
		++heapSize;
		minHeap[heapSize].first = info.first;
		minHeap[heapSize].second = info.second;
		// 새로운 간선 정보의 목적지 정점이 이미 있는 정점인지 확인.
		bool sameVertex = false;
		for (int i = 1; i < heapSize; i++) {
			// 동일한 정점이 있는 경우.
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
				// 중복된 정점에 대한 처리를 완료했으므로 바로 종료.
				break;
			}
		}
		// 중복된 정점이 없는 경우.
		if (!sameVertex)
			updateUp(heapSize);
	}

	// heap에서 가장 높은 우선순위를 제거한다.
	void pop() {
		if (heapSize != 0) {
			// 가장 높은 우선순위와 낮은 우선순위의 자리를 바꾸고 삭제한다.
			swap(1, heapSize);
			minHeap[heapSize].first = 0;
			minHeap[heapSize].second = 0;
			heapSize--;
			// 가장 낮은 우선순위가 가장 높은 우선순위 자리에 있으므로 올바른 자리를 찾아준다.
			updateDown();
		}
	}

	// 가장 낮은 우선순위가 가장 높은 우선순위 자리에 있을 때 올바른 자리를 찾아주는 함수.
	// pop할 때 사용.
	void updateDown() {
		int cur = 1;
		int leftChild = cur * 2;
		int rightChild = cur * 2 + 1;
		// 자식과 비교하여 올바른 자리를 찾아준다.
		// 올바른 자리를 찾거나 자식이 더이상 없으면 break.
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
				// 현재 정점의 가중치가 왼쪽 자식보다 작고 오른쪽 자식과 같은 경우.
				else if (minHeap[cur].second < minHeap[leftChild].second && minHeap[cur].second == minHeap[rightChild].second) {
					// 현재 정점이 오른쪽 자식의 정점보다 작은 경우. break
					if (minHeap[cur].first < minHeap[rightChild].first) break;
					// 현재 정점이 오른쪽 자식의 정점보다 큰 경우.
					else if (minHeap[cur].first > minHeap[rightChild].first) {
						swap(cur, rightChild);
						cur = rightChild;
					}
					// 정점이 서로 같은 경우는 push에서 이미 예외처리를 해주었기에 생략
				}
				// 현재 정점의 가중치가 왼쪽 자식보다 크고 오른쪽 자식보다 작은 경우.
				else if (minHeap[cur].second > minHeap[leftChild].second && minHeap[cur].second < minHeap[rightChild].second) {
					swap(cur, leftChild);
					cur = leftChild;
				}
				// 현재 정점의 가중치가 왼쪽 자식과 같고 오른쪽 자식보다 작은 경우.
				else if (minHeap[cur].second == minHeap[leftChild].second && minHeap[cur].second < minHeap[rightChild].second) {
					// 현재 정점이 왼쪽 자식의 정점보다 작은 경우. break
					if (minHeap[cur].first < minHeap[leftChild].first) break;
					// 현재 정점이 왼쪽 자식의 정점보다 큰 경우.
					else if (minHeap[cur].first > minHeap[leftChild].first) {
						swap(cur, leftChild);
						cur = leftChild;
					}
					// 정점이 서로 같은 경우는 push에서 이미 예외처리를 해주었기에 생략
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
				// 자리교체로 자식들이 변했으므로 자식을 가리키는 변수도 다시 설정.
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
				// 자리교체로 자식들이 변했으므로 자식을 가리키는 변수도 다시 설정.
				leftChild = cur * 2;
			}
			else break;
		}
	}

	// 해당 정점부터 가장 높은 우선순위의 자리까지 올라가며 올바른 자리 탐색.
	// push할 때 사용.
	void updateUp(int cur) {
		int parent = cur / 2;
		while (parent >= 1) {
			// 현재 weight이 부모 weight보다 작은 경우.
			if (minHeap[cur].second < minHeap[parent].second) {
				swap(cur, parent);
				cur = parent;
				parent = cur / 2;
			}
			// 현재 weight과 부모 weight이 같은 경우.
			else if (minHeap[cur].second == minHeap[parent].second) {
				//정점의 우선순위가 현재 정점이 높은 경우.
				if (minHeap[cur].first < minHeap[parent].first) {
					swap(cur, parent);
					cur = parent;
					parent = cur / 2;
				}
				// 정점의 우선순위가 현재 정점이 낮거나 같은 경우.
				// 낮은 경우는 교체할 필요가 없고
				// 정점이 같은 경우는 push하면서 이미 고려한 경우이므로 나올 수 없다.
				else break;
			}
			// 현재 weight이 부모 weight 보다 큰 경우. break
			else break;
		}
	}

	// heap이 비었는지 확인하는 함수.
	bool empty() {
		if (heapSize == 0) return true;
		else return false;
	}

	// 가장 높은 우선순위의 정보를 내보내는 함수.
	pair<int, int> top() {
		return minHeap[1];
	}
};
void prim(int startVertex) {
	// tree 상태의 정점을 순서대로 저장하기 위한 벡터.
	vector<int> vertexNumber;
	// min heap을 이용해 프림 알고리즘 수행.
	PriorityQueue pq;
	// tree 상태에 들어있지 않은 정점들은 false
	// tree 상태에 들어있으면 true
	bool check[MAX_SIZE] = { false, };
	check[startVertex] = true;
	int sumMinWeight = 0;
	// base case
	// 처음 정점과 연결된 정보들을 인접 리스트에서 가져와 heap에 저장한다.
	for (int i = 0; i < adj[startVertex].size(); i++) {
		pq.push(make_pair(adj[startVertex][i].first, adj[startVertex][i].second));
	}
	// tree 상태의 정점을 저장.
	vertexNumber.push_back(startVertex);

	while (!pq.empty()) {
		// 가장 높은 우선순위, 가장 낮은 가중치를 가진 정보를 가져온다.
		pair<int, int> p = pq.top();
		pq.pop();
		// tree 상태가 아닌 경우에 대해 수행.
		if (!check[p.first]) {
			// fringe 상태를 tree 상태로 변경.
			check[p.first] = true;
			// tree 상태의 정점을 저장.
			vertexNumber.push_back(p.first);
			// 최종 결과물 출력을 위한 가중치 저장.
			sumMinWeight += p.second;

			// tree 상태로 옮겨진 정점과 연결된 간선 정보를 heap에 저장.
			for (int j = 0; j < adj[p.first].size(); j++) {
				// tree 상태인 정점과는 다시 연결할 필요가 없기 때문에 tree 상태가 아닌 경우에 대해 수행.
				if (!check[adj[p.first][j].first])
					pq.push(make_pair(adj[p.first][j].first, adj[p.first][j].second));
			}
		}
	}
	
	// 만들어진 최소 가중치 출력.
	cout << sumMinWeight << " ";
	// 최소 가중치를 만들기 위해 tree 상태로 저장된 정점을 순서대로 출력.
	for (int i = 0; i < vertexNumber.size()-1; i++)
		cout << vertexNumber[i] << " ";
	cout << vertexNumber[vertexNumber.size()-1] << "\n";
}
int main() {
	// 조형물 정보의 수, 간선 정보 수, 질의의 수 입력
	int vertexSize, edgeSize, questionCnt;
	cin >> vertexSize >> edgeSize >> questionCnt;
	// 출발지점에 해당하는 인접 리스트에 목적지와 가중치의 정보를 저장.
	for (int i = 0; i < edgeSize; i++) {
		int source, destination, weight;
		cin >> source >> destination >> weight;
		// 무방향 그래프이므로 서로 연결.
		adj[source].push_back(make_pair(destination, weight));
		adj[destination].push_back(make_pair(source, weight));
	}
	// 질의의 대한 함수 수행.
	for (int i = 0; i < questionCnt; i++) {
		char cmd; cin >> cmd;
		if (cmd == 'P') {
			int startVertex; cin >> startVertex;
			prim(startVertex);
		}
	}
}
