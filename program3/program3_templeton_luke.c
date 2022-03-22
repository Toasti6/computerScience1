//Name: Luke Templeton
//Dr. Steinberg
//COP3502 Spring 2022
//Programming Assignment 3
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct card_s{
	int rank;    //number
	char * type; //type of card
	struct card_s * nextptr;
}card_t;

//function prototypes
void rules(); //display rules
int playRound(); //simulate round
card_t * openCardDeck(); //open the card deck and place into a linkedlist setup
card_t * insertBackSetup(card_t *node, char *name, int cardrank); //take card from orginial deck and place in back of linked list for setup of game
int empty(card_t * node); //check to see if linked list is empty
void cleanUp(card_t * head); //free memory to prevent memory leaks
int deckSize(card_t * head); //count number of nodes in the linked list
card_t * search(card_t * node, int spot); //search list for a specific spot in the card deck indexing is similar to array setup
card_t * copyCard(card_t * node); //make a deep copy of card
card_t * removeCard(card_t * node, int spot); //remove card from linkedlist
card_t * insertBackDeck(card_t *head, card_t *node); //place card at end of pile
int compareCard(card_t * cardp1, card_t * cardp2); //compare cards
card_t * moveCardBack(card_t *head); //place card at top of deck to the bottom of the deck
card_t * warSetup(card_t *node); //sets up war deck

int main()
{
	int seed;
	printf("Enter Seed: ");
	scanf("%d", &seed);
	
	srand(seed); //seed set
	rules();
	
	int player; //1 or 2
	int result;
	
	printf("Would you like to be player 1 or 2?\n");
	printf("Enter 1 or 2: ");
	scanf("%d", &player); //choose player
	
	for(int game = 1; game <= 5; ++game) //simulate games
	{
		printf("Alright lets play game %d.\n", game);
		printf("Lets split the deck.\n");

		result = playRound(); //play game
		
		if((result == 1 && player == 1) || (result == 2 && player == 2)) //determine who won
			printf("You won game %d.\n", game);
		else
			printf("I won game %d.\n", game);
	}
	
	return 0;
}

void rules()
{
	printf("Welcome to the card game war!\n");
	printf("Here are the rules.\n");
	printf("You have a pile of cards and I have a pile of cards.\n");
	printf("We will each pull the top card off of our respective deck and compare them.\n");
	printf("The card with the highest number will win the round and take both cards.\n");
	printf("However if there is a tie, then we have to we have to place one card faced down and the next one faced up and compare the results.\n");
	printf("Winner of the tie, will grab all the cards out. However if it's a tie again, then we repeat the same action.\n");
	printf("Ready? Here we go!\n");
}

