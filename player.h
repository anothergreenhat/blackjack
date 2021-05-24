#define END_DECK 52
#ifndef _PLAYER_H_
#define _PLAYER_H_

struct card
{
	int val;
	int suit;
	bool p; //picked/played
	card operator=(card *c)
	{
		return *this;
	}
	card()
	{
		val=0;
		suit=0;
		p=0;
	}
};

class player
{
friend class dealer_operations;
public:
	//a callable "destructor"
	void destruct()
	{ 
		for(int i = 0; i < END_DECK; ++i)
		{
			hand[i].val = 0; hand[i].suit=0; hand[i].p=0;
		}
	} 
	player():wins(0) {};
private:
	card hand[END_DECK];
	int wins;
};

#endif // _PLAYER_H_

