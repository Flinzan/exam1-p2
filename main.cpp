#include "mbed.h"
#include "uLCD_4DGL.h"
using namespace std;

InterruptIn sw2(BUTTON1);
EventQueue queue(32 * EVENTS_EVENT_SIZE);
uLCD_4DGL uLCD(D1,D0,D2);
Thread t;
int mode;
float timer;

void uLCD_displaymode(){
    uLCD.color(GREEN);
    uLCD.text_width(3);
    uLCD.text_height(3);
    uLCD.printf("%d\n",mode);
}

void switchmode(){
    mode=mode+1;
    if(mode>2){
        mode=0;
    }
    //queue.call(uLCD_display);
    
}
void display0(){
    timer=0;
    uLCD.reset();
    uLCD.printf("reset");

}
void display1(){

    uLCD.reset();
    uLCD.printf("start");
    
    while(1){
        
        
        
            ThisThread::sleep_for(100ms);
            timer=timer+0.1;
            if(sw2.read()==1){
                break;
        
        }
        
        
    }
    
}
void display2(){
    uLCD.reset();
    uLCD.printf("stop\n");
    uLCD.printf("%f",timer);
}
void uLCD_info() {
   // Note that printf is deferred with a call in the queue
   // It is not executed in the interrupt context
   printf("uLCD is triggered! \r\n");
}
void operate(){
    if(mode==0){
        queue.call(display0);
    }
    else if(mode==1){
        queue.call(display1);
    }
    else if(mode==2){
        queue.call(display2);
    }
}
void Trig_uLCD()  {
   // Execute the time critical part first
   


   // Ask the queue to schedule led1_info() immediately
   //queue.call(uLCD_display);
   queue.call(uLCD_info);
}

int main() {
   // callback() is used to wrap a API call to a queue object.
   // So, t will call queue.dispatch_forever(),
   // and it will start and run the queue scheduler of the EventQueue
   t.start(callback(&queue, &EventQueue::dispatch_forever));

   // 'Trig_led1' will execute in IRQ context
   while(1){
       sw2.rise(switchmode);
       sw2.fall(operate);
   }
   
}