#ifndef __READFILE_H__
#define __READFILE_H__
#include"paras.h"
#include"GPSalgo.h"
extern Params* readData(size_t*);
extern Params* splitData(Params* params,size_t* size);
extern void write3DCoord(Coord_3D* process_data,size_t size);
#endif