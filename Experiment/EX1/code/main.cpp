#include"Process.h"
//used in sort() function
bool sort_Arrive(PROCESS &a,PROCESS &b) {
	return a.arrive_t < b.arrive_t;
}

bool unfinish() {
	for (int i = 0; i < number; i++) {
		if (!Process[i].is_finish) {
			return true;
		}
	}
	return false;
}

void Initial() {
	int num;
	string name;
	int arrive_time;
	int request_time;
	cout << "Please input the number of process:" << endl;
	cin >> num;
	while (num <= 0) {
		cout << "Check your input!!" << endl;
		cin >> num;
	}
	number = num;
	cout << "Please input the information of process:" << endl;
	cout << "name" << setw(1) << "";
	cout << "arrive_time" << setw(1) << "";
	cout << "cpu_burst" << setw(1) << endl;
	for (int i = 0; i < number; i++) {
		cout << "-------" << endl;
		cin >> name >> arrive_time >> request_time;
		Process[i].tag = name;
		Process[i].arrive_t = arrive_time;
		Process[i].cpu_burst = request_time;
		Process[i].remain_t = request_time;
		Process[i].is_finish = false;
		Process[i].start_t = 0;
		Process[i].end_t = 0;
		Process[i].turnaround_t = 0;
		Process[i].wait_t = 0;
		Process[i].order_flag = 0;
	}
	cout << "-------" << endl;
	cout << "****************************************************" << endl;
}

void FCFS() {
	float sum_turnaround = 0;
	float sum_wait = 0;
	cout << "****************************************************" << endl;
	cout << "Your choice is FCFS" << endl;
	sort(Process, Process + number, sort_Arrive);	//sort according to arrival time
	Process[0].start_t = Process[0].arrive_t;	//start time of the earliest process is its arrival time
	for (int i = 0; i < number; i++) {
		Process[i].end_t = Process[i].start_t + Process[i].cpu_burst;
		Process[i].turnaround_t = Process[i].end_t - Process[i].arrive_t;
		Process[i].wait_t = Process[i].start_t - Process[i].arrive_t;
		Process[i].order_flag = i + 1;
		if (i != number - 1) {
			Process[i + 1].start_t = max(Process[i].end_t, Process[i + 1].arrive_t);	//Compare the next arrival time with the current end time to determine the next start time
		}
		sum_turnaround += Process[i].turnaround_t;
		sum_wait += Process[i].wait_t;
	}
	cout << "average_turnaround_time: " << sum_turnaround / number << endl;
	cout << "average_waiting_time: " << sum_wait / number << endl;
}

void SJF() {
	int current = 0;
	float sum_turnaround = 0;
	float sum_wait = 0;
	cout << "****************************************************" << endl;
	cout << "Your choice is SJF" << endl;
	sort(Process, Process + number, sort_Arrive);
	//sorted by shortest job
	for (int i = 0; i < number; i++) {
		for (int j = i + 1; j < number; j++) {
			if (Process[i].arrive_t <= current && Process[j].arrive_t <= current) {
				if (Process[j].cpu_burst < Process[i].cpu_burst) {
					PROCESS temp;
					temp = Process[j];
					Process[j] = Process[i];
					Process[i] = temp;
				}
			}
		}
		Process[i].start_t = max(current, Process[i].arrive_t);
		Process[i].end_t = Process[i].start_t + Process[i].cpu_burst;
		current = Process[i].end_t;
	}
	//calculate data of finished Process[MAX]
	for (int i = 0; i < number; i++) {
		Process[i].order_flag = i + 1;
		Process[i].turnaround_t = Process[i].end_t - Process[i].arrive_t;
		Process[i].wait_t = Process[i].start_t - Process[i].arrive_t;
		sum_turnaround += Process[i].turnaround_t;
		sum_wait += Process[i].wait_t;
	}
	cout << "average_turnaround_time: " << sum_turnaround / number << endl;
	cout << "average_waiting_time: " << sum_wait / number << endl;
}

