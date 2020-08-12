#include"base.h"

class Solution {
private:
	int PageNum;	//the number of pages
	int BlockNum;	//the number of page frames
	vector<int> PageOrder;	//reference string
	vector<bool> isFault;	//whether the page is fault
	vector<vector<int>> Simulate;	//simulation table of algorithm
public:
	Solution() {
		cout << "Input the number of page frames:" << endl;
		cin >> BlockNum;
		cout << "Input the number of pages:" << endl;
		cin >> PageNum;
		PageOrder.resize(PageNum);
		cout << "Input the reference string:" << endl;
		for (int i = 0; i < PageNum; i++) {
			cin >> PageOrder[i];
		}
		isFault.resize(PageNum);
		cout << "*************************Reference String*************************" << endl;
		cout << endl;
		for (auto x : PageOrder) {
			cout << x << "  ";
		}
		cout << endl;
		cout << endl;
		cout << "******************************************************************" << endl;
	}

	void Initial() {
		Simulate.resize(PageNum);
		for (auto &it : Simulate) {
			it.resize(BlockNum);
		}
		for (int i = 0; i < PageNum; i++) {
			isFault[i] = false;
			for (int j = 0; j < BlockNum; j++) {
				Simulate[i][j] = 0;
			}
		}
	}

	static bool compare(const PAGE &temp1, const PAGE &temp2) {
		return temp1.flag < temp2.flag;
	}

	//find whther the current page is in frames
	bool Find(vector<vector<int>> Simulate, int row, int page) {
		if (row == 0) {
			return false;
		}
		vector<int>::iterator it = find(Simulate[row - 1].begin(), Simulate[row - 1].end(), page);
		if (it != Simulate[row - 1].end()) {
			return true;
		}
		else {
			return false;
		}
	}

	//find position of next recent acceesing string 
	int Distance(vector<vector<int>> Simulate, int row) {
		int Max = 0;
		vector<PAGE> distance;
		distance.resize(BlockNum);
		for (int i = 0; i < BlockNum; i++) {
			distance[i] = PAGE(Simulate[row - 1][i], 1000, i);
			vector<int>::iterator it = find(PageOrder.begin() + row, PageOrder.end(), distance[i].number);
			//find
			if (it != PageOrder.end()) {
				distance[i].flag = it - (PageOrder.begin() + row);
			}
			//not find (flag is 1000 first)
			else {
				for (int j = row - 1; j >= 0; j--) {
					//add FIFO weight when doesn't find
					if (Simulate[j][i] == distance[i].number) {
						distance[i].flag++;
					}
					else{
						break;
					}
				}
			}
		}
		sort(distance.begin(), distance.end(), compare);
		//return position of frames which has the biggest flag
		return distance[BlockNum - 1].position;
	}

	void FIFO() {
		Initial();
		queue<PAGE> q;
		bool isfull = false;
		vector<int> temp(BlockNum);
		int num = 0;
		for (int i = 0; i < PageNum; i++) {
			if (!Find(Simulate, i, PageOrder[i])) {
				isFault[i] = true;
				if (!isfull) {
					q.push(PAGE(PageOrder[i], 0, num));
					temp[num++] = PageOrder[i];
					if (num >= BlockNum) {
						isfull = true;
					}
				}
				else {
					PAGE p = q.front();
					q.pop();
					q.push(PAGE(PageOrder[i], 0, p.position));
					temp[p.position] = PageOrder[i];
				}
			}
			for (int j = 0; j < BlockNum; j++) {
				Simulate[i][j] = temp[j];
			}
		}
	}

	void OPT() {
		Initial();
		bool isfull = false;
		vector<int> temp(BlockNum);
		int num = 0;
		for (int i = 0; i < PageNum; i++) {
			if (!Find(Simulate, i, PageOrder[i])) {
				isFault[i] = true;
				if (!isfull) {
					temp[num++] = PageOrder[i];
					if (num >= BlockNum) {
						isfull = true;
					}
				}
				else {
					temp[Distance(Simulate, i)] = PageOrder[i];
				}
			}
			for (int j = 0; j < BlockNum; j++) {
				Simulate[i][j] = temp[j];
			}
		}
	}

	void LRU() {
		Initial();
		vector<PAGE> q;
		bool isfull = false;
		vector<int> temp(BlockNum);
		int num = 0;
		for (int i = 0; i < PageNum; i++) {
			//find in frames
			if (!Find(Simulate, i, PageOrder[i])) {
				isFault[i] = true;
				if (!isfull) {
					q.push_back(PAGE(PageOrder[i], i + 1, num));
					temp[num++] = PageOrder[i];
					if (num >= BlockNum) {
						isfull = true;
					}
				}
				else {
					PAGE p = q[0];
					q[0] = PAGE(PageOrder[i], i + 1, p.position);
					temp[p.position] = PageOrder[i];
				}
			}
			//not find in frames
			else {
				for (int k = 0; k < q.size(); k++) {
					//update flag with current new position
					if (q[k].number == PageOrder[i]) {
						q[k].flag = i + 1;
					}
				}
			}
			sort(q.begin(), q.end(), compare);
			for (int j = 0; j < BlockNum; j++) {
				Simulate[i][j] = temp[j];
			}
		}
	}

	void Print() {
		int fault = 0;
		for (int i = 0; i < PageNum; i++) {
			cout << setw(3) << PageOrder[i] << ": ";
			if (!isFault[i]) {
				cout << "hit!" << endl;
			}
			else {
				fault++;
				for (int j = 0; j < BlockNum; j++) {
					if (Simulate[i][j] != 0) {
						cout << setw(3) << Simulate[i][j] << " ";
					}
				}
				cout << endl;
			}
		}
		cout << "The number of page fault: " << fault << endl;
		cout << "The rate of page fault: " << fault << "/" << PageNum << " = " << (double)fault / (double)PageNum << endl;
	}
};

int main() {
	int state = 0;
	do {
		cout << endl;
		cout << "What would you do: 1.FIFO  2.OPT  3.LRU  4.Quit" << endl;
		cout << endl;
		cin >> state;
		switch (state) {
			case 1:
			{
				Solution *test1 = new Solution();
				cout << "The result of FIFO:" << endl;
				test1->FIFO();
				test1->Print();
			}
				break;
			case 2:
			{
				Solution *test2 = new Solution();
				cout << "The result of OPT:" << endl;
				test2->OPT();
				test2->Print();
			}
				break;
			case 3:
			{
				Solution *test3 = new Solution();
				cout << "The result of LRU" << endl;
				test3->LRU();
				test3->Print();
			}
				break;
			default:
				break;
		}
	} while (state != 4);
	system("pause");
	return 0;
}