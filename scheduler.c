#include <stdio.h>

#define MAX_INPUT       100
#define MAX_LINES       20
#define QUEUE_ELEMENTS  100
#define QUEUE_SIZE      (QUEUE_ELEMENTS + 1)
#define MAXINT          32767

/* Queue stuff */
int Queue[QUEUE_SIZE];
int queueIn, queueOut;
int enqueue(int new);
int dequeue(int * old);
int isQueueEmpty();
void printQueue(void);

void initArrays();
void printArray(int * arr, int len);
int findIndexWithMin(int * arr, int n);
void fcfs();

void initfcfs();
void startContextSwitch();

/* Used for for loops */
int i, j;

/* Quantum, and context switch cost */
int quantum, context;

/* Scheduling algorithm */
char * schedAlgo;

/* Arrays that input will go into */
int input[MAX_LINES][MAX_INPUT];

/* The CPU bursts and wait time alternation array */
int cpu[MAX_LINES][MAX_INPUT];

/* The arrival time for ith process */
int at[MAX_LINES];

/* The number of cpu bursts for ith process */
int numcpu[MAX_LINES];

/* Number of processes that was found */
int numProcesses;

/* At process [i], stores which state that the process
 * is in, so if process[i] has completed one cpu burst
 * then it is in state++ */
int processState[MAX_LINES];

/* When a process is done running, update waitUntil 
 * to represent when the process will start running
 * again. */
int waitUntil[MAX_LINES];

/* Running time of process */
int runningTime[MAX_LINES][MAX_INPUT];

/* FCFS variables */
/* CPU State. 0 = free, 1 = context switching, 2 = running process */
int CPUstate;

int time;
int done;

/* Current running process */
int deq;

/* Return value from dequeue() */
int dequeueRet;
int runUntil;

/* If allDoneSum = numProcesses, then all processes are done running */
int allDoneSum;

int freshProcess;
int processWantsToRun;

int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        printf("Usage: <input file> <scheduling algorithm> <scheduling parameters>\n");
        printf("Supported schedulers:\n1) FCFS - Parameters: cost of half C.S.\n2) SJNFP - Parameters: Cost of half C.S.\n");
    }
    else
    {
        int x;
        char ch;
        int c = 0;
        int lc = 0;
        FILE *fp;

        if ((fp = fopen(argv[1], "r")) == NULL)
        {
            printf("File not found!\n");
            return 0;
        }

        /* TODO: Algorithm picking logic */
        schedAlgo = argv[2];
        sscanf(argv[3], "%d", &quantum);
        sscanf(argv[4], "%d", &context);
        printf("schedAlgo = %s, Quantum = %d, context = %d\n", schedAlgo, quantum, context);

        /* Convert stdin into various input arrays */
        while (fscanf(fp, "%d%c", &x, &ch) != EOF){
            input[lc][c] = x;
            c++;
            if (ch == '\n' && ((ch + 1) != EOF)){
                at[lc] = input[lc][0];
                numcpu[lc] = input[lc][1];
                for (i = 2; i < c; i++)
                    cpu[lc][i - 2] = input[lc][i];
                c = 0;
                lc++;
            }
        }
        numProcesses = lc;
        printf("at:\n");
        for (i = 0; i < numProcesses; i++)
            printf("[%d]\n", at[i]);

        printf("\n\nnumcpu:\n");
        for (i = 0; i < numProcesses; i++)
            printf("[%d]\n", numcpu[i]);

        printf("\n\ncpu:\n");
        for (i = 0; i < numProcesses; i++){
            for (j = 0; j < (numcpu[i] * 2 - 1); j++)
                printf("[%d]", cpu[i][j]);
            putchar('\n');
        }

        printf("\nnumProcesses = %d\n", numProcesses);

        putchar('\n');

        /* Fills some arrays starting numbers*/
        initArrays();
        
        /* Do fcfs. TODO: Handle user shell input */
        fcfs();
    }
    return 0;
}

