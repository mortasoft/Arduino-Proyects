## Parke.io
## Mortasoft.xyz
## Parking Test
import RPi.GPIO as GPIO
import time,sys,datetime
GPIO.setmode(GPIO.BCM)

TRIG = 23
ECHO = 24
costoHora = 1000
print("Iniciando medicion de distancia...")
GPIO.setup(TRIG,GPIO.OUT)
GPIO.setup(ECHO,GPIO.IN)

try:
	GPIO.output(TRIG, False)
	print("Calentando el sensor favor espere.....")
	time.sleep(2)
	cont=0
	while True:
		horaInicio = datetime.datetime.now()
		minutos = cont/60
		if cont%60==0:
			print("Lleva {} minutos de parqueo".format(minutos))
		GPIO.output(TRIG, True)
		time.sleep(0.00001)
		GPIO.output(TRIG, False)

		while GPIO.input(ECHO)==0:
		  pulse_start = time.time()

		while GPIO.input(ECHO)==1:
		  pulse_end = time.time()

		pulse_duration = pulse_end - pulse_start

		distance = pulse_duration * 17150

		distance = round(distance, 2)

		print("Distancia: {0} cm  ----Seg: {1}".format(distance,cont))
		time.sleep(1)
		cont = cont+1
except KeyboardInterrupt: # If CTRL+C is pressed, exit cleanly:
   print("Saliendo....")
   horaFinal = datetime.datetime.now()
except:
   e = sys.exc_info()[0]
   print("Error {}".format(e)) 
finally:
   if minutos < 60:
      costoFinal = costoHora
   else:
   	  costoFinal = costoHora * minutos/60
 
   tiempoTotal = horaFinal- horaInicio
   print("------------Hora Inicio: {0}------------ ".format(horaInicio))
   print("------------Hora Final: {0}------------ ".format(horaFinal))
   print("------------Tiempo: {0}------------ ".format(tiempoTotal))
   print("------------Total a pagar: {0} colones------------ ".format(costoFinal))
   print("Limpiando GPIO") 
   GPIO.cleanup() # cleanup all GPIO 