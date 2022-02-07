//Name: Luke Templeton
//Dr. Steinberg
//COP3502C Spring 2022
//Programming Assignment 1

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define LIMIT 20

char ** doubleIt(char **arr, int *maxsize); 
char ** populate(char ** words, FILE *fptr, int *currentsize, int *maxsize);
char* generateSentence(char ** noun, char ** verb, char ** adjective, char ** preposition, char ** article, int nounsize, int verbsize, int adjsize, int prepositionsize, int articlesize); //write sentence to console window
void generateStory(char ** noun, char ** verb, char ** adjective, char ** preposition, char ** article, int nounsize, int verbsize, int adjsize, int prepositionsize, int articlesize, FILE *fptr); //write story to txt file
void displaySentence(char * sentence);
void cleanUp(char ** nouns, char ** verbs, char ** adjectives, char ** prepositions, char ** articles, int nounsize, int verbsize, int adjsize, int prepositionsize, int articlesize);

int main()
{
	//setup randomness
	int x;
	printf("Enter seed: ");
	scanf("%d", &x);
	srand(x); //set seed
	
	//declare necessary variables
	int nounsize = 0;
	int verbsize = 0;
	int adjsize = 0;
	int prepositionsize = 0;
	
	int nounmaxsize = 5;
	int verbmaxsize = 5;
	int adjmaxsize = 5;
	int prepositionmaxsize = 5;
	const int articlemaxsize = 3; //there are only 3 articles in the english language a, an, and the. that is why const is applied
	
	printf("Welcome to the random sentence generator.\n");
	
	//double pointers
	char ** nouns = NULL;
	char ** verbs = NULL;
	char ** adjectives = NULL;
	char ** preposition = NULL;
	char ** articles = NULL;

	nouns = (char **) malloc(sizeof(char *) * nounmaxsize); 	
	verbs = (char **) malloc(sizeof(char *) * verbmaxsize);
	adjectives = (char **) malloc(sizeof(char *) * adjmaxsize); 
	preposition = (char **) malloc(sizeof(char *) * prepositionmaxsize); 
	articles = (char **) malloc(sizeof(char *) * articlemaxsize); 
	
	//make sure malloc was able to allocate memory
	if(nouns == NULL || verbs == NULL || adjectives == NULL || preposition == NULL || articles == NULL) 
	{
		printf("malloc was not successful\n");
		printf("Program will now terminate.\n");
		exit(1);
	}
	
	//populate articles using strcpy. numbers represent how many characters are needed. this includes \0 character
	articles[0] = (char *) malloc(sizeof(char) * 2);
	strcpy(articles[0], "A");
	articles[1] = (char *) malloc(sizeof(char) * 3);
	strcpy(articles[1], "An");
	articles[2] = (char *) malloc(sizeof(char) * 4);
	strcpy(articles[2], "The");
	
	//open the file of words
	FILE *fptr = fopen("nouns.txt", "r");
	FILE *fptr2 = fopen("verbs.txt", "r");
	FILE *fptr3 = fopen("adjectives.txt", "r");
	FILE *fptr4 = fopen("preposition.txt", "r");
	
	//make sure the files were opened properly
	if(fptr == NULL || fptr2 == NULL || fptr3 == NULL || fptr4 == NULL)
	{
		printf("file was not successful in opening.");
		printf("Program will now terminate.\n");
		exit(1);
	}
	
	//populate the dynamic array
	nouns = populate(nouns, fptr, &nounsize, &nounmaxsize); //populate nouns
	verbs = populate(verbs, fptr2, &verbsize, &verbmaxsize); //populate verbs
	adjectives = populate(adjectives, fptr3, &adjsize, &adjmaxsize); //populate adjectives
	preposition = populate(preposition, fptr4, &prepositionsize, &prepositionmaxsize); //populate prepositions
	
	//close the files
	fclose(fptr);
	fclose(fptr2);
	fclose(fptr3);
	fclose(fptr4);
	
	//now lets generate 5 sentences and write them to the console window
	
	printf("Let's generate some random sentences that don't make sense.\n");
	
	for(int x = 0; x < 5; ++x)
	{
		char * sentence = generateSentence(nouns, verbs, adjectives, preposition, articles, nounsize, verbsize, adjsize, prepositionsize, articlemaxsize);
		displaySentence(sentence);
		free(sentence);
	}
	
	printf("Now let's create three stories that just don't make sense.\n");
	fptr = fopen("story1.txt", "w");
	generateStory(nouns, verbs, adjectives, preposition, articles, nounsize, verbsize, adjsize, prepositionsize, articlemaxsize, fptr); //story 1
	fclose(fptr);
	
	fptr = fopen("story2.txt", "w");
	generateStory(nouns, verbs, adjectives, preposition, articles, nounsize, verbsize, adjsize, prepositionsize, articlemaxsize, fptr); //story 2
	fclose(fptr);

	fptr = fopen("story3.txt", "w");
	generateStory(nouns, verbs, adjectives, preposition, articles, nounsize, verbsize, adjsize, prepositionsize, articlemaxsize, fptr); //story 3
	fclose(fptr);

	cleanUp(nouns, verbs, adjectives, preposition, articles, nounmaxsize, verbmaxsize, adjmaxsize, prepositionmaxsize, 3);
	
	printf("The stories were generated successfully and stored in their respective text files.\n");
	printf("Check them out!\n");
	
	return 0;
}

