#pragma once
#ifndef PROCESS_H_
#define PRCESS_H_
#include<iostream>
#include<iomanip>
#include<string>
#include<vector>
#include<queue>
#include<algorithm>
constexpr auto MAX = 1000;
using namespace std;

int number;	//number of processes
int quantum_slot = 0;	//time quantum
typedef struct Process
{
	string tag;	//process name
	int arrive_t;	//arrive time
	int cpu_burst;
	int priority;
	int remain_t;
	bool is_finish;
	int start_t;
	int end_t;
	int turnaround_t;
	int wait_t;
	int order_flag;	//Record excution order
}PROCESS;
PROCESS Process[MAX];

#endif
