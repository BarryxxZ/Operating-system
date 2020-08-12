/*
	created by Qiu yiqin
	2020/5/29
	BANKER.cpp
*/
#include<iostream>
#include<iomanip>
#include<vector>
#include<queue>
#include<string>
using namespace std;

class BANKER {
private:
	int PROCESS_NUM;
	int RESOURCE_NUM;
	vector<bool> Finish;
	vector<int> Resource;
	vector<int> Available;
	vector<vector<int>> Max;
	vector<vector<int>> Allocation;
	vector<vector<int>> Need;
	queue<int> SafeSecurity;
public:
	BANKER() {
		cout << "The number of processes: ";
		cin >> PROCESS_NUM;

		cout << "The number of resource type: ";
		cin >> RESOURCE_NUM;

		Finish.resize(PROCESS_NUM);
		Resource.resize(RESOURCE_NUM);
		Available.resize(RESOURCE_NUM);
		Max.resize(PROCESS_NUM);
		for (auto &it : Max) {
			it.resize(RESOURCE_NUM);
		}
		Allocation.resize(PROCESS_NUM);
		for (auto &it : Allocation) {
			it.resize(RESOURCE_NUM);
		}
		Need.resize(PROCESS_NUM);
		for (auto &it : Need) {
			it.resize(RESOURCE_NUM);
		}
	}

	void intial() {
		cout << "The numbers of the resources for each resource type:" << endl;
		for (int i = 0; i < RESOURCE_NUM; i++) {
			cin >> Resource[i];
		}

		cout << "Input the resource allocation of each processes:" << endl;
		for (int i = 0; i < PROCESS_NUM; i++) {
			for (int j = 0; j < RESOURCE_NUM; j++) {
				cin >> Allocation[i][j];
			}
		}

		cout << "Input each process's max need for the resources:" << endl;
		for (int i = 0; i < PROCESS_NUM; i++) {
			for (int j = 0; j < RESOURCE_NUM; j++) {
				cin >> Max[i][j];
			}
		}

		cout << "Input the available resources:" << endl;
		for (int i = 0; i < RESOURCE_NUM; i++) {
			cin >> Available[i];
		}

		for (int i = 0; i < PROCESS_NUM; i++) {
			for (int j = 0; j < RESOURCE_NUM; j++) {
				Need[i][j] = Max[i][j] - Allocation[i][j];
			}
		}
		cout << "**************************************" << endl;
		cout << "This is the data that you input:" << endl;
		cout << "-------------------------------------" << endl;
		cout << setw(15) << "Allocation";
		cout << setw(6) << "Max";
		cout << setw(14) << "Available" << endl;
		for (int i = 0; i < PROCESS_NUM; i++) {
			cout << "P" << i;
			cout << "   ";
			for (int j = 0; j < RESOURCE_NUM; j++) {
				cout << Allocation[i][j] << " ";
			}
			cout << "       ";
			for (int j = 0; j < RESOURCE_NUM; j++) {
				cout << Max[i][j] << " ";
			}
			if (0 == i) {
				cout << "  ";
				for (int j = 0; j < RESOURCE_NUM; j++) {
					cout << Available[j] << " ";
				}
				cout << endl;
			}
			else {
				cout << endl;
			}
		}
		cout << "-------------------------------------" << endl;
		cout << "**************************************" << endl;
	}

