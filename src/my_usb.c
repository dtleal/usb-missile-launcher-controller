#include<stdio.h>
#include<stdlib.h>
#include<usb.h>
#include<string.h>

int main(){
    int ret=0;
    struct usb_bus *bus;
    struct usb_device *dev;
    struct usb_dev_handle *launcher;
    char msg[8]={}, opt;

    usb_init();
    usb_find_busses();
    usb_find_devices();

    for (bus = usb_busses; bus; bus = bus->next) {
        for (dev = bus->devices; dev; dev = dev->next){
            if (dev->descriptor.idVendor == 0x2123 &&
                dev->descriptor.idProduct == 0x1010)
            {
                printf("Trying device %s/%s\n", bus->dirname, dev->filename);
                printf("\tID_VENDOR = 0x%04x\n", dev->descriptor.idVendor);
                printf("\tID_PRODUCT = 0x%04x\n", dev->descriptor.idProduct);

                launcher = usb_open(dev);
                if (launcher == NULL)
                    printf("fodeu\n");
            }
        }
    }

    do {
        memset(msg, 0, 8);
        opt = getc(stdin); 
    
        /*
         up 0x02
         down 0x01
         lef 0x04
         rig 0x08
         stop 0x20
         fire 0x10
        */

        msg[0]=0x2;
        msg[2]=0x0;
        msg[3]=0x0;
        msg[4]=0x0;
        msg[5]=0x0;
        msg[6]=0x0;
        msg[7]=0x0;
      
        if (!opt)
            continue;
        else {
            switch(opt) {
                // up 
                case 'u'://0x26:
                    msg[1] = 2;
                    break;
        
                // down 
                case 'd'://0x28:
                    msg[1] = 1;
                    break;

                // left  
                case 'l'://0x25:
                    msg[1] = 4;
                    break;

                // right
                case 'r'://0x27:
                    msg[1] = 8;
                    break;

                // fire 
                case 'f'://0xD:
                    msg[1] = 16;
                    break;
     
                // stop
                case 's'://0xD:
                    msg[1] = 0x20;
                    break;
                  
            }
            ret = usb_control_msg(launcher, 0x21, 0x09, 0, 0, (char*)msg, 8,100);
            usleep(100000);
            msg[1]=0x20;
            ret = usb_control_msg(launcher, 0x21, 0x09, 0, 0, (char*)msg, 8,100);
        }
    }while(opt != 'e');

    usb_close(launcher);
    return 0;
}

