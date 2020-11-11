# Project VEGA of the InvD Technology in Linux platform
The goal of this project is to interface a router to an IOT device having Embedded linux OS.
The router and the IOT device communaicate over TCP/IP.
The communication includes, Sending health check report of the router to the IOT device through the port 41 and also to recieves pre-defined commands from the IOT device through the port 40.

# Future implementation : 
A separate application has to be developed in the router, whose objective is to send the processing report of the App. "rtr_bridge.arm.bin" through the port 43, also the same has to monitor the application "rtr_bridge.arm.bin".

# Monitoring includes :
Bringing back the application "rtr_bridge.arm.bin" when it becomes dead.
