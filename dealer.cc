
#include "dealer.h"
#include <iostream>
#include <random>
#include <chrono>
#include <thread>

//slowdown terminal output to emulate realtime gameplay, modify input to chrono::milliseconds to increase/decrease delay
//NOTE: all single line cout statements must be guarded
#define cout this_thread::sleep_for(chrono::milliseconds(40));cout

using namespace std;

//generate a deck in order and commence play
void dealer_operations::start_of_play(player &me, player &com,bool e_flag)
{
	/* 
		in a previous life, the following code existed in the constructor.
		but since the games repeat without returning to main, this SOP call
		ensures that the deck is reorganized
	*/
	if(e_flag != 0) goto SKIP_SOP; // if==1, function called from EOP, skip begin prompt
	char y;
GET_CHAR_SOP:
	cout << "Shall we begin? [y/n]: ";
	cin >> y;
	if((y == 'y') || (y == 'Y'))
	{
SKIP_SOP:
		init(me,com); //deal the first four cards, 2 to the dealer, 2 to me
		get_state(me,com);
		end_of_play(me,com); //explicit call to end the game in case the get_state chain fails, which is get_state > deal > get_state > ...
	}
	else if((y == 'n') || (y == 'N'))
		end_of_play(me,com,1);
	else
		goto GET_CHAR_SOP;
}

//give readout of final hands and ask to play again
void dealer_operations::end_of_play(player& me, player& com, bool t_flag)
{
    if(t_flag != 0) goto SKIP_EOP;
    ++games_played;
	cout <<"You have "; 
	syntax_name(me); 
	cout << ": " << calc(me) << '\n' << '\n' << "The dealer has "; 
	syntax_name(com); 
	cout << ": " << calc(com) << '\n';
	cout << '\n' << '\n';
	cout << "You have won " << me.wins; if(me.wins==1){ cout << " time.\n"; }else {cout << " times.\n";}
	printf("That's a win percentage of %.1f%% for %i", (me.wins/(double)games_played)*100, games_played); 
	if(me.wins==1) {cout << " game played.\n";}
	else {cout << " games played.\n";}

	char y;
GET_CHAR_EOP:
    cout << "The round is over... play again? [y/n]: ";
	cin >> y;
	if ((y == 'y') || (y == 'Y'))
	{
		me.destruct(); com.destruct();
		start_of_play(me,com,1);
	}
	else if((y == 'n') || (y == 'N'))
	{
SKIP_EOP:
		cout << "Goodbye!" << '\n';
		exit(0);
	}
	else
		goto GET_CHAR_EOP;
}

//deal the initial hand
void dealer_operations::init(player& me, player& com)
{
    sort();
	shuffle();

	get_card(me.hand[0]);
	get_card(com.hand[0]);
	get_card(me.hand[1]);
	get_card(com.hand[1]);
    cout << "************************START**************************" << '\n' << '\n';
	cout << "On the dealer's side of the table, there are two cards." << '\n';
	cout << "The first face down, and the second: "; name(com.hand[1]); cout << '\n'; //display the dealer's second card
    cout << "From what you can see, the dealer is best valued at a score of: " << calc(com,1) << "\n\n";

}

//count the number of cards a player has
int dealer_operations::count(player &guy)
{
	int cnt=0;
	for(int i=0;i<END_DECK;++i)
		if(guy.hand[i].val!=0)
			cnt++;
	return cnt;
}

