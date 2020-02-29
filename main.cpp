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
#include "water.h"
#include "mycube.h"

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

void loudou( int tv) {
  unsigned char cube[8][8] = {0};
  unsigned int x,y,z;
  int times = tv;
  for(z = 0; z < 8; z++) {
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
    times= tv;
    //printf("Z=%d\n",z);
  }
}

void displayking(int tv)
{
  uint8_t x,y,z,i=0,j=0,num;
  int times = tv;
  uint8_t Cube[8][8] = {0};
  uint8_t cen[8]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};

  for(i = 0;i < 8;i++) {
    if(i < 4) z = -8*i + 28;
	  else z = 8*i - 28;

      j=i;
      for(num = 0;num < z;num++) {
        if(i < 4) {
          switch(num/(z/4)) {
            case 0:{Cube[i][j] |= Cube[i][j]>>1 | cen[7-i];}break;
            case 1:{Cube[i][j++] |= cen[i];}break;
            case 2:{Cube[i][7-i] |= Cube[i][7-i]<<1 | cen[i];}break;
            case 3:{Cube[i][j] |= Cube[i][j] | cen[7-i];j--;}break;
          }
        } else {
          switch(num/(z/4)) {	
            case 0:{Cube[i][7-j] |= Cube[i][7-i]>>1 | cen[i];}break;
            case 1:{Cube[i][7-j] |= cen[7-i];j--;}break;
            case 2:{Cube[i][i] |= Cube[i][i]<<1 | cen[7-i];}break;
            case 3:{Cube[i][7-j] |= Cube[i][7-j] | cen[i];j++;}break;			
          }
        }
          
        while(times--) {
        for(y = 0; y < 8; y++) {
          for(x = 0; x < 8; x++) {
            hc595(Cube[y][x]);
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
}

int main(int argc, char *argv[])
{
    init_gpio();

    signal(SIGINT, sigint_handler);//信号处理

    while(1) {

#if 0
      for_lynette_(10);
#else 
      for(int i = 0; i <= 3; i++) //心跳加速
        blew_heart(15 - 5*i);
      _my_heart(20);              //停止跳动 渐渐暗淡
      _fail_heart(20);            //渐渐落下
      _display(40, 0);            //世界一片黑暗
      mycube(20);                 //死水微澜
      for(int i = 0; i < 3; i++)  //落下一滴眼泪 两滴 三滴
        cube_water1(8);
      for(int i = 0; i < 5; i++)  //顿时下起了大雨
        rain_cube(5);
      for(int i = 0; i < 3; i++)  //浪涛翻涌
        _sin_cube(sin_cube_table, 14, 6);
      rotating_mycube_(10);       //扭曲的升起
      for(int i = 0; i < 3; i++)  //旋转着
        general(warping,15,8);
      displayking(7);             //时间的沙漏
      general( king,8,10);        //抚平一切
      _display(40, 0);            //世界仍旧黑暗

#endif
    }

  cube_close();
}
