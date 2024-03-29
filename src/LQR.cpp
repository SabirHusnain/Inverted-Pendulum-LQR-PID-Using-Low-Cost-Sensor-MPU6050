#include "LQR.h"

void LQR::configLQR(int max_Motor_Voltage, int pin_Pwm, int pwm_Channel, int pwm_Resolution, int pwm_Frequency, int enc_1, int enc_2, int pin_Clockwise, int pin_Anti_Clockwise, double *K, double *K_I)
{
   this->max_Motor_Voltage = (uint8_t)max_Motor_Voltage;
   this->pin_Pwm = pin_Pwm;
   this->pwm_Channel = (uint8_t)pwm_Channel;
   this->pwm_Resolution = pwm_Resolution;
   this->pwm_Frequency = pwm_Frequency;
   this->enc_1 = (uint8_t)enc_1;
   this->enc_2 = (uint8_t)enc_2;
   this->pin_Clockwise = (uint8_t)pin_Clockwise;
   this->pin_Anti_Clockwise = (uint8_t)pin_Anti_Clockwise;
   for (int i = 0; i < 4; i++)
   {
      this->K[i] = *(K++);
      this->K_I[i] = *(K_I++);
   }
   for (int i = 0; i < 4; i++)
   {
      this->err[i] = 0.0;
      this->e_Int[i] = 0.0;
   }
}

void LQR::beginLQR()
{
   pinMode(this->enc_1, INPUT_PULLUP);
   pinMode(this->enc_2, INPUT_PULLUP);
   pinMode(this->pin_Clockwise, OUTPUT);
   pinMode(this->pin_Anti_Clockwise, OUTPUT);
   ledcSetup(this->pwm_Channel, this->pwm_Frequency, this->pwm_Resolution);
   ledcAttachPin(this->pin_Pwm, this->pwm_Channel);
}

void LQR::evaluateLQR(double dTime, double cPos, double dPos, double cTheta, double dTheta)
{
   float U = 0.0, V = 0.0;
   double cVel = (dPos / (dTime / 1e6));
   double cOmega = (dTheta / (dTime / 1e6));
   double x[4] = {cPos, cVel, cTheta, cOmega};

   for (int i = 0; i < 4; i++)
      this->e_Int[i] += err[i] * (dTime / 1.0e6);

   for (int i = 0; i < 4; i++)
      U += (-this->K[i] * x[i]) + (this->K_I[i] * e_Int[i]);

   if (abs(U) > this->max_Motor_Voltage)
      V = max_Motor_Voltage;
   else
      V = abs(U);

   // Integral Windup
   if (abs(U) >= max_Motor_Voltage)
      for (int i = 0; i < 4; i++)
         this->e_Int[i] = 0;

   this->myPwm = map(V, 0, max_Motor_Voltage, 0, pow(2, pwm_Resolution) - 1); // Voltage to PWM Conversion

   if (U < 0)
      this->dir = __anti_Clockwise;
   else
      this->dir = __clockwise;
}

void LQR::runLQR()
{
   digitalWrite(this->pin_Clockwise, this->dir);
   digitalWrite(this->pin_Anti_Clockwise, !(this->dir));
   ledcWrite(this->pwm_Channel, this->myPwm);
}

void LQR::runLQR(uint8_t pwmChannelB, uint8_t dirCPinB, uint8_t dirACPinB)
{
   digitalWrite(this->pin_Clockwise, this->dir);
   digitalWrite(this->pin_Anti_Clockwise, !(this->dir));
   ledcWrite(this->pwm_Channel, this->myPwm);
   digitalWrite(dirCPinB, this->dir);
   digitalWrite(dirACPinB, !(this->dir));
   ledcWrite(pwmChannelB, this->myPwm);
}