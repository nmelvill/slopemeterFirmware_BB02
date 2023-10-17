# slopemeterFirmware_BB02
Firmware for the second slopemeter breadboard


The magnetometer is located on it's own I2C line and is acessed via the aux I2C bus on the ICM20948.  The aux I2C bus seems like it can be configured to pass through to the main I2C bus.  Alternately the aux I2C can be directly connected to the main I2C bus with wiring.
