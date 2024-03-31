#include"readfile.h"
#include"paras.h"
#include"list.h"
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
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
/*
struct Params{
    char _tag[3]; //卫星号int _year;    //日期int _month;int _day;int _hour;int _minutes;int _second;double _S;double _S_S;
    double _S_SS;double _data_valid_time;double _Crs;double _c_n;double _M0toe;double _CUc;double _e;double _Cus;double _sqrt_A;
    double _toe;double _CIc;double _Cic;double _Omu;double _Cis;double _I;double _Crc;double _w;double _Odot;double _Idot;
    double _L2;double _GPS_week_num;
    double _L2P;double _m;double _MSB;double _TOE;double _TGD;double _IODC;double _MSG_SEND_TIME;double _EMPTY;
};
*/
Params*  readData(size_t *psize){
    FILE * pf = fopen("./data.rnx","r");
    if(pf==NULL){
        perror("fopen error");
        exit(1);
    }
    Params p;    //12
    List l ;
    initList(&l);
    while(fscanf(pf,"%s %d%d%d%d%d%d%lf%lf%lf\n%lf%lf%lf%lf\n%lf%lf%lf%lf\n%lf%lf%lf%lf\n%lf%lf%lf%lf\n%lf%lf%lf%lf\n%lf%lf%lf%lf\n%lf%lf%lf%lf",\
    p._tag,\
    &p._t0c_year,\
    &p._t0c_month,\
    &p._t0c_day,\
    &p._t0c_hour,\
    &p._t0c_minutes,\
    &p._t0c_second,\
    &p._af0,\
    &p._af1,\
    &p._af2,\
    &p._data_valid_time,\
    &p._Crs,\
    &p._deltan,\
    &p._M0,\
    &p._Cuc,\
    &p._e,\
    &p._Cus,\
    &p._sqrtA,\
    &p._toe,\
    &p._Cic,\
    &p._omega0,\
    &p._Cis,\
    &p._I0,\
    &p._Crc,\
    &p._omega,\
    &p._omegaDot,\
    &p._IDot,\
    &p._Idot,\
    &p._L2,\
    &p._GPS_week_num,\
    &p._L2P,\
    &p._m,\
    &p._TGD1,\
    &p._TOE,\
    &p._TGD,\
    &p._IODC,\
    &p._MSG_SEND_TIME,\
    &p._EMPTY)!=EOF){
        insert(&l,*psize,p);
        (*psize)++;
    }
    fclose(pf);
    return get(&l);
}

Params* splitData(Params* params,size_t* size)
{
    List l;
    initList(&l);
    int index = 0;
    for(int i = 0 ; i < *size ; i++)
    {
        if(params[i]._tag[0]=='C'){
            insert(&l,index,params[i]);
            index++;
        }
    }
    *size=(size_t)index;
    return get(&l);
}

void write3DCoord(Coord_3D* process_data,size_t size)
{
    FILE*pf=fopen("3d_data","w+");
    if(pf==NULL){
        perror("fopen error");
        exit(3);
    }
    for(size_t i = 0 ; i<size; i++)
    {
        fprintf(pf,"%lf %lf %lf %lf %lf %lf %lf\n",process_data[i]._X,process_data[i]._Y,process_data[i]._Z,process_data[i]._x,process_data[i]._y,process_data[i]._azimuth,process_data[i]._elevation);
    }
    fclose(pf);
}