#include "externalFarm.h"

Farm::Farm()
{
	cpuCount = thread::hardware_concurrency() - 1;
}

Farm::~Farm()
{
}

//Task::~Task()
//{}

void Farm::new_task(HashTask *task)
{
	inActive.push(task);
};

void Farm::Active_popAll(uint8_t &currentL)
{
	do
	{
		if (!Active.empty()) //error proofing, yes it will make it marginally slower
		{
			// if its only joinable at the enddone at the end
			Active.back().join();
			Active.pop_back();
		}
		currentL--;
	} while (currentL != 0);

	return;
};

void Farm::Active_pushAll()
{ // Start a loop here once the task are all in
	for (auto i = 0; i < cpuCount; i++)
	{
		Active.push_back(thread([this]() {this->Active.back(); }));
		inActive.pop();
	}
};

void HashTask::run()
{
	do {
		//Grab next available vector
		currentWorkerP->lock();
		currentVector = &vecPairs->at(*currentWorkerPointer);
		*currentWorkerPointer = *currentWorkerPointer + 1;
		currentWorkerP->unlock();

		//Hash the vectors contents
		for (int curr = 0; curr < currentVector->size(); curr++)
		{
			currHash.update(currentVector->at(curr).input);
			currentVector->at(curr).output = currHash.final();
		}
		//std::cerr << *currentWorkerPointer << " a" << endl; //debug
		//std::cerr << *total << " b" << endl; //debug
	} while (*currentWorkerPointer < *total - 1);
	
	return;
}