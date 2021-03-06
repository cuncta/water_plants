import datetime
import time
import bluetooth
from apscheduler.schedulers.background import BlockingScheduler
import json
import ast
import Socket

#bluetooth address of arduino
bd_addr = "00:21:13:02:C2:54"
port = 1

init = False


def get_last_watered():
	try:
		with open("last_watered.txt", "r") as f:
			return tail(f,1)
	except:
		return "NEVER!"

def get_next_water():
	update_logbook("checked next water")
	try:
		with open("next_water.txt", "r") as f:
			return tail(f,1)
	except:
		return "NEVER!"

def get_watering_duration():
	update_logbook("check watering duration")
	with open('watering_parameters.txt') as handle:
		wp = json.loads(handle.read())
	m = "The watering duration is set to "+str(wp["watering_time"])+" seconds"
	return m

def get_soil_status():
	update_logbook("get soil status")
	m = receive_soil_sensor_data()
	m = ast.literal_eval(m)
	text = "Soil status in format plant:value \n\r W1:"+str(m["S0"])
	return text

def receive_soil_sensor_data():
		sock = bluetooth.BluetoothSocket( bluetooth.RFCOMM )
		sock.connect((bd_addr, port))
		sock.send('s')
		print "message sent"
		count = 0
		message = []
		while (count < 1000):
				print "test"
				data = sock.recv(1)
				print "received"
				message.append(data)
				count += 1
		sock.close
		message =''.join(message)
		print 'message', message
		n = find(message, "#")
		update_logbook("received soil sensor data")
		return message[n[0]+1:n[1]]

def auto_water():
	with open('watering_parameters.txt') as handle:
		wp = json.loads(handle.read())
	with open("next_water.txt", "w+") as f1:
	   f1.write("Watering twice a day at "+str(wp["first_water"])+":00 and at "+str(wp["second_water"])+":00 for "+str(wp["watering_time"])+" seconds")
	update_logbook("Started auto watering")
	global scheduler
	scheduler = BlockingScheduler()
	scheduler.add_job(pump_on, 'cron',  day_of_week='mon-sun', hour=wp["first_water"], id='first_water')
	scheduler.add_job(pump_on, 'cron',  day_of_week='mon-sun', hour=wp["second_water"], id='second_water')
	scheduler.print_jobs()
	scheduler.start()
	return

def auto_water_off():
	update_logbook("Stopped auto watering")
	scheduler.remove_job('first_water')
	scheduler.remove_job('second_water')
	scheduler.print_jobs()

def pump_on():
	print 'watering'
	internet_connection = internet()
	update_logbook("checked internet connection: " + str(internet_connection))
	with open("last_watered.txt", "a+") as f:
		f.write("Last watered {:%Y-%m-%d %H:%M}\n".format(datetime.datetime.now()))
	update_logbook("Pump on -- watering")
	sock = bluetooth.BluetoothSocket( bluetooth.RFCOMM )
	sock.connect((bd_addr, port))
	sock.send('w')
	time.sleep(10)
	sock.send('h')
	sock.close()

def set_watering_time():
	with open('watering_parameters.txt') as handle:
		wp = json.loads(handle.read())
	dur = wp["watering_time"]
	message = "watering time set to" + dur + "seconds"
	update_logbook(message)
	print 'watering duration ',dur, 's'
	sock = bluetooth.BluetoothSocket( bluetooth.RFCOMM )
	sock.connect((bd_addr, port))
	sock.send(dur)
	time.sleep(10)
	sock.send('h')
	sock.close()

def find(s, ch):
	return [i for i, ltr in enumerate(s) if ltr == ch]

def tail( f, lines=20 ):
	total_lines_wanted = lines

	BLOCK_SIZE = 1024
	f.seek(0, 2)
	block_end_byte = f.tell()
	lines_to_go = total_lines_wanted
	block_number = -1
	blocks = [] # blocks of size BLOCK_SIZE, in reverse order starting
				# from the end of the file
	while lines_to_go > 0 and block_end_byte > 0:
		if (block_end_byte - BLOCK_SIZE > 0):
			# read the last block we haven't yet read
			f.seek(block_number*BLOCK_SIZE, 2)
			blocks.append(f.read(BLOCK_SIZE))
		else:
			# file too small, start from begining
			f.seek(0,0)
			# only read what was not read
			blocks.append(f.read(block_end_byte))
		lines_found = blocks[-1].count('\n')
		lines_to_go -= lines_found
		block_end_byte -= BLOCK_SIZE
		block_number -= 1
	all_read_text = ''.join(reversed(blocks))
	return '\n'.join(all_read_text.splitlines()[-total_lines_wanted:])

def update_logbook(message):
	with open("log_Book.txt", "a+") as f:
		f.write("{:%Y-%m-%d %H:%M} ".format(datetime.datetime.now()) + message + "\n")

def internet(host="8.8.8.8", port=53, timeout=5):
    """
    Host: 8.8.8.8 (google-public-dns-a.google.com)
    OpenPort: 53/tcp
    Service: domain (DNS/TCP)
    """
    try:
        socket.setdefaulttimeout(timeout)
        socket.socket(socket.AF_INET, socket.SOCK_STREAM).connect((host, port))
        return True
    except Exception as ex:
        print ex.message
        return False
