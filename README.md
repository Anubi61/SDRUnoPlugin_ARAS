# SDRUnoPlugin_ARAS
 AutomaticRemoteAntennaSwitch (on LAN)

Background.

My radio shack is composed by two pc, the older one is fully dedicated to the use of RSP1A (I'm proud owner of).
I control that pc by the newer one, via RDP. Although the two pc are on the same network, the dedicated one is connected through a wireless bridge, so I've decided to don't stream audio through the RDP connection but reroute to wireless bluetooth earphones.
It is not perfect but I eliminated a lot of audio drops.

Since RSP1A has a single antenna input, but my antennas are 3, I have to disconnect/reconnect cables very often. Too much often to me :)
So I "created" this simple project.
The hardware is based on RasperryPi3b. with a 4 ports relay board controlled by gpio pins.
On Raspberry a very simple socket server, which accepts remote commands and switch the pins accordingly.
It is on this repository but I didn't make any makefile. 
You can compile with: gcc -o rfswitch rfswitch.c -lwiringPi -lpthread
It uses wiringPi library, download/install it first.

The plugin is the client, and is loaded by SDRUno.
Once the two are connected (there is a Connect button), the plugin asks the status of the server,
then read the FrequencyChanged event from SDRUno and choose which antenna has to be inline.
The system works with two hardware setup. 

The normal one is one antenna each relay, 
the Preamp one follows this scheme:

RF1 ON = Magloop 0-30Mhz,
RF1 OFF & RF2 ON & RF3 OFF = Bias Tee preamp + horizontal V dipole for Meteor/NOAA satellites on 137Mhz
RF1 OFF & RF2 ON & RF3 ON  = Bias Tee preamp + home made dual band 144/430 Mhz 

In both cases, the dual band antenna is used for any frequency above 30 Mhz with the exclusion of the range 137-138Mhz

If anyone interested feel free to clone this repository and do whatever you want.
Pay attention I am unable to load the needed nana library, due to github limits.
But you can find it already compiled somewere else. The version used is nana_v142_Release_x86.lib
A mention to jcma68 for his plugin SDRUnoPlugin_SatTrack which was the base to understand how to code a plugin. 

BR





