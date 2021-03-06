#include <iostream>
#include <string>
#include <cmath>
using namespace std;
enum class Color {
	RED,
	BLACK
};
class App {
public:
	int id;
	string name;
	int size;
	int price;

	App(int id, string name, int size, int price) {
		this->id = id;
		this->name = name;
		this->size = size;
		this->price = price;
	}
};
class Node {
public:
	App* app;
	Node* leftNode;
	Node* rightNode;
	Node* parentNode;
	Color color;

	Node() {
		this->app = NULL;
		this->leftNode = NULL;
		this->rightNode = NULL;
		this->parentNode = NULL;
		this->color = Color::BLACK;
	}

	Node(App* newApp) {
		app = newApp;
		this->leftNode = new Node();
		this->rightNode = new Node();
		this->color = Color::RED;
	}
};
class RedblackTree {
private:
	Node* root;
	
	// Restructuring
	Node* getSubtreeRootAfterRestructuring(Node* grandParent, Node* parent, Node* child) {
		if (grandParent->leftNode == parent)
			// Rotation for the following format.
			//		grandParent 
			//			/
			//		parent
			return getSubtreeRootAfterRightRotation(grandParent, parent, child);
		else
			// Rotation for the following format.
			// grandParent 
			//		＼
			//		parent
			return getSubtreeRootAfterLeftRotation(grandParent, parent, child);
	}

	Node* getSubtreeRootAfterLeftRotation(Node* grandParent, Node* parent, Node* child) {
		if (parent->rightNode == child)
			// Rotation for the following format.
			// grandParent 
			//		＼
			//		parent
			//			＼
			//			child
			return getSubtreeRootAfterGRR(grandParent, parent, child);
		else
			// Rotation for the following format.
			// grandParent 
			//		＼
			//		parent
			//		 /
			//	  child
			return getSubtreeRootAfterGRL(grandParent, parent, child);
	}
	Node* getSubtreeRootAfterRightRotation(Node* grandParent, Node* parent, Node* child) {
		if (parent->leftNode == child)
			// Rotation for the following format.
			//		grandParent 
			//			/
			//		parent
			//		 /
			//	  child
			return getSubtreeRootAfterGLL(grandParent, parent, child);
		else
			// Rotation for the following format.
			//		grandParent 
			//			/
			//		parent
			//			＼
			//			child
			return getSubtreeRootAfterGLR(grandParent, parent, child);
	}

	Node* getSubtreeRootAfterGRR(Node* grandParent, Node* parent, Node* child) {
		Node* subtreeRootNode = parent;
		Node* saveLeft = parent->leftNode;
		// set subtree root
		if (!isRoot(grandParent)) {
			// subtree의 부모는 기존 tree.
			subtreeRootNode->parentNode = grandParent->parentNode;
			// 기존 tree의 자식으로 subtree 설정.
			if (grandParent->parentNode->leftNode == grandParent) grandParent->parentNode->leftNode = subtreeRootNode;
			else if (grandParent->parentNode->rightNode == grandParent) grandParent->parentNode->rightNode = subtreeRootNode;
		}
		else {
			// subtree를 root로 설정.
			subtreeRootNode->parentNode = NULL;
			root = subtreeRootNode;
		}
		// set subtree left child.
		subtreeRootNode->leftNode = grandParent;
		grandParent->parentNode = subtreeRootNode;
		// set subtree left child's child
		saveLeft->parentNode = grandParent;
		grandParent->rightNode = saveLeft;
		// restructuring 후에 노드들의 색 변환.
		setSubtreeNodeColor(subtreeRootNode);
		return subtreeRootNode;
	}

