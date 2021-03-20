using namespace std;

#include <vector>
#include "Hacker.h"

#ifndef Event_H
#define Event_H

class Event{

public:
	int id;
	float timeOfEvent;
	int deskId;
	Event(int id, float time);
	static void stckrQEntries(Event& e, Hacker& h);
	static void hoodieQEntries(vector<Event>& stickerDesks);
	static void hoodieQExit(float deskTime, vector<Hacker>& hackers);
	static int invAttToEnt;
	static int extrGiftAtt;
	static vector<Event> stickerQueue;
	static vector<Event> hoodieQueue;
	static vector<Event> hoodieQExits;

};

#endif
