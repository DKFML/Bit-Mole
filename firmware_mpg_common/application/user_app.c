/**********************************************************************************************************************
File: user_app.c                                                                

----------------------------------------------------------------------------------------------------------------------
To start a new task using this user_app as a template:
 1. Copy both user_app.c and user_app.h to the Application directory
 2. Rename the files yournewtaskname.c and yournewtaskname.h
 3. Add yournewtaskname.c and yournewtaskname.h to the Application Include and Source groups in the IAR project
 4. Use ctrl-h (make sure "Match Case" is checked) to find and replace all instances of "user_app" with "yournewtaskname"
 5. Use ctrl-h to find and replace all instances of "UserApp" with "YourNewTaskName"
 6. Use ctrl-h to find and replace all instances of "USER_APP" with "YOUR_NEW_TASK_NAME"
 7. Add a call to YourNewTaskNameInitialize() in the init section of main
 8. Add a call to YourNewTaskNameRunActiveState() in the Super Loop section of main
 9. Update yournewtaskname.h per the instructions at the top of yournewtaskname.h
10. Delete this text (between the dashed lines) and update the Description below to describe your task
----------------------------------------------------------------------------------------------------------------------

Description:
This is a user_app.c file template 

------------------------------------------------------------------------------------------------------------------------
API:

Public functions:


Protected System functions:
void UserAppInitialize(void)
Runs required initialzation for the task.  Should only be called once in main init section.

void UserAppRunActiveState(void)
Runs current task state.  Should only be called once in main loop.


**********************************************************************************************************************/

#include "configuration.h"

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32UserAppFlags;                       /* Global state flags */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern AntSetupDataType G_stAntSetupData;                         /* From ant.c */

extern u32 G_u32AntApiCurrentDataTimeStamp;                       /* From ant_api.c */
extern AntApplicationMessageType G_eAntApiCurrentMessageClass;    /* From ant_api.c */
extern u8 G_au8AntApiCurrentData[ANT_APPLICATION_MESSAGE_BYTES];  /* From ant_api.c */

extern volatile u32 G_u32SystemFlags;                  /* From main.c */
extern volatile u32 G_u32ApplicationFlags;             /* From main.c */

extern volatile u32 G_u32SystemTime1ms;                /* From board-specific source file */
extern volatile u32 G_u32SystemTime1s;                 /* From board-specific source file */


/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp_" and be declared as static.
***********************************************************************************************************************/
static fnCode_type UserApp_StateMachine;            /* The state machine function pointer */
static u32 UserApp_u32Timeout;                      /* Timeout counter used across states */


/**********************************************************************************************************************
Function Definitions
**********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Public functions                                                                                                   */
/*--------------------------------------------------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------------------------------------------------*/
/* Protected functions                                                                                                */
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------
Function: UserAppInitialize

Description:
Initializes the State Machine and its variables.

Requires:
  -

Promises:
  - 
*/
void UserAppInitialize(void)
{
  u8 au8WelcomeMessage[] = "ANT Master";

  /* Write a weclome message on the LCD */
#if MPG1
  /* Set a message up on the LCD. Delay is required to let the clear command send. */
  LCDCommand(LCD_CLEAR_CMD);
  for(u32 i = 0; i < 10000; i++);
  LCDMessage(LINE1_START_ADDR, au8WelcomeMessage);
#endif /* MPG 1*/
  
#if 0 // untested for MPG2
  
#endif /* MPG2 */

 /* Configure ANT for this application */
  G_stAntSetupData.AntChannel          = ANT_CHANNEL_USERAPP;
  G_stAntSetupData.AntSerialLo         = ANT_SERIAL_LO_USERAPP;
  G_stAntSetupData.AntSerialHi         = ANT_SERIAL_HI_USERAPP;
  G_stAntSetupData.AntDeviceType       = ANT_DEVICE_TYPE_USERAPP;
  G_stAntSetupData.AntTransmissionType = ANT_TRANSMISSION_TYPE_USERAPP;
  G_stAntSetupData.AntChannelPeriodLo  = ANT_CHANNEL_PERIOD_LO_USERAPP;
  G_stAntSetupData.AntChannelPeriodHi  = ANT_CHANNEL_PERIOD_HI_USERAPP;
  G_stAntSetupData.AntFrequency        = ANT_FREQUENCY_USERAPP;
  G_stAntSetupData.AntTxPower          = ANT_TX_POWER_USERAPP;

  
  /* If good initialization, set state to Idle */
  if( AntChannelConfig(ANT_MASTER) )
  {
    AntOpenChannel();
    UserApp_StateMachine = UserAppSM_Idle;
  }
  else
  {
    /* The task isn't properly initialized, so shut it down and don't run */
    UserApp_StateMachine = UserAppSM_FailedInit;
  }

} /* end UserAppInitialize() */


