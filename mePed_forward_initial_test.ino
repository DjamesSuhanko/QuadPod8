#include <Servo.h>

//Define 8 Servos
Servo myServo1; // Front Left Pivot Servo
Servo myServo2; // Front Left Lift Servo
Servo myServo3; // Back Left Pivot Servo
Servo myServo4; // Back Left Lift Servo
Servo myServo5; // Back Right Pivot Servo
Servo myServo6; // Back Right Lift Servo
Servo myServo7; // Front Right Pivot Servo
Servo myServo8; // Front Right Lift Servo

void setup()
{
  // Attach servos to Arduino Pins
  myServo1.attach(2);
  myServo2.attach(3);
  myServo3.attach(4);
  myServo4.attach(5);
  myServo5.attach(6);
  myServo6.attach(7);
  myServo7.attach(8);
  myServo8.attach(9);

  Serial.begin(9600);  // Set Serial Baud Rate/Speed
}//setup

// calibration
int da =  0 - 30,
    db =  0 + 30,
    dc =  0 - 30,
    dd =  0 + 30;

// servo initial positions + calibration
int a90  = (90  + da),
    a120 = (120 + da),
    a150 = (150 + da),
    a180 = (180 + da);

int b0   = (0   + db),
    b30  = (30  + db),
    b60  = (60  + db),
    b90  = (90  + db);

int c90  = (90  + dc),
    c120 = (120 + dc),
    c150 = (150 + dc),
    c180 = (180 + dc);

int d0   = (0   + dd),
    d30  = (30  + dd),
    d60  = (60  + dd),
    d90  = (90  + dd);

// start points for servo
int s11 = 90; // Front Left Pivot Servo
int s12 = 90; // Front Left Lift Servo
int s21 = 90; // Back Left Pivot Servo
int s22 = 90; // Back Left Lift Servo
int s31 = 90; // Back Right Pivot Servo
int s32 = 90; // Back Right Lift Servo
int s41 = 90; // Front Right Pivot Servo
int s42 = 90; // Front Right Lift Servo

int f    = 0;
int b    = 0;
int l    = 0;
int r    = 0;
int spd  = 25;  // Speed of walking motion
int high = 0;   // How high the robot is standing

//== Loop ==============================================================================

void loop()
{
  int comando = 1;
  if (comando)
  {
    if (comando == 1)
      forward();

    if (comando == 2)
      back();

    if (comando == 3)
      turn_left();

    if (comando == 4)
      turn_right();
  }

  else
    srv(a180,b0,c120,d60,42,33,33,42,1,1,1,1);

  // speed slider display
  spd = 25;
  spd = map(spd,0,100,25,5);
  spd = constrain(spd,5,25);

  // high slider display
  high = 20;
  high = map(high,0,100,1,15);
  high = constrain(high,1,15);

  itoa (map(spd,5,25,25,5) , "a", 10);
  itoa (high, "b", 10);

}//loop

//== Forward ==============================================================================

void forward()
{
  // calibration
  da = -6-6;
  db = 6+24;
  dc = 6-24;
  dd = 6+6;

  // calculation of points
  a90=(90+da), a120=(120+da), a150=(150+da), a180=(180+da);
  b0=(0+db),   b30=(30+db),  b60=(60+db),   b90=(90+db);
  c90=(90+dc), c120=(120+dc), c150=(150+dc), c180=(180+dc);
  d0=(0+dd),   d30=(30+dd),  d60=(60+dd),   d90=(90+dd);

  // set servo positions and speeds needed to walk forward one step
  srv(a180, b0 ,c120,  d60, 42, 33, 33, 42, 1, 3, 1, 1);
  srv( a90, b30, c90,  d30, 6,  33, 33, 42, 3, 1, 1, 1);
  srv( a90, b30, c90,  d30, 42, 33, 33, 42, 3, 1, 1, 1);
  srv(a120, b60, c180, d0,  42, 33, 6,  42, 1, 1, 3, 1);
  srv(a120, b60, c180, d0,  42, 33, 33, 42, 1, 1, 3, 1);
  srv(a150, b90, c150, d90, 42, 33, 33, 6,  1, 1, 1, 3);
  srv(a150, b90, c150, d90, 42, 33, 33, 42, 1, 1, 1, 3);
  srv(a180, b0,  c120, d60, 42, 6,  33, 42, 1, 3, 1, 1);
}

//== Back ==================================================================================

void back ()
{
  // calibration
  da=-6-30, db=6+6, dc=6+6, dd=6+30;

  // calculation of points
  a90=(90+da), a120=(120+da), a150=(150+da), a180=(180+da);
  b0=(0+db),   b30=(30+db),  b60=(60+db),   b90=(90+db);
  c90=(90+dc), c120=(120+dc), c150=(150+dc), c180=(180+dc);
  d0=(0+dd),   d30=(30+dd),  d60=(60+dd),   d90=(90+dd);

  // set servo positions and speeds needed to walk backward one step
  srv(a180, b0,  c120, d60, 42, 33, 33, 42, 3,  1, 1, 1);
  srv(a150, b90, c150, d90, 42, 18, 33, 42, 1,  3, 1, 1);
  srv(a150, b90, c150, d90, 42, 33, 33, 42, 1,  3, 1, 1);
  srv(a120, b60, c180, d0,  42, 33, 33, 6,  1,  1, 1, 3);
  srv(a120, b60, c180, d0,  42, 33, 33, 42, 1,  1, 1, 3);
  srv(a90,  b30, c90,  d30, 42, 33, 18, 42, 1,  1, 3, 1);
  srv(a90,  b30, c90,  d30, 42, 33, 33, 42, 1,  1, 3, 1);
  srv(a180, b0,  c120, d60, 6,  33, 33, 42, 3,  1, 1, 1);
}

