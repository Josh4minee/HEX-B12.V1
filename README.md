##
<img src="/pictures/hexapod.png" height="500" />

# HEX-B12.V1
HEX-B12.V120 is a custom-designed hexapod using MG996R servos. Its theme is based on the beloved bot B-12 from the popular video game [Stray](https://en.wikipedia.org/wiki/Stray_(video_game)). It is inspired by the Make Your Pet project and is powered by a custom inverse kinematics-based algorithm.

The mechanical part dimensions are inspired by the [Make Your Pet](https://www.makeyourpet.com/) Chipo project; everything else is custom-made by me.

# Information
Consult the following based on your needs:
- [Hexapod](#hexapod)
- [Remote](#remote)
- [Printing](#printing)
- [Wiring diagram](#wiring-diagram)
- [Hexapod Assembly](#hexapod-assembly)
- [Pre-powering on precautions](#pre-powering-on-precautions)
- [Remote Assembly](#remote-assembly)
- [How to use the remote](#how-to-use-the-remote)
- [Sources](#sources)
- [License](#license)

# Hexapod

Why I made this hexapod is because since I was young I have always wanted to build "stuff", making robots was always very appealing to me and this project is simpler than a quadruped yet just as fun for a beginner like me and I hope the people that come across this project like it! It is worth mentionning that this is also a college project hense why I have put so much time and effort on it in such a short ammount of time.

As per the title, this hexapod uses MG996R servos, this decision was made because of cost, any other servo is 3x to 4x the price of these ones.

The power system consists of one big 5200mAh 2S lipo battery for optimal opperation time, three UBECs for stable 7v to 5v conversion and one 10A manual reset fuse.

An arduino nano was used as the brains of this project simply because it was a requirement from my college and it is thanfully fast enough for its use case.

An [Adafruit PCA9685](https://learn.adafruit.com/16-channel-pwm-servo-driver/overview) servo driver was chosen to controll the huge amount fof servos this robot requires.

The nRF24L01+PA+LNA modules were chosen for wireless communication, the power emplified version was used for its antenna (to keep the resemblance of [B-12](https://stray.fandom.com/wiki/B-12)) and for its range.

Finally, relays were added for ease of use and safety, like only turning on the servos 5s after the battery is pluged in.


# Remote

The main reason the remote was made is because I did not want to work with bluetooth or web based control. 

Making a remote seemed more interesting anyways and it allowed me to design my very first custom PCB:

  <img src="/pictures/PCB_front.jpg" height="400" />
  <img src="/pictures/PCB_back.jpg" height="400" />

I ended up making 2 versions because I tried to directly hook up an NRF module on the first version, this ended up failing to I just made a custom footprint for the NRF module power boards.

# Printing

The following are the settings used to print this hexapod:

- 15% grid infill everywhere, 0.28mm height and 3 walls on every part except for the 0.16mm servo shields to keep them smooth.
- Every STL file is pre-oriented and ready to print, the only parts requiring support material are the top plate, the shields, the coxa and femur horn parts, the guides. Any additional supports are not usefull.
- Printing the remote uses the same settings and the files are also pre-oriented, only the mid plate requires supports.
- The number of parts needed to be printed is indicated at the end of the names of every STL file.
- All STL files can be found the the [STL](./STL) folder.

# Wiring diagram

Here you can find the wiring diagram, component placements, and an IRL example of the completed build.
<div>
  <img src="/pictures/hexapod circuit.jpg" height="400" />
  <img src="/pictures/Components_layout.png" height="400" />
  <img src="/pictures/IRL_wiring.jpg" width="500" />
<div>
  
# Hexapod assembly
- [Coxa](./pictures/coxa_page_1.jpg)
- [Femur](./pictures/femur_page_1.jpg)
- [Tibia](./pictures/tibia_page_1.jpg)
- [Leg](./pictures/LEG_left_page_1.jpg)
- [Mid Frame](./pictures/mid_frame_page_1.jpg)
- [Frame and Legs](./pictures/frampluslegs_page_1.jpg)
- [Hexapod](./pictures/hexapod_page_1.jpg)
- [Electronics Mounting](./pictures/electronics_mounting_page_1.jpg)
- [Servo pinouts](./pictures/servo_order.png)

**NOTE: ALL SERVOS MUST BE SET TO 90 PRIOR TO ANY ASSEMBLY**

**NOTE: KEEP IN MIND THAT 3 OF THESE LEGS ARE FLIPPED**

**NOTE: DO NOT FORGET THE 18 FRICTION CAP RINGS**

The above document provides a visual on how parts are assembled, to assemble this in real life you must follow the [Frame and Legs](./pictures/frampluslegs_page_1.jpg) and [Hexapod](./pictures/hexapod_page_1.jpg) technical drawings. The order of operations is very important here, so making sure proper understanding is acquired before continuing is mandatory.

For screw sizes needed:
- Every screw needed to attach a servo is M3 35mm.
- Every screw needed to clamp together femur/coxa parts is M3 25mm.
- Every screw that holds down the friction caps is M2 10mm.
- Every other screw is M2 8mm.
- Follow [this](./pictures/Hexapod_Heated_Insert_placements.png) diagram for M2 heat insert assembly.

Follow [this](./pictures/leg_orientations.png) technical drawing for proper leg orientation assembly.

It is very recommended to apply sillicone based grease between the friction caps and legs to prevent plastic wear.

# Pre-powering on precautions

- Make sure the legs of the hexapod are clear of any obstacles.
- Make sure all the legs are **RESTING** on the ground, the hexapod should **NOT** be standing.
- After plugging in the battery, the hexapod will enter a 10 second timer before entering its start animation.

# Remote assembly

Start by buying the PCB on [JLCPCB.](https://cart.jlcpcb.com/quote?stencilLayer=2&stencilWidth=100&stencilLength=100&stencilCounts=5&plateType=1&spm=Jlcpcb.Homepage.1010) by simply drag and droping the [gerber](PCB/HEX_remote_GERBERS.zip) files.

Once you have received and soldered your PCB, follow the diagram to assemble the remote, every screw used is M2 8mm.

  <img src="/pictures/HEX_remote_assembly.jpg" height="400" />

# How to use the remote

The remote has 3 buttons, 2 knobs, and a joystick.
The lower knob is for global height offset, the upper knob is used to control which mode the user is in, and the 3 buttons are for 3 different states in these modes:

- Normal walking mode: the hexapod turns and follows the joystick’s direction. Each button selects the current walking GAIT: 1 for tripod, 2 for ripple, and 3 is actually an Easter egg.

- Crab mode: the hexapod follows joystick directions without ever turning its body. The GAIT selection is the same as above.

- Body control mode: here you can control the hexapod body. Button 1 is for planar offset mode, button 2 is for XY rotation offset mode, and button 3 is for Z rotation mode.

- Animation mode: in this mode, 3 animations can be selected using the 3 buttons.

# Sources

Huge thanks to to [MYP](https://www.makeyourpet.com/) for the inspiration and just overall help their discord community provided to this project!

Here is a list of every model I used from GrabCad to display electronic components and screws in my assem,blies:

- [All M3-M2 screws](https://grabcad.com/library/m2-metric-socket-cap-head-screws-1)
- [M3 square nuts](https://www.traceparts.com/en/product/din-square-nut?CatalogPath=TRACEPARTS%3ATP01001007016&Product=10-11062001-225045)
- [NRF anttenna module and powe board](https://github.com/KrishnaHariharan/NRF24-Breakout-Board)
- [NRF anttenna module and powe board](https://github.com/Ahsayn/nRF24-Breakout)
- [MG996R servo & servo horns](https://grabcad.com/library/mg996r-servo-3)
- [UBEC](https://grabcad.com/library/ubec-5v-5a-1)
- [Lipo alarm](https://grabcad.com/library/1-8s-lipo-li-ion-fe-battery-voltage-2in1-tester-1)
- [PCA servo driver](https://grabcad.com/library/pca9685-8)
- [Double relay](https://grabcad.com/library/2ph63091a-2-channel-5v-relay-module-1)

# License
[License](LICENSE)
