import RPi.GPIO as GPIO
import time
import math

GPIO.setmode(GPIO.BOARD)

x = 100
vals = []
s = 0.0
while x > 0:
	GPIO.setup(11,GPIO.OUT)
	GPIO.setup(7,GPIO.OUT)

	GPIO.output(11, False)
	GPIO.output(7, False)
	time.sleep(0.1)
	#print ("Discharged")

	GPIO.setup(11, GPIO.IN)

	t1 = time.time()
	GPIO.output(7, True)
	GPIO.wait_for_edge(11,GPIO.RISING) 
	t2 = time.time()

	e = t2-t1
	#print (e)
	vals.append(e)
	s += e
	x -= 1

m = s / 100
print("Mitja")
print(m)

GPIO.cleanup()