	Node* getSubtreeRootAfterGRL(Node* grandParent, Node* parent, Node* child) {
		Node* subtreeRootNode = child;
		Node* saveLeft = child->leftNode;
		Node* saveRight = child->rightNode;
		if (!isRoot(grandParent)) {
			// subtree의 부모는 기존 tree.
			subtreeRootNode->parentNode = grandParent->parentNode;
			// 기존 tree의 자식으로 subtree 설정.
			if (grandParent->parentNode->leftNode == grandParent) grandParent->parentNode->leftNode = subtreeRootNode;
			else if (grandParent->parentNode->rightNode == grandParent) grandParent->parentNode->rightNode = subtreeRootNode;
		}
		else {
			// subtree를 root로 설정.
			subtreeRootNode->parentNode = NULL;
			root = subtreeRootNode;
		}
		// set subtree left, right child.
		subtreeRootNode->leftNode = grandParent;
		subtreeRootNode->rightNode = parent;
		// set subtree left child.
		grandParent->parentNode = subtreeRootNode;
		// set subtree left child's child.
		saveLeft->parentNode = grandParent;
		grandParent->rightNode = saveLeft;
		// set subtree right child.
		parent->parentNode = subtreeRootNode;
		// set subtree right child's child.
		saveRight->parentNode = parent;
		parent->leftNode = saveRight;
		// restructuring 후에 노드들의 색 변환.
		setSubtreeNodeColor(subtreeRootNode);
		return subtreeRootNode;
	}

	Node* getSubtreeRootAfterGLL(Node* grandParent, Node* parent, Node* child) {
		Node* subtreeRootNode = parent;
		Node* saveRight = parent->rightNode;
		if (!isRoot(grandParent)) {
			// subtree의 부모는 기존 tree.
			subtreeRootNode->parentNode = grandParent->parentNode;
			// 기존 tree의 자식으로 subtree 설정.
			if (grandParent->parentNode->leftNode == grandParent) grandParent->parentNode->leftNode = subtreeRootNode;
			else if (grandParent->parentNode->rightNode == grandParent) grandParent->parentNode->rightNode = subtreeRootNode;
		}
		else {
			// subtree를 root로 설정.
			subtreeRootNode->parentNode = NULL;
			root = subtreeRootNode;
		}
		// set subtree right child.
		subtreeRootNode->rightNode = grandParent;
		grandParent->parentNode = subtreeRootNode;
		// set subtree right child's child
		saveRight->parentNode = grandParent;
		grandParent->leftNode = saveRight;
		// restructuring 후에 노드들의 색 변환.
		setSubtreeNodeColor(subtreeRootNode);
		return subtreeRootNode;
	}

	Node* getSubtreeRootAfterGLR(Node* grandParent, Node* parent, Node* child) {
		Node* subtreeRootNode = child;
		Node* saveLeft = child->leftNode;
		Node* saveRight = child->rightNode;
		if (!isRoot(grandParent)) {
			subtreeRootNode->parentNode = grandParent->parentNode;
			// tree의 자식으로 subtree 설정.
			if (grandParent->parentNode->leftNode == grandParent) grandParent->parentNode->leftNode = subtreeRootNode;
			else if (grandParent->parentNode->rightNode == grandParent) grandParent->parentNode->rightNode = subtreeRootNode;
		}
		else {
			// subtree를 root로 설정.
			subtreeRootNode->parentNode = NULL;
			root = subtreeRootNode;
		}
		// set subtree left, right child.
		subtreeRootNode->leftNode = parent;
		subtreeRootNode->rightNode = grandParent;
		// set subtree left child.
		parent->parentNode = subtreeRootNode;
		// set subtree left child's child.
		saveLeft->parentNode = parent;
		parent->rightNode = saveLeft;
		// set subtree right child.
		grandParent->parentNode = subtreeRootNode;
		// set subtree right child's child.
		saveRight->parentNode = grandParent;
		grandParent->leftNode = saveRight;
		// restructuring 후에 노드들의 색 변환.
		setSubtreeNodeColor(subtreeRootNode);
		return subtreeRootNode;
	}

