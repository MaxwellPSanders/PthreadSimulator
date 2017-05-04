Joint project between Maxwell Sanders and David Theodore

Kerbal Pthread Simulator

The goal:
The goal of this project is to set a certain amount of resources and then let multiple
threads, which we will call Kerbals, grab at. We want the Kerbals to be able to grab all
3 of the different resources, build their ship, fly, land, and dissassemble. And then
beyond all of that we have to watch for deadlocks and make sure our critical regions are guarded.

To Compile:
g++ ksf.c -o ksf -lpthread
*******This will compile on OS X, DO NOT BELIEVE THE LIES IT WILL NOT WORK*******
*******        semaphores do not work in OS X, but they do in linux       *******

To Run:
./ksf 2 3 4 5

2 Engines
3 Fuselages
4 Fuel Tanks
5 Kerbals

The numbers that can be input is any number between 1-255
Any invalid input had will notify you of your wrongdoings
