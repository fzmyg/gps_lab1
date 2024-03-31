#include"GPSalgo.h"
#include<math.h>
#include<stdlib.h>

/*     3.04           N: GNSS NAV DATA    M (MIXED)           RINEX VERSION / TYPE
G-Nut/Aset_2.0.5    GOP/RIGTC           20240220 224611 UTC PGM / RUN BY / DATE 
Multi-GNSS (GPS/GLO/GAL/BDS/QZS/SBS/IRN) navigation data    COMMENT             
merged from all available IGS-MGEX (and other) files        COMMENT             
Contact: gnss@pecny.cz                                      COMMENT             
GAL    1.4025e+02 -1.8750e-01  1.4099e-02  0.0000e+00       IONOSPHERIC CORR    
GPSA   2.3283e-08  0.0000e+00 -1.1921e-07  1.1921e-07       IONOSPHERIC CORR    
GPSB   1.3517e+05 -8.1920e+04  6.5536e+04 -4.5875e+05       IONOSPHERIC CORR    
QZSA   2.7940e-08 -7.4510e-09 -3.5760e-07 -5.3640e-07       IONOSPHERIC CORR    
QZSB   1.1260e+05 -1.6380e+05  3.9320e+05  5.8980e+05       IONOSPHERIC CORR    
BDSA   4.0047e-08  2.6077e-07 -2.0862e-06  2.9802e-06       IONOSPHERIC CORR    
BDSB   1.1878e+05 -2.2938e+05  1.0486e+06 -1.9661e+05       IONOSPHERIC CORR    
IRNA   8.1025e-08  5.7369e-07 -2.9206e-06 -7.5102e-06       IONOSPHERIC CORR    
IRNB   1.4746e+05  5.4067e+05 -4.5875e+05 -8.3231e+06       IONOSPHERIC CORR    
XXXX -2.9976945370e-09-1.154631946e-14 518400 2301          TIME SYSTEM CORR    
GPUT  1.8626451492e-09-2.664535259e-15 147456 2302          TIME SYSTEM CORR    
GLUT -8.3819031715e-09 0.000000000e+00      0    0          TIME SYSTEM CORR    
GAUT  1.8626451492e-09-8.881784197e-16 518400 2301          TIME SYSTEM CORR    
BDUT  0.0000000000e+00 0.000000000e+00 517985  945          TIME SYSTEM CORR    
QZUT  4.6566128730e-09 0.000000000e+00  94208 2302          TIME SYSTEM CORR    
IRUT  2.7648638934e-09-3.996802889e-15 517200 1277          TIME SYSTEM CORR    
GLGP -1.0782000000e+04 0.000000000e+00 515718 2301          TIME SYSTEM CORR    
GAGP -2.9976945370e-09-1.154631946e-14 518400 2301          TIME SYSTEM CORR    
IRGP -1.1641532183e-09 4.884981308e-15 603600 2301          TIME SYSTEM CORR    
IRGL  2.7765054256e-08 5.195843755e-14 603600 2301          TIME SYSTEM CORR    
    18                                                      LEAP SECONDS        
                                                             END OF HEADER     */
static double getE(Params*p,double M,double Ei_1)
{
    
    double Ei = M + sin(Ei_1) * p->_e;
    if(abs(Ei-Ei_1)<1e-12)
        return Ei;
    else{
        return getE(p,M,Ei);
    }
}
static double transfomeBDS(Params*p)
{
    double time = 6*24*3600+p->_t0c_hour*3600+p->_t0c_minutes*60+p->_t0c_second;
    return time;
}

static void handleEachData(Params*P,Coord_3D*ans)
{
    double tk = transfomeBDS(P) - P->_toe;
    tk = 0 ;
    double a = P->_sqrtA * P->_sqrtA;
    double n = sqrt(GM/pow(a,3))+P->_deltan;
    double M = P->_M0+n*tk;
    /*En = M + e sin(E(n-1))*/
    double E = getE(P,M,M);
    double nu = atan2(sqrt(1-pow(P->_e,2))*sin(E),(cos(E)-P->_e));
    double phi = nu + P -> _omega;
    double delta_u = P->_Cuc * cos(2*phi)+P->_Cus*sin(2*phi);
    double delta_r = P->_Crc * cos(2*phi)+P->_Crs*sin(2*phi);
    double delta_i = P->_Cic * cos(2*phi)+P->_Cis*sin(2*phi);
    double u = phi + delta_u;
    double r = a * (1-P->_e*cos(E))+delta_r;
    double i = P->_I0 + P->_IDot*tk +delta_i;
    double omega = P->_omega0+P->_omegaDot*tk-radv*P->_toe;
    double X = cos(u)*r*cos(omega)-sin(u)*r*cos(i)*sin(omega);
    double Y = cos(u)*r*sin(omega)+sin(u)*r*cos(i)*cos(omega);
    double Z = sin(u)*r*sin(i);
    double f = (-5)/180*pi;
    double p = radv*tk;
    double GEO_X = cos(p)*X+sin(p)*cos(f)*Y+sin(p)*sin(f)*Z;
    double GEO_Y = -1*sin(p)*X+cos(f)*cos(p)*Y+cos(p)*sin(f)*Z;
    double GEO_Z = -1*sin(f)*Y+cos(f)*Z;
    ans->_X = GEO_X;
    ans->_Y = GEO_Y;
    ans->_Z = GEO_Z;
    ans->_x = cos(u)*r;
    ans->_y = sin(u)*r;
    double X_receiver = 4382846.71;  
    double Y_receiver = 4796729.78;
    double Z_receiver = 4061245.77;
    double delta_X = GEO_X - X_receiver;
    double delta_Y = GEO_Y - Y_receiver;
    double delta_Z = GEO_Z - Z_receiver;
    double azimuth = atan2(delta_X,delta_Y);
    double horizontal_projection_length = sqrt(delta_X*delta_X + delta_Y*delta_Y);
    double elevation = atan2(delta_Z, horizontal_projection_length);
    ans->_azimuth = azimuth;
    ans->_elevation = elevation;
}
Coord_3D* handleData(Params*vpara,size_t size)
{
    Coord_3D* ans = malloc(sizeof(Coord_3D)*size);
    if(ans == NULL){
        exit(2);
    }
    for(int i = 0; i<size ;i++){
        handleEachData(vpara+i,ans+i);
    } 
    return ans;  
}


/*  # 接收器的地心坐标（以北京市为准）
            X_receiver = 4382846.71  # 接收器的 X 坐标
            Y_receiver = 4796729.78  # 接收器的 Y 坐标
            Z_receiver = 4061245.77  # 接收器的 Z 坐标

            # 计算方向向量
            direction_vector = np.array([Xk - X_receiver,
                                        Yk - Y_receiver,
                                        Zk - Z_receiver])

            # 计算方向向量与地心正北方向之间的夹角（方位角）
            azimuth = np.arctan2(direction_vector[0], direction_vector[1])

            # 计算方向向量的水平投影长度
            horizontal_projection_length = np.sqrt(direction_vector[0] ** 2 + direction_vector[1] ** 2)

            # 计算方向向量与地心到接收器的向量之间的夹角（高度角）
            elevation = np.arctan2(direction_vector[2], horizontal_projection_length)

            # 将弧度转换为度
            azimuth_deg = np.degrees(azimuth)
            elevation_deg = np.degrees(elevation)
*/