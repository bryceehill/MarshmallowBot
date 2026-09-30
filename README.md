# MarshmallowBot
Files and code to build the Marshmallow Bot.  Launches a marshmallow via a mouse trap.

List of Components:
1x ESP32 C3 Super Mini
1x ESP32 C3 Super Mini Expansion Board  (Can be purchased with the ESP32 C3 Super Mini)
2x Continuous rotation micro servo with wheel
1x 4-LED battery clip
1x Servo Wire Connector
1x #8 screw 2" long and nut
1x Micro servo (optional)
1x Standard Mouse Trap (optional)
2x #8 tapered head screws 3/4" long and nut

Parts to 3D print
1x  Chassis
1x  Tail Wheel
1x  Mouse Trap launcher holder (optional)

Intructions
Part 1.  Mounting hardware
Place the two continuous rotation servos in the left and right wheel locations of the chassis. Hopefully this is obvious.  Mount them with the pivot high.  Use one of the longer screws to hold the servo in place.
Mount the two wheels on the two continuous rotation servos.
Use the 2" #8 screw to mount the tail wheel on the robot
Place the battery clip in the appropriate slot

Part 2.  Soldering
Cut the servo wire connector and strip the wire on the middle wire and one of the two outside wires.  Discard the other outside wire.  
Solder or crimp the red wire of the battery holder to the outside wire of the servo connector.  
Solder or crimp the black wire of the battery older to the middle wire of the servo connector.
If possible use heat-shrink tubing to cover the solder joint.  Electricians tape is appropriate as well, but certainly not as cool.

Part 3.  Connecting the components.
Dock the ESP32 C3 Super Mini to the expansion board.  It can go on backwards.  The USB port needs to be on the side of the white connector of the expansion board.
Read the instructions in this repository on how to use the marshmallow bot.  Left Servo connector via the images shown, and right servo connector via the images.

Part 4.  Programming
Download Arduino IDE to whatever computer you have.
Download the MarshmallowBot.ino file to your computer and open it with Arduino IDE
Open the "Boards Manager" and search for ESP32 boards.  Install the one by "Espressif"
Plug the usb port into the computer and the ESP32 C3
Press the "Upload" button

Part 5.  Marshmallow Launcher (optional)
Mount the 3D printed marshmallow launcher on the chassis via the two #8 tapered screws.
Place the standard micro servo in the holder on the launcher.  The pivot should be on the low side this time.  Use a screw to secure it to the structure.
Place the mouse trap in the holder.  It will be a challenge to pull it back partially.
Before putting the horn on the servo connect the servo to the ESP32 C3 Super Mini Expansion board
Use the instructions to know how to power the board.  You need to connect it correctly otherwise you will either melt components or wires.  Pay close attention to the instructions.
You need to put the horn on the correct angle.  With the board powered and programmed, connect a wifi connected device to the wifi networked called "MarshmallowBot" 
Use the password 12345678 to gain access.
Point the browser on that device to 192.168.4.1  (Put it in the address bar)
At this point you should have an interface to control the bot.
Use the "fire" button to move the trigger servo.  Install the horn so that it will press the trigger of the mouse trap.  Screw it in.
Now you get to build your own method to launch the marshmallow.  Be creative.  You can 3D print, or use a plastic spoon, or popscicle sticks.  Duct tape is helpful.  
Place a marshmallow on your launcher and set the trigger mechanism.  Drive and shoot.



Links to hardware that's compatible
