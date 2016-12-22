//========================================================================
// mePed sample code - Scott Pierce - scott@spierce.com - spiercetech.com
//========================================================================
#include <Servo.h>

#define NUM_LEGS 4
#define NUM_SERVOS 8
#define DELAY_TIME 100
#define INCREMENT 5
#define LEG_NUM_STEPS 8

byte servoPins[] = {2, 3, 4, 5, 6, 7, 8, 9};

struct legStruct
{
  byte    pinPivot;
  byte    pinLift;
  byte    curPivot;
  byte    curLift;
  byte    minPivot;
  byte    minLift;
  byte    maxPivot;
  byte    maxLift;
  byte    cntrPivot;
  byte    cntrLift;
  byte    homePivot;
  byte    homeLift;
  boolean revPivot;
  boolean revLift;
  boolean backLeg;
  
  Servo   Pivot;
  Servo   Lift;  
};//legStruct

legStruct leg[NUM_LEGS];

struct legPositions
{
  int Pivot;
  int Lift;
};//legPositions

legPositions legFPos[LEG_NUM_STEPS];
legPositions legBPos[LEG_NUM_STEPS];

//================================================================================

void setup()
{

  
  Serial.begin(57600);
  
  for(int i=0; i < NUM_LEGS; i++)
  {
    int servoNum = i*2;
    
    leg[i].pinPivot  = servoPins[servoNum];
    leg[i].pinLift = servoPins[servoNum+1];
    
    leg[i].curPivot  = 0;
    leg[i].curLift   = 0;
    leg[i].minPivot  = 25;
    leg[i].minLift   = 90;  //0;
    leg[i].maxPivot  = 125;
    leg[i].maxLift   = 0;  //100;
    leg[i].cntrPivot = 75; //((leg[i].maxPivot - leg[i].minPivot)/2)+leg[2].minPivot;
    leg[i].cntrLift  = 0; //((leg[i].maxLift - leg[i].minLift)/2)+leg[2].minPivot;
    leg[i].homePivot = leg[i].cntrPivot;
    leg[i].homeLift  = leg[i].cntrLift;
    leg[i].revPivot  = false;
    leg[i].revLift   = false;
    leg[i].backLeg   = false;

    leg[i].Pivot.attach(leg[i].pinPivot);
    leg[i].Lift.attach(leg[i].pinLift);

    leg[i].Pivot.write(leg[i].homePivot);
    leg[i].Lift.write(leg[i].homeLift);

    Serial.println("=================================");
    Serial.print("Leg ");
    Serial.println(i);
    
    Serial.print(" - Pivot Pin = ");
    Serial.println(leg[i].pinPivot);    

    Serial.print(" - Lift Pin = ");
    Serial.println(leg[i].pinLift);    
    
    Serial.print(" - minPivot = ");
    Serial.println(leg[i].minPivot);    

    Serial.print(" - minLift = ");
    Serial.println(leg[i].minLift);    
    
    Serial.print(" - maxPivot = ");
    Serial.println(leg[i].maxPivot);    

    Serial.print(" - maxLift = ");
    Serial.println(leg[i].maxLift);    

    Serial.print(" - curPivot = ");
    Serial.println(leg[i].curPivot);    

    Serial.print(" - curLift = ");
    Serial.println(leg[i].curLift);    
    
    Serial.print(" - cntrPivot = ");
    Serial.println(leg[i].cntrPivot);
    
    Serial.print(" - cntrLift = ");
    Serial.println(leg[i].cntrLift);
    
    Serial.print(" - homePivot = ");
    Serial.println(leg[i].homePivot);

    Serial.print(" - homeLift = ");
    Serial.println(leg[i].homeLift);    
    
    Serial.print(" - revPivot = ");
    Serial.println(leg[i].revPivot);

    Serial.print(" - revLift = ");
    Serial.println(leg[i].revLift);    
    
    Serial.println("");
  }//for
  
  leg[0].revPivot = true;  
  leg[1].revPivot = true;
  
  leg[1].backLeg  = true;
  leg[2].backLeg  = true;  

  //Angle Reletive to Center Position
  legFPos[0].Pivot = 25;
  legFPos[1].Pivot = 0;
  legFPos[2].Pivot = -25;
  legFPos[3].Pivot = -50;
  legFPos[4].Pivot = -50;
  legFPos[5].Pivot = -25;
  legFPos[6].Pivot = 0;
  legFPos[7].Pivot = 25;
  
  //Angle Reletive to Center Position
  legBPos[0].Pivot = 50;
  legBPos[1].Pivot = 25;
  legBPos[2].Pivot = 0;
  legBPos[3].Pivot = -25;
  legBPos[4].Pivot = -25;
  legBPos[5].Pivot = 0;
  legBPos[6].Pivot = 25;
  legBPos[7].Pivot = 50;
  
  int lower = 20;
  int lift = -20;
  
  //Angle Reletive to Center Position
  legFPos[0].Lift = lower;
  legFPos[1].Lift = lower;
  legFPos[2].Lift = lower;
  legFPos[3].Lift = lower;
  legFPos[4].Lift = lift;
  legFPos[5].Lift = lift;
  legFPos[6].Lift = lift;
  legFPos[7].Lift = lift;

  //Angle Reletive to Center Position
  legBPos[0].Lift = lower;
  legBPos[1].Lift = lower;
  legBPos[2].Lift = lower;
  legBPos[3].Lift = lower;
  legBPos[4].Lift = lift;
  legBPos[5].Lift = lift;
  legBPos[6].Lift = lift;
  legBPos[7].Lift = lift;

  leg[0].cntrPivot = 85;
  leg[1].cntrPivot = 85;
  leg[2].cntrPivot = 80;
  leg[3].cntrPivot = 70;
  
}//Setup

