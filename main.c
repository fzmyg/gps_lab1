#include"readfile.h"
#include"GPSalgo.h"
#include"paras.h"
#include<stdio.h>
#include<errno.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>
/*
G01 2020 01 01 02 00 00-2.479893155396e-04-1.216449163621e-11 0.000000000000e+00
     7.300000000000e+01-2.093750000000e+01 4.376610874839e-09-4.335905824796e-01
    -1.069158315659e-06 9.235757635906e-03 3.784894943237e-06 5.153638858795e+03
     2.664000000000e+05 6.332993507385e-08-5.787103223850e-01 1.303851604462e-07
     9.785088475069e-01 3.136562500000e+02 7.594576020004e-01-8.299274269514e-09
    -5.857386840816e-11 1.000000000000e+00 2.086000000000e+03 0.000000000000e+00
     2.000000000000e+00 0.000000000000e+00 5.587935447693e-09 7.300000000000e+01
     2.592180000000e+05 4.000000000000e+00 
*/
void exeDraw(void)
{
    pid_t pid =  fork();
    if(pid == 0){
        execl("/usr/bin/python3","zbccc","draw.py",NULL);
    }else{
        int stat;
        wait(&stat);
    }
}
int main()
{
    size_t size=0;
    Params*params = readData(&size);
    Params*pdata=splitData(params,&size);
    Coord_3D*process_data = handleData(pdata,size);
    write3DCoord(process_data,size);
    exeDraw();
    printf("successful\n");
    return 0;    
}
