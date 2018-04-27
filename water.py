import datetime
import time
import bluetooth
from apscheduler.schedulers.background import BlockingScheduler
import json

#bluetooth address of arduino
bd_addr = "00:21:13:02:C2:54"
port = 1

init = False

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
def get_last_watered():
	try:
		f = open("last_watered.txt", "r")
		return tail(f,1)
	except:
		return "NEVER!"
def get_next_water():
	try:
		f = open("next_water.txt", "r")
		return tail(f,1)
	except:
		return "NEVER!"

def get_watering_duration():
    with open('watering_parameters.txt') as handle:
    	wp = json.loads(handle.read())
    m = "The watering duration is set to "+str(wp["watering_time"])+" seconds"
    return m
    
def auto_water():
    with open('watering_parameters.txt') as handle:
    	wp = json.loads(handle.read())

    f1 = open("next_water.txt", "w+")
    f1.write("Watering twice a day at "+str(wp["first_water"])+":00 and at "+str(wp["second_water"])+":00 for "+str(wp["watering_time"])+" seconds")
    f1.close()
    global scheduler
    scheduler = BlockingScheduler()
    scheduler.add_job(pump_on, 'cron',  day_of_week='mon-sun', hour=wp["first_water"], id='first_water')
    scheduler.add_job(pump_on, 'cron',  day_of_week='mon-sun', hour=wp["second_water"], id='second_water')
    scheduler.print_jobs()
    scheduler.start()
    return
def auto_water_off():
    scheduler.remove_job('first_water')
    scheduler.remove_job('second_water')
    scheduler.print_jobs()
def pump_on():
    print 'watering'
    f = open("last_watered.txt", "a+")
    f.write("Last watered {:%Y-%m-%d %H:%M}\n".format(datetime.datetime.now()))
    f.close()
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
    print 'watering duration ',dur, 's'
    sock = bluetooth.BluetoothSocket( bluetooth.RFCOMM )
    sock.connect((bd_addr, port))
    sock.send(dur)
    time.sleep(10)
    sock.send('h')
    sock.close()
