/**********************************************************************
 * Student 1 Last Name:     Woods-Smith
 * Student 1 First Name:    Zachary
 * Student 1 ID:            30086474
 * Student 2 Last Name:     Kaur
 * Student 2 First Name:    Amandeep
 * Student 2 ID:            30153923
 * Course:        CPSC 359
 * Assignment:    2
 * 
 * File Name:     race.c
 * 
 * 
 * This program simulates the game hiking race. 
 * The main thread waits for all the hikers to reach the mountain top.
 * All hikers carry a flag which can be mistakenly dropped. On dropping 
 * the hiker goes back to the position where flag was dropped. The race 
 * process is simulated by sleeping. The hikers are ranked as per their progress.
 * 
 **********************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>

#define RANDOM_NUMBER 100


struct shared
{
    int race[RANDOM_NUMBER];        //An array of integers keeping track of race progress.
    int flagDropped[RANDOM_NUMBER]; //An array keeping the track of flag drop.
    bool raceFinished;              //Signals the end of the race.
    int rank[RANDOM_NUMBER];        //An array recording the rank of a hiker in the race.
    int numberOfHikers;             //Count of the hikers.
    int currentIndexRank;           //Index for rank.
};

//struct sharing hiker_data;

struct shared hikers;
int finishLine;

void * raceEachOther(void * hikerNumber)
{
    //int thisHiker = atoi(hikerNumber); 
    int thisHiker = (int *)hikerNumber; 
    // printf("This hiker # is: %d\n", thisHiker);
    // for (int i = 0; i < hikers.numberOfHikers; i ++ )
    // {
    //     printf("\nThread #%d. Current Position %d, flagDropped = %d\n",thisHiker, hikers.race[i], hikers.flagDropped[i]);
    // }

    while(hikers.race[thisHiker] < finishLine)
    {
        printf("\nThread %d)  Current Position : %d, flagDropped : %d\n",thisHiker, hikers.race[thisHiker], hikers.flagDropped[thisHiker]);
        sleep(rand()%5+1); 
        int coin = rand()%2;
        if (coin == 0 && hikers.flagDropped[thisHiker] == -1) //they cant drop it a second time if they haven't even noticed the first drop right? 
        {
            //case hiker drops their flag at current location
            hikers.flagDropped[thisHiker] = hikers.race[thisHiker];
        }
        //second coin flip for whether or not hiker notices they dropped the flag
        int coin2 = rand()%2;
        if (coin2 == 0 && hikers.flagDropped[thisHiker] != -1)
        {
            hikers.race[thisHiker] = hikers.flagDropped[thisHiker];
            hikers.flagDropped[thisHiker] = -1;
        }

        hikers.race[thisHiker]++;

        if(hikers.race[thisHiker] >= finishLine && hikers.flagDropped[thisHiker] == -1)
        {
            printf("\nThis hiker is %d\n", thisHiker);
            hikers.rank[hikers.currentIndexRank] = thisHiker; 
            hikers.currentIndexRank++; 
            }
        }

    }





int main()
{
    srand(time(NULL));
    hikers.raceFinished = false; 
    int max = 0;

    finishLine = rand()%10+1;
    //finishLine = 7;


    int hikerNumber;
    hikers.currentIndexRank = 0; 

    hikerNumber = 3;
    hikers.numberOfHikers = hikerNumber;
    pthread_t threads[hikerNumber];

    //Create variable number of threads 
    for(int i = 0; i < hikerNumber; i++)
    {
        //all hikers should have 0 movement at start of race
        hikers.race[i] = 0; 
        //all hikers should start with flag in hand (i.e., not dropped)
        hikers.flagDropped[i] = -1; 
        //initialize position in race as -1 (to not have any issues of course)
        hikers.rank[i] = -1; 
        //initialize all dropped positions to -1 to ensure no issues happen here if it thinks we dropped at start of course. 
        //hikers.flagDroppedPosition[i] = -1; 
        pthread_create(&threads[i], NULL, raceEachOther , (void *)i);
    }

    //Joining the threads.
    for(int i = 0; i< hikerNumber; i++)
    {
        pthread_join(threads[i], NULL);
    }

    //Printing the Hiker 
    printf("\nHiker Rankings\n");

    for(int i = 0 ; i < hikerNumber; i++)
    {
        printf("Rank:   %d is Hiker:   %d\n",i,hikers.rank[i]);
    }
    return 0; 
}