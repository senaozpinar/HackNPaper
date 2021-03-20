#include "Hacker.h"
#include <iostream>
using namespace std;

int Hacker::nCreatedInstance = 0;

Hacker::Hacker(int id, float arrivalTime){
    this->id = id;
    Hacker::nCreatedInstance++;
    this->arrivalTime = arrivalTime;
    this->totCodeCommit = 0;
    this->giftsReceived = 0;
    this->timeWaiting = 0;
}

void Hacker::commitCode(int lineChange, float time){
    if(lineChange >= 20){
        this->codeCommits.push_back(time);
    }
    this->totCodeCommit += lineChange;
}

int Hacker::getCommitSize(float time){
    int commits = 0;
    for(int i = 0; i < this->codeCommits.size(); i++){
        if(this->codeCommits[i] <= time){
            commits++;
        }
    }
    return commits;
}

int Hacker::getId(){
    return this->id;
}

Hacker::~Hacker(){
    //cout <<"Destructing";
}