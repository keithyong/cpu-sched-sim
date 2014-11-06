#include <stdio.h>

#define MAXINPUT        1000

int main(int argc, char *argv[])
{
    int x;
    int count = 0;
    int input[MAXINPUT];
    char c;
    while (scanf("%d%c", &x, &c) != EOF){
        input[count] = x;
        count++;
    }
    printf("%d", input[1]);
    return 0;
}