void dealer_operations::get_state(player &me, player &com, bool d_flag,bool t_flag)
{
    if(calc(me)>21)
	{
        cout << '\n' << "***********************RESULTS*************************" << '\n';
		cout << '\n' << "You bust! :( The dealer wins by default!"<< '\n' << '\n';
		end_of_play(me,com);
	}
	else if((calc(com)==21)||(calc(me)==21)) //someone has blackjack right now i know it i know it...
	{
        cout << '\n' << "***********************RESULTS*************************" << '\n';
		if((calc(me)==21) && (calc(com)!=21)) //the dealer does not have blackjack but i do
		{
			cout << '\n' << "You have Black Jack; you win! :)" << '\n' << '\n';
			me.wins++;
		}
		else if(calc(me)==calc(com)) //we might both have it (but this would only happen at the beginning implicitly)
		{
			cout << '\n' << "It's a Black Jack Push from the beginning!" << '\n' << '\n';
		}
		else if(count(com)==2) //if my score is not the same as the dealers and one of us has blackjack, the dealer has it.
		{
			cout << '\n' << "The dealer was dealt Black Jack at the start :(" << '\n' << '\n';
		}
		end_of_play(me,com);
	}
	if(d_flag) //SET, control is passed to the dealer
	{
		if(!t_flag)
			deal(me,com,1);
		else
		{
            cout << '\n' << "***********************RESULTS*************************" << '\n';
			if((calc(com)==calc(me)))
			{
				cout << '\n' << "It's a push :/" << '\n' << '\n';
				end_of_play(me,com);
			}
			else if((calc(me))>(calc(com)))
			{
				cout << '\n' << "You win!" << '\n' << '\n';
				me.wins++;
				end_of_play(me,com);
			}
			else if(calc(com)==21)
			{
				cout << '\n' << "The dealer has Black Jack :(" << '\n' << '\n';
                end_of_play(me,com);
			}
			else if((calc(com)<21) && ((calc(me)<calc(com)) || (calc(me)>21)))
			{
				cout << '\n' << "The dealer wins by default :(" << '\n' << '\n';
				end_of_play(me,com);
			}
			else
			{
				cout << '\n' << "The dealer bust; you win! :)" << '\n' << '\n';
				me.wins++;
				end_of_play(me,com);
			}
		}
	}
	else //i.e. d_flag is clear, i'm in control
	{
		cout << '\n' << "Your cards are: "; syntax_name(me); cout << '\n';
		cout << "Your cards are best valued at a score of: " << calc(me) << '\n' << '\n';
		char y;
GET_CHAR2:
		cout << "Would you like to be hit? [y/n]: ";
		cin >> y;
		if((y=='y') || (y=='Y'))
			deal(me,com);
		else if((y=='n') || (y=='N'))
			deal(me,com,1);//i.e. I stand and now the dealer plays..
		else
			goto GET_CHAR2;
	}
}

void dealer_operations::deal(player &me,player &com,bool s_flag)
{
	int i=count(me),j=count(com);
	if(!s_flag) //i.e. the s_flag is CLEAR..i.e. I am hitting
	{
		cout << "You are hit with ";
		get_card(me.hand[i]); name(me.hand[i]);
		cout << '\n';
		if(calc(me)>21) //do i bust?
			get_state(me,com,1);
		else
			get_state(me,com);
	}
	else //i.e. i stay and the dealer makes its claim and the scores are produced
	{
		//cout << '\n';
		if((calc(com)<21) && (calc(com)>=17))//COM will hit IFF his score is less than 17 (i.e. here it stays and the final score is evaluated)
		{
			cout << "The dealer stands... " << '\n';
			get_state(me,com,1,1);
		}
        else if((calc(com)>21) && (calc(com)>=17)) //the case in which COM "stands" but has actually bust
        {
            get_state(me,com,1,1);
        }
		else //the dealer hits
		{
			cout << "The dealer is hit with... ";
			get_card(com.hand[j]); name(com.hand[j]); cout << '\n';
			get_state(me,com,1);
		}
		get_state(me,com,1);//stay and return the win condition result
	}
	//TODO: organize flag structure such that I pass one value and the baton passes to dealer control until bust or stay
	//I need to ensure that when I stay, control is successfully passed to the dealer.
    //(i.e. (?) instead of passing raw bits, flags are "automatic")
}

