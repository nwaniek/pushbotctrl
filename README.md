# Basic control infrastructure for eDVS (embedded Dynamic Vision Sensor) equipped PushBot robots. #

## TODO ##
* port edvs and cmd and control interfaces  ... DONE
* add emergency stop (bypass) user function is failing ... DONE
* add simple command interface (e.g. keys and mouse / gamepad) ...
* single event vs. event frame as input for user functions (events only)
* sync inertial sensory data (i.e. gyro, acc, mag) with events and provide only high-level info (i.e. RPY information)
* Linux and mswin compatibility using Qt5 and pack all needed library in stand-alone deliverable

![pbrc](doc/pbrc-2015-08-21.png)

```
#!c++
                                                                                                            +--------------------------------------------------------+
                                                                                                             |    PushBot Control Interface                           |
                                                                                                             |                                                        |
                                                                                                             |    +--------------------------------------------+      |
              XXX                                                                                            |    | Sensory Data Parser                        |      |
            XX   XX                                                                                          |    |                     +                      |      |
          XX       XX                Sensory Data             XXXXXXXXXX  XXXXXX           Sensory Data      |    |       eDVS          |    Inertial data     |      |
         +   XXXXX   +          +-------------------->        X         XXX    X         +-------------->    |    |                     |  Gyro, Acc, Magneto  |      |
         |   X   X   |                                        X                XXXXXX                        |    +---------+-----------+-----------+----------+      |
         |   XXXXX   |                                      XXX                X    XX                       |              |                       |                 |
         |           |                                    XXX                       XX                       | E^ent Frames v                       v  Roll+Pitch+Yaw |
         |           |  XXXXXXXX                          XX                     XXXX                        |                                             Angles     |
         |           |       XXXX                          XX        WIFI       XXXXXX                       |        +--------------------------------------+        |
         |           |      XXX XX                       XXX                         XX                      |        |          User Function               |        |
         |           |    XXXX   X                       X                         XXX                       |        +-----------------+--------------------+        |
         |           |   XX X    X                       XXX                    XXXX                         |                          |                             |
        X+-----------+XXXXX X    X                         XXXX                   XX                         |                          v                             |
     XX              XXX   X X   X                            XX       XXX        XX                         |                                                        |
   +-----------------+ X   X XXXX        Motor Commands        XXX      XXXXXXXXXXX        Motor Commands    |    +--------------------------------------------+      |
  X                  X X   XX       <---------------------+       XXXXXXX                <---------------+   |    |  Motor Command Block                       |      |
 X      NST          X  XXX                                                                                  |    |                     +                      |      |
X                   X                           +----+                                      User Input       |    |  Function Control   |   User Input Control |      |
+-------------------+                           |    |                                   +--------------->   |    |       Signal        |         Signal       |      |
                                                |    |                                                       |    +---------------------+----------------------+      |
                                                |    |                                                       |                                                        |
                                                |    |                                                       +--------------------------------------------------------+
                                                |    |
                                                |    |                                 +------+
                                                |    |                                 |      |
                                                |    |                                 |   ^  |
                                                |    |                                 |   |  |
                                            +-- |    | ------------+           +----------------------+
                                           X    |    |           XXX           |       |   |  |       |
                                          X     |    |          X  X           |   <---+   v  +---->  |
                                         X      +XXXX+         X   X           |       |      |       |
                                        X         X           X    X           +-------+------+-------+
                                       X       XXXXXX        XX   XX
                                      X       XX    XX      XX   XX                  Keyboard
                                     X                     XX   XX
                                    X                      X    X
                                    +----------------------+   X
                                    X                      X  X
                                    X                      XXX
                                    +----------------------+

                                          Joystick / Gamepad
```
