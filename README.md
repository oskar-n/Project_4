# Project 4 - Elevator simulation
----
### Project Description

<font size="5">The main objective of this project is to simulate a simple elevator. User can create passengers that travel to chosen floors.
 </font>

----
### How to use
<font size="5">User of the program need to first click the numbered button above one of the platforms. Then a human appears on the screen who travels to the floor that was previously selected. Elevator can take up to 8 passengers. The overall weight is displayed on the counter at the top of the screen. After 5 seconds without any user input or action, elevator goes back to first floor.  </font>

----
### Technical description

<font size="5">Project uses SFML library to display a window. Every object has it's own separate class and 'ObjectManager' operates the whole program. 'ObjectManager.loop' is the main loop of the program that contains all of the functions. Floors and elevator have their own containers for humans. Elevator movement is based on a queue of paths that chooses optimal route to drop off passengers.
</font>

---
### Creators
<font size="5">Hubert Rotkiewicz, Oskar Nakielski </font>

