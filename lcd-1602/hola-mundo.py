import I2C_LCD_driver
from time import *

mylcd = I2C_LCD_driver.lcd()

mylcd.lcd_display_string("Hola mundo.", 1)
mylcd.lcd_display_string("Informatica 2020", 2)
