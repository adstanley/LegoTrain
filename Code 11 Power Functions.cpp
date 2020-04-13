#include <legopowerfunctions.h>

LEGOPowerFunctions lego(1);

void setup() {}

void loop() {
 lego.SingleOutput(0, PWM_FWD4, RED, CH1);
 delay(3000);
 lego.SingleOutput(0, PWM_FLT, RED, CH1);
 delay(1000);
 lego.SingleOutput(0, PWM_REV4, RED, CH1);
 delay(3000);
 lego.SingleOutput(0, PWM_FLT, RED, CH1);
 delay(1000);
}
