using namespace std;

#include <fstream>
#include <iostream>
#include <vector>
#include "Hacker.h"
#include "Event.h"
#include <queue>
#include <cstdio>

float avrgWaitTimeforStckr(vector<Event>& start, vector<Event>& end, vector<Event>& desks){
	float time = 0; 
	for(int i = 0; i < start.size(); i++){
		time += end[i].timeOfEvent - start[i].timeOfEvent - desks[start[i].deskId-1].timeOfEvent;
	}
	return time / start.size();
}

float avrgWaitTimeforHoodie(vector<Event>& start, vector<Event>& end, float deskTime){
	float time = 0; 
	for(int i = 0; i < start.size(); i++){
		time += end[i].timeOfEvent - start[i].timeOfEvent - deskTime;
	}
	return time / start.size();
}

int maxLenOfQ(vector<Event>& v, float deskTime){
	int maxLen = 0;
	for(int i = 0; i<v.size()-1; i++){
		int currentLen = 0;
		if(v[i].timeOfEvent + deskTime <= v[i+1].timeOfEvent){
			continue;
		}
		else{
			for(int j = i; j < v.size(); j++){
				if(v[j+1].timeOfEvent < v[i].timeOfEvent + deskTime){
					currentLen++;
				}
				else{
					break;
				}
			}
			if(currentLen > maxLen){
				maxLen = currentLen;
			}
		}
	}
	return maxLen;
}

int main(int argc, char* argv[]) {

	string infile_name = argv[1];
	char* outfile_name = argv[2];

	//reading all the input
	ifstream infile;

	infile.open(infile_name); 

	int numbOfHckrs;
	infile >> numbOfHckrs;

	vector<Hacker> hackers;
	//placing hackers in a vector
	for(int i = 1; i <= numbOfHckrs; i++){
		float arrivalTime;
		infile >> arrivalTime;
		hackers.push_back(Hacker(i, arrivalTime));
	}

	int numbOfCommits;
	float timeOfLastCommit;
	infile >> numbOfCommits;
	//placing commits in vector
	for(int i = 0; i < numbOfCommits; i++){
		int hckrNo, lineChange; float time;
		infile >> hckrNo >> lineChange >> time;
		timeOfLastCommit = time;
		hackers[hckrNo-1].commitCode(lineChange, time);
	}

	int queueAttempts;
	float lastAttempToEnter;
	infile >> queueAttempts;
	//hackers attempting to get into sticker queue
	for(int i = 0; i < queueAttempts; i++){
		int hckrNo; float time;
		infile >> hckrNo >> time;
		lastAttempToEnter = time;
		Event e = Event(hckrNo, time);
		Event::stckrQEntries(e, hackers[hckrNo-1]);
	}
	int stckrDeskNum; float stckrDeskTime;
	infile >> stckrDeskNum;
	vector<Event> stckrDesks;
	for(int i = 0; i < stckrDeskNum; i++){
		infile >> stckrDeskTime;
		stckrDesks.push_back(Event(i+1, stckrDeskTime));
	}
	int hoodieDeskNum; float hoodieDeskTime;
	infile >> hoodieDeskNum;
	vector<Event> hoodieDesks;
	for(int i = 0; i < hoodieDeskNum; i++){
		infile >> hoodieDeskTime;
		hoodieDesks.push_back(Event(i+1, hoodieDeskTime));
	}
	
	infile.close();

	Event::hoodieQEntries(stckrDesks);
	Event::hoodieQExit(hoodieDeskTime, hackers);

	//average number of gifts
	float totNumOfGifts = 0;
	for(int i = 0; i < hackers.size(); i++){
		totNumOfGifts += hackers[i].giftsReceived;
	}
	float avrgNumOfGiftsPerHckr = totNumOfGifts / hackers.size();


	//average code length
	float totCodeLength = 0 ;
	for(int i = 0; i < hackers.size(); i++){
		totCodeLength += hackers[i].totCodeCommit;
	}

	//average turnaround time
	float totTime = 0;
	for(int i = 0; i < totNumOfGifts; i++){
		totTime += Event::hoodieQExits[i].timeOfEvent-Event::stickerQueue[i].timeOfEvent;
	}

	//most time spent
	vector<Event>& vs = Event::stickerQueue;
	vector<Event>& vh = Event::hoodieQueue;
	for(int i = 0; i < Event::stickerQueue.size(); i++){
		hackers[vs[i].id-1].timeWaiting += (vh[i].timeOfEvent-vs[i].timeOfEvent-stckrDeskTime) + (Event::hoodieQExits[i].timeOfEvent-vh[i].timeOfEvent-hoodieDeskTime);
	}
	int idOfLongestWaiter = 1;
	for(int i = 1; i < hackers.size(); i++){
		if(hackers[i].timeWaiting > hackers[idOfLongestWaiter-1].timeWaiting){
			idOfLongestWaiter = i+1;
		}
	}

	//least time spent among those with 3 gifts
	int idOfLeastWaiter = -1;;
	float waitTimeOfLW = -1;;
	for(int i = 0; i < hackers.size(); i++){
		if(hackers[i].giftsReceived == 3 && idOfLeastWaiter == -1 && waitTimeOfLW == -1){
			idOfLeastWaiter = i+1;
			waitTimeOfLW = hackers[i].timeWaiting;
		}
		if(hackers[i].giftsReceived == 3 && hackers[i].timeWaiting < waitTimeOfLW){
			idOfLeastWaiter = i+1;
			waitTimeOfLW = hackers[i].timeWaiting;
		}
	}

	//total time passed
	float totalTimePassed = Event::hoodieQExits[Event::hoodieQExits.size()-1].timeOfEvent;
	if(std::max(lastAttempToEnter, timeOfLastCommit) > totalTimePassed){
		totalTimePassed = std::max(lastAttempToEnter, timeOfLastCommit);
	}

	//writing to the output file
	//ofstream outfile;
	//outfile.open(outfile_name);
	FILE* fp;
	fp = fopen(outfile_name, "w");

	//1
	fprintf(fp,"%d\n", maxLenOfQ(Event::stickerQueue, stckrDeskTime));
	//2
	fprintf(fp, "%d\n", maxLenOfQ(Event::hoodieQueue, hoodieDeskTime));
	//3
	fprintf(fp, "%.3f\n", avrgNumOfGiftsPerHckr);
	//4
	fprintf(fp, "%.3f\n", avrgWaitTimeforStckr(Event::stickerQueue, Event::hoodieQueue, stckrDesks));
	//5
	fprintf(fp, "%.3f\n", avrgWaitTimeforHoodie(Event::hoodieQueue, Event::hoodieQExits, hoodieDeskTime));
	//6
	fprintf(fp, "%.3f\n", (float)numbOfCommits / numbOfHckrs);
	//7
	fprintf(fp, "%.3f\n", totCodeLength / numbOfCommits);
	//8
	fprintf(fp, "%.3f\n", totTime / totNumOfGifts);
	//9
	fprintf(fp, "%d\n", Event::invAttToEnt);
	//10
	fprintf(fp, "%d\n", Event::extrGiftAtt);
	//11
	fprintf(fp, "%d %.3f\n", idOfLongestWaiter, hackers[idOfLongestWaiter-1].timeWaiting);
	//12
	fprintf(fp, "%d %.3f\n", idOfLeastWaiter, waitTimeOfLW);
	//13
	fprintf(fp, "%.3f", totalTimePassed);

    return 0;
}