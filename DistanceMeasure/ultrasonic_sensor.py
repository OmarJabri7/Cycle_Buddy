import RPi.GPIO as GPIO
import time
GPIO.setmode(GPIO.BCM)

TRIG = 23
ECHO = 24

print("Distance measuring in progress...")

GPIO.setup(TRIG,GPIO.OUT)
GPIO.setup(ECHO,GPIO.IN)

try:
	while(True):
		GPIO.output(TRIG,False)
		print("Waiting for sensor to settle...")
		time.sleep(2)
		GPIO.output(TRIG,True)
		time.sleep(0.00001)
		GPIO.output(TRIG,False)
		while(GPIO.input(ECHO) == 0):
			pulse_start = time.time()
			
		while(GPIO.input(ECHO) == 1):
			pulse_end = time.time()
		
		pulse_duration = pulse_end - pulse_start
		distance = pulse_duration*17150
		distance = round(distance,2)
		
		print("Distance", str(distance), " cm")
except KeyboardInterrupt:
	print("Done ! Cleaning up...")
	GPIO.cleanup() 
		
		