void preemptive_SJF() {
	int t = 0;	//start the timer
	int flag = 0;	//record end sequence order
	float sum_turnaround = 0;
	float sum_wait = 0;
	cout << "****************************************************" << endl;
	cout << "Your choice is preemptive_SJF:" << endl;
	cout << "The procedure detail is: " << endl;
	while (unfinish()) {
		int index = -1;	//index of shortest remainder job
		int current_remain = 2147483647;	//record current shortest remainder time
		//find shortest remainder job that has arrived
		for (int i = 0; i < number; i++) {
			if (!Process[i].is_finish) {
				if (Process[i].arrive_t <= t) {
					if (Process[i].remain_t < current_remain) {
						current_remain = Process[i].remain_t;
						index = i;
					}
				}
			}
		}
		cout << "time= " << t << ", " << Process[index].tag << " is running" << endl;	//print the running process of the current time
		if (-1 == index) {
			t++;	//not find, time++
		}
		else {
			if (Process[index].remain_t == Process[index].cpu_burst) {
				Process[index].start_t = t;
			}
			Process[index].remain_t--;	//remainder time--
			t++;
			//when remainder time=0, this process is finish
			if (Process[index].remain_t == 0) {
				flag++;
				Process[index].end_t = t;
				Process[index].turnaround_t = Process[index].end_t - Process[index].arrive_t;
				Process[index].wait_t = Process[index].turnaround_t - Process[index].cpu_burst;
				Process[index].order_flag = flag;
				sum_turnaround += Process[index].turnaround_t;
				sum_wait += Process[index].wait_t;
				Process[index].is_finish = true;
			}
		}
	}
	cout << "average_turnaround_time: " << sum_turnaround / number << endl;
	cout << "average_waiting_time: " << sum_wait / number << endl;
	cout << setw(10) << "name" << " ";
	cout << setw(10) << "start_time" << " ";
	cout << setw(10) << "end_time" << " ";
	cout << setw(10) << "turnaround_time" << " ";
	cout << setw(10) << "wait_time" << " ";
	cout << setw(10) << "finish_order" << endl;
	for (int i = 0; i < number; i++) {
		cout << setw(10) << Process[i].tag << " ";
		cout << setw(10) << Process[i].start_t << " ";
		cout << setw(10) << Process[i].end_t << " ";
		cout << setw(10) << Process[i].turnaround_t << " ";
		cout << setw(10) << Process[i].wait_t << " ";
		cout << setw(10) << Process[i].order_flag << endl;
	}
}

void display() {
	cout << setw(10) << "name" << " ";
	cout << setw(10) << "start_time" << " ";
	cout << setw(10) << "end_time" << " ";
	cout << setw(10) << "turnaround_time" << " ";
	cout << setw(10) << "wait_time" << " ";
	cout << setw(10) << "sequence_order" << " ";
	cout << setw(10) << "ready_queue" << endl;
	for (int i = 0; i < number; i++) {
		cout << setw(10) << Process[i].tag << " ";
		cout << setw(10) << Process[i].start_t << " ";
		cout << setw(10) << Process[i].end_t << " ";
		cout << setw(10) << Process[i].turnaround_t << " ";
		cout << setw(10) << Process[i].wait_t << " ";
		cout << setw(10) << Process[i].order_flag << " ";
		cout << setw(10);
		for (int j = i + 1; j < number; j++) {
			cout << Process[j].tag << " ";
		}
		cout << endl;
	}
	cout << "****************************************************" << endl;
}

int main() {
	string result;
	Initial();
	do {
		cout << "Please choose algorithm: 1.FCFS, 2.SJF, 3.preemptive_SJF :" << endl;
		int flag;
		cin >> flag;
		if (1 == flag) {
			FCFS();
			display();
		}
		if (2 == flag) {
			SJF();
			display();
		}
		if (3 == flag) {
			preemptive_SJF();
		}
		cout << "Input 'Q' to quit, 'C' to continue:" << endl;
		cin >> result;
	} while (result != "Q");
	system("pause");
	return 0;
}