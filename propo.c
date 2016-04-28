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


int main (int argc, char *argv[]){
   int buttonData[_MAXBUTTON_];
   int stichData[_MAXSTICK_];
   int buf;

   int fd_joy, fd_sock;
   struct js_event event;
   if (argc != 2){
      puts("PUT_IPofPi\n");
      return -1;
   }
   puts("proc_getting_started\n");
   struct sockaddr_in svaddr;
   socklen_t len;
   len = sizeof(struct sockaddr_in);

   fd_sock = socket(AF_INET,SOCK_DGRAM,0);
   if (fd_sock == -1){
      puts("ERROR_socket_missing\n");
      return -1;
   }



   memset(&svaddr, 0, sizeof(struct sockaddr_in));
   svaddr.sin_family = AF_INET;
   svaddr.sin_port = htons(PORT);

   if (inet_pton(AF_INET, argv[1], &svaddr.sin_addr) <= 0){
      printf("ERROR_CANNOT_PACKADDR\n");
      return -1; 
   }

   fd_joy = open("/dev/input/js0", O_RDONLY);
   fcntl(fd_joy, F_SETFL, O_NONBLOCK);
   if (fd_joy == -1){
      puts("ERROR_Missing.JS\n");
      return -1;
   }
 
   puts("AllSettings_done\n");
   
   while(1){
 
   if (read(fd_joy,&event,sizeof(struct js_event)) < sizeof(struct js_event))continue;
   

   switch (js_event.type & ~JS_EVENT_INIT){
      case JS_EVENT_AXIS:
         
         buf  = (stickData[JSinfo.number] = JSinfo.value);
         if (event.value < 0){
            buf = 'L';
         }
         if (event.value == 0){
            buf = 'K';
         }
         if (event.value >0){
            buf = 'R';
         }
         sendto(fd_sock, &buf, sizeof(char), 0, (struct sockaddr *) &svaddr, len);
         break;
         
      case JS_EVENT_BUTTON:
         
         buttonData[JSinfo.number] = JSinfo.value;
         sendto(fd_sock, &buf, sizeof(char), 0, (struct sockaddr *) &svaddr, len);
         break;
  }

   }//while
}//main




































//   if ((event.type == JS_EVENT_AXIS) && (event.number == 0)){
//      puts("JS_resolved\n");
//   }
//   if (event.value < 0){
//       buf = 'L';puts("L");
//      }
//      else if (event.value == 0){
//        buf = 'K';puts("k");
//      }
//      else{
//        buf = 'R';puts("R");
//      }
//   puts("buf_done\n");
//   sendto(fd_sock, &buf, sizeof(char), 0, (struct sockaddr *) &svaddr, len);
//   puts("dataFront_send\n");
//   
//    if ((event.type == JS_EVENT_BUTTON)){
//           if (buttonData[8] == buttonData[9])buf = 'N';puts("N");
//           if (buttonData[8] == 1)buf = 'B';puts("B");
//           if (buttonData[9] == 1)buf = 'F';puts("F");
//        }
//   sendto(fd_sock, &buf, sizeof(char), 0, (struct sockaddr *) &svaddr, len);
//   puts("dataRear_send\n");
//