char ** populate(char ** words, FILE *fptr, int *currentsize, int *maxsize)
{
	int status = 0; //status of eof

	for(int i = 0; i < *maxsize; i++) //allocates memory for words maxsize times
	{
		words[i] = (char*) malloc(sizeof(char) * LIMIT);	

		if(words[i] == NULL) //checks for allocation
		{
			printf("malloc was not successful\n");
			printf("Program will now terminate.\n");
			exit(1);
		}	
	}

	int j = 0; //counter

	do
	{
		status = fscanf(fptr, "%s", words[j]); //scans words into array

		if(status != EOF)
		{
			if((*currentsize)+1 == *maxsize) //executes if array limit is reached
			{
				words = doubleIt(words, maxsize); //doubles array length
			}
			j++;
			(*currentsize)++;
		}

	} while(status != EOF); //scans words if not reached eof

	return words;
}

char* generateSentence(char ** noun, char ** verb, char ** adjective, char ** preposition, char ** article, int nounsize, int verbsize, int adjsize, int prepositionsize, int articlesize)
{
	char ** wordsGen = NULL;
	wordsGen = (char **) malloc(sizeof(char *) * 10); //words randomly generated and combined in one array
	char * sentenceBuffer = NULL;
	sentenceBuffer = (char*)malloc(sizeof(char) * 100); //sentence returned

	if(sentenceBuffer == NULL) //malloc check
	{
		printf("malloc was not successful\n");
		printf("Program will now terminate.\n");
		exit(1);
	}

	sentenceBuffer[0] = '\0';

	// looping 1 time for each word in the sentence (max sentence size is 8)
	for(int i = 0; i < 8; ++i)
	{
		wordsGen[i] = (char*) malloc(sizeof(char) * LIMIT);	//allocates memory for each word

		if(wordsGen[i] == NULL) 
		{
			printf("malloc was not successful\n");
			printf("Program will now terminate.\n");
			exit(1);
		}
	}

	//randomly generated words are sotred in wordsGen[i]
	strcpy(wordsGen[0], article[rand() % articlesize]);
	strcpy(wordsGen[1], adjective[rand() % adjsize]);
	strcpy(wordsGen[2], noun[rand() % nounsize]);
	strcpy(wordsGen[3], verb[rand() % verbsize]);
	strcpy(wordsGen[4], preposition[rand() % prepositionsize]);
	strcpy(wordsGen[5], article[rand() % articlesize]);
	strcpy(wordsGen[6], adjective[rand() % adjsize]);
	strcpy(wordsGen[7], noun[rand() % nounsize]);

	//arranges sentence in order with spaces and period
	for(int i = 0; i < 8; i++)
	{
		strcat(sentenceBuffer, wordsGen[i]);
		if (i < 7)
		{
			strcat(sentenceBuffer, " ");
		}
		else
		{
			strcat(sentenceBuffer, ".");
		}
		free(wordsGen[i]);
	}

	free(wordsGen);
	wordsGen = NULL;

	return sentenceBuffer;
} 

void displaySentence(char * sentence)
{
	printf("%s\n", sentence); //displays sentence with new line
}

void generateStory(char ** noun, char ** verb, char ** adjective, char ** preposition, char ** article, int nounsize, int verbsize, int adjsize, int prepositionsize, int articlesize, FILE *fptr)
{
	int loopTimes = (rand() % 401) + 100; //random number between 100-500
	for(int i = 0; i < loopTimes; i++)
	{
		char * storySentence = generateSentence(noun, verb, adjective, preposition, article, nounsize, verbsize, adjsize, prepositionsize, articlesize);
		fprintf(fptr, "%s\n", storySentence); //prints to story file
		free(storySentence);
	}
}

char ** doubleIt(char **arr, int *maxsize)
{
	int oldMaxSize = *maxsize; //keeps track of original max size

	*maxsize = *maxsize * 2;

	char **doubleWords = NULL; //doublewords is the new arr
	doubleWords = (char **) malloc(sizeof(char*) * *maxsize);

	for(int i = 0; i < *maxsize; i++)
	{
		if (i < oldMaxSize) //copies everything in from old array
		{
			doubleWords[i] = arr[i];
		}
		else //allocates memory for new words
		{
			doubleWords[i] = (char*) malloc(sizeof(char) * LIMIT);
			*doubleWords[i] = '\0'; //sets newly allocated memory for words
		}
	}

	free(arr);

	return doubleWords;
}

void cleanUp(char ** nouns, char ** verbs, char ** adjectives, char ** prepositions, char ** articles, int nounsize, int verbsize, int adjsize, int prepositionsize, int articlesize)
{
	for(int i = 0; i < nounsize; i++)
	{
		free(nouns[i]); //frees the individual nouns
	}
	free(nouns); //frees nouns arr

	for(int i = 0; i < verbsize; i++)
	{
		free(verbs[i]);
	}
	free(verbs);

	for(int i = 0; i < adjsize; i++)
	{
		free(adjectives[i]);
	}
	free(adjectives);

	for(int i = 0; i < prepositionsize; i++)
	{
		free(prepositions[i]);
	}
	free(prepositions);

	for(int i = 0; i < articlesize; i++)
	{
		free(articles[i]);
	}
	free(articles);

	return;
}