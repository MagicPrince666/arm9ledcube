#include "contrl.h"


int hc595_dat = -1;
int hc595_st = -1;
int hc595_sh = -1;
int gpio_layer[8] = {-1};



int init_gpio(void)
{
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

void cen_on(u_int8_t y)
{
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

int cube_close(void)
{
    cen_on(8);
    close(hc595_dat);
    close(hc595_st);
    close(hc595_sh);
    for (int i = 0 ;i < 8; i++){
        close(gpio_layer[i]);
    }
    return 0;
}
