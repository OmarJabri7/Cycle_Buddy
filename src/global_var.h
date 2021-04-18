#ifndef __GLOBAL_VAR_h
#define __GLOBAL_VAR_H


extern char hostIp[MAXLINE];
/** Set old distance to calculate the derivative and get velocity */
extern volatile double old_distance ;
extern volatile int upcoming_car;
extern volatile int bike_flag;

struct conditions {
  double car_distance;
  double car_velocity;
  double bike_velocity;
};

extern volatile conditions conds;
#endif