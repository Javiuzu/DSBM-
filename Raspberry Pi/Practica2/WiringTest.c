#include <wiringPi.h>
int main (void)
{
  wiringPiSetupPhys () ;
  pinMode (37, OUTPUT) ;
  for (;;)
  {
    digitalWrite (37, HIGH) ; delay (5) ;
    digitalWrite (37,  LOW) ; delay (5) ;
  }
  return 0 ;
}
