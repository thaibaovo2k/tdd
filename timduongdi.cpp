#include <stdio.h>
#include <string>
#include <fstream>
#include "Main.h"
using namespace std;

#define MAX 1e9

bool getInput(string fileName, int& numVetex, int& beginVertex, int& endVertex, int C[][100]) {
	fstream fs;
	fs.open(fileName, ios::in);

	if (fs.fail()) {
		printf("Khong the mo file %s!", fileName.c_str());
		return false;
	}

	string firstRow;
	getline(fs, firstRow);

	int posSpace = firstRow.find(' ');
	string numVertexData = firstRow.substr(0, posSpace);
	sscanf_s(numVertexData.c_str(), "%d", &numVetex);
	
	firstRow = firstRow.substr(posSpace + 1);
	posSpace = firstRow.find(' ');
	string beginVertexData = firstRow.substr(0, posSpace);
	sscanf_s(beginVertexData.c_str(), "%d", &beginVertex);

	firstRow = firstRow.substr(posSpace + 1);
	string endVertexData = firstRow.substr(0);
	sscanf_s(endVertexData.c_str(), "%d", &endVertex);
	
	for (int row = 0; row < numVetex; ++row) {
		string dataRow;
		getline(fs, dataRow);
		
		for (int col = 0; col < numVetex; ++col) {
			int posSpace = dataRow.find(' ');
			string vertexData = dataRow.substr(0, posSpace);
			sscanf_s(vertexData.c_str(), "%d", &C[row][col]);
			C[row][col] = C[row][col] != 0 ? C[row][col] : MAX;
			dataRow = dataRow.substr(posSpace + 1);
		}
	}

	fs.close();

	return true;
}

void printMatrix(const int C[][100], const int &numVertex) {
	for (int i = 0; i < numVertex; ++i) {
		for (int j = 0; j < numVertex; ++j) {
			if (C[i][j] != MAX)
				printf("%d\t", C[i][j]);
			else if (i == j)
				printf("0\t");
			else
				printf("Inf\t");
		}
		printf("\n");
	}
}

void changeVertexWeight(int C[][100]) {
	printf("Nhap lan luot 2 dinh (u, v) can thay doi trong so: ");
	int u, v;
	scanf_s("%d %d", &u, &v);

	printf("Nhap gia tri trong so can thay doi: ");
	int newWeight;
	scanf_s("%d", &newWeight);

	C[u][v] = newWeight;
	C[v][u] = newWeight;

	printf("Da thay doi trong so cua 2 dinh (%d, %d) thanh %d\n", u + 1, v + 1, newWeight);
}

bool save(string fileName, int& numVertex, int& beginVertex, int& endVertex, int C[][100]) {
	fstream fs;
	fs.open(fileName, ios::out);

	if (fs.fail()) {
		printf("Khong the luu du lieu vao file %s!", fileName.c_str());
		return false;
	}

	fs << numVertex << " " << beginVertex << " " << endVertex << "\n";

	for (int i = 0; i < numVertex; ++i) {
		for (int j = 0; j < numVertex; ++j) {
			fs << C[i][j] << " ";
		}
		fs << "\n";
	}

	fs.close();

	return true;
}

void findWayByDijkstra1_1(const int& numVetex, const int& beginVertex, const int& endVertex, int C[][100]) {
	// Khoi tao gia tri dinh bat dau va dinh ket thuc
	int beginNode = beginVertex - 1;
	int endNode = endVertex - 1;

	// Khoi tao mang S va so luong phan tu cua mang S
	int* S = new int[numVetex];
	int numS = 0;

	// Khoi tao mang cost va mang P
	int* cost = new int[numVetex];
	int* P = new int[numVetex];

	// Khoi tao gia tri cho mang cost va P
	for (int N = 0; N < numVetex; ++N) {
		cost[N] = MAX;
		P[N] = -1;
	}

	// Khoang cach tu beginNode den beginNode = 0
	cost[beginNode] = 0;

	// Lap cho den khi nao checkContinue = false
	bool checkContinue = true;
	while (checkContinue) {
		int currentNode = -1;

		// Kiem tra xem con co dinh nao phu hop de lam currentNode khong
		bool hasNextNode = false;
		for (int i = 0; i < numVetex; ++i) {

			// Kiem tra xem co dinh nao khong co trong S khong
			bool checkExist = false;
			for (int num = 0; num < numS; ++num) {
				if (S[num] == i) {
					checkExist = true;
					break;
				}
			}

			if (checkExist) {
				continue;
			}

			hasNextNode = true;

			// Neu do dai tu dinh beginNode toi dinh i nho hon tu beginNode toi dinh currentNode thì gan currentNode = i
			if (currentNode == -1 || cost[currentNode] > cost[i])
				currentNode = i;
		}

		if (hasNextNode) {

			// Neu chua dinh currentNode la dinh endNode thi xuat ket qua va ket thuc thuat toan
			if (currentNode == endNode) {
				printf("Da tim thay duong di ngan nhat tu %d den %d:\n", beginVertex, endVertex);
				printf("Do dai duong di: %d\n", cost[endNode]);

				// Mang luu cac thu tu duong di de truy xuat nguoc
				int* path = new int[numVetex];
				int num = 0;

				// Thuc hien truy xuat nguoc
				int current = endNode;
				while (P[current] != -1) {
					current = P[current];
					path[num] = current;
					++num;
				}

				// In ra thu tu cua duong di
				printf("Thu tu cua duong di: ");

				for (int i = num - 1; i >= 0; --i)
					printf("%d -> ", path[i] + 1);

				printf("%d\n", endNode + 1);

				checkContinue = false;
			} else {

				// Them currentNode vao S
				S[numS] = currentNode;
				++numS;

				// Cap nhat do dai duong di ngan nhat tu dinh bat dau toi cac dinh con lai
				for (int i = 0; i < numVetex; ++i) {
					bool checkExist = false;
					for (int num = 0; num < numS; ++num) {
						if (S[num] == i) {
							checkExist = true;
							break;
						}
					}

					if (checkExist) {
						continue;
					}

					// Thuc hien cap nhat
					if (cost[currentNode] + C[currentNode][i] < cost[i]) {
						cost[i] = cost[currentNode] + C[currentNode][i];
						P[i] = currentNode;
					}
				}
			}
		} else { // Neu sau khi duyet xong van khong tim thay dinh endNode
			printf("Khong tim thay duong di tu %d den %d\n", beginVertex, endVertex);
			checkContinue = false;
		}
	}
}

