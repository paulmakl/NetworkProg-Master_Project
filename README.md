NetworkProg_Master_Project
==========================

Our team is trying to simulate a combined LLC and MAC 802.11 wireless layer. This layer is not a strict Link layer specified by the IEEE 802.11 standard, rather it is a modified one. Assumptions made for simplification: No base station or distribution system, All machines are within range of all others, No fragmentation or reassembly, No power management mechanisms, WEP will not be supported, Calls to interface routines will not be overlapped.

Usage
=====
This system was designed on MAC OS 10.8 running java 7. The current version will only work on an ethernet network. All computers using this system must have a copy of the RF layer in thier working directory. To run the current version:
* Run the make file with flag -f
* Run the newly created java file WifiClient
* Issue command 0 to see diagnostic settings and debugging options

Testing
=======
* Can be tested by communicating with client wifi.jar


Designed by Erin Jamroz, Paul Makl & Weston Thornburg 
