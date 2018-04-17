from flask import Flask, render_template, redirect, url_for, request
import psutil
import datetime
import water
import os
import json
import time

app = Flask(__name__)

def template(title = "HELLO!", text = ""):
    now = "{:%Y-%m-%d %H:%M}\n".format(datetime.datetime.now())
    timeString = now
    templateDate = {
        'title' : title,
        'time' : timeString,
        'text' : text
        }
    return templateDate

@app.route("/")
def hello():
    templateData = template()
    return render_template('main.html', **templateData)

@app.route("/last_watered")
def check_last_watered():
    templateData = template(text = water.get_last_watered())
    return render_template('main.html', **templateData)

@app.route("/water_schedule")
def check_water_schedule():
    templateData = template(text = water.get_next_water())
    return render_template('main.html', **templateData)

@app.route("/water")
def action2():
    water.pump_on()
    templateData = template(text = "Watered Once")
    return render_template('main.html', **templateData)

@app.route("/auto/water/<toggle>")
def auto_water(toggle):
    running = False
    if toggle == "ON":
        templateData = template(text = "Auto Watering On")
        for process in psutil.process_iter():
            try:
                if process.cmdline()[1] == 'auto_water.py':
                    templateData = template(text = "Already running")
                    running = True
            except:
                pass
        if not running:
            os.system("python2.7 auto_water.py&")
    else:
        f1 = open("next_water.txt", "w+")
        f1.write("Auto Watering off")
        f1.close()
        templateData = template(text = "Auto Watering Off")
        os.system("pkill -f water.py")
#        water.auto_water_off()

    return render_template('main.html', **templateData)

@app.route("/result",methods = ['GET', 'POST'])
def result():
   if request.method == 'POST':
      result = request.form
      if result["first_water"].isdigit() == True and  \
	 result["second_water"].isdigit() == True and \
	 result["watering_time"].isdigit() == True:
         if int( result["first_water"]) <= 23 and  \
            int(result["second_water"]) <= 23 and \
            int(result["watering_time"]) <= 9:
             with open('watering_parameters.txt', 'w+') as file:
                 file.write(json.dumps(result)) # use `json.loads` to do the reverse
     	     templateData = template(text = "New Parameters Set")
     	     time.sleep(1)
     	     water.set_watering_time()
     	     return render_template('main.html', **templateData)
         elif  int(result["first_water"]) == 42 or  \
               int(result["second_water"]) == 42 or \
               int(result["watering_time"]) == 42:
               templateData = template(text = "We are still calculating the question!!!")
               return render_template('main.html', **templateData)
         else:
              templateData = template(text = "check your numbers, you did something wrong!!!")
              return render_template('main.html', **templateData)
      else:
         templateData = template(text = "INSERT ONLY NUMBERS!!!")
         return render_template('main.html', **templateData)



if __name__ == "__main__":
    app.run(host='0.0.0.0', port=12345, debug=True)
