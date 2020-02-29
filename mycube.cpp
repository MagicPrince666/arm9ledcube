#include "mycube.h"
#include "contrl.h"

void mycube(int tv)
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
  int layer = 0;
  int magic = 0;

  for(z = 0; z < 13; z++){
    if(z < 6) magic = z;
    else magic = 12 - z;
    switch(magic){
      case 0: cube[layer][7] = 0x00;
              cube[layer][6] = 0x00;
              cube[layer][5] = 0x00;
              cube[layer][4] = 0x00;
              cube[layer][3] = 0x08;
              cube[layer][2] = 0x00;
              cube[layer][1] = 0x00;
              cube[layer][0] = 0x00;
      break;
      case 1: cube[layer][7] = 0x00;
              cube[layer][6] = 0x00;
              cube[layer][5] = 0x00;
              cube[layer][4] = 0x08;
              cube[layer][3] = 0x14;
              cube[layer][2] = 0x08;
              cube[layer][1] = 0x00;
              cube[layer][0] = 0x00;
      break;
      case 2: cube[layer][6] = 0x00;
              cube[layer][5] = 0x08;
              cube[layer][4] = 0x14;
              cube[layer][3] = 0x22;
              cube[layer][2] = 0x14;
              cube[layer][1] = 0x08;
              cube[layer][0] = 0x00;
      break;
      case 3: cube[layer][7] = 0x00;
              cube[layer][6] = 0x08;
              cube[layer][5] = 0x22;
              cube[layer][4] = 0x00;
              cube[layer][3] = 0x41;
              cube[layer][2] = 0x00;
              cube[layer][1] = 0x22;
              cube[layer][0] = 0x08;
      break;
      case 4: cube[layer][7] = 0x08;
              cube[layer][6] = 0x41;
              cube[layer][5] = 0x00;
              cube[layer][4] = 0x00;
              cube[layer][3] = 0x80;
              cube[layer][2] = 0x00;
              cube[layer][1] = 0x00;
              cube[layer][0] = 0x41;
      break;
      case 5: cube[layer][7] = 0x80;
              cube[layer][6] = 0x00;
              cube[layer][5] = 0x00;
              cube[layer][4] = 0x00;
              cube[layer][3] = 0x00;
              cube[layer][2] = 0x00;
              cube[layer][1] = 0x00;
              cube[layer][0] = 0x00;
      break;
      case 6: cube[layer][7] = 0x00;
              cube[layer][6] = 0x00;
              cube[layer][5] = 0x00;
              cube[layer][4] = 0x00;
              cube[layer][3] = 0x00;
              cube[layer][2] = 0x00;
              cube[layer][1] = 0x00;
              cube[layer][0] = 0x00;
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


void rotating_mycube_(int tv)
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
    switch(z) {
      case 0: for( uint8_t i = z; i < 8 - z; i++) {
                cube[7 - z][i] = 0x01<<i;
              }
      break;
      case 1: for( uint8_t i = z; i < 8 - z; i++) {
                cube[7 - z][i] = 0x01<<i;
              }
              cube[7 - z][7] = 0x40;
              cube[7 - z][0] = 0x02;
      break;
      case 2: for( uint8_t i = z; i < 8 - z; i++) {
                cube[7 - z][i] = 0x01<<i;
              }
              cube[7 - z][7] = 0x20;
              cube[7 - z][6] = 0x20;
              cube[7 - z][1] = 0x04;
              cube[7 - z][0] = 0x04;
      break;
      case 3: for( uint8_t i = 0; i < 4; i++) {
                cube[7 - z][i] = 0x08;
                cube[7 - z][7 - i] = 0x10;
              }
      break;
      case 4: for( uint8_t i = 0; i < 4; i++) {
                cube[7 - z][7 - i] = 0x08;
                cube[7 - z][i] = 0x10;
              }
      break;
      case 5: for( uint8_t i = 2; i < 6; i++) {
                cube[7 - z][7 - i] = 0x01<<i;
              }
              cube[7 - z][7] = 0x04;
              cube[7 - z][6] = 0x04;
              cube[7 - z][1] = 0x20;
              cube[7 - z][0] = 0x20;
      break;
      case 6: for( uint8_t i = 1; i < 7; i++) {
                cube[7 - z][7 - i] = 0x01<<i;
              }
              cube[7 - z][7] = 0x02;
              cube[7 - z][0] = 0x40;
      break;
      case 7: for( uint8_t i = 0; i < 8; i++) {
                cube[7 - z][7 - i] = 0x01<<i;
              }
      break;
    }
    
    while(times--) {
      for(y = 0; y < 8; y++) {
        for(x = 0; x < 8; x++) {
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
#if 0
  int ctl = 0;
  for(z = 0; z < 8; z++) {
    while(times--) {
      for(y = 0; y < 8; y++) {
        for(x = 0; x < 8; x++) {
            hc595(cube[y][x]);
        }
        hc595out();
        ctl = z + y;
        if (ctl <= 7) {
          cen_on(ctl);
        } else {
          cen_on(ctl - 8);
        }
        
        usleep(200);
        cen_on(8);
        usleep(100);
      }  
    }
    times = tv;
  }
#endif
}

void _sin_cube(const unsigned char po[][8][8], unsigned int cnt, int tv) {
  unsigned int x,y,z;
  int times = tv;
  for(z = 0; z < cnt; z++){
    while(times--) {
      for(y = 0; y < 8; y++){
        for(x = 0; x < 8; x++){
          hc595(po[z][7-y][7-x]);
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
