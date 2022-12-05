# Switch-Controller-UPS
Modification of firmware of STC 15W401AS of a ups module to switch to input 2 if input1 < 11.8V

The module that is sold on ebay/aliexpress/etc accepts 2 inputs of 12V each.
![image of switch module](./switch%20ups%20module.jpg)

I wanted to use it as a ups to switch to battery if the main input is lost. But I noticed that the module switches to input 2 only if input 1 is bellow 8V. 

If the load is high the drop is quick and the module switches almost immediatelly. 
But if the load is low, the output voltage will slowly drop below 12V and if your powered device needs more than 10V to work, there is a problem.

So I modified the firmware of the TSC 15W401AS to switch to input 2 if input 1 is bellow 11.8V

Compiled with μVision V5.11.2.0

Burned with stc-isp-15xx-v6.86E.exe

### Notice

This is my first (and hopefully my last) try with this mcu.
