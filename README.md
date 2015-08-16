
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