	// Recoloring.
	Node* getSubtreeRootAfterRecoloring(Node* grandParent, Node* parent, Node* sibling) {
		if (!isRoot(grandParent)) grandParent->color = Color::RED;
		else grandParent->color = Color::BLACK;
		parent->color = Color::BLACK;
		sibling->color = Color::BLACK;
		return grandParent;
	}

	// restructuring 한 subtree 노드의 색을 변경.
	void setSubtreeNodeColor(Node* subtreeRoot) {
		subtreeRoot->color = Color::BLACK;
		subtreeRoot->leftNode->color = Color::RED;
		subtreeRoot->rightNode->color = Color::RED;
	}
	// 중위순회를 수행하면서 범위에 해당하는 app들의 가격을 할인.
	void discountApp(Node* curNode, int startId, int endId, double p) {
		if (curNode != NULL && curNode->app != NULL) {
			discountApp(curNode->leftNode, startId, endId, p);
			if (curNode->app->id >= startId && curNode->app->id <= endId) {
				double originalPrice = (double)(curNode->app->price);
				curNode->app->price = (int)floor(originalPrice * ((((double)100 - p) / (double)100)));
			}
			discountApp(curNode->rightNode, startId, endId, p);
		}
	}
	// 해당 노드의 깊이를 리턴.
	int getNodeDepth(Node* node) {
		int depth = 0;
		// 해당 노드부터 위로 올라갈때마다 +1.
		while (node != NULL && node != root) {
			depth++;
			node = node->parentNode;
		}
		return depth;
	}
	// 해당 노드가 루트인지 판별.
	bool isRoot(Node* node) {
		if (getNodeDepth(node) == 0) return true;
		else return false;
	}
	// 부모와 자식 노드의 색이 모두 RED인지 판별.
	bool isDoubleRed(Node* parentNode, Node* newNode) {
		if (parentNode->color == Color::RED && newNode->color == Color::RED) return true;
		else return false;
	}
	// sibling 노드의 색이 RED인지 판별.
	bool isSiblingRed(Node* siblingNode) {
		if (siblingNode->color == Color::RED) return true;
		else if (siblingNode->color == Color::BLACK) return false;
		return false;
	}
	// 부모의 형제 노드를 리턴. 없으면 NULL 리턴.
	Node* getSibling(Node* parentNode) {
		Node* sibling = NULL;
		// grandparent의 왼쪽 자식이 부모노드일 경우. 형제는 오른쪽 자식.
		if (parentNode->parentNode != NULL && parentNode->parentNode->leftNode == parentNode) {
			sibling = parentNode->parentNode->rightNode;
			sibling->parentNode = parentNode->parentNode;
			parentNode->parentNode->rightNode = sibling;
		}
		// grandparent의 오른쪽 자식이 부모노드일 경우. 형제는 왼쪽 자식.
		else if (parentNode->parentNode != NULL && parentNode->parentNode->rightNode == parentNode) {
			sibling = parentNode->parentNode->leftNode;
			sibling->parentNode = parentNode->parentNode;
			parentNode->parentNode->leftNode = sibling;
		}
		return sibling;
	}
	// 해당 id와 동일한 id를 가진 노드를 리턴. 없으면 NULL 리턴.
	Node* getNodeUsing(int id) {
		Node* curNode = root;
		Node* getNode = NULL;
		if (curNode != NULL) {
			while (curNode->app != NULL) { 
				getNode = curNode;
				if (curNode->app->id == id) return getNode;
				else if (curNode->app->id > id)
					curNode = curNode->leftNode;
				else curNode = curNode->rightNode;
			}
		}
		getNode = NULL;
		return getNode;
	}
public:
	RedblackTree() {
		this->root = NULL;
	}