	bool IsSafe() {
		vector<int> Work(Available);
		vector<vector<int>> CurrentWork;
		vector<vector<int>> CurrentWorkA;
		CurrentWork.resize(PROCESS_NUM);
		for (auto &it : CurrentWork) {
			it.resize(RESOURCE_NUM);
		}
		CurrentWorkA.resize(PROCESS_NUM);
		for (auto &it : CurrentWorkA) {
			it.resize(RESOURCE_NUM);
		}
		Finish.assign(PROCESS_NUM, false);
		while (!SafeSecurity.empty()) {
			SafeSecurity.pop();
		}
		for (int k = 0; k < PROCESS_NUM; k++) {
			int isFind = false;
			for (int i = 0; i < PROCESS_NUM; i++) {
				if (!Finish[i]) {
					bool flag = true;
					for (int j = 0; j < RESOURCE_NUM; j++) {
						if (Need[i][j] > Work[j]) {
							flag = false;
							break;
						}
					}
					if (flag) {
						for (int j = 0; j < RESOURCE_NUM; j++) {
							CurrentWork[i][j] = Work[j];
							Work[j] += Allocation[i][j];
							CurrentWorkA[i][j] = Work[j];
						}
						Finish[i] = true;
						isFind = true;
						SafeSecurity.push(i);
						cout << "The current result of safety algorithm:" << endl;
						cout << "----------------------------------------------------------" << endl;
						cout << setw(9) << "Work";
						cout << setw(8) << "Need";
						cout << setw(14) << "Allocation";
						cout << setw(18) << "Work+Allocation";
						cout << setw(9) << "Finish" << endl;
						for (int i = 0; i < PROCESS_NUM; i++) {
							cout << "P" << i;
							cout << setw(4);
							for (int j = 0; j < RESOURCE_NUM; j++) {
								cout << CurrentWork[i][j] << " ";
							}
							cout << setw(4);
							for (int j = 0; j < RESOURCE_NUM; j++) {
								cout << Need[i][j] << " ";
							}
							cout << setw(4);
							for (int j = 0; j < RESOURCE_NUM; j++) {
								cout << Allocation[i][j] << " ";
							}
							cout << setw(9);
							for (int j = 0; j < RESOURCE_NUM; j++) {
								cout << CurrentWorkA[i][j] << " ";
							}
							cout << setw(14);
							if (Finish[i]) {
								cout << "true" << endl;
							}
							else {
								cout << "false" << endl;
							}
						}
						cout << "----------------------------------------------------------" << endl;
					}
				}
			}
			if (!isFind) {
				break;
			}
		}
		bool res = true;
		for (auto result : Finish) {
			if (result == false) {
				res = false;
			}
		}
		if (res == true) {
			cout << "The current state is safe!" << endl;
			cout << "Safe Sequence is: ";
			while (!SafeSecurity.empty()) {
				if (SafeSecurity.size() == 1) {
					cout << SafeSecurity.front();
				}
				else {
					cout << SafeSecurity.front() << "-->";
				}
				SafeSecurity.pop();
			}
			cout << endl;
		}
		else {
			cout << "The current state is not safe!" << endl;
		}
		cout << "**************************************" << endl;
		return res;
	}
	
	void CanRequest(int process, vector<int> Request) {
		Request.resize(RESOURCE_NUM);
		cout << "Input which one the request resources: ";
		for (int i = 0; i < RESOURCE_NUM; i++) {
			cin >> Request[i];
		}
		for (int i = 0; i < RESOURCE_NUM; i++) {
			if (Request[i] > Need[process][i]) {
				cout << "Can't request, Request > Need" << endl;
				return;
			}
		}
		for (int i = 0; i < RESOURCE_NUM; i++) {
			if (Request[i] > Available[i]) {
				cout << "Can't request, Request > Available" << endl;
				return;
			}
		}
		for (int i = 0; i < RESOURCE_NUM; i++) {
			Available[i] -= Request[i];
			Allocation[process][i] += Request[i];
			Need[process][i] -= Request[i];
		}
		if (this->IsSafe()) {
			cout << "Request can be granted" << endl;
			return;
		}
		else {
			cout << "Request can't be granted" << endl;
			return;
		}
	}
};

int main() {
	BANKER *bank = new BANKER();
	bank->intial();
	bank->IsSafe();
	int process;
	string input;
	cout << "Request resource? y or n" << endl;
	cin >> input;
	if (input == "y") {
		vector<int> Request;
		cout << "Input request process:";
		cin >> process;
		bank->CanRequest(process, Request);
	}
	system("pause");
	return 0;
}