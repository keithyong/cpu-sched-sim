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

int findIndexWithMin(int * arr, int n);
void fcfs();

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

int main(int argc, char *argv[])
{
    /* Used for for loops */
    int i, j;

    /* Input block */
    {
        int x;
        char ch;
        int c = 0;
        int lc = 0;

        /* Convert stdin into various input arrays */
        while (scanf("%d%c", &x, &ch) != EOF){
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
        printf("\nat:\n");
        for (i = 0; i < numProcesses; i++)
            printf("[%d]", at[i]);

        printf("\n\nnumcpu:\n");
        for (i = 0; i < numProcesses; i++)
            printf("[%d]", numcpu[i]);

        printf("\n\ncpu:\n");
        for (i = 0; i < numProcesses; i++){
            for (j = 0; j < (numcpu[i] * 2); j++)
                printf("[%d]", cpu[i][j]);
            putchar('\n');
        }

        printf("\nnumProcesses = %d\n", numProcesses);

        putchar('\n');
    }
    /* End of input block */
    
    fcfs();
    return 0;
}

void fcfs()
{
    /* Find the first process to arrive */
    int firstArrivedProcess = findIndexWithMin(at, numProcesses);

    /* First process is actually 0, not 1 */
    printf("First arrived process found: P%d\n", firstArrivedProcess + 1);
    enqueue(firstArrivedProcess);
    while (isQueueEmpty() != 1)
    {

    }
    printQueue();
}

int enqueue(int new)
{
    if(queueIn == ((queueOut - 1 + QUEUE_SIZE) % QUEUE_SIZE))
        return -1; /* Queue is full */
    
    Queue[queueIn] = new;
    queueIn = (queueIn + 1) % QUEUE_SIZE;

    printf("Enqueued P%d\n", new + 1);
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

int isQueueEmpty(){
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

int findIndexWithMin(int * arr, int len){
    int i;
    int min = MAXINT;
    int minIndex = MAXINT;
    for (i = 0; i < len; i++){
        if (arr[i] < min){
            min = arr[i];
            minIndex = i;
        }
    }

    return minIndex;
}
