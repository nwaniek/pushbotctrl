
# Basic control infrastructure for eDVS (embedded Dynamic Vision Sensor) equipped PushBot robots. #

                                                                                                                 +--------------------------------------------------------+
                                                                                                                 |    PushBot Control Interface                           |
                                                                                                                 |                                                        |
                                                                                                                 |    +--------------------------------------------+      |
                 XXXX                                                                                            |    | Sensory Data Parser                        |      |
                XX  XXX                                                                                          |    |                     +                      |      |
              XX       XXX               Sensory Data             XXXXXXXXXX  XXXXXX           Sensory Data      |    |       eDVS          |    Inertial data     |      |
             XX  XXXXX   X          +-------------------->        X         XXX    X         +-------------->    |    |                     |  Gyro, Acc, Magneto  |      |
             X   X   X   X                                        X                XXXXXX                        |    +---------+-----------+-----------+----------+      |
             X   XXXXX   X                                      XXX                X    XX                       |              |                       |                 |
             X           X                                    XXX                       XX                       | Event Frames v                       v  Roll+Pitch+Yaw |
             X    XXXXXXXXXXXXXXXXXXX                         XX                     XXXX                        |                                             Angles     |
             X   XX      X       XXXX                          XX        WIFI       XXXXXX                       |        +--------------------------------------+        |
             X XX        X      XXX XX                       XXX                         XX                      |        |          User Function               |        |
             XXX         X    XXXX   X                       X                         XXX                       |        +-----------------+--------------------+        |
           XXX           X   XX X    X                       XXX                    XXXX                         |                          |                             |
          XX XXXXXXXXXXXXX XXXX XX   X                         XXXX                   XX                         |                          v                             |
         XX              XXX  XX XXXXXX                           XX       XXX        XX                         |                                                        |
   XXXXXXXXXXXXXXXXXXXXXXXX    X  XXX        Motor Commands        XXX      XXXXXXXXXXX        Motor Commands    |    +--------------------------------------------+      |
  XX                     XXX  XXXXX     <---------------------+       XXXXXXX                <---------------+   |    |  Motor Command Block                       |      |
 XX         NST          X XXXXXX                                                                                |    |                     +                      |      |
XX                      X                           XXXXX                                       User Input       |    |  Function Control   |   User Input Control |      |
XXXXXXXXXXXXXXXXXXXXXXXXX                           X   XX                                   +--------------->   |    |       Signal        |         Signal       |      |
                                                    X    X                                                       |    +---------------------+----------------------+      |
                                                    X    X                                                       |                                                        |
                                                    X    X                                                       +--------------------------------------------------------+
                                                    X    X                                                                                                                 
                                                    X    X                                 +------+                                                                        
                                                    X    X                                 |      |                                                                        
                                                    X    X                                 |   ^  |                                                                        
                                                    X    X                                 |   |  |                                                                        
                                              XXXXXXXXXXXXXXXXXXXXXXXXXX           +----------------------+                                                                
                                             X      X   XX           XXX           |       |   |  |       |                                                                
                                             X      X    X          X  X           |   <---+   v  +---->  |                                                                
                                            X       XXXXXX         X   X           |       |      |       |                                                                
                                            X         X           X    X           +-------+------+-------+                                                                
                                           X       XXXXXX        XX   XX                                                                                                   
                                          X       XX    XX      XX   XX                  Keyboard                                                                          
                                         X                     XX   XX                                                                                                     
                                        X                      X    X                                                                                                      
                                        XXXXX XXXX XXXXXXXXXXXXX   X                                                                                                       
                                        X                      X  X                                                                                                        
                                        X                      XXX                                                                                                         
                                        XXXXXXXXXXXXXXXXXXXXXXXXX                                                                                                          
                                                                                                                                                                           
                                              Joystick / Gamepad                                                                                                           


## TODO ##
* port edvs and cmd and control interfaces 
* add emergency stop (bypass) user function if failing
* add simple command interface (e.g. keys and mouse / gamepad)
* single event vs. event frame as input for user functions
* sync inertial sensory data (i.e. gyro, acc, mag) with events and provide only high-level info (i.e. RPY information)
* Linux and mswin compatibility using Qt5 and pack all needed library in stand-alone deliverable


