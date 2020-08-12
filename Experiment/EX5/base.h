/*
	written by Qiu yiqin
	OS Experiment 5 in HQU
	2020/6/9
*/
#pragma once
#include<iostream>
#include<vector>
#include<queue>
#include<algorithm>
#include<iomanip>
using namespace std;

//page information in block
struct PAGE {
	int number;	//reference string
	int flag;	//position of recent accessing in LRU, distance of next recent accessing string in OPT with some weight
	int position;	//position in block
	PAGE() {}
	PAGE(int _number, int _flag, int _position) :number(_number), flag(_flag), position(_position) {}
};