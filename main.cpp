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

using namespace std;

int hc595_dat = -1;
int hc595_st = -1;
int hc595_sh = -1;
int gpio_layer[8] = {-1};

int init_gpio(void){
    hc595_dat = open("/sys/class/leds/hc595:dat/brightness", O_RDWR);
    if(hc595_dat <= 0){
        printf ("can not open hc595_dat\n");
        return -1;
    }
    hc595_st = open("/sys/class/leds/hc595:st/brightness", O_RDWR);
    if(hc595_st <= 0){
        printf ("can not open hc595_st\n");
        return -1;
    }
    hc595_sh = open("/sys/class/leds/hc595:sh/brightness", O_RDWR);
    if(hc595_sh <= 0){
        printf ("can not open hc595_sh\n");
        return -1;
    }

    gpio_layer[0] = open("/sys/class/leds/led0/brightness", O_RDWR);
    if( gpio_layer[0] <= 0){
        printf ("can not open  gpio_layer[0]\n");
        return -1;
    }
    gpio_layer[1] = open("/sys/class/leds/led1/brightness", O_RDWR);
    if( gpio_layer[1] <= 0){
        printf ("can not open  gpio_layer[1]\n");
        return -1;
    }
    gpio_layer[2] = open("/sys/class/leds/led2/brightness", O_RDWR);
    if( gpio_layer[2] <= 0){
        printf ("can not open  gpio_layer[2]\n");
        return -1;
    }
    gpio_layer[3] = open("/sys/class/leds/led3/brightness", O_RDWR);
    if( gpio_layer[3] <= 0){
        printf ("can not open  gpio_layer[3]\n");
        return -1;
    }
    gpio_layer[4] = open("/sys/class/leds/led4/brightness", O_RDWR);
    if( gpio_layer[4] <= 0){
        printf ("can not open  gpio_layer[4]\n");
        return -1;
    }
    gpio_layer[5] = open("/sys/class/leds/led5/brightness", O_RDWR);
    if( gpio_layer[5] <= 0){
        printf ("can not open  gpio_layer[5]\n");
        return -1;
    }
    gpio_layer[6] = open("/sys/class/leds/led6/brightness", O_RDWR);
    if( gpio_layer[6] <= 0){
        printf ("can not open  gpio_layer[6]\n");
        return -1;
    }
    gpio_layer[7] = open("/sys/class/leds/led7/brightness", O_RDWR);
    if( gpio_layer[7] <= 0){
        printf ("can not open  gpio_layer[7]\n");
        return -1;
    }

    return 0;
}

void hc595(unsigned char dat)
{
  unsigned char i;
  for(i = 0; i < 8; i++)
  {
      if((dat<<i)&0x80)
        write(hc595_dat,"0",1);
      else
        write(hc595_dat,"1",1);

     write(hc595_sh,"1",1);
     write(hc595_sh,"0",1);
  }  
}

void hc595out()
{
    write(hc595_st,"1",1);
    write(hc595_st,"0",1);
}

void cen_on(u_int8_t y){
    //for(int i = 0 ; i < 8; i++)
    //    write(gpio_layer[i],"1",1);

    switch(y){
      case 0x00:
            write(gpio_layer[7],"1",1);
            write(gpio_layer[0],"0",1);break;
      case 0x01:
            write(gpio_layer[0],"1",1);
            write(gpio_layer[1],"0",1);break;
      case 0x02:
            write(gpio_layer[1],"1",1);
            write(gpio_layer[2],"0",1);break;
      case 0x03:
            write(gpio_layer[2],"1",1);
            write(gpio_layer[3],"0",1);break;
      case 0x04:
            write(gpio_layer[3],"1",1);
            write(gpio_layer[4],"0",1);break;
      case 0x05:
            write(gpio_layer[4],"1",1);
            write(gpio_layer[5],"0",1);break;
      case 0x06:
            write(gpio_layer[5],"1",1);
            write(gpio_layer[6],"0",1);break;
      case 0x07:
            write(gpio_layer[6],"1",1);
            write(gpio_layer[7],"0",1);break;
      case 0x08:
            for(int i = 0 ; i < 8; i++)
              write(gpio_layer[i],"1",1);
          break;
      default:break;
  }
}

void display(u_int16_t time,u_int8_t dat)
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
  int times = 0;
  for(z = 0; z < cnt; z++){
    while(times < tv){
      for(y = 0; y < 8; y++){
        for(x = 0; x < 8; x++){
          hc595(po[z][y][x]);
        }
        hc595out();
        cen_on(y);
        usleep(2000);
        cen_on(8);
        usleep(1000);
      }
      times++;
	  }
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
      for(y = 0; y < 8; y++){
        for(x = 0; x < 8; x++){
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

void blew_heart(int tv) {
  unsigned char heart[2][8][8] = {
    {
    	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
      {0x00,0x00,0x00,0x18,0x00,0x00,0x00,0x00},
      {0x00,0x00,0x18,0x3c,0x18,0x00,0x00,0x00},
      {0x00,0x00,0x18,0x3c,0x18,0x00,0x00,0x00},
      {0x00,0x3c,0x7e,0x3c,0x18,0x00,0x00,0x00},
      {0x00,0x00,0x18,0x7e,0x18,0x00,0x00,0x00},
      {0x00,0x00,0x00,0x24,0x00,0x00,0x00,0x00},
      {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}
    },
    {
      {0x00,0x00,0x00,0x18,0x18,0x00,0x00,0x00},
      {0x00,0x00,0x00,0x3c,0x3c,0x00,0x00,0x00},
      {0x00,0x00,0x18,0x7e,0x7e,0x18,0x00,0x00},
      {0x00,0x00,0x3c,0x7e,0x7e,0x3c,0x00,0x00},
      {0x00,0x18,0x7e,0xff,0xff,0x7e,0x18,0x00},
      {0x00,0x00,0x7e,0xff,0xff,0x7e,0x00,0x00},
      {0x00,0x00,0x00,0x66,0x66,0x00,0x00,0x00},
      {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}
    }
  };

  unsigned int x,y,z;
  int times = tv;
  for(z = 0; z <7; z++) {
    while(times--) {
      for(y = 0; y < 8; y++) {
        for(x = 0; x < 8; x++) {
            hc595(heart[0][y][x]);    
        }
        hc595out();
        cen_on(y);
        usleep(200);
        cen_on(8);
        usleep(100);
      }  
    }
    times = tv;
    while(times--) {
      for(y = 0; y < 8; y++) {
        for(x = 0; x < 8; x++) {
            hc595(heart[1][y][x]);    
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

static void sigint_handler(int sig)
{
    cen_on(8);
    close(hc595_dat);
    close(hc595_st);
    close(hc595_sh);
    for (int i = 0 ;i < 8; i++){
        close(gpio_layer[i]);
    }
    cout << "--- quit the loop! ---" << endl;
    exit(0);
}

int main(int argc, char *argv[])
{
    init_gpio();

    signal(SIGINT, sigint_handler);//信号处理

    while(1){
        blew_heart(5);
#if 0
        mycube(20);
        for(int i = 0; i < 4; i++) {
          cube_water1(7);
        }
        //cube_water2(8);
        for(int i = 0; i < 5; i++) {
          rain_cube(5);
        }
        //break;
#endif   
    }
    close(hc595_dat);
    close(hc595_st);
    close(hc595_sh);
    for (int i = 0 ;i < 8; i++){
        close(gpio_layer[i]);
    }
}