using namespace std;

#include <vector>

#ifndef Hacker_H
#define Hacker_H

class Hacker {
private:
	static int nCreatedInstance;
    int id;
    float arrivalTime;
    vector<float> codeCommits;
    

public:
	Hacker(int id, float arrivalTime);
	void commitCode(int lineChange, float time);
	int getCommitSize(float time);
	~Hacker();
	int giftsReceived;
	int getId();
	int totCodeCommit;
	float timeWaiting;

};

#endif