//== Left =================================================================================

void turn_left ()
{
  srv(a150,  b90, c90,  d30, 42, 6,  33, 42, 1, 3, 1, 1);
  srv(a150,  b90, c90,  d30, 42, 33, 33, 42, 1, 3, 1, 1);
  srv(a120,  b60, c180, d0,  42, 33, 6,  42, 1, 1, 3, 1);
  srv(a120,  b60, c180, d0,  42, 33, 33, 24, 1, 1, 3, 1);
  srv(a90,   b30, c150, d90, 42, 33, 33, 6,  1, 1, 1, 3);
  srv(a90,   b30, c150, d90, 42, 33, 33, 42, 1, 1, 1, 3);
  srv(a180,  b0,  c120, d60, 6,  33, 33, 42, 3, 1, 1, 1);
  srv(a180,  b0,  c120, d60, 42, 33, 33, 33, 3, 1, 1, 1);
}

//== Right ================================================================================

void turn_right ()
{
  srv( a90, b30, c150, d90, 6,  33, 33, 42, 3, 1, 1, 1);
  srv( a90, b30, c150, d90, 42, 33, 33, 42, 3, 1, 1, 1);
  srv(a120, b60, c180, d0,  42, 33, 33, 6,  1, 1, 1, 3);
  srv(a120, b60, c180, d0,  42, 33, 33, 42, 1, 1, 1, 3);
  srv(a150, b90, c90,  d30, 42, 33, 6,  42, 1, 1, 3, 1);
  srv(a150, b90, c90,  d30, 42, 33, 33, 42, 1, 1, 3, 1);
  srv(a180, b0,  c120, d60, 42, 6,  33, 42, 1, 3, 1, 1);
  srv(a180, b0,  c120, d60, 42, 33, 33, 42, 1, 3, 1, 1);
}

//== Srv ==================================================================================

void srv( int  p11, int p21, int p31, int p41, int p12, int p22, int p32, int p42, int sp1, int sp2, int sp3, int sp4)
{

  // p11: Front Left Pivot Servo
  // p21: Back Left Pivot Servo
  // p31: Back Right Pivot Servo
  // p41: Front Right Pivot Servo
  // p12: Front Left Lift Servo
  // p22: Back Left Lift Servo
  // p32: Back Right Lift Servo
  // p42: Front Right Lift Servo
  // sp1: Speed 1
  // sp2: Speed 2
  // sp3: Speed 3
  // sp4: Speed 4

  // Multiply lift servo positions by manual height adjustment
  p12 = p12 + high * 3;
  p22 = p22 + high * 3;
  p32 = p32 + high * 3;
  p42 = p42 + high * 3;

  while((s11 != p11) || (s21 != p21) || (s31 != p31) || (s41 != p41) || (s12 != p12) || (s22 != p22) || (s32 != p32) || (s42 != p42))
  {
    // Front Left Pivot Servo
    if (s11 < p11)        // if start position is less than programmed position
      s11 = s11 + sp1;    // set start position equal to start position plus speed constant
    if (s11 > p11)        // if start position is greater than programmed position
      s11 = s11 - sp1;    // set start position equal to start position minus speed constant

    // Back Left Pivot Servo
    if (s21 < p21)
      s21 = s21 + sp2;
    if (s21 > p21)
      s21 = s21 - sp2;

    // Back Right Pivot Servo
    if (s31 < p31)
      s31 = s31 + sp3;
    if (s31 > p31)
      s31 = s31 - sp3;

    // Front Right Pivot Servo
    if (s41 < p41)
      s41 = s41 + sp4;
    if (s41 > p41)
      s41 = s41 - sp4;

    // Front Left Lift Servo
    if (s12 < p12)
      s12 = s12 + sp1;
    if (s12 > p12)
      s12 = s12 - sp1;

    // Back Left Lift Servo
    if (s22 < p22)
      s22 = s22 + sp2;
    if (s22 > p22)
      s22 = s22 - sp2;

    // Back Right Lift Servo
    if (s32 < p32)
      s32 = s32 + sp3;
    if (s32 > p32)
      s32 = s32 - sp3;

    // Front Right Lift Servo
    if (s42 < p42)
      s42 = s42 + sp4;
    if (s42 > p42)
      s42 = s42 - sp4;

    // Write Pivot Servo Values
    myServo1.write(s11);
    myServo3.write(s21);
    myServo5.write(s31);
    myServo7.write(s41);

    // Write Lift Servos Values
    myServo2.write(s12);
    myServo4.write(s22);
    myServo6.write(s32);
    myServo8.write(s42);

    delay(spd); // Delay before next movement

  }//while
} //srv
