/***********************************************
 * 
 * 上善若水
 * 
 ***********************************************/
#include "water.h"
#include "contrl.h"

void cube_water1(int tv)
{
  unsigned char cube[8][8] = {
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}
    };

  unsigned int x,y,z;
  int times = tv;
  for(z = 0; z < 8; z++) {
    cube[z][3] = 0x08;
    while(times--){
      for(y = 0; y < 8; y++){
        for(x = 0; x < 8; x++){
          hc595(cube[y][x]);
        }
        hc595out();
        cen_on(y);
        usleep(200);
        cen_on(8);
        usleep(100);
      }
    }
    cube[z][3] = 0x00;
    if(z < 7)
      cube[z + 1][3] = 0x08;
    times = tv;
  }

  for(z = 0; z < 5; z++){
    switch(z){
      case 0: cube[7][4] = 0x08;
              cube[7][3] = 0x14;
              cube[7][2] = 0x08;
      break;
      case 1: cube[7][5] = 0x08;
              cube[7][4] = 0x14;
              cube[7][3] = 0x22;
              cube[7][2] = 0x14;
              cube[7][1] = 0x08;
      break;
      case 2: cube[7][7] = 0x00;
              cube[7][6] = 0x08;
              cube[7][5] = 0x22;
              cube[7][4] = 0x00;
              cube[7][3] = 0x41;
              cube[7][2] = 0x00;
              cube[7][1] = 0x22;
              cube[7][0] = 0x08;
      break;
      case 3: cube[7][7] = 0x08;
              cube[7][6] = 0x41;
              cube[7][5] = 0x00;
              cube[7][4] = 0x00;
              cube[7][3] = 0x80;
              cube[7][2] = 0x00;
              cube[7][1] = 0x00;
              cube[7][0] = 0x41;
      break;
      case 4: cube[7][7] = 0x80;
              cube[7][6] = 0x00;
              cube[7][5] = 0x00;
              cube[7][4] = 0x00;
              cube[7][3] = 0x00;
              cube[7][2] = 0x00;
              cube[7][1] = 0x00;
              cube[7][0] = 0x00;
      break;
    }
    
    while(times--){
      for(y = 0; y < 8; y++){
        for(x = 0; x < 8; x++){
          hc595(cube[y][x]);
        }
        hc595out();
        cen_on(y);
        usleep(200);
        cen_on(8);
        usleep(100);
      }  
    }
    times = tv;
  }
}

void cube_water2(int tv)
{
  unsigned char cube[8][8] = {
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}
    };

  unsigned int x,y,z;
  int times = tv;
  for(z = 0; z < 8; z++) {
    cube[z][3] = 0x08;
    cube[z][4] = 0x20;
    while(times--){
      for(y = 0; y < 8; y++){
        for(x = 0; x < 8; x++){
          hc595(cube[y][x]);
        }
        hc595out();
        cen_on(y);
        usleep(200);
        cen_on(8);
        usleep(100);
      }
    }
    cube[z][3] = 0x00;
    cube[z][4] = 0x00;
    if(z < 7){
      cube[z + 1][3] = 0x08;
      cube[z + 1][4] = 0x20;
    }
    times = tv;
  }

  for(z = 0; z < 5; z++){
    switch(z){
      case 0: cube[7][7] = 0x00;
              cube[7][6] = 0x00;
              cube[7][5] = 0x00;
              cube[7][6] = 0x20;
              cube[7][3] = 0x58;
              cube[7][2] = 0x32;
              cube[7][1] = 0x08;
              cube[7][0] = 0x00;
      break;
      case 1: cube[7][7] = 0x00;
              cube[7][6] = 0x00;
              cube[7][5] = 0x20;
              cube[7][4] = 0x58;
              cube[7][3] = 0x22;
              cube[7][2] = 0x9c;
              cube[7][1] = 0x34;
              cube[7][0] = 0x80;
      break;
      case 2: cube[7][7] = 0x00;
              cube[7][6] = 0x20;
              cube[7][5] = 0x88;
              cube[7][4] = 0x22;
              cube[7][3] = 0x04;
              cube[7][2] = 0x41;
              cube[7][1] = 0x88;
              cube[7][0] = 0x22;
      break;
      case 3: cube[7][7] = 0x20;
              cube[7][6] = 0x0c;
              cube[7][5] = 0x41;
              cube[7][4] = 0x00;
              cube[7][3] = 0x02;
              cube[7][2] = 0x80;
              cube[7][1] = 0x00;
              cube[7][0] = 0x04;
      break;
      case 4: cube[7][7] = 0xa0;
              cube[7][6] = 0x80;
              cube[7][5] = 0x00;
              cube[7][4] = 0x00;
              cube[7][3] = 0x01;
              cube[7][2] = 0x00;
              cube[7][1] = 0x00;
              cube[7][0] = 0x00;
      break;
    }
    
    while(times--){
      for(y = 0; y < 8; y++){
        for(x = 0; x < 8; x++){
          hc595(cube[y][x]);
        }
        hc595out();
        cen_on(y);
        usleep(200);
        cen_on(8);
        usleep(100);
      }  
    }
    times = tv;
  }
}

void rain_cube(int tv)
{
  unsigned char rain[8][8] = {
    {0X00,0X84,0X01,0X00,0X00,0X01,0X02,0X44},
    {0X84,0X00,0X24,0X08,0X04,0X20,0X80,0X00},
    {0X00,0X20,0X00,0X00,0X20,0X00,0X40,0X00},
    {0X00,0X04,0X00,0X40,0X00,0X00,0X08,0X00},
    {0X40,0X00,0X10,0X00,0X00,0X04,0X04,0X11},
    {0X00,0X41,0X00,0X00,0X80,0X00,0X40,0X00},
    {0X00,0X00,0X00,0X24,0X01,0X18,0X00,0X00},
    {0X22,0X10,0X02,0X00,0X40,0X00,0X02,0X00}
    };

  unsigned int x,y,z;
  int times = tv;
  int ctl = 0;
  for(z = 0; z <7; z++){
    while(times--){
      for(y = 0; y < 8; y++) {
        for(x = 0; x < 8; x++) {
          ctl = 8 - z + y;
          if(ctl <= 7)
            hc595(rain[ctl][x]);
          else{
            hc595(rain[ctl - 8][x]);
          }
          
        }
        hc595out();
        cen_on(y);
        usleep(200);
        cen_on(8);
        usleep(100);
      }  
    }
    times = tv;
  }
}
