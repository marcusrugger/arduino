//  Demo function:The application method to drive the DC motor.
//  Author:Frankie.Chu
//  Date:20 November, 2012

#include "MotorDriver.h"
#include <Servo.h>

Servo myservo;  // create servo object to control a servo 
                // a maximum of eight servo objects can be created 

void setup()
{
    /*Configure the motor A to control the wheel at the left side.*/
    /*Configure the motor B to control the wheel at the right side.*/
    motordriver.init();
    motordriver.setSpeed(255, MOTORA);
    motordriver.setSpeed(255, MOTORB);
//    motordriver.stop();

//    myservo.attach(4);  // attaches the servo on pin 9 to the servo object 
}
 
void loop()
{
    // myservo.write(0);
    // delay(750);
    // myservo.write(180);
    // delay(750);
    // for (int s = 63; s < 256; s += 32)
    // {
    //     motordriver.setSpeed(s, MOTORA);
    //     motordriver.setSpeed(s, MOTORB);
    //     motordriver.goForward();
    //     delay(1000);
    // }
    // motordriver.goForward();
    // delay(2000);
    // motordriver.stop();
    // delay(1000);
    // motordriver.goBackward();
    // delay(2000);
    // motordriver.stop();
    // delay(1000);
    // motordriver.goLeft();
    // delay(2000);
    // motordriver.stop();
    // delay(1000);
    // motordriver.goRight();
    // delay(2000);
    // motordriver.stop();
    // delay(1000);

    motordriver.goForward();
    delay(5000);
    motordriver.stop();
    delay(1000);
    motordriver.goBackward();
    delay(5000);
    motordriver.stop();
    delay(1000);
}
