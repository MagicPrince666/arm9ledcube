#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>
#include <string.h>
#include <sys/types.h>
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <iostream>

#include "move.h"
#include "contrl.h"
#include "heart.h"

using namespace std;



static void sigint_handler(int sig)
{
    cube_close();
    cout << "--- quit the loop! ---" << endl;
    exit(0);
}

void _display(u_int16_t time,u_int8_t dat)
{
  unsigned char x,y,z;
  int times = 0;
  for(z = 0; z < 1; z++)
  {
    while(times < time)
	{
	  	for(y = 0;y < 8; y++)
		{
		    for(x = 0;x < 8; x++){
		        hc595(dat);
		    }
		    hc595out();
        cen_on(y);
        usleep(2000);
        cen_on(8);
        usleep(1000);
		}
        times++;
	}
  }
}

const unsigned char test[][8][8]= {
 {
 {0xFF,0x81,0x81,0x81,0x81,0x81,0x81,0xFF},		//8
 {0x00,0x7E,0x42,0x42,0x42,0x42,0x7E,0x00},
 {0x00,0x00,0x3C,0x24,0x24,0x3C,0x00,0x00},
 {0x00,0x00,0x00,0x18,0x18,0x00,0x00,0x00},
 {0x00,0x00,0x00,0x18,0x18,0x00,0x00,0x00},
 {0x00,0x00,0x3C,0x24,0x24,0x3C,0x00,0x00},
 {0x00,0x7E,0x42,0x42,0x42,0x42,0x7E,0x00},
 {0xFF,0x81,0x81,0x81,0x81,0x81,0x81,0xFF}
 }
};

void general(const unsigned char po[][8][8], unsigned int cnt, int tv) {
  unsigned int x,y,z;
  int times = tv;
  for(z = 0; z < cnt; z++) {
    while(times--) {
      for(y = 0; y < 8; y++) {
        for(x = 0; x < 8; x++) {
          hc595(po[z][y][x]);
        }
        hc595out();
        cen_on(y);
        usleep(200);
        cen_on(8);
        usleep(100);
      }
	  }
    times= tv;
    //printf("Z=%d\n",z);
  }
}

