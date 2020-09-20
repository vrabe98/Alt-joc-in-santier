#pragma once
#include <list>
#include <map>
#include <string>

#define OBJECTIVE_IN_PROGRESS 0
#define OBJECTIVE_FINISHED 1

#define NOT_TAKEN 0
#define ACTIVE 1
#define FINISHED 2


class Quest_flag {
	short int counter;
	std::string name;
public:
	void Load(std::ifstream&);
	void Set_counter(int cnt) { counter = cnt; }
	int Change_counter(int diff) {
		counter += diff;
		if (counter < 0) {
			diff = -1 * counter;
			counter = 0;
		}
		else diff = 0;
		return diff;
	}
	std::string Name() { return name; }
	short int Counter() { return counter; }
};

class Quest_objective {
	short int status;
	std::list<Quest_flag> needed_flags;
	std::string description;
public:
	std::string Description() { return description; }
	void Load(std::ifstream&);
	void Refresh();
	bool Finished() { return status == OBJECTIVE_FINISHED; }
	short int Status() { return status; }
};

class Quest{
protected:
	short int status, id;
	std::string name, description;
	std::list<Quest_objective> objectives;
public:
	std::string Info();
	std::string Name() { return name; }
	void Take() { status = ACTIVE; }
	virtual void Load(std::ifstream&)=0;
	virtual void Refresh()=0;
	short int Status() { return status; }
	bool Finished() { return status == FINISHED; }
};

class Regular_quest :public Quest {
public:
	void Load(std::ifstream&);
	void Refresh();
};

class Chain_quest:public Quest {
	short int next_quest;
public:
	void Load(std::ifstream&);
	void Refresh();
};