int playRound()
{
	int result;

	card_t *fullDeck = openCardDeck();
	int originalDeckSize = deckSize(fullDeck); //constant that is equal to full open deck
	int fullDeckSize = originalDeckSize; //fullDeckSize is a counter

	card_t *p1Deck = NULL; //player 1 and 2 decks
	card_t *p2Deck = NULL;

	card_t *insertCard; //card to be inserted into decks

	int randCard; //random card drawn from original deck

	printf("There are %d cards in the deck.\n", originalDeckSize);

	for(int i = 0; i < originalDeckSize; i++) //loops for every card in original deck
	{	
		randCard = rand() % fullDeckSize; //random spot

		insertCard = NULL;

		if(randCard % 2 == 0) //executes if random number is even
		{
			insertCard = search(fullDeck, randCard); //finds the card to insert
			p1Deck = insertBackDeck(p1Deck, copyCard(insertCard)); //inserts card to deck
			fullDeck = removeCard(fullDeck, randCard); //removes card from original deck
		}
		else if(randCard % 2 != 0) //executes if random number is odd
		{
			insertCard = search(fullDeck, randCard);
			p2Deck = insertBackDeck(p2Deck, copyCard(insertCard));
			fullDeck = removeCard(fullDeck, randCard);
		}

		fullDeckSize -= 1;
	}

	cleanUp(fullDeck); //frees mallocs, original deck no longer necessary

	int p1DeckSize = deckSize(p1Deck); //gets original deck size
	int p2DeckSize = deckSize(p2Deck);
	int roundResult = 0; //win condition

	while(p1DeckSize > 0 || p2DeckSize > 0) //while cards are in deck
	{
		printf("Player 1 pulled out %s.\t", p1Deck->type);
		printf("Player 2 pulled out %s.\n", p2Deck->type);

		roundResult = compareCard(p1Deck, p2Deck); //gets round win result

		if(roundResult == 1) //if p1 wins
		{
			printf("Player 1 won that round.\n");

			p1Deck = moveCardBack(p1Deck); //moves card to back of deck
			p1Deck = insertBackDeck(p1Deck, copyCard(p2Deck)); //inserts p2 card
			p2Deck = removeCard(p2Deck, 0); //removes p2 card from deck

			p1DeckSize = deckSize(p1Deck); //updates deck sizes
			p2DeckSize = deckSize(p2Deck);

			printf("Player 1 has %d cards.\n", p1DeckSize);
			printf("Player 2 has %d cards.\n", p2DeckSize);
		}
		else if(roundResult == 2) //if p2 wins
		{
			printf("Player 2 won that round.\n");

			p2Deck = moveCardBack(p2Deck);
			p2Deck = insertBackDeck(p2Deck, copyCard(p1Deck));
			p1Deck = removeCard(p1Deck, 0);

			p1DeckSize = deckSize(p1Deck);
			p2DeckSize = deckSize(p2Deck);

			printf("Player 1 has %d cards.\n", p1DeckSize);
			printf("Player 2 has %d cards.\n", p2DeckSize);
		}
		else
		{
			int war = 1; //war condition
			int warResult = 0;

			card_t *warDeck = NULL; //separate war deck

			card_t *p1Copy = p1Deck;
			card_t *p2Copy = p2Deck;

			while(compareCard(p1Copy->nextptr->nextptr, p2Copy->nextptr->nextptr) == 0)
			{
				warDeck = warSetup(warDeck);
				p1Copy = p1Copy->nextptr;
				p2Copy = p2Copy->nextptr;

			}

			if(p1DeckSize == 1 || p2DeckSize == 1) //if one card is left
			{
				if(p1DeckSize > p2DeckSize) //larger deck wins
				{
					return 1;
				}
				else
				{
					return 2;
				}
			}

			if(p1DeckSize == 2 || p2DeckSize == 2) //if 2 cards are left in one deck
			{
				printf("Ugh oh...We have a tie! W-A-R!\n");
				printf("Player 1 pulled out %s.\t", p1Deck->nextptr->type);
				printf("Player 2 pulled out %s.\n", p2Deck->nextptr->type);

				warResult = compareCard(p1Deck->nextptr, p2Deck->nextptr); //compares last card

				if(warResult == 1) //if p1 wins war
				{
					printf("Player 1 won that W-A-R!\n");

					p1Deck = moveCardBack(p1Deck); 
					p1Deck = insertBackDeck(p1Deck, copyCard(p2Deck));
					p2Deck = removeCard(p2Deck, 0);
					p1Deck = moveCardBack(p1Deck);
					p1Deck = insertBackDeck(p1Deck, copyCard(p2Deck));
					p2Deck = removeCard(p2Deck, 0);

					war = 0; //war is finished
				}
				else if(warResult == 2)
				{
					printf("Player 2 won that W-A-R!\n");

					p2Deck = moveCardBack(p2Deck);
					p2Deck = insertBackDeck(p2Deck, copyCard(p1Deck));
					p1Deck = removeCard(p1Deck, 0);
					p2Deck = moveCardBack(p2Deck);
					p2Deck = insertBackDeck(p2Deck, copyCard(p1Deck));
					p1Deck = removeCard(p1Deck, 0);

                    war = 0;
				}
			}

			while(war == 1) //if war is active
			{
				warDeck = insertBackDeck(warDeck, copyCard(p1Deck)); //inserts cards to separate war deck
                p1Deck = removeCard(p1Deck, 0);
                warDeck = insertBackDeck(warDeck, copyCard(p1Deck));
                p1Deck = removeCard(p1Deck, 0);
                warDeck = insertBackDeck(warDeck, copyCard(p2Deck));
                p2Deck = removeCard(p2Deck, 0);
                warDeck = insertBackDeck(warDeck, copyCard(p2Deck));
               	p2Deck = removeCard(p2Deck, 0);

               	p1DeckSize = deckSize(p1Deck); 
				p2DeckSize = deckSize(p2Deck);

				printf("Ugh oh...We have a tie! W-A-R!\n");
				printf("Player 1 pulled out %s.\t", p1Deck->type);
				printf("Player 2 pulled out %s.\n", p2Deck->type);

				printf("Player 1 has %d cards.\n", p1DeckSize);
				printf("Player 2 has %d cards.\n", p2DeckSize);

				warResult = compareCard(p1Deck, p2Deck); //gets result for war

				if(warResult == 1) //if p1 wins war
				{
					printf("Player 1 won that W-A-R!\n");

					while(warDeck != NULL)
                    {
                        p1Deck = insertBackDeck(p1Deck, copyCard(warDeck)); //inserts cards from war deck
                        warDeck = removeCard(warDeck, 0); //removes cards from war deck
                    }

                    p1Deck = moveCardBack(p1Deck);
                    p1Deck = insertBackDeck(p1Deck, copyCard(p2Deck));
                    p2Deck = removeCard(p2Deck, 0);

					war = 0; //war is over

					p1DeckSize = deckSize(p1Deck);
					p2DeckSize = deckSize(p2Deck);
				}
				else if(warResult == 2) //if p2 wins war
				{
					printf("Player 2 won that W-A-R!\n");

 					while(warDeck != NULL)
                    {
                        p2Deck = insertBackDeck(p2Deck, copyCard(warDeck));
                        warDeck = removeCard(warDeck, 0);
                    }

					p2Deck = moveCardBack(p2Deck);
					p2Deck = insertBackDeck(p2Deck, copyCard(p1Deck));
					p1Deck = removeCard(p1Deck, 0);

					war = 0;

					p1DeckSize = deckSize(p1Deck);
					p2DeckSize = deckSize(p2Deck);
				}
				else
				{
					war = 1; //war continues if tie
				}
			}
		}

		if(p1DeckSize == 0) //if p1 wins
		{
			return 1;
		}
		else if(p2DeckSize == 0) //if p2 wins
		{
			return 2;
		}
	}

	return result;
}

