# Electronic nose utilities

These are some raspberry pi utilities for interacting with Michael Madsen's 8-sensor electronic noses.

My setup is to keep a screen session to the sensor board via `screen -L /dev/ttyUSB0`. This way I can detach the session and keep logging data even when not logged in to the Raspberry Pi. It accumulates a logfile in `screenlog.0` and from there I use a few utilities to process it.

They include:
* csver -- converts the `screenlog.0` formatted data to CSV. I prefer the Arduino to print out daa in the form of:

```
Time, A0: aaa, A1: bbb, A2: ccc, ...
```

because each of those lines stands on their own and you know everything there is to know, but it's not a very efficient data format. So csver converts it to CSV.
* plot -- plot the csv file with gnuplot
* updater -- i have this set to run every minute via cron. currently puts the last 100 data points in a file where plot will read it (array-0-small-3.csv)
* Also included is 8-sensor-sketch, the sketch uploaded to the Arduino that prints out each sensor value every 30 seconds. It needs to be seeded with the time by connecting to it and typing "T<seconds since epoch>". <seconds since epoch> can be determined by running `date +%s` on a linux/osx terminal. This also expects a DS18B20 temperature sensor, and requires the OneWire and DallasTemperature Arduino libraries.