void findWayByDijkstra1_n(const int& numVetex, const int& beginVertex, int C[][100]) {
	int beginNode = beginVertex - 1;

	int* S = new int[numVetex];
	int numS = 0;

	int* cost = new int[numVetex];
	int* P = new int[numVetex];

	for (int N = 0; N < numVetex; ++N) {
		cost[N] = MAX;
		P[N] = -1;
	}

	cost[beginNode] = 0;

	bool checkContinue = true;
	while (checkContinue) {
		int currentNode = -1;

		bool hasNextNode = false;
		for (int i = 0; i < numVetex; ++i) {
			bool checkExist = false;
			for (int num = 0; num < numS; ++num) {
				if (S[num] == i) {
					checkExist = true;
					break;
				}
			}

			if (checkExist) {
				continue;
			}

			hasNextNode = true;

			if (currentNode == -1 || cost[currentNode] > cost[i])
				currentNode = i;
		}

		if (hasNextNode) {
			printf("Duong di ngan nhat tu %d den %d:\n", beginVertex, currentNode + 1);
			printf("Do dai duong di: %d\n", cost[currentNode]);

			int* path = new int[numVetex];
			int num = 0;

			int current = currentNode;
			while (P[current] != -1) {
				current = P[current];
				path[num] = current;
				++num;
			}

			printf("Thu tu cua duong di: ");

			for (int i = num - 1; i >= 0; --i)
				printf("%d -> ", path[i] + 1);

			printf("%d\n", currentNode + 1);

			S[numS] = currentNode;
			++numS;

			for (int i = 0; i < numVetex; ++i) {
				bool checkExist = false;
				for (int num = 0; num < numS; ++num) {
					if (S[num] == i) {
						checkExist = true;
						break;
					}
				}

				if (checkExist) {
					continue;
				}

				if (cost[currentNode] + C[currentNode][i] < cost[i]) {
					cost[i] = cost[currentNode] + C[currentNode][i];
					P[i] = currentNode;
				}
			}
		} else {
			checkContinue = false;
		}
	}
}

void findWayByFloydn_n(const int& numVetex, int C[][100]) {
	//Khai bao mang D
	int **D = new int* [numVetex];
	for (int i = 0; i < numVetex; ++i) {
		D[i] = new int[numVetex];
		for (int j = 0; j < numVetex; ++j)
			D[i][j] = MAX;
	}

	// Khai bao mang T
	int** T = new int* [numVetex];
	for (int i = 0; i < numVetex; ++i) {
		T[i] = new int[numVetex];
		for (int j = 0; j < numVetex; ++j)
			T[i][j] = -1;
	}

	// Khoi tao gia tri cho mang D va mang T
	for (int u = 0; u < numVetex; ++u)
		for (int v = 0; v < numVetex; ++v) 
			if (C[u][v] != 0) {
				D[u][v] = C[u][v];
				T[u][v] = v;
				T[v][u] = u;
			}

	// Thuc hien tim kiem duong di
	for (int currentNode = 0; currentNode < numVetex; ++currentNode) 
		for (int u = 0; u < numVetex; ++u)
			for (int v = 0; v < numVetex; ++v)
				if (D[u][currentNode] + D[v][currentNode] < D[u][v]) {
					D[u][v] = D[u][currentNode] + D[currentNode][v];
					T[u][v] = currentNode;
				}

	for (int u = 0; u < numVetex; ++u)
		for (int v = 0; v < numVetex; ++v)
			if (u != v) {
				printf("Duong di ngan nhat tu dinh %d den dinh %d la:\n", u + 1, v + 1);
				printf("\tDo dai: %d\n", D[u][v]);
				printf("\tThu tu duong di: ");
				
				// Mang luu cac thu tu duong di de truy xuat nguoc
				int* path = new int[numVetex];
				int num = 0;

				// Thuc hien truy xuat nguoc
				int current = v;
				
				while (T[u][current] != current) {
					path[num] = T[u][current];
					++num;
					current = T[u][current];
				}

				printf("%d -> ", u + 1);

				for (int i = num - 1; i >= 0; --i)
					printf("%d -> ", path[i] + 1);

				printf("%d\n", v + 1);
			}
}

int main() {
	int numVertex, beginVertex, endVertext;
	int C[100][100];
	getInput("Input.txt", numVertex, beginVertex, endVertext, C);

	printf("%d, %d, %d\n", numVertex, beginVertex, endVertext);
	printMatrix(C, numVertex);

	/*findWayByDijkstra1_1(numVertex, beginVertex, endVertext, C);
	findWayByDijkstra1_n(numVertex, beginVertex, C);*/
	findWayByFloydn_n(numVertex, C);

	getchar();
}
