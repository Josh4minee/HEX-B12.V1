##
<img src="/pictures/hexapod.png" height="500" />

# HEX-B12.V1
HEX-B12.V120 is a custom-designed hexapod using MG996R servos. Its theme is based on the beloved bot B-12 from the popular video game [Stray](https://en.wikipedia.org/wiki/Stray_(video_game)). It is inspired by the Make Your Pet project and is powered by a custom inverse kinematics-based algorithm.

The mechanical part dimensions are inspired by the [Make Your Pet](https://www.makeyourpet.com/) Chipo project; everything else is custom-made by me.

# Information
Consult the following based on your needs:
- [Hexapod](#hexapod)
- [Remote](#remote)
- [Wiring diagram](#wiring-diagram)
- [Hexapod Assembly](#hexapod-assembly)
- [Pre-powering on precautions](#pre-powering-on-precautions)
- [Remote Assembly](#remote-assembly)
- [How to use the remote](#how-to-use-the-remote)
- [Sources](#sources)
- [License](#license)

# Hexapod

# Remote


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

**NOTE: ALL SERVOS MUST BE SET TO 90 PRIOR TO ANY ASSEMBLY**

**NOTE: KEEP IN MIND THAT THREE OF THESE LEGS MUST BE FLIPPED**

**NOTE: DO NOT FORGET THE 18 FRICTION CAP RINGS**

The above document provides a visual on how parts are assembled, to assemble this in real life you must follow the [Frame and Legs](./pictures/frampluslegs_page_1.jpg) and [Hexapod](./pictures/hexapod_page_1.jpg) technical drawings. The order of operations is very important here, so making sure proper understanding is acquired before continuing is mandatory.

For screw sizes needed:
- Every screw needed to attach a servo is M3 35mm.
- Every screw needed to clamp together femur/coxa parts is M3 25mm.
- Every screw that holds down the friction caps is M2 10mm.
- Every other screw is M2 8mm.
- Follow [this](./pictures/Hexapod_Heated_Insert_placements.png) diagram for M2 heat insert assembly.

Follow [this](./pictures/leg_orientations.png) technical drawing for proper leg orientation assembly.

# Pre-powering on precautions

- Make sure the legs of the hexapod are clear of any obstacles.
- Make sure all the legs are **RESTING** on the ground, the hexapod should **NOT** be standing.
- After plugging in the battery, the hexapod will enter a 10 second timer before entering its start animation.

# Remote assembly

Follow the diagram to assemble the remote, every screw used is M2 8mm.

  <img src="/pictures/HEX_remote_assembly.jpg" height="400" />

# How to use the remote

The remote has 3 buttons, 2 knobs, and a joystick.
The lower knob is for global height offset, the upper knob is used to control which mode the user is in, and the 3 buttons are for 3 different states in these modes:

- Normal walking mode: the hexapod turns and follows the joystick’s direction. Each button selects the current walking GAIT: 1 for tripod, 2 for ripple, and 3 is actually an Easter egg.

- Crab mode: the hexapod follows joystick directions without ever turning its body. The GAIT selection is the same as above.

- Body control mode: here you can control the hexapod body. Button 1 is for planar offset mode, button 2 is for XY rotation offset mode, and button 3 is for Z rotation mode.

- Animation mode: in this mode, 3 animations can be selected using the 3 buttons.

# Sources

# License
[License](LICENSE)