void cube_water1(int tv) {
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

void cube_water2(int tv) {
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

void mycube(int tv) {
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

void rain_cube(int tv) {
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


void rotating_mycube_(int tv) {
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
          hc595(po[z][y][x]);
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

void _run_cube( int tv) {
  unsigned char run[][8][8] = {
    {
      {0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01},
      {0x40,0x40,0x40,0x10,0x08,0x02,0x02,0x02},
      {0x20,0x20,0x20,0x10,0x08,0x04,0x04,0x04},
      {0x10,0x10,0x10,0x10,0x08,0x08,0x08,0x08},
      {0x08,0x08,0x08,0x08,0x10,0x10,0x10,0x10},
      {0x04,0x04,0x04,0x08,0x10,0x20,0x20,0x20},
      {0x02,0x02,0x04,0x08,0x10,0x20,0x40,0x40},
      {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80}
    },
    {
      {0x00,0xc0,0x20,0x10,0x08,0x04,0x03,0x00},
      {0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01},
      {0x40,0x40,0x40,0x10,0x08,0x02,0x02,0x02},
      {0x20,0x20,0x20,0x10,0x08,0x04,0x04,0x04},
      {0x10,0x10,0x10,0x10,0x08,0x08,0x08,0x08},
      {0x08,0x08,0x08,0x08,0x10,0x10,0x10,0x10},
      {0x04,0x04,0x04,0x08,0x10,0x20,0x20,0x20},
      {0x02,0x02,0x04,0x08,0x10,0x20,0x40,0x40}
    },
    {
      {0x00,0x00,0xe0,0x10,0x08,0x07,0x00,0x00},
      {0x00,0xc0,0x20,0x10,0x08,0x04,0x03,0x00},
      {0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01},
      {0x40,0x40,0x40,0x10,0x08,0x02,0x02,0x02},
      {0x20,0x20,0x20,0x10,0x08,0x04,0x04,0x04},
      {0x10,0x10,0x10,0x10,0x08,0x08,0x08,0x08},
      {0x08,0x08,0x08,0x08,0x10,0x10,0x10,0x10},
      {0x04,0x04,0x04,0x08,0x10,0x20,0x20,0x20}
    },
    {
      {0x00,0x00,0x00,0xf0,0x0f,0x00,0x00,0x00},
      {0x00,0x00,0xe0,0x10,0x08,0x07,0x00,0x00},
      {0x00,0xc0,0x20,0x10,0x08,0x04,0x03,0x00},
      {0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01},
      {0x40,0x40,0x40,0x10,0x08,0x02,0x02,0x02},
      {0x20,0x20,0x20,0x10,0x08,0x04,0x04,0x04},
      {0x10,0x10,0x10,0x10,0x08,0x08,0x08,0x08},
      {0x08,0x08,0x08,0x08,0x10,0x10,0x10,0x10}
    },
    {
      {0x00,0x00,0x00,0x0f,0xf0,0x00,0x00,0x00},
      {0x00,0x00,0x00,0xf0,0x0f,0x00,0x00,0x00},
      {0x00,0x00,0xe0,0x10,0x08,0x07,0x00,0x00},
      {0x00,0xc0,0x20,0x10,0x08,0x04,0x03,0x00},
      {0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01},
      {0x40,0x40,0x40,0x10,0x08,0x02,0x02,0x02},
      {0x20,0x20,0x20,0x10,0x08,0x04,0x04,0x04},
      {0x10,0x10,0x10,0x10,0x08,0x08,0x08,0x08}
    },
    {
      {0x00,0x00,0x07,0x08,0x10,0xe0,0x00,0x00},
      {0x00,0x00,0x00,0x0f,0xf0,0x00,0x00,0x00},
      {0x00,0x00,0x00,0xf0,0x0f,0x00,0x00,0x00},
      {0x00,0x00,0xe0,0x10,0x08,0x07,0x00,0x00},
      {0x00,0xc0,0x20,0x10,0x08,0x04,0x03,0x00},
      {0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01},
      {0x40,0x40,0x40,0x10,0x08,0x02,0x02,0x02},
      {0x20,0x20,0x20,0x10,0x08,0x04,0x04,0x04}
    },
    {
      {0x00,0x03,0x04,0x08,0x10,0x20,0xc0,0x00},
      {0x00,0x00,0x07,0x08,0x10,0xe0,0x00,0x00},
      {0x00,0x00,0x00,0x0f,0xf0,0x00,0x00,0x00},
      {0x00,0x00,0x00,0xf0,0x0f,0x00,0x00,0x00},
      {0x00,0x00,0xe0,0x10,0x08,0x07,0x00,0x00},
      {0x00,0xc0,0x20,0x10,0x08,0x04,0x03,0x00},
      {0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01},
      {0x40,0x40,0x40,0x10,0x08,0x02,0x02,0x02}
    },
    {
      {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80},
      {0x00,0x03,0x04,0x08,0x10,0x20,0xc0,0x00},
      {0x00,0x00,0x07,0x08,0x10,0xe0,0x00,0x00},
      {0x00,0x00,0x00,0x0f,0xf0,0x00,0x00,0x00},
      {0x00,0x00,0x00,0xf0,0x0f,0x00,0x00,0x00},
      {0x00,0x00,0xe0,0x10,0x08,0x07,0x00,0x00},
      {0x00,0xc0,0x20,0x10,0x08,0x04,0x03,0x00},
      {0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01}
    },
    {
      {0x02,0x02,0x04,0x08,0x10,0x20,0x40,0x40},
      {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80},
      {0x00,0x03,0x04,0x08,0x10,0x20,0xc0,0x00},
      {0x00,0x00,0x07,0x08,0x10,0xe0,0x00,0x00},
      {0x00,0x00,0x00,0x0f,0xf0,0x00,0x00,0x00},
      {0x00,0x00,0x00,0xf0,0x0f,0x00,0x00,0x00},
      {0x00,0x00,0xe0,0x10,0x08,0x07,0x00,0x00},
      {0x00,0xc0,0x20,0x10,0x08,0x04,0x03,0x00}
    },
    {
      {0x04,0x04,0x04,0x08,0x10,0x20,0x20,0x20},
      {0x02,0x02,0x04,0x08,0x10,0x20,0x40,0x40},
      {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80},
      {0x00,0x03,0x04,0x08,0x10,0x20,0xc0,0x00},
      {0x00,0x00,0x07,0x08,0x10,0xe0,0x00,0x00},
      {0x00,0x00,0x00,0x0f,0xf0,0x00,0x00,0x00},
      {0x00,0x00,0x00,0xf0,0x0f,0x00,0x00,0x00},
      {0x00,0x00,0xe0,0x10,0x08,0x07,0x00,0x00}
    },
    {
      {0x08,0x08,0x08,0x08,0x10,0x10,0x10,0x10},
      {0x04,0x04,0x04,0x08,0x10,0x20,0x20,0x20},
      {0x02,0x02,0x04,0x08,0x10,0x20,0x40,0x40},
      {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80},
      {0x00,0x03,0x04,0x08,0x10,0x20,0xc0,0x00},
      {0x00,0x00,0x07,0x08,0x10,0xe0,0x00,0x00},
      {0x00,0x00,0x00,0x0f,0xf0,0x00,0x00,0x00},
      {0x00,0x00,0x00,0xf0,0x0f,0x00,0x00,0x00}
    },
    {
      {0x10,0x10,0x10,0x10,0x08,0x08,0x08,0x08},
      {0x08,0x08,0x08,0x08,0x10,0x10,0x10,0x10},
      {0x04,0x04,0x04,0x08,0x10,0x20,0x20,0x20},
      {0x02,0x02,0x04,0x08,0x10,0x20,0x40,0x40},
      {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80},
      {0x00,0x03,0x04,0x08,0x10,0x20,0xc0,0x00},
      {0x00,0x00,0x07,0x08,0x10,0xe0,0x00,0x00},
      {0x00,0x00,0x00,0x0f,0xf0,0x00,0x00,0x00}
    },
    {
      {0x20,0x20,0x20,0x10,0x08,0x04,0x04,0x04},
      {0x10,0x10,0x10,0x10,0x08,0x08,0x08,0x08},
      {0x08,0x08,0x08,0x08,0x10,0x10,0x10,0x10},
      {0x04,0x04,0x04,0x08,0x10,0x20,0x20,0x20},
      {0x02,0x02,0x04,0x08,0x10,0x20,0x40,0x40},
      {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80},
      {0x00,0x03,0x04,0x08,0x10,0x20,0xc0,0x00},
      {0x00,0x00,0x07,0x08,0x10,0xe0,0x00,0x00}
    },
    {
      {0x40,0x40,0x20,0x10,0x08,0x04,0x02,0x02},
      {0x20,0x20,0x20,0x10,0x08,0x04,0x04,0x04},
      {0x10,0x10,0x10,0x10,0x08,0x08,0x08,0x08},
      {0x08,0x08,0x08,0x08,0x10,0x10,0x10,0x10},
      {0x04,0x04,0x04,0x08,0x10,0x20,0x20,0x20},
      {0x02,0x02,0x04,0x08,0x10,0x20,0x40,0x40},
      {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80},
      {0x00,0x03,0x04,0x08,0x10,0x20,0xc0,0x00}
    },
    {
      {0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01},
      {0x40,0x40,0x20,0x10,0x08,0x04,0x02,0x02},
      {0x20,0x20,0x20,0x10,0x08,0x04,0x04,0x04},
      {0x10,0x10,0x10,0x10,0x08,0x08,0x08,0x08},
      {0x08,0x08,0x08,0x08,0x10,0x10,0x10,0x10},
      {0x04,0x04,0x04,0x08,0x10,0x20,0x20,0x20},
      {0x02,0x02,0x04,0x08,0x10,0x20,0x40,0x40},
      {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80}
    }
  };
  unsigned int x,y;
  int times = tv;
  for(int z = 0; z < 15; z++) {
    while(times--) {
      for(y = 0; y < 8; y++) {
        for(x = 0; x < 8; x++) {
          hc595(run[z][y][x]);
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


int main(int argc, char *argv[])
{
    init_gpio();

    signal(SIGINT, sigint_handler);//信号处理

    while(1) {

#if 0
      _fail_heart(20);
#else
      for(int i = 0; i <= 3; i++) {
        blew_heart(15 - 5*i);
      }
      //_heartbeat(20);
      _my_heart(20);
      _fail_heart(20);
      _display(40, 0);
      mycube(20);
      for(int i = 0; i < 3; i++) {
        cube_water1(7);
      }
      //cube_water2(8);
      for(int i = 0; i < 5; i++) {
        rain_cube(5);
      }
      rotating_mycube_(10);
      _run_cube(10);
      for(int i = 0; i < 3; i++) {
        _sin_cube(sin_cube_table, 14, 6);
      }
#endif
    }

  cube_close();
}