//calculate the value of a player's hand
int dealer_operations::calc(player &guy,bool b_flag)
{
	card tmp[END_DECK]; int sumH = 0, sumL= 0, i = 0;
	copy(begin(guy.hand), end(guy.hand), begin(tmp));//copy the contents of guy.hand to tmp...from std library
	if(b_flag==1)//i.e. I am calculating the hand of the dealer at deal time.
	{
		if((tmp[1].val==13)||(tmp[1].val==12)||(tmp[1].val==11))
			tmp[1].val=10; //if first card is face, set to 10
		if(tmp[1].val==1)
			tmp[1].val=11; //if first card is ace, set high if it benefits the player
		sumH=tmp[1].val; //the starting sum is now the value of this evaluation
		return sumH; //using ace_val high;
	}
	while(tmp[i].val!=0) //while there are cards in the player's hand
	{
		if((tmp[i].val==13)||(tmp[i].val==12)||(tmp[i].val==11))
			tmp[i].val=10; //resolve face cards to 10 points
		if(tmp[i].val==1)//an ace is always high if the score is < 10 or blackjack is nigh (==10)
		{
			if((sumH==10)||(sumL==10)) //if the current card is an ace
				tmp[i].val=11;
			else if(sumH<10)
			{
				sumH+=tmp[i].val=11;  sumL+=tmp[i].val=1;
				++i; continue;
			}

		}
		sumH+=tmp[i].val;
		sumL+=tmp[i].val;
		++i;
	}
	if((sumH>sumL) && (sumH>21))
		return sumL;
	else
		return sumH;
}

void dealer_operations::get_card(card &c)
{	
	int i=0;
	if(deck[END_DECK-1].p==1)//if the last card has been picked
	{
		cerr << " ERROR: the deck is empty, all cards have been dealt\nThis should never happen...\n\n\n";
		exit(0); //in the meanwhile before I find a user-friendly error handling solution, this is the best option.
	}
	for(int i=0; i<END_DECK;++i)
	{
		if(deck[i].p==1)
			continue;
		else
		{
			c=deck[i];
			deck[i].p=1;
			break;
		}
	}
}

//sort the global deck
void dealer_operations::sort()
{
	card j;
	j.suit=1; j.val=1;
	cout << '\n';
	for(int i=0; i<END_DECK; i++)
	{
		deck[i]=j;
		j.val++;
		if(j.val==14)//if the card count reaches 13, increment suit and reset to val=1
		{
			j.suit++;
			j.val=1;
		}
	}
}

//shuffle the global deck using a standard swap
void dealer_operations::shuffle()
{
	random_device rd;  //Will be used to obtain a seed for the random number engine
	mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
	uniform_int_distribution<> dis1(0,(END_DECK-1)); //i.e. a deck of 52 cards
	int i,n = END_DECK; //start at location 52
	while(n > 1) 
    {
		i = dis1(gen);  --n;  
		swap(deck[i],deck[n]);
	}
}

void dealer_operations::syntax_name(player &guy)
{
	int q=count(guy);
	for(int j=0;j<q;++j)
	{
		if(j==0)
		{
			name(guy.hand[j]);
			continue;
		}
		if(j==(q-1))
		{
			cout << ", and ";
			name(guy.hand[j]);
			break;
		}
		else
		{
			cout << ", ";
			name(guy.hand[j]);
		} 
	}
}

void dealer_operations::name(card& c)
{
	if((c.val==1)||(c.val==8)) {
		cout << "an ";
	}
	else {
		cout << "a ";
	}
	switch(c.val)
	{
		case(1):
			cout << "Ace";break;
		case(2):
			cout << "Two";break;
		case(3):
			cout << "Three";break;
		case(4):
			cout << "Four";break;
		case(5):
			cout << "Five";break;
		case(6):
			cout << "Six";break;
		case(7):
			cout << "Seven";break;
		case(8):
			cout << "Eight";break;
		case(9):
			cout << "Nine";break;
		case(10):
			cout << "Ten";break;
		case(11):
			cout << "Jack";break;
		case(12):
			cout << "Queen";break;
		case(13):
			cout << "King";break;
		default:
			cout << c.val;break;
	}
	cout << " of ";
	switch(c.suit)
	{
		case(1):
			cout << "Hearts";break;
		case(2):
			cout << "Spades";break;
		case(3):
			cout << "Diamonds";break;
		case(4):
			cout << "Clubs";break;
		default:
			cout << c.suit;break;
	}
}
