#ifndef _LQR_H_
#define _LQR_H_

#include <Arduino.h>

#define __clockwise 0
#define __anti_Clockwise 1

class LQR
{
private:
   uint8_t max_Motor_Voltage;
   uint8_t pwm_Channel;
   int pwm_Resolution, pwm_Frequency;
   uint8_t enc_1, enc_2;
   uint8_t pin_Clockwise, pin_Anti_Clockwise, pin_Pwm;
   double K[4];
   double K_I[4];

   float err[4] = {0.0}, e_Int[4] = {0.0};
   int myPwm;
   bool dir;

public:
   void configLQR(int max_Motor_Voltage, int pin_Pwm, int pwm_Channel, int pwm_Resolution, int pwm_Frequency, int enc_1, int enc_2, int pin_Clockwise, int pin_Anti_Clockwise, double *K, double *K_I);
   void beginLQR();
   void evaluateLQR(double dTime, double cPos, double dPos, double cTheta, double dTheta);
   void runLQR();
   void runLQR(uint8_t pwmChannelB, uint8_t dirCPinB, uint8_t dirACPinB);
};

#endif