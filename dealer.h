#include "player.h"
#ifndef _DEALER_H_
#define _DEALER_H_

class dealer_operations
{
public:
	void get_state(player&, player&,bool d_flag=false,bool t_flag=false);//SET for dealer control, CLEAR for a continue; SET=play is terminated
	void init(player&, player&);
	void end_of_play(player&, player&, bool t_flag=false); //SET play terminates without info display;
	void start_of_play(player&,player&,bool e_flag=false);//SET to indicate called from EOP, ie no need for cin prompt, CLEAR for prompt
	dealer_operations():games_played(0){};
	~dealer_operations()
	{
		for(int i=0;i<END_DECK;++i)
			deck[i]=0;
	}
	void shuffle();
	void sort();
protected:
	void name(card&);
	void get_card(card&);
	int calc(player&,bool b_flag=false);//SET to indicate blind first card, CLEAR for all cards
	void deal(player&,player&,bool s_flag=false);//SET to indicate I am staying, CLEAR for hit
	int count(player&);
	void syntax_name(player&);
private:
	card deck[END_DECK];
	int games_played;
};

#endif // _DEALER_H_

