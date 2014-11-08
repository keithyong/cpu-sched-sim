#include <stdio.h>

#define MAX_INPUT       100
#define MAX_LINES       20
#define QUEUE_ELEMENTS  100
#define QUEUE_SIZE      (QUEUE_ELEMENTS + 1)

int Queue[QUEUE_SIZE];
int queueIn, queueOut;
int enqueue(int new);
int dequeue();
void printQueue(void);

int main(int argc, char *argv[])
{
    /* Arrays that input will go into */
    int input[MAX_LINES][MAX_INPUT];

    /* The CPU bursts and wait time alternation array */
    int cpu[MAX_LINES][MAX_INPUT];

    /* The arrival time for ith process */
    int at[MAX_LINES];

    /* The number of cpu bursts for ith process */
    int numcpu[MAX_LINES];
    
    /* Input block */
    {
        int i, j;
        int x;
        char ch;
        int c = 0;
        int lc = 0;

        /* Convert stdin into various input arrays */
        while (scanf("%d%c", &x, &ch) != EOF){
            input[lc][c] = x;
            c++;
            if (ch == '\n'){
                at[lc] = input[lc][0];
                numcpu[lc] = input[lc][1];
                for (i = 2; i < c; i++)
                    cpu[lc][i - 2] = input[lc][i];
                c = 0;
                lc++;
            }
        }
        printf("\nat:\n");
        for (i = 0; i < 10; i++)
            printf("[%d]", at[i]);

        printf("numcpu:\n");
        for (i = 0; i < 10; i++)
            printf("[%d]", numcpu[i]);

        printf("cpu:\n");
        for (i = 0; i < 10; i++)
            for (j = 0; j < 10; j++)
                printf("[%d]", cpu[i][j]);
            putchar('\n');

        putchar('\n');
    }

    return 0;
}

int enqueue(int new)
{
    if(queueIn == ((queueOut - 1 + QUEUE_SIZE) % QUEUE_SIZE))
        return -1; /* Queue is full */
    
    Queue[queueIn] = new;
    queueIn = (queueIn + 1) % QUEUE_SIZE;

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