//================================================================================

void loop()
{
  //wait 3 seconds before starting to walk
  delay(3000);
  
  while(1==1)
  {
    if ( Serial.available())  
    {
      char ch = Serial.read();
      if(ch == 'a')
        adjustServos();
      
    }//if Serial
    
    testWalk();
    
  }//while
}//loop

//================================================================================

void testWalk()
{
  int pos;
  int posNum;
  int incNum = LEG_NUM_STEPS / 4;
  int revLag = 10;
  
  
  for (int i; i<LEG_NUM_STEPS; i++)
  {
    
    //===== Leg 0 =================================
    
    if(leg[0].backLeg)
      pos = legBPos[i].Pivot;
    else
      pos = legFPos[i].Pivot;
      
    if(leg[0].revPivot)
      leg[0].Pivot.write(leg[0].cntrPivot - pos + revLag);
    else
      leg[0].Pivot.write(leg[0].cntrPivot + pos);
      
    leg[0].Lift.write(leg[0].cntrLift + legFPos[i].Lift);

    //===== Leg 1 =================================
    posNum = getNext(i+(incNum*2));

    if(leg[1].backLeg)
      pos = legBPos[posNum].Pivot;
    else
      pos = legFPos[posNum].Pivot;
    
    if(leg[1].revPivot)
      leg[1].Pivot.write(leg[1].cntrPivot - pos + revLag);
    else
      leg[1].Pivot.write(leg[1].cntrPivot + pos);
      
    leg[1].Lift.write(leg[1].cntrLift + legFPos[posNum].Lift);
    
    //===== Leg 2 =================================    
    posNum = getNext(i+incNum);
    
    if(leg[2].backLeg)
      pos = legBPos[posNum].Pivot;
    else
      pos = legFPos[posNum].Pivot;
    
    if(leg[2].revPivot)
      leg[2].Pivot.write(leg[2].cntrPivot - pos + revLag);
    else
      leg[2].Pivot.write(leg[2].cntrPivot + pos);
      
    leg[2].Lift.write(leg[2].cntrLift + legFPos[posNum].Lift);

    //===== Leg 3 =================================
    posNum = getNext(i+(incNum*3));
    
    if(leg[3].backLeg)
      pos = legBPos[posNum].Pivot;
    else
      pos = legFPos[posNum].Pivot;
    
    if(leg[3].revPivot)
      leg[3].Pivot.write(leg[3].cntrPivot - pos + revLag);
    else
      leg[3].Pivot.write(leg[3].cntrPivot + pos);
      
    leg[3].Lift.write(leg[3].cntrLift + legFPos[posNum].Lift);
    
    
    
    delay(DELAY_TIME);
  }//for i
  
}//testWalk

//================================================================================

int getNext(int val)
{
  int newVal = val;
  
  if(val == 8)
    newVal = 0;
  else if(val == 9)
    newVal = 1;
  else if(val == 10)
    newVal = 2;
  else if(val == 11)
    newVal = 3;
  else if(val == 12)
    newVal = 4;
  else if(val == 13)
    newVal = 5;
    
  return newVal; 
  
}//getNext

//================================================================================