/*----------------------------------------------------------------------------------------------------------------------
Function UserAppRunActiveState()

Description:
Selects and runs one iteration of the current state in the state machine.
All state machines have a TOTAL of 1ms to execute, so on average n state machines
may take 1ms / n to execute.

Requires:
  - State machine function pointer points at current state

Promises:
  - Calls the function to pointed by the state machine function pointer
*/
void UserAppRunActiveState(void)
{
  UserApp_StateMachine();

} /* end UserAppRunActiveState */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Private functions                                                                                                  */
/*--------------------------------------------------------------------------------------------------------------------*/


/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/

/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for a message to be queued */
static void UserAppSM_Idle(void)
{
  static u8 au8TestMessage[] = {0, 2, 2, 2, 3, 3, 3, 0};
  static u8 au8DataContent[] = "xxxxxxxxxxxxxxxx";
  
  static u8 u8PositionOfTheMloeWillBe[]={0,6,13,19,19,13,6,0,
  13,19,6,0,13,13,13,19,
  19,0,0,6,19,13,0,6,19,
  13,19,19,19,19,13,13,0,
  13,19,19,0,0,6,6,6,
  0,0,6,6,0,6,13,19,6,
  0,0,6,13,13,13,19,6,
  0,19,13,13,6,6,13,13};
  static u8 *u8PointToPositionOfTheMloeWillBe=u8PositionOfTheMloeWillBe;
  static u8 u8CountForBit = 0;
  static u8 u8ClockDownForBegin[]="5";
  static u32 u32ClockDownForGame=0;
  static u16 u16ClockDownForBeginning=0;
  static u8 u8count=0;
  static u8 u8clockdown[]="10";
  static bool flag = 0;
  static bool flag0 = 0;
  static u16 u16Count = 0;
  static bool flagforgame =0;
  static u8 u8tempfordisplay[3];
  u16ClockDownForBeginning++;
  u8count++;
  u32ClockDownForGame++;
  u16Count++;
  /*if(u16Count==500&&flagforgame==0)
  {
    u16Count=0;
    LCDMessage(LINE1_START_ADDR, "WAITING FOR PLAYER..");
  }*/
  if( AntReadData() )
  {
     /* New data message: check what it is */
    if(G_eAntApiCurrentMessageClass == ANT_DATA)
    {
      /* We got some data: parse it into au8DataContent[] */
      for(u8 i = 0; i < ANT_DATA_BYTES; i++)
      {
        au8DataContent[2 * i]     = HexToASCIICharUpper(G_au8AntApiCurrentData[i] / 16);
        au8DataContent[2 * i + 1] = HexToASCIICharUpper(G_au8AntApiCurrentData[i] % 16);
      }

#ifdef MPG1
      if((au8DataContent[0]-48)*16+au8DataContent[1]-48 == 97)
      {
        flagforgame =1;
        LCDMessage(LINE1_START_ADDR, "                    ");
        u16ClockDownForBeginning=0;
        u8count=0;
        u32ClockDownForGame=0;
      }
#endif /* MPG1 */
      
#ifdef MPG2
#endif /* MPG2 */
      
    }
    else if(G_eAntApiCurrentMessageClass == ANT_TICK)
    {
      au8TestMessage[0]=u8CountForBit;
      AntQueueBroadcastMessage(au8TestMessage);
    }
  } /* end AntReadData() */
  if(1)
  {
    if(u16ClockDownForBeginning<8000 && (u16ClockDownForBeginning == 1000 ||u16ClockDownForBeginning == 2000 ||u16ClockDownForBeginning == 3000 ||u16ClockDownForBeginning == 4000 ||u16ClockDownForBeginning == 5000 ||u16ClockDownForBeginning == 6000 ||u16ClockDownForBeginning == 7000 ))
    {
      LCDClearChars(LINE1_START_ADDR, 20);
      LCDMessage(LINE1_START_ADDR, "Clock Down:  S");
      LCDMessage(LINE1_START_ADDR+12, u8ClockDownForBegin);
      u8ClockDownForBegin[0]--;
      if(u8ClockDownForBegin[0] < '/')
      {
        flag = 1;
        u32ClockDownForGame=0;
        LCDCommand(LCD_CLEAR_CMD);
        LCDMessage(LINE1_START_ADDR, "Player1 T:");
        LCDMessage(LINE1_START_ADDR+10,u8clockdown);
        u8clockdown[0]='0';
        u8clockdown[1]='9';
      }
    }
    
    //Game Start
    if(flag == 1&&u8count == 200)
    {
      u8count = 0;
      LedOff(GREEN);
      LedOff(RED);
      if(!flag0)
      {
        flag0=1;
        ButtonAcknowledge(BUTTON0);
        ButtonAcknowledge(BUTTON1);
        ButtonAcknowledge(BUTTON2);
        ButtonAcknowledge(BUTTON3);
      }
      LCDClearChars(LINE2_START_ADDR, 20);
      LCDMessage(LINE2_START_ADDR+*u8PointToPositionOfTheMloeWillBe, "M");
      if(WasButtonPressed(BUTTON0))
      {
         ButtonAcknowledge(BUTTON0);
         if(*u8PointToPositionOfTheMloeWillBe == 0)
         {
           u8CountForBit++;
           LedOn(GREEN);
         }
         else
         {
           LedOn(RED);
         }
         LCDClearChars(LINE2_START_ADDR,20);
         
         u8PointToPositionOfTheMloeWillBe++;
      }
      if(WasButtonPressed(BUTTON1))
      {
         ButtonAcknowledge(BUTTON1);
         if(*u8PointToPositionOfTheMloeWillBe == 6)
         {
           u8CountForBit++;
           LedOn(GREEN);
         }
         else
         {
           LedOn(RED);
         }
         LCDClearChars(LINE2_START_ADDR,20);
         
         u8PointToPositionOfTheMloeWillBe++;
      }
      if(WasButtonPressed(BUTTON2))
      {
         ButtonAcknowledge(BUTTON2);
         if(*u8PointToPositionOfTheMloeWillBe == 13)
         {
           u8CountForBit++;
           LedOn(GREEN);
         }
         else
         {
           LedOn(RED);
         }
         LCDClearChars(LINE2_START_ADDR,20);
         
         u8PointToPositionOfTheMloeWillBe++;
      }
      if(WasButtonPressed(BUTTON3))
      {
         ButtonAcknowledge(BUTTON3);
         if(*u8PointToPositionOfTheMloeWillBe == 19)
         {
           u8CountForBit++;
           LedOn(GREEN);
         }
         else
         {
           LedOn(RED);
         }
         LCDClearChars(LINE2_START_ADDR,20);
         
         u8PointToPositionOfTheMloeWillBe++;
      }
      
      

      NumberToAscii(u8CountForBit,u8tempfordisplay);
      LCDClearChars(LINE1_START_ADDR+19,2);
      LCDMessage(LINE1_START_ADDR+14,"Num:");
      LCDMessage(LINE1_START_ADDR+18,u8tempfordisplay);
      
    }
    
    //Game Clock Down
    if(flag == 1)
    {
      
      for(u8 i=1;i<11;i++)
      {
        if(u32ClockDownForGame == 1000*i)
        {
          LCDClearChars(LINE1_START_ADDR+10,4);
          LCDMessage(LINE1_START_ADDR+10,u8clockdown);
          u8clockdown[1]=u8clockdown[1]-1;
        }
      }
    }
    //Game End
    if(u32ClockDownForGame == 10000)
    {
      flag = 0;
      LCDCommand(LCD_CLEAR_CMD);
      LCDMessage(LINE1_START_ADDR, "TIME OUT!");
      LedOff(GREEN);
      LedOff(RED);
    }
    if(u32ClockDownForGame == 12000)
    {
      if((au8DataContent[0]-48)*16+au8DataContent[1]-48>u8CountForBit)
      {
        LCDMessage(LINE1_START_ADDR, "You are the loser!");
      }
      else if((au8DataContent[0]-48)*16+au8DataContent[1]-48<u8CountForBit)
      {
        LCDMessage(LINE1_START_ADDR, "You are the winner!");
      }
      
      else
      {
        LCDMessage(LINE1_START_ADDR, "Draw!               ");
      }
      LCDMessage(LINE2_START_ADDR, "Press B0 Again ");
    }
    if(WasButtonPressed(BUTTON0)&&flag==0)
      {
        ButtonAcknowledge(BUTTON0);
        LCDCommand(LCD_CLEAR_CMD);
        u8PointToPositionOfTheMloeWillBe=u8PositionOfTheMloeWillBe;
        u8CountForBit = 0;
        u8ClockDownForBegin[0]='5';
        u32ClockDownForGame=0;
        u16ClockDownForBeginning=0;
        u8count=0;
        u8clockdown[0]='1';
        u8clockdown[1]='0';
        flag = 0;
        flag0 = 0;
        u16Count = 0;
        flagforgame =0;
      
      }
    //Contact to another player by ANT if the game is beginning
  }
  
} /* end UserAppSM_Idle() */


/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserAppSM_Error(void)          
{
  UserApp_StateMachine = UserAppSM_Idle;
  
} /* end UserAppSM_Error() */


/*-------------------------------------------------------------------------------------------------------------------*/
/* State to sit in if init failed */
static void UserAppSM_FailedInit(void)          
{
    
} /* end UserAppSM_FailedInit() */


/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
