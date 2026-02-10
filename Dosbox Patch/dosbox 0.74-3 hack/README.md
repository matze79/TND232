DOSBox 0.74.3 – Serial Port Hack



This repository contains a modified File of DOSBox 0.74.3 with the serial device hardcoded in the source.



⚠️ Warning:

This is not a proper DOSBox serial implementation.

It is only a quick hack to make serial communication work in a limited way.



Serial Port Configuration



Edit the serial device directly in the source code.



Example devices:



Linux USB serial #1:

/dev/ttyUSB0



Adjust the device path as needed for your system.



Replace the modified file in the DOSBox 0.74.3 source tree.



Build \& Install



Install into a separate prefix to avoid conflicts.



./configure --prefix=/opt/dosbox-tandyserial

make

make install



Platform Status



NetBSD code - tested only on NetBSD 10.1 / sparc64 



Limitations



Experimental / hacky



Not upstream-ready



Minimal functionality only

