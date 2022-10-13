#ifndef APP_H
#define APP_H

#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<string.h>
#include<fcntl.h>
#include<unistd.h>
#include<time.h>
#include<pthread.h>
#include<termios.h>
#include"gpio.h"
#include"mlauncher.h"

#define OK              0
#define ERROR           -1
#define HIGH            1
#define LOW             0
#define GPIO_DIR        "/sys/class/gpio/"
#define GPIO_DIR_SIZE   sizeof(GPIO_DIR)

/* Inicia o sensor hc04 */
int init_hc04_sensor();

/* Inicia a comunicação com o lançador de misseis */
int init_launcher();

/* Inicia a comunicação com o servo motor */
int init_servo_motor();

#endif
