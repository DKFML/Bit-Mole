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
extern u8 G_u8SendNumber;
extern u8 G_u8ReceiveNumber;
/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
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
  //Clear and message
  u8 au8WelcomeMessage[]="Beat Mole !";
  //u8 au8Hint[]="Press B0 to start";
  LCDCommand(LCD_CLEAR_CMD);
  LCDMessage(LINE1_START_ADDR, au8WelcomeMessage);
  //LCDMessage(LINE1_SRART_ADDR, au8Hint);
  /* If good initialization, set state to Idle */
  if( 1 )
  {
    UserApp_StateMachine = UserAppSM_NULL;
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
  static bool flag =0;
  UserApp_StateMachine();
  if(&&flag==0)
  {
    ButtonAcknowledge(BUTTON3);
    UserApp_StateMachine = UserAppSM_Idle;
    flag=1;
  }
  

} /* end UserAppRunActiveState */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Private functions                                                                                                  */
/*--------------------------------------------------------------------------------------------------------------------*/
static void UserAppSM_NULL(void)
{
  static u16 u16Count=0;
  static bool flag=1;
  u16Count++;
  if(u16Count == 500)
  {
    u16Count=0;
    flag=!flag;
    LCDClearChars(LINE2_START_ADDR, 20);
    if(flag == 1)
    {
      LCDMessage(LINE1_START_ADDR, "WAITING FOR PLAYER..");
    }
    else
    {
      LCDMessage(LINE1_START_ADDR, "WAITING FOR PLAYER. ");
    }
  }
}

/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/

/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for a message to be queued */
static void UserAppSM_Idle(void)
{
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
  static u8 u8ReceiveFromAnother[8];
  static u8 u8tempfordisplay[3];
  u16ClockDownForBeginning++;
  u8count++;
  u32ClockDownForGame++;
  //The message watting for player
  //LCDMesage(LINE1_START_ADDR, "Watting for player..");
  
  //Open the ANT and waiting for player
  AntToAnotherRunActiveState();
  //Clock Down For Beginning 
  if(u16ClockDownForBeginning<8000 && (u16ClockDownForBeginning == 1000 ||u16ClockDownForBeginning == 2000 ||u16ClockDownForBeginning == 3000 ||u16ClockDownForBeginning == 4000 ||u16ClockDownForBeginning == 5000 ||u16ClockDownForBeginning == 6000 ||u16ClockDownForBeginning == 7000 ))
  {
    LCDClearChars(LINE1_START_ADDR, 20);
    LCDMessage(LINE1_START_ADDR, "Clock Down:");
    LCDMessage(LINE1_START_ADDR+12, u8ClockDownForBegin);
    LCDMessage(LINE1_START_ADDR+13, "s");
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
    
    
    G_u8SendNumber = u8CountForBit;
    NumberToAscii(u8CountForBit,u8tempfordisplay);
    LCDClearChars(LINE1_START_ADDR+19,2);
    LCDMessage(LINE1_START_ADDR+14,"Num:");
    LCDMessage(LINE1_START_ADDR+18,u8tempfordisplay);
    
  }
  
  //Game Clock Down
  if(flag == 1)
  {
    
    switch(u32ClockDownForGame)
    {
    case 1000:LCDClearChars(LINE1_START_ADDR+10,4);
    LCDMessage(LINE1_START_ADDR+10,u8clockdown);
    u8clockdown[1]=u8clockdown[1]-1;
    break;
    
    case 2000:LCDClearChars(LINE1_START_ADDR+10,4);
    LCDMessage(LINE1_START_ADDR+10,u8clockdown);
    u8clockdown[1]=u8clockdown[1]-1;
    break;
    
    case 3000:LCDClearChars(LINE1_START_ADDR+10,4);
    LCDMessage(LINE1_START_ADDR+10,u8clockdown);
    u8clockdown[1]=u8clockdown[1]-1;
    break;
    
    case 4000:LCDClearChars(LINE1_START_ADDR+10,4);
    LCDMessage(LINE1_START_ADDR+10,u8clockdown);
    u8clockdown[1]=u8clockdown[1]-1;
    break;
    
    case 5000:LCDClearChars(LINE1_START_ADDR+10,4);
    LCDMessage(LINE1_START_ADDR+10,u8clockdown);
    u8clockdown[1]=u8clockdown[1]-1;
    break;
    
    case 6000:LCDClearChars(LINE1_START_ADDR+10,4);
    LCDMessage(LINE1_START_ADDR+10,u8clockdown);
    u8clockdown[1]=u8clockdown[1]-1;
    break;
    
    case 7000:LCDClearChars(LINE1_START_ADDR+10,4);
    LCDMessage(LINE1_START_ADDR+10,u8clockdown);
    u8clockdown[1]=u8clockdown[1]-1;
    break;
    
    case 8000:LCDClearChars(LINE1_START_ADDR+10,4);
    LCDMessage(LINE1_START_ADDR+10,u8clockdown);
    u8clockdown[1]=u8clockdown[1]-1;
    break;
    
    case 9000:LCDClearChars(LINE1_START_ADDR+10,4);
    LCDMessage(LINE1_START_ADDR+10,u8clockdown);
    u8clockdown[1]=u8clockdown[1]-1;
    break;
    
    case 10000:LCDClearChars(LINE1_START_ADDR+10,4);
    LCDMessage(LINE1_START_ADDR+10,u8clockdown);
    break;
    
    default:break;
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
    G_u8SendNumber = u8CountForBit;
    if(G_u8SendNumber==0)
    {
      G_u8SendNumber=1;
      DebugPrintNumber(G_u8SendNumber);
    }
    else
    {
      DebugPrintNumber(G_u8SendNumber);
    }
    AntToAnotherRunActiveState();
  }
  if(u32ClockDownForGame == 12000)
  {
    if(G_u8SendNumber < G_u8ReceiveNumber)
    {
      LCDMessage(LINE1_START_ADDR, "You are the loser!");
    }
    else if(G_u8SendNumber > G_u8ReceiveNumber)
    {
      LCDMessage(LINE1_START_ADDR, "You are the winner!");
    }
    
    else
    {
      LCDMessage(LINE1_START_ADDR, "Draw");
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
    }
  //Contact to another player by ANT if the game is beginning


} /* end UserAppSM_Idle() */
     

/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserAppSM_Error(void)          
{
  
} /* end UserAppSM_Error() */


/*-------------------------------------------------------------------------------------------------------------------*/
/* State to sit in if init failed */
static void UserAppSM_FailedInit(void)          
{
    
} /* end UserAppSM_FailedInit() */


/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
