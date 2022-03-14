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
//void cleanUp(card_t * head); //free memory to prevent memory leaks
int deckSize(card_t * head); //count number of nodes in the linked list
card_t * search(card_t * node, int spot); //search list for a specific spot in the card deck indexing is similar to array setup
card_t * copyCard(card_t * node); //make a deep copy of card
card_t * removeCard(card_t * node, int spot); //remove card from linkedlist
card_t * insertBackDeck(card_t *head, card_t *node); //place card at end of pile
int compareCard(card_t * cardp1, card_t * cardp2); //compare cards
card_t * moveCardBack(card_t *head); //place card at top of deck to the bottom of the deck

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
	int fullDeckSize = deckSize(fullDeck);

	card_t *p1Deck = (card_t*) malloc(fullDeckSize);
	card_t *p2Deck = (card_t*) malloc(fullDeckSize);

	p1Deck = NULL;
	p2Deck = NULL;

	printf("There are %d cards in the deck.\n", fullDeckSize);

	while(fullDeckSize > 0)
	{
		printf("%d\n", fullDeckSize);

		int randCard = rand() % fullDeckSize;
		card_t *insertCard = search(fullDeck, randCard);
		p1Deck = insertBackDeck(p1Deck, copyCard(insertCard));
		fullDeck = removeCard(fullDeck, randCard);
		fullDeckSize = deckSize(fullDeck);

		randCard = rand() % fullDeckSize;;
		insertCard = search(fullDeck, randCard);
		p2Deck = insertBackDeck(p2Deck, copyCard(insertCard));
		fullDeck = removeCard(fullDeck, randCard);
		fullDeckSize = deckSize(fullDeck);

		printf("%d\n", fullDeckSize);
	}

	int p1DeckSize = deckSize(p1Deck);
	int p2DeckSize = deckSize(p2Deck);
	int roundResult = 0;

	while(p1DeckSize > 0 || p2DeckSize > 0)
	{
		printf("Player 1 pulled out %s.\n", p1Deck->type);
		printf("\tPlayer 2 pulled out %s.", p2Deck->type);

		roundResult = compareCard(p1Deck, p2Deck);

		if(roundResult == 1)
		{
			printf("Player 1 won that round.\n");

			p1Deck = moveCardBack(p1Deck);
			p1Deck = insertBackDeck(p1Deck, copyCard(p2Deck));
			p2Deck = removeCard(p2Deck, 0);

			p1DeckSize = deckSize(p1Deck);
			p2DeckSize = deckSize(p2Deck);

			printf("Player 1 has %d cards.\n", p1DeckSize);
			printf("Player 2 has %d cards.\n", p2DeckSize);
		}
		else if(roundResult == 2)
		{
			printf("Player 1 won that round.\n");

			p2Deck = moveCardBack(p2Deck);
			p2Deck = insertBackDeck(p2Deck, copyCard(p1Deck));
			p1Deck = removeCard(p1Deck, 0);

			p2DeckSize = deckSize(p2Deck);
			p1DeckSize = deckSize(p1Deck);

			printf("Player 1 has %d cards.\n", p1DeckSize);
			printf("Player 2 has %d cards.\n", p2DeckSize);
		}
		else
		{
			int war = 1;
			int warResult = 0;

			card_t *warDeck = (card_t*) malloc(fullDeckSize);

			printf("Ugh oh...We have a tie! W-A-R!\n");
			printf("Player 1 pulled out %s.\n", p1Deck->->nextptr->nextptr->type);
			printf("\tPlayer 2 pulled out %s.", p2Deck->->nextptr->nextptr->type);

			while(war == 1)
			{
				warResult = cardCompare(p1Deck->nextptr->nextptr, p2Deck->nextptr->nextptr);


				if(warResult == 1)
				{
					printf("Player 1 won that W-A-R!\n");

/*					p1Deck = moveCardBack(p1Deck);
					p1Deck = moveCardBack(p1Deck);

					p1Deck = insertBackDeck(p1Deck, copyCard(p2Deck));
					p2Deck = removeCard(p2Deck, 0);
					p1Deck = insertBackDeck(p1Deck, copyCard(p2Deck));
					p2Deck = removeCard(p2Deck, 0);

					p1Deck = moveCardBack(p1Deck);
					p1Deck = insertBackDeck(p1Deck, copyCard(p2Deck));
					p2Deck = removeCard(p2Deck, 0);
*/
					warDeck = 

					war = 0;
				}
				else if(warResult == 2)
				{
					printf("Player 2 won that W-A-R!\n")
/*					p2Deck = moveCardBack(p2Deck);
					p2Deck = moveCardBack(p2Deck);

					p2Deck = insertBackDeck(p2Deck, copyCard(p1Deck));
					p1Deck = removeCard(p1Deck, 0);
					p2Deck = insertBackDeck(p2Deck, copyCard(p1Deck));
					p1Deck = removeCard(p1Deck, 0);

					p2Deck = moveCardBack(p2Deck);
					p2Deck = insertBackDeck(p2Deck, copyCard(p1Deck));
					p1Deck = removeCard(p1Deck, 0);
*/
					war = 0;
				}
				else
				{
					war = 1;
				}
			}
		}

		if(p1DeckSize == 0)
		{
			return 1;
		}
		else if(p2DeckSize == 0)
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

	printf("yer\n");

	if(empty(head))
    {
        return 0;
    }

    printf("bruhhhh\n");

	while(temp->nextptr != NULL)
	{
		temp = temp->nextptr;
		count++;
	}

	return count;
}

card_t * search(card_t * node, int spot)
{
	card_t *card = node;

	for(int i = 0; i < spot; i++)
	{
		card = card->nextptr;
	}

	return card;
}

card_t * copyCard(card_t * node)
{
	int nodeLength = strlen(node->type);

	card_t *copyCard = (card_t *) malloc(sizeof(card_t));
	copyCard->type = (char *) malloc(sizeof(char) * nodeLength + 1);

	strcpy(copyCard->type, node->type);
	copyCard->rank = node->rank;

	return copyCard;
}

card_t * removeCard(card_t * node, int spot)
{
	card_t *head = node;
	card_t *temp = search(head, spot);

	if(empty(node))
	{
       return NULL;
   	}
	else if(node == temp)
	{
		node = head->nextptr;
		head = NULL;
		free(temp);
		return node;
	}
	else
	{
		node = search(head, spot - 1);
		node->nextptr = temp->nextptr;

		free(temp);

		return node;
	}
}

card_t * insertBackDeck(card_t *head, card_t *node)
{
	card_t *temp = head;

	if(empty(head))
    {
        head = node;
        return head;
    }

    printf("testtttt\n");

	while(temp->nextptr != NULL)
	{
		printf("testt\n");
		temp = temp->nextptr;
	}

	temp->nextptr = node;

	return head;
}

int compareCard(card_t * cardp1, card_t * cardp2)
{
	if(cardp1->rank > cardp2->rank)
	{
		return 1;
	}
	else if(cardp2->rank > cardp1->rank)
	{
		return 2;
	}
	else
	{
		return 0;
	}
}

card_t * moveCardBack(card_t *head)
{
	card_t *temp = head;
	card_t *newHead = head->nextptr;

	while(temp->nextptr != NULL)
	{
		temp = temp->nextptr;
	}	

	head = temp->nextptr;

	return newHead;
}