card_t * openCardDeck()
{
	FILE *fptr = fopen("deck.txt", "r");
	
	char *name = (char *) malloc(sizeof(char) * 20);
	
	if (name == NULL)
	{
		printf("Error in malloc...\n");
		exit(1);
	}
	
	card_t * head = NULL;
	
	int cardrank = 13;
	int tracker = 1;
	while(fscanf(fptr, "%s", name) == 1)
	{			
		strcat(name, " ");
		
		if(tracker % 5 == 1)
		{
			strcat(name, "of Clubs");
			head = insertBackSetup(head, name, cardrank);
		}
		else if(tracker % 5 == 2)
		{
			strcat(name, "of Diamonds");
			head = insertBackSetup(head, name, cardrank);
		}
		else if(tracker % 5 == 3)
		{
			strcat(name, "of Hearts");
			head = insertBackSetup(head, name, cardrank);
		}
		else if(tracker % 5 == 4)
		{
			strcat(name, "of Spades");
			head = insertBackSetup(head, name, cardrank);
			tracker = 0;
			--cardrank;
		}
		
		++tracker;
		
	}
	
	free(name);
	fclose(fptr);
	
	return head;
}

card_t * insertBackSetup(card_t *node, char *name, int cardrank)
{
	
    if(empty(node)) //check to see if list is empty
    {
		node = (card_t *) malloc(sizeof(card_t));
		
		if(empty(node)) //check to see if malloc worked
		{
			printf("Did not allocate head successfully...");
			printf("Program Terminating...\n");
			exit(1);
		}
		else //otherwise populate data of new head node
		{
			node->type = (char *) malloc(sizeof(char) * 20);
			
			if(node->type == NULL)
			{
				printf("Error with malloc...\n");
				exit(1);
			}
			
			strcpy(node->type, name);
			node->rank = cardrank;
			node->nextptr = NULL; //must make new tail nextptr NULL!!!
			
			return node;  //terminate
		}
      
    }
	
	//here we know that the list has at least one node
	
	card_t *head = node; //keep pointer to head since we will need to return this address
	
	while(node->nextptr != NULL) //traverse to tail
		node = node->nextptr;
	
	node->nextptr = (card_t *) malloc(sizeof(card_t)); 
	
	if(node->nextptr == NULL) //see if new tail was allocated successfully
	{
		printf("Did not allocate node successfully...");
		return head; //terminate if tail didn't get created
	}
	
	//populate new node
	node->nextptr->type = (char *) malloc(sizeof(char) * 20);
	
	if(node->nextptr->type == NULL)
	{
		printf("Error with malloc...\n");
		exit(1);
	}
	
	strcpy(node->nextptr->type, name);
	node->nextptr->rank = cardrank;
	node->nextptr->nextptr = NULL; //very important
			
	return head; //return head node since we need our starting point of the linked list
}

