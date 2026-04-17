##
<img src="/pictures/hexapod.png" height="500" />

# HEX-B12.V1
HEX-B12.V120 is a custom designed hexapod using mg996r servos and has it's theme based on the beloved bot B-12 from the popular video game STRAY, it is inspired by the Make Your Pet project and it is powered with a custom inverse kinematics based algorithm. 
(the mechanical part dimensions are inspired by the [Make Your Pet](https://www.makeyourpet.com/) Chipo project, everything else is custom and made by me).
#
# Information
Consult the following based on your needs:
- [Wiring diagram](#wiring-diagram)
- [Hexapod Assembly](#hexapod-assembly)
- [Pre-powering on precautions](#pre-powering-on-precautions)
- [Remote Assembly](#remote-assembly)
- [How to use the remote](#how-to-use-the-remote)
- [Sources](#sources)
- [License](#license)

# Wiring diagram

Here you can find the wiring diagram, the component positions and the real world example of how it looks once completed.
  <img src="/pictures/hexapod circuit.jpg" height="350" />
  <img src="/pictures/Components_layout.png" height="350" />
  <img src="/pictures/IRL_wiring.jpg" height="400" />
  
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

**NOTE: KEEP IN MIND THAT 3 OF THESE LEGS MUST BE FLIPPED**

The above document prodive a visual on how parts are assembled, however, to assemble this in real life you must follow the [Frame and Legs](./pictures/frampluslegs_page_1.jpg) and [Hexapod](./pictures/hexapod_page_1.jpg) technical drawings. The order of operations is crucial here so making sure proper understanding is aquired before continuing is crucial.

For screw sizes needed:
- Every screw needed to attach a servo is M3 35mm.
- Every screw needed to clamp together femur/coxa parts is M3 25mm.
- Every screw that holds down the friction caps is M2 10mm.
- Every other screw is M2 8mm.
- Follow [this](./pictures/Hexapod_Heated_Insert_placements.png) diagram for M2 heat insert assembly.

Follow [this](./pictures/leg_orientations.png) technical drawing for proper leg orientation assembly.

# Remote assembly

# How to use the remote

The remote has 3 button, 2 knobs and a joystick.
The lower knob is for global height offset, the upper knob is used to control what mode the user is on, and the 3 buttons are for 3 different states in these modes:
- Normal walking mode, the hexapod turns and follows the joysticks direction, each button selects the current walking GAIT, 1 for tripod, 2 for wripple and 3 is actually an easter egg.
- Crab mode, the hexapod follows joystick directions with out ever turning its body, the GAIT selection is the same as above.
- Body Control Mode, here you can controll the hexapod body. Button 1 is for planar offset mode, button 2 is for XY rotation offset mode and button 3 is for Z rotation mode.
- Animation Mode, in this mode, 3 animations can be selected using the 3 buttons.

# Sources

# License