	// App의 내용을 가진 node를 redblacktree에 삽입.
	void insertAppInNode(App* app) {
		Node* newNode = new Node(app);
		Node* saveNewNode = newNode;
		bool isDuplicate = false;
		if (root == NULL) {
			newNode->parentNode = NULL;
			root = newNode;
			root->color = Color::BLACK;
			cout << getNodeDepth(newNode) << "\n";
		}
		else {
			Node* curNode = root;
			// newNode를 집어넣을 parentNode
			Node* toParentNode = curNode;
			// newNode를 집어넣을 parentNode를 찾는다.
			while (curNode != NULL && curNode->app != NULL) {
				toParentNode = curNode;
				if (curNode->app->id == newNode->app->id) {
					cout << getNodeDepth(curNode) << "\n";
					isDuplicate = true;
					break;
				}
				else if (newNode->app->id < curNode->app->id)
					curNode = curNode->leftNode;
				else {
					curNode = curNode->rightNode;
				}
			}
			// 중복된 id가 아닌 경우.
			if (!isDuplicate) {
				// save new id smaller than parent Id 
				if (toParentNode->app->id > newNode->app->id) {
					newNode->parentNode = toParentNode;
					toParentNode->leftNode = newNode;
				}
				// save new id larger than parent id
				else {
					newNode->parentNode = toParentNode;
					toParentNode->rightNode = newNode;
				}
				// check double red
				while (isDoubleRed(toParentNode, newNode)) {
					// get sibling node
					Node* siblingNode = getSibling(toParentNode);
					Node* subtreeRoot = nullptr;
					// sibling 노드의 색에 따라 다른 연산 수행.
					if (isSiblingRed(siblingNode))
						subtreeRoot = getSubtreeRootAfterRecoloring(toParentNode->parentNode, toParentNode, siblingNode);
					else
						subtreeRoot = getSubtreeRootAfterRestructuring(toParentNode->parentNode, toParentNode, newNode);
					// double red 처리 후에도 double red가 발생했는지 알기 위해 비교 노드를 바꿔준다.
					if (getNodeDepth(subtreeRoot) >= 2) {
						// newNode = subtreeRoot
						newNode = subtreeRoot;
						// toParentNode = subtreeRoot parent
						toParentNode = subtreeRoot->parentNode;
					}
				}
				cout << getNodeDepth(saveNewNode) << "\n";
			}
		}
	}
	// 해당 id를 가진 노드의 깊이와 App 정보를 출력.
	void printDepthAndAppInformationUsing(int id) {
		Node* node = getNodeUsing(id);
		if (node == NULL) cout << "NULL\n";
		else if (node->app != NULL)
			cout << getNodeDepth(node) << " " << node->app->name << " " << node->app->size << " " << node->app->price << "\n";
	}
	// 해당 id를 가진 노드의 App 정보를 변경.
	void updateAppInformation(App* app) {
		Node* node = getNodeUsing(app->id);
		if (node == NULL) cout << "NULL\n";
		else if (node->app != NULL) {
			node->app->name = app->name;
			node->app->size = app->size;
			node->app->price = app->price;
			cout << getNodeDepth(node) << "\n";
		}
	}
	// 범위 안에 있는 App의 가격을 할인.
	void discountAppBetweenByPercent(int startId, int endId, double p) {
		if (root != NULL)
			discountApp(root, startId, endId, p);
	}
};
int main() {
	int tc; cin >> tc;
	RedblackTree rbt;
	while (tc--) {
		string cmd; cin >> cmd;
		// 애플리케이션 등록
		if (cmd == "I") {
			int id, size, price; string name;
			cin >> id >> name >> size >> price;
			rbt.insertAppInNode(new App(id, name, size, price));
		}
		// 애플리케이션 검색
		else if (cmd == "F") {
			int id; cin >> id;
			rbt.printDepthAndAppInformationUsing(id);
		}
		// 애플리케이션 업데이트
		else if (cmd == "R") {
			int id, size, price; string name;
			cin >> id >> name >> size >> price;
			rbt.updateAppInformation(new App(id, name, size, price));
		}
		// 애플리케이션 할인
		else if (cmd == "D") {
			int x, y; double P; cin >> x >> y >> P;
			rbt.discountAppBetweenByPercent(x, y, P);
		}
	}
}