void fcfs()
{
    printf("--------------------------\n");
    printf("- First Come First Serve -\n");
    printf("--------------------------\n");

    while (!done)
    {
        time++;
        printf("* Time: %d, ", time);

        switch (CPUstate) 
        {
            case 0:
                printf("CPU is free\n");
                break;
            case 1:
                printf("CPU is busy context switching\n");
                break;
            case 2:
                printf("CPU is running a process\n");
                break;
        }

        if (time == runUntil) 
        {
            /* If the context switching is finished */
            if ( CPUstate == 1 )
            {
                /* Then the CPU becomes free */
                CPUstate = 0;
            }
            /* If CPU was running a process when we reached this part */
            else if ( CPUstate == 2 )
            {
                /* Then we know that a process must have finished its burst */
                processState[deq]++;

                /* If the process burst i is still less than
                 * total number of bursts, put in waiting */
                if (processState[deq] < numcpu[deq])
                {
                    waitUntil[deq] = time + cpu[deq][processState[deq]];
                    printf("P%d waiting for IO until %d\n", deq + 1, waitUntil[deq]);
                    /* Of course, pay the context switching cost! */
                    printf("Putting process back into wait mode, ");
                    startContextSwitch();
                }
                else {
                    printf("P%d done running all its CPU bursts.\n", deq + 1);
                    CPUstate = 0;
                    allDoneSum++;

                    /* Check if all processes are done running */
                    if (allDoneSum == numProcesses){
                        done = 1;
                        printf("All processes are done running.\n");
                    }
                }
                /* Enqueue if the process has more to go */
            }
        }


        /* For all processes, check if any of them have arrived, or 
         * have finished waiting.*/
        for (i = 0; i < numProcesses; i++)
        {
            /* If Pi has arrived, put in ready queue */ 
            if (time == at[i])
            {
                printf("P%d has arrived\n", i + 1);
                enqueue(i);
            }
            /* If Pi is done waiting, put in ready queue but also do context switching */
            if (time == waitUntil[i])
            {
                processState[i]++;
                printf("P%d has completed IO\n", i + 1);
                enqueue(i);
                printf("Putting process back into ready queue, so ");
                startContextSwitch();
            }
        }

        /* If the CPU is free */
        if ( CPUstate == 0 )
        {
            /* Attempt to dequeue from the ready queue */
            dequeueRet = dequeue(&deq);

            /* If dequeue was sucessful then we found a process off the rdyqueue to run */
            if (dequeueRet == 0)
            {
                /* If the process we dequeued is a fresh new process 
                 * we ignore the context switch cost */
                if (processState[deq] == 0)
                {
                    freshProcess = 1;
                }
                else
                {
                    processWantsToRun = 1;
                }
            }
            else if (allDoneSum == numProcesses)
            {
                done = 1;
                printf("All processes are done running.\n");
            }
        }

        if (( CPUstate == 0 ) && (( processWantsToRun == 1 ) || ( freshProcess == 1 )))
        {
            processWantsToRun = 0;
            freshProcess = 0;
            runUntil = time + cpu[deq][processState[deq]];
            printf("P%d is running until %d\n", deq + 1, runUntil);
            CPUstate = 2;
        }
    }
}
void initfcfs()
{
    CPUstate = 0;
    time = -1;
    done = 0;
    runUntil = -1;
    allDoneSum = 0;
    freshProcess = 0;
}

void startContextSwitch()
{
    /* If the CPU is currently context switching */
    if (CPUstate == 1)
        runUntil = runUntil + context;
    else if (CPUstate == 2)
    {
        if (time == runUntil)
        {
            CPUstate = 1;
            runUntil = time + context;
        }
    }
    else
    {
        CPUstate = 1;
        runUntil = time + context;
    }
    printf("context switching until %d\n", runUntil);
}

int enqueue(int new)
{
    if(queueIn == ((queueOut - 1 + QUEUE_SIZE) % QUEUE_SIZE))
        return -1; /* Queue is full */
    
    Queue[queueIn] = new;
    queueIn = (queueIn + 1) % QUEUE_SIZE;

    printf("P%d put in ready queue\n", new + 1);
    return 0; /* Successfully enqueued */
}

int dequeue(int * old)
{
    if (queueIn == queueOut)
        return -1; /* Queue is empty - nothing to get */

    *old = Queue[queueOut];
    queueOut = (queueOut + 1) % QUEUE_SIZE;

    return 0; /* Successfully dequeued */
}

int isQueueEmpty()
{
    if (queueIn == queueOut)
        return 1;

    return 0;
}

void printQueue(void)
{
    printf("Queue: ");
    int i;
    for(i = queueOut; i <= queueIn; i++)
    {
        if(Queue[i] != '\0')
        printf("[%d]", Queue[i]);
    }
    putchar('\n');
}

void initArrays()
{
    for (i = 0; i < MAX_LINES; i++)
    {
        processState[i] = 0;
        waitUntil[i] = -1;
    }
}
/* Given an array of integers and length, returns the
 * index that has the smallest integer in that array. */
int findIndexWithMin(int * arr, int len)
{
    int i;
    int min = MAXINT;
    int minIndex = MAXINT;
    for (i = 0; i < len; i++)
    {
        if (arr[i] < min)
        {
            min = arr[i];
            minIndex = i;
        }
    }

    return minIndex;
}

void printArray(int * arr, int len)
{
    int i;
    for (i = 0; i < len; i++)
        printf("[%d]", arr[i]);
    putchar('\n');
}
