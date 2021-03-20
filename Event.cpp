#include "Hacker.h"
#include "Event.h"
#include <vector>
#include <iostream>

using namespace std;

int Event::invAttToEnt = 0;
int Event::extrGiftAtt = 0;
vector<Event> Event::stickerQueue;
vector<Event> Event::hoodieQueue;
vector<Event> Event::hoodieQExits;

Event::Event(int id, float time){
	this->id = id;
	this->timeOfEvent = time;
}
	
void Event::stckrQEntries(Event& e, Hacker& h){
	if(h.getCommitSize(e.timeOfEvent) < 3){
		Event::invAttToEnt++;
		return;
	}
	int giftCount = 0;
	for(int i = 0; i < Event::stickerQueue.size(); i++){
		if(Event::stickerQueue[i].id == h.getId()){
			giftCount++;
		}
	}
	if(giftCount == 3){
		Event::extrGiftAtt++;
		return;
	}
	Event::stickerQueue.push_back(e);
	h.giftsReceived++;
}

void Event::hoodieQEntries(vector<Event>& stickerDesks){
	vector<Event>& vs = Event::stickerQueue;
	vector<Event>& vh = Event::hoodieQueue;
	vh.push_back(Event(vs[0].id, vs[0].timeOfEvent + stickerDesks[0].timeOfEvent));
	vs[0].deskId = 1;
	for(int i = 1; i < vs.size(); i++){
		vh.push_back(Event(vs[i].id, 0));
	}

	for(int i = 1; i < vs.size();i++){
		for(int j = 0; j < stickerDesks.size(); j++){
			int k;
			for(k = 0; k < i; k++){
				if(vh[k].timeOfEvent > vs[i].timeOfEvent){
					if(vs[k].deskId == j+1){
						break;
					}
				}
			}
			if(k == i){
			vs[i].deskId = j +1;
			vh[i].timeOfEvent = vs[i].timeOfEvent + stickerDesks[j].timeOfEvent;
			break;
			}
		}
		if(vh[i].timeOfEvent == 0){
			int k;
			for(k = 0; k < i; k++){
				if(vh[k].timeOfEvent > vh[i-1].timeOfEvent - stickerDesks[vs[i-1].deskId -1].timeOfEvent){
					if(vs[k].deskId == 1){
						break;
					}
				}
			}
			for(int j = 0; j < stickerDesks.size(); j++){
				for(int a = 0; a < i;a++){
					if(vh[a].timeOfEvent > vh[i-1].timeOfEvent - stickerDesks[vs[i-1].deskId -1].timeOfEvent){
						if(vs[a].deskId == j+1){
							if(vh[a].timeOfEvent < vh[k].timeOfEvent){
								k = a;
							}
						}
					}
				}
			}
			vs[i].deskId = vs[k].deskId;
			vh[i].timeOfEvent = vh[k].timeOfEvent + stickerDesks[vs[i].deskId - 1].timeOfEvent;
		}
	}
	
}

void Event:: hoodieQExit(float deskTime, vector<Hacker>& hackers){
	vector<Event>& vh = Event::hoodieQueue;
	vector<Event>& ve = Event::hoodieQExits;
	vector<Hacker> waiters;
	ve.push_back(Event(vh[0].id, vh[0].timeOfEvent + deskTime));
	for(int i = 1; i <vh.size(); i++){
		ve.push_back(Event(vh[i].id, 0));
	}

	for(int i = 1; i < vh.size(); i++){
		if(vh[i].timeOfEvent >= ve[i-1].timeOfEvent){
			ve[i].timeOfEvent = vh[i].timeOfEvent + deskTime;
		}
		else{
			if(ve[i-1].timeOfEvent <= vh[i+1].timeOfEvent ){
				ve[i].timeOfEvent = ve[i-1].timeOfEvent + deskTime;
			}
			else{
				int j = i;
				while(vh[j].timeOfEvent < ve[i-1].timeOfEvent){
					waiters.push_back(hackers[vh[j].id-1]);
					j++;
				}
				int maxCommitSize = waiters[0].getCommitSize(ve[i-1].timeOfEvent);
				int id = 0;
				for(int k = 1; k < waiters.size(); k++){
					if(waiters[k].getCommitSize(ve[i-1].timeOfEvent) > maxCommitSize){
						maxCommitSize = waiters[k].getCommitSize(ve[i-1].timeOfEvent);
						id = k;
					}
				}
				waiters.erase(waiters.begin() + id );
				ve[i+id].timeOfEvent = ve[i-1].timeOfEvent + deskTime;
				i = j-1;
			}
		}
	}
}