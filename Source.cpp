//
//	C.Forbes - 1500812
//

#include "externalFarm.h"

// Define"the_clock" for timing to only be done in main
typedef std::chrono::steady_clock the_clock;

void read_list(ifstream &infile, vector<vector<pair>> &wordList, uint32_t &total)
{
	uint32_t prevTotal = 0;
	string currentLine;
	pair currentItem;
	vector<pair> currentCent; //Cent being groups of 100, Trying to keep to 1 KB for Cache optimisation
	/**/

	// Future implementation could contain barriers allowing for reading whats required.
	// This could stop any issues reading files larger that Memory although the OS/Compiler
	// might deal with these which I believe might have shown up, might of been because
	// of empty vector fill however.

	while (getline(infile, currentLine)) //gets till EOF
	{
		// would prefer to get chunks to check but dont think I can without
		// doing multiple getlines and shoving them into an array

		currentItem.input = currentLine; // adds to 'chunks' of a vector, 
										 // this should remove the need for mutex as each task/thread will
										 // have their own section predefined
		currentCent.push_back(currentItem);
		total++;

		if (total % 100 == 0)
		{
			wordList.push_back(currentCent);
			currentCent.clear();
			prevTotal++;
		}
	}
	
	/* Chunk Alternative that would cause concerns
	ifstream bigFile("mybigfile.dat");
	constexpr size_t bufferSize = 1024 * 1024 * 1024;
	unique_ptr<char[]> buffer(new char[bufferSize]);
	while (bigFile)
	{
	bigFile.read(buffer.get(), 1024 * 1024 * 1024);
	// process data in buffer
	}
	/**/

	infile.close();

	cout << total << " - Hashes read" << endl;
	cout << prevTotal << " - Full chunks" << endl << endl;
	//Should add to a queue of items to be read

	//cin.ignore();//cheeky pause

	// full total not required from this point on
	// extra brackets encase BODMAS not being followed
	total = (total / 100) + 1;

	return; // no-issue run
			  /**/
}

bool write_to_list(ofstream &outfile, vector<vector<pair>> &vectorList) // this could be done as I go
{
	for (uint32_t outCurr = 0; outCurr < vectorList.size(); outCurr++)
	{
		for (uint32_t currPair = 0; currPair < vectorList.at(outCurr).size(); currPair++)
		{
			outfile << vectorList.at(outCurr).at(currPair).input << vectorList.at(outCurr).at(currPair).output << endl;
	}	}

	if (!outfile)
	{
		// An error has occurred at some point since we opened the file.
		cout << "Error somewhere writing out" << endl
			 << "(Did you pull out a usb containing this?)";
		return 0;
	}
	return 1;
}

int main()
{
	/**/
	string inFile;
	vector<vector<pair>> wordList;
	uint8_t currentL = 0;
	uint32_t total = 0, currentWorkerPoint = 0;
	Farm hash;
	HashTask mainHash(wordList, currentWorkerPoint, total, hash.currentWorkerP);
	
	cout << "Name of Input file (With Extention): " << endl;
	cin >> inFile;
	ifstream infile(inFile, ifstream::binary);

	if (infile.is_open())
	{
		ofstream outfile("HashList" + inFile, ofstream::binary);

		// Start timing
		the_clock::time_point start = the_clock::now();

		read_list(infile, wordList, total);//Try to do this in parralell without promises

		for (; currentL < hash.cpuCount; currentL++)
		{
			hash.new_task(new HashTask(wordList, currentWorkerPoint, total,hash.currentWorkerP));
		}

		hash.Active_pushAll();
		mainHash.run();
		hash.Active_popAll(currentL);

		write_to_list(outfile, wordList);

		outfile.close();

		// Stop timing
		the_clock::time_point end = the_clock::now();
		/**/

		// Compute the difference between the two times in milliseconds
		auto time_taken = duration_cast<milliseconds>(end - start).count();
		cout << endl << "Completing the given task took " << time_taken << " ms." << endl;
	}
	else
		cout << "Couldnt open specified file, relaunch and try again" << endl
			 << "(Did you put the dictionary in the same folder?)" << endl;

	cin.ignore(2);

	return 0;
}