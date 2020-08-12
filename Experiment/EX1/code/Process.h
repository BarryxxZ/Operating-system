#pragma once
#ifndef PROCESS_H_
#define PRCESS_H_
#include<iostream>
#include<iomanip>
#include<string>
#include<vector>
#include<algorithm>
constexpr auto MAX = 1000;
using namespace std;

int number;	//number of processes
typedef struct Process
{
	string tag;	//process name
	int arrive_t;	//arrive time
	int cpu_burst;
	//For preemptive_SJF
	int remain_t;
	bool is_finish;
	//
	int start_t;
	int end_t;
	int turnaround_t;
	int wait_t;
	int order_flag;	//Record excution order
}PROCESS;
PROCESS Process[MAX];

#endif

