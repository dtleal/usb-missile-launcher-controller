#include"app.h"

/* Controle interno */
struct usb_dev_handle *launcher;
static pthread_mutex_t lock;
pthread_t tid_hc04;
pthread_t tid_launcher;
float target;

void *t_init_hc04_sensor()
{
    int ret=0, aux=0;
    struct gpio_st *app_st_tr;
    struct gpio_st *app_st_ec;
    float elapsed=0;
    struct timespec ts_start;
    struct timespec ts_end;

    app_st_tr = malloc(sizeof(struct gpio_st));
    app_st_ec = malloc(sizeof(struct gpio_st));

    if (app_st_tr == NULL)
        goto error_block;
    if (app_st_ec == NULL)
        goto error_block;

    // TRIGGER 
    strcpy(app_st_tr->pin, "23");
    strcpy(app_st_tr->direction, "out");

    // ECHO 
    strcpy(app_st_ec->pin, "24");
    strcpy(app_st_ec->direction, "in");
  
    ret = export_gpio(app_st_tr);
    if (!ret)
        goto error_block;
    ret = export_gpio(app_st_ec);
    if (!ret)
        goto error_block;

    ret = direction_gpio(app_st_tr);
    if (!ret)
        goto error_block;
    ret = direction_gpio(app_st_ec);
    if (!ret)
        goto error_block;

    app_st_ec->file = NULL;

init_loop:    
    fprintf(stdout, "thread hc04...\n");
    ret = set_gpio_pin_value("1", app_st_tr);
    if (!ret)
        goto error_block;

    sleep(0.00001);

    ret = set_gpio_pin_value("0", app_st_tr);
    if (!ret)
        goto error_block;

    clock_gettime(CLOCK_REALTIME, &ts_start);
    clock_gettime(CLOCK_REALTIME, &ts_end);

    do {     
        ret = get_gpio_pin_value(app_st_ec);
        clock_gettime(CLOCK_REALTIME, &ts_start);
    } while(ret == 0);

    do {    
        ret = get_gpio_pin_value(app_st_ec);     
        clock_gettime(CLOCK_REALTIME, &ts_end);
    } while(ret == 1);

    elapsed = ts_end.tv_nsec - ts_start.tv_nsec;
    elapsed /= 1000000000;

    /* Writing distance */
    pthread_mutex_lock(&lock);

    target = (elapsed * 34300) / 2;

    pthread_mutex_unlock(&lock);

    fprintf(stdout, "Distance: %.1f cm\n", target);

    sleep(1);

    goto init_loop;

error_block:

     pthread_exit(NULL);

     return NULL;
}

void *t_init_launcher()
{
    while(1) {
    fprintf(stdout, "thread launcher...\n");
    sleep(1);
    }
}

void *t_init_servo_motor()
{

}

int init_hc04_sensor()
{
    return pthread_create(&tid_hc04, NULL, &t_init_hc04_sensor, NULL);
}

int init_launcher()
{
    struct usb_bus *bus;
    struct usb_device *dev;

    usb_init();
    usb_find_busses();
    usb_find_devices();

    for (bus = usb_busses; bus; bus = bus->next) {
        for (dev = bus->devices; dev; dev = dev->next){
            if (dev->descriptor.idVendor == 0x2123 &&
                dev->descriptor.idProduct == 0x1010)
            {
                launcher = usb_open(dev);
                if (launcher == NULL)
                    return ERROR;
            }
        }
    }
   
    return pthread_create(&tid_launcher, NULL, &t_init_launcher, NULL);
}

int init_servo_motor()
{

}

int getkey()
{
    struct termios old, new;
    int x;

    tcgetattr( STDIN_FILENO, &old);
    new = old;
    new.c_lflag &= ~( ICANON | ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &new);
    x = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &old);

    return x;
}

int main(int argc, char *argv[])
{
    int ret=0;
    char opt;

    /* Implementar getotp */

    /* global mutex init */
    if (pthread_mutex_init(&lock, NULL)) {
        fprintf(stderr, "Global mutex init ERROR\n");
        goto end_block;
    }

    /* init thread sensor */
    if(init_hc04_sensor()) {
        fprintf(stderr, "init sensor ERROR\n");
        goto end_block;
    }

    /* init thread launcher */
    if(init_launcher()) {
        fprintf(stderr, "init launcher ERROR\n");
        goto end_block;
    }

    do {
        opt = getkey();

        switch(opt) {
            case 'a':
                printf("a\n");
                break;
  
            case 'b':
                printf("b\n");

                break;

            case 'c':
                printf("c\n");

                break;

            case 'd':
                printf("d\n");

                break;

            case 'q':
                printf("digitou: q\n");

                goto end_block;
                break;
               
        }

    } while(opt != 'q');

end_block:

    /* Finishing thread hc04 */
    pthread_join(tid_hc04, NULL);
    
    /* Finishing thread launcher */
    pthread_join(tid_launcher, NULL);
    
    /* Finishing mutex */
    pthread_mutex_destroy(&lock);

    return 0;
}
