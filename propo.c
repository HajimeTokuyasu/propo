#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <linux/joystick.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>

#define PORT 50000
#define _MAXBUTTON_ 20
#define _MAXSTICK_ 3


//*******************************************
//4FirstRobot_HajimeTokuyasu
//
//MEMO:
//LAST:20160429
//*******************************************

int main (int argc, char *argv[]){
   int buttonData[_MAXBUTTON_];
   int stickData[_MAXSTICK_];
   int buf;

   int fd_joy, fd_sock;
   struct js_event event;
   if (argc != 2){
      puts("PUT_IPofPi");
      return -1;
   }
   puts("proc_getting_started");
   struct sockaddr_in svaddr;
   socklen_t len;
   len = sizeof(struct sockaddr_in);

   fd_sock = socket(AF_INET,SOCK_DGRAM,0);
   if (fd_sock == -1){
      puts("ERROR_socket_missing");
      return -1;
   }



   memset(&svaddr, 0, sizeof(struct sockaddr_in));
   svaddr.sin_family = AF_INET;
   svaddr.sin_port = htons(PORT);

   if (inet_pton(AF_INET, argv[1], &svaddr.sin_addr) <= 0){
      puts("ERROR_CANNOT_PACKADDR");
      return -1; 
   }

   fd_joy = open("/dev/input/js0", O_RDONLY);
   //fcntl(fd_joy, F_SETFL, O_NONBLOCK);
   if (fd_joy == -1){
      puts("ERROR_Missing.JS");
      return -1;
   }
 
   puts("AllSettings_done");
   
   while(1){
 
     if (read(fd_joy,&event,sizeof(struct js_event)) < sizeof(struct js_event))continue;
   

     switch (event.type & ~JS_EVENT_INIT){
     
     case JS_EVENT_AXIS:

       switch (event.number) {
       case 0:
         if (event.value < 0) {
           buf = 'L';
           puts("L");
         } else if (event.value == 0 ) {
           buf = 'K';
           puts("K");
         } else {
           buf = 'R';
           puts("R");
         }
         sendto(fd_sock, &buf, sizeof(char), 0, (struct sockaddr *) &svaddr, len);

         puts("FRONTDATA__SEND");
         break;
       }

       break;

     case JS_EVENT_BUTTON:

       switch (event.value){
       case 1:
         switch (event.number){
         case 9:
           buf = 'F';
           puts("F");
           break;
         case 8:
           buf = 'B';
           puts("B");
           break;
         }
         break;

       case 0:
         buf = 'N';
         puts("N");
         break;

       default:
         puts("\n\n\n_____!!ERROR!!ofBUTTTON_____\n\n\n");
       }
         
       sendto(fd_sock, &buf, sizeof(char), 0, (struct sockaddr *) &svaddr, len);
//         switch (event.number){
//            case 8:
//               switch (event.value){
//
//               puts("___FWD___");
//               break;
//            case 9:
//               buttonData[event.number] = ;
//               puts("_____BACK_____");
//               break;
//            default:
//               puts("__KEEP__");
//               
//         }
       break;
     }

   }//while
}//main

