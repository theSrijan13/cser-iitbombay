#include<windows.h>
#include<stdio.h>
#include<time.h>
#include<process.h>

void task1(void*)
{
    while(1)
    {
        clock_t= wakeup+50;
        while(clock ()< wakeup){}
        sleep(50);
    }

}

int main(int, char**)
{
    int ThreadNr;
    for(int i=0;i<4;i++)_beginthread(task1,0,&ThreadNr);
    (void) getchar();
    return 0;
}