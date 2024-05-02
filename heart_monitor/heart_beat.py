import serial
import pymysql
from datetime import datetime
import time

ser = serial.Serial('/dev/cu.usbserial-110', 9600, timeout=1)
last_time = 0

conn=pymysql.connect(user="sonngo", password="", host="localhost",port=3306, database="ividual_asm_IoT")

with conn.cursor() as cur:
	while True: 
		if ser.in_waiting > 0:
			line=ser.readline().decode('utf-8').strip()
			if line.isdigit() and int(line) > 512:
				current_time = time.time()
				if last_time != 0: 
					bpm = 60 / (current_time - last_time)
					print(f"BPM: {bpm:.2f}")
					ser.write(b'B')
					ser.write(f"{bpm:.2f}\n".encode())
					date = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
					cur.execute('insert into heatbeat_monitor (BPM, Time) values (%s, %s);', (bpm, date))
					conn.commit()
				last_time = current_time
			elif line == "!":
				print("Leads off detected!")

			