void testLeg(int legNum)
{
  for (int i; i<LEG_NUM_STEPS; i++)
  {
    leg[legNum].Pivot.write(leg[legNum].cntrPivot + legFPos[i].Pivot);
    leg[legNum].Lift.write(leg[legNum].cntrLift + legFPos[i].Lift);
    delay(DELAY_TIME);
  }//for
}//testLeg

//================================================================================

void demo1()
{
  for(int i = leg[0].minLift; i <= leg[0].maxLift; i++)
  {
    leg[0].Lift.write(i);
    leg[1].Lift.write(i);
    leg[2].Lift.write(i);
    leg[3].Lift.write(i);
    Serial.println(i);
    delay(DELAY_TIME);
  }//for

  for(int i = leg[0].maxLift; i >= leg[0].minLift; i--)
  {
    leg[0].Lift.write(i);
    leg[1].Lift.write(i);
    leg[2].Lift.write(i);
    leg[3].Lift.write(i);
    Serial.println(i);
    delay(DELAY_TIME);
  }//for

  for(int i = leg[0].minPivot; i <= leg[0].maxPivot; i++)
  {
    leg[0].Pivot.write(i);
    leg[1].Pivot.write(i);
    leg[2].Pivot.write(i);
    leg[3].Pivot.write(i);
    Serial.println(i);
    delay(DELAY_TIME);
  }//for

  for(int i = leg[0].maxPivot; i >= leg[0].minPivot; i--)
  {
    leg[0].Pivot.write(i);
    leg[1].Pivot.write(i);
    leg[2].Pivot.write(i);
    leg[3].Pivot.write(i);
    Serial.println(i);
    delay(DELAY_TIME);
  }//for
}//demo1

void adjustServos()
{
  int exit = 0;
  byte curLeg = 0;
  byte curPos = 0;

  while(!exit)
  {
    if ( Serial.available())  
    {
      char ch = Serial.read();
    
      switch(ch){
        case '8':
          curPos = leg[curLeg].Lift.read();
          if(curPos+1 > 179)
          {
            Serial.println("You can't turn it up anymore (you might damage it!");
          }
          else
          {
            leg[curLeg].Lift.write(++curPos);
            Serial.print("Lift: ");
            Serial.println(curPos);
            delay(100);
          }
          break;
    
        case '2':
          curPos = leg[curLeg].Lift.read();
          if(curPos == 0)
          {
            Serial.println("You can't turn it up anymore (you might damage it!");
          }
          else
          {
            leg[curLeg].Lift.write(--curPos);
            Serial.print("Lift: ");
            Serial.println(curPos);
            delay(100);
          }
          break;
    
        case '4':
          curPos = leg[curLeg].Pivot.read();
          if(curPos == 0)
          {
            Serial.println("You can't turn it up anymore (you might damage it!");
          }
          else
          {
            leg[curLeg].Pivot.write(--curPos);
            Serial.print("Pivot: ");
            Serial.println(curPos);
            delay(100);
          }
          break;
    
        case '6':
          curPos = leg[curLeg].Pivot.read();
          if(curPos+1 > 179)
          {
            Serial.println("You can't turn it up anymore (you might damage it!");
          }
          else
          {
            leg[curLeg].Pivot.write(++curPos);
            Serial.print("Pivot: ");
            Serial.println(curPos);
            delay(100);
          }
          break;
          
        case 'n':
          if(++curLeg > 3)
          {
            curLeg = 3;
            Serial.println("We have reached last servo");
          }
          
          else
          {
            Serial.print("Switched to Leg ");
            Serial.println(curLeg);
          }
          break;
    
        case 'p':
          if(--curLeg < 0)
          {
            curLeg = 0;
            Serial.println("We have reached first servo");
          }
          else
          {
            Serial.print("Switched to Leg ");
            Serial.println(curLeg);
          }
          break;
  
        case 'x':
          leg[curLeg].maxPivot = curPos;
          leg[curLeg].cntrPivot = (leg[curLeg].maxPivot - leg[curLeg].minPivot)/2;
          break;
  
        case 'm':
          leg[curLeg].minPivot = curPos;
          leg[curLeg].cntrPivot = (leg[curLeg].maxPivot - leg[curLeg].minPivot)/2;
          break;
  
        case 'q':
          exit = 1;
          break;      
  
        default:
          Serial.println("Unknown Command... ");
          break;
      }//Switch
    }//if Serial
  }//while  
}//adjustServos