int empty(card_t * node)
{
	return (node == NULL); //return condition result
}

int deckSize(card_t * head)
{
	card_t *temp = head;

	int count = 1;

	if(empty(head)) //if there is nothing in deck
    {
        return 0;
    }

	while(temp->nextptr != NULL) //traverses linked list
	{
		temp = temp->nextptr;
		count++; //counts nodes
	}

	return count;
}

card_t * search(card_t * node, int spot)
{
	card_t *card = node;

	for(int i = 0; i < spot; i++) //traverses linked list
	{
		card = card->nextptr;
	}

	return card; //returns card when spot is reached
}

card_t * copyCard(card_t * node)
{
  card_t* copy = (card_t*)malloc(sizeof(card_t)); //memory for copied card

  copy->type = (char*)malloc(sizeof(char) * (strlen(node->type) + 1)); 

  strcpy(copy->type, node->type); //copies card type

  copy->rank = node->rank; //and rank

  copy->nextptr = NULL;

  return copy;
}

card_t * removeCard(card_t * node, int spot)
{
	card_t *head = node;
	card_t *temp = search(node, spot); //finds card to remove

	if(empty(node)) //if theres no cards in deck
	{
       return NULL;
   	}
	else if(head == temp) //if head is card to be removed
	{
		head = head->nextptr; //new head
		free(node); //frees original head
		return head;
	}
	else
	{
		while(node->nextptr != temp) //traverses to card
		{
        	node = node->nextptr;
    	}

		node->nextptr = temp->nextptr; //removes card
		return head;
	}
}

card_t * insertBackDeck(card_t *head, card_t *node)
{
	card_t *temp = head;

	if(empty(head))
    {
        head = node; //sets head to new card if list is empty
        return head;
    }

	while(temp->nextptr != NULL) //traverses to the back
	{
		temp = temp->nextptr;
	}

	temp->nextptr = node; //assigns new card

	return head;
}

int compareCard(card_t * cardp1, card_t * cardp2)
{
	if(cardp1->rank > cardp2->rank) //if p1 card is higher than p2
	{
		return 1;
	}
	else if(cardp2->rank > cardp1->rank) //if p2 card is higher than p1
	{
		return 2;
	}
	else //tie
	{
		return 0;
	}
}

card_t * moveCardBack(card_t *head)
{
	card_t *temp = head;
	card_t *newHead = head->nextptr; //next card is new head

	while(temp->nextptr != NULL)
	{
		temp = temp->nextptr; //traverses to back
	}	

	temp->nextptr = head; //moves old head
	head->nextptr = NULL; //cuts off old head nextptr

	return newHead; //new head returned
}

void cleanUp(card_t * head)
{
	card_t *temp;

	while(head != NULL) //while there are still nodes
	{
		temp = head;
		head = head->nextptr; //traverses list
		free(temp->type); //free everything within struct
		free(temp); //free node
	}
}

card_t * warSetup(card_t *node)
{
    if(empty(node)) //check to see if list is empty
    {
		node = (card_t *) malloc(sizeof(card_t));
		
		if(empty(node)) //check to see if malloc worked
		{
			printf("Did not allocate head successfully...");
			printf("Program Terminating...\n");
			exit(1);
		}
		else //otherwise populate data of new head node
		{
			node->type = (char *) malloc(sizeof(char) * 20);
			
			if(node->type == NULL)
			{
				printf("Error with malloc...\n");
				exit(1);
			}

			node->nextptr = NULL; //must make new tail nextptr NULL!!!
			
			return node;  //terminate
		}
      
    }
	
	//here we know that the list has at least one node
	
	card_t *head = node; //keep pointer to head since we will need to return this address
	
	while(node->nextptr != NULL) //traverse to tail
		node = node->nextptr;
	
	node->nextptr = (card_t *) malloc(sizeof(card_t)); 
	
	if(node->nextptr == NULL) //see if new tail was allocated successfully
	{
		printf("Did not allocate node successfully...");
		return head; //terminate if tail didn't get created
	}
	
	//populate new node
	node->nextptr->type = (char *) malloc(sizeof(char) * 20);
	
	if(node->nextptr->type == NULL)
	{
		printf("Error with malloc...\n");
		exit(1);
	}
	
	node->nextptr->nextptr = NULL; //very important

	return head;
}
