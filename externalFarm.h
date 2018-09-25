#pragma once
#include "sha1\sha1.hpp"
#include <iostream>
#include <chrono>
#include <fstream>
#include <vector>
#include <mutex>
#include <queue>
#include <thread>

// Import things that has been needed
//Timing
using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::cout;
using std::cerr;
using std::cin;
//InfileStream
using std::ifstream;
using std::getline;
//OutFileStream
using std::ofstream;
//Constructs
using std::string;
using std::vector;
using std::queue;
//Thread Items
using std::thread;
using std::mutex;

#define endl '\n'



struct pair
{
	string input, output;
};

/*
class Task
{
public:
	virtual ~Task();
	virtual void run() = 0; //pointer (abstract variable) for tasks
							// https://msdn.microsoft.com/en-us/library/c8whxhf1.aspx
};
/**/

// replace this for the required tasking
class HashTask// : public Task
{
	vector<vector<pair>> * vecPairs;
	vector<pair> * currentVector;

	mutex * currentWorkerP;
	uint32_t *currentWorkerPointer,
		     *total;
	SHA1 currHash;

public:
	HashTask(vector<vector<pair>> &in_, uint32_t &currentWP_, uint32_t &total_, mutex &currentWorkerP_)
		: vecPairs(&in_), currentWorkerPointer(&currentWP_), total(&total_), currentWorkerP(&currentWorkerP_)
	{
		//std::cerr << currentWP_ << " - " << *currentWorkerPointer << endl; //debug
	}
	void run();
};

class Farm
{
	//queue is used for ease, better structures could be used in future
	vector<thread> Active; //or busy
	queue <HashTask *> inActive; //or Idle
public:
	mutex currentWorkerP;
	uint8_t cpuCount;
	void new_task(HashTask * task);
	void Active_popAll(uint8_t &currentL);
	void Active_pushAll ();
	Farm();
	~Farm();
};
/**/