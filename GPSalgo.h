#ifndef __GPSALGO_H__
#define __GPSALGO_H__
#include"paras.h"
    typedef struct {
        double _X,_Y,_Z,_x,_y;
        double _azimuth,_elevation;
    }Coord_3D;
    extern Coord_3D* handleData(Params*,size_t size);
#endif