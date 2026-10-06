# Application

## Environment

ex:

```
source "/Users/username/.espressif/tools/activate_idf_v5.4.2.sh"
```


## Build and Deploy

```
idf.py set-target esp32
idf.py menuconfig
```

Go to Component config --> Hardware Settings --> Main XTAL Config --> Main XTAL frequency, then change CONFIG_XTAL_FREQ to 26 MHz.

```
idf.py -p /dev/ttyUSB0 flash monitor
```
