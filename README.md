# Consensus
C++ program to model consensus spreading

To build run ```make```.
For full list of makefile functionality run ```make help```.
Once built, to run code run ```./consensus```.
For full list of command line arguments and options run ```./consensus -h```.
To animate run ```./consensus -a -o "your-output-directory"``` in one terminal,
then ```gnuplot -e "filename='your-output-directory/lattice.dat' animate.gp```
where your-output-directory is the name of a user defined directory which the
program will create and output its data to. Note that if you use the same directory
name more than once the program will try and create a new unique directory so
as not to overwrite it - this is glitchy so I suggest either naming a new directory
each time you animate it, or removing the old directory first then using the same name.
