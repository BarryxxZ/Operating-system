#include"Process.h"
//used in sort() function
bool sort_Arrive(PROCESS &a, PROCESS &b) {
	return a.arrive_t < b.arrive_t;
}

//used in preemptive_Priority to find the whether process is finished
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
	int prio;
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
	cout << "cpu_burst" << setw(1) << "";
	cout << "priority" << setw(1) << endl;
	for (int i = 0; i < number; i++) {
		cout << "-------" << endl;
		cin >> name >> arrive_time >> request_time>>prio;
		Process[i].tag = name;
		Process[i].arrive_t = arrive_time;
		Process[i].cpu_burst = request_time;
		Process[i].remain_t = request_time;
		Process[i].priority = prio;
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

void Priority() {
	int current = 0;
	float sum_turnaround = 0;
	float sum_wait = 0;
	cout << "****************************************************" << endl;
	cout << "Your choice is Priority" << endl;
	sort(Process, Process + number, sort_Arrive);
	for (int i = 0; i < number; i++) {
		for (int j = i + 1; j < number; j++) {
			if (Process[i].arrive_t <= current && Process[j].arrive_t <= current) {
				if (Process[j].priority < Process[i].priority) {
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

void RR() {
	int t = 0;	//start the timer
	int flag = 0;	//record end sequence order
	int spend_t = 0;	//spend time in every quantum
	float sum_turnaround = 0;
	float sum_wait = 0;
	queue<PROCESS> ready;	//ready queue
	queue<PROCESS> result;	//store the finish result
	cout << "****************************************************" << endl;
	cout << "Your choice is preemptive_SJF:" << endl;
	cout << "The procedure detail is: " << endl;
	sort(Process, Process + number, sort_Arrive);
	//push process into ready queue accoring to priority and arrive_time
	for (int i = 0; i < number; i++) {
		ready.push(Process[i]);
	}
	PROCESS temp;
	while (!ready.empty()) {
		temp = ready.front();	//get the front of queue
		ready.pop();	//pop out
		queue<PROCESS> current = ready;	//store the process in current quantum
		flag++;
		if (temp.remain_t == temp.cpu_burst) {
			temp.start_t = t;
		}
		temp.remain_t -= quantum_slot;
		//not finish, push into ready queue
		if (temp.remain_t > 0) {
			ready.push(temp);
			t += quantum_slot;
			spend_t = quantum_slot;
		}
		//finish, not push
		if (temp.remain_t <= 0) {
			t += (temp.remain_t + quantum_slot);
			spend_t = temp.remain_t + quantum_slot;
			temp.end_t = t;
			temp.turnaround_t = temp.end_t - temp.arrive_t;
			temp.wait_t = temp.turnaround_t - temp.cpu_burst;
			sum_turnaround += temp.turnaround_t;
			sum_wait += temp.wait_t;
			result.push(temp);
		}
		cout << "quantum " << flag << ":  spend time=> " << spend_t << "  " << temp.tag << " is running!" << "=> ";
		cout << "ready queue is: ";
		while (!current.empty()) {
			cout << current.front().tag << " ";
			current.pop();
		}
		cout << endl;
	}
	cout << "average_turnaround_time: " << sum_turnaround / number << endl;
	cout << "average_waiting_time: " << sum_wait / number << endl;
	cout << setw(10) << "name" << " ";
	cout << setw(10) << "start_time" << " ";
	cout << setw(10) << "end_time" << " ";
	cout << setw(10) << "turnaround_time" << " ";
	cout << setw(10) << "wait_time" << endl;
	while (!result.empty()) {
		cout << setw(10) << result.front().tag << " ";
		cout << setw(10) << result.front().start_t << " ";
		cout << setw(10) << result.front().end_t << " ";
		cout << setw(10) << result.front().turnaround_t << " ";
		cout << setw(10) << result.front().wait_t << endl;
		result.pop();
	}
}

void preemptive_Priority() {
	int t = 0;	//start the timer
	int flag = 0;	//record end sequence order
	float sum_turnaround = 0;
	float sum_wait = 0;
	cout << "****************************************************" << endl;
	cout << "Your choice is preemptive_Priority:" << endl;
	cout << "The procedure detail is(every time_slot is 1): " << endl;
	while (unfinish()) {
		int index = -1;	//index of sleast priority job
		int current_remain = 2147483647;	//record current shortest remainder time
		int current_prio = 2147483647;	//record current least priority
		//find least priority job that has arrived
		for (int i = 0; i < number; i++) {
			if (!Process[i].is_finish) {
				if (Process[i].arrive_t <= t) {
					if (Process[i].priority < current_prio) {
						current_prio = Process[i].priority;
						current_remain = Process[i].remain_t;
						index = i;
					}
				}
			}
		}
		cout << t <<"th "<< "time_slot" << ", " << Process[index].tag << " is running" << endl;	//print the running process of the current time
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
		cout << "Please choose algorithm: 1.Priority, 2.RR, 3.preemptive_Priority :" << endl;
		int flag;
		cin >> flag;
		if (1 == flag) {
			Priority();
			display();
		}
		if (2 == flag) {
			cout << "please input the time quantum: " << endl;
			cin >> quantum_slot;
			RR();
		}
		if (3 == flag) {
			preemptive_Priority();
		}
		cout << "Input 'Q' to quit, 'C' to continue:" << endl;
		cin >> result;
	} while (result != "Q");
	system("pause");
	return 0;
}