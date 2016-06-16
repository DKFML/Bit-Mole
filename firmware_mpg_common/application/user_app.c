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
extern volatile u32 G_u32SystemFlags;                  /* From main.c */
extern volatile u32 G_u32ApplicationFlags;             /* From main.c */

extern volatile u32 G_u32SystemTime1ms;                /* From board-specific source file */
extern volatile u32 G_u32SystemTime1s;                 /* From board-specific source file */

/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern AntSetupDataType G_stAntSetupData;                         /* From ant.c */

extern u32 G_u32AntApiCurrentDataTimeStamp;                       /* From ant_api.c */
extern AntApplicationMessageType G_eAntApiCurrentMessageClass;    /* From ant_api.c */
extern u8 G_au8AntApiCurrentData[ANT_APPLICATION_MESSAGE_BYTES];  /* From ant_api.c */
/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp_" and be declared as static.
***********************************************************************************************************************/
static fnCode_type UserApp_StateMachine;            /* The state machine function pointer */
static u32 UserApp_u32Timeout;                      /* Timeout counter used across states */

static u32 UserApp_u32DataMsgCount = 0;   /* ANT_DATA packets received */
static u32 UserApp_u32TickMsgCount = 0;   /* ANT_TICK packets received */
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
  /* Clear screen and place start messages */
  LCDCommand(LCD_CLEAR_CMD);
  LCDMessage(LINE1_START_ADDR, "Beat Mole !"); 
  LCDMessage(LINE2_START_ADDR, "Player2"); 

  /* Start with LED0 in PURPLE state = channel is not configured */
  LedOn(PURPLE);
  
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
  if( AntChannelConfig(ANT_SLAVE) )
  {
    /* Channel is configured, so change LED to yellow */
    LedOff(PURPLE);
    LedOn(YELLOW);
    AntOpenChannel();
    UserApp_StateMachine = UserAppSM_Idle;
  }
  else
  {
    /* The task isn't properly initialized, so shut it down and don't run */
    LedBlink(RED, LED_4HZ);
    UserApp_StateMachine = UserAppSM_Error;
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
//Winner Music
static void WinnerMusic(void)
{
  static u8 u8notecounter = 0;
  static u16 u16buzzerrate[]  = {586,586,523,586,697,697,783,
  586,586,523,586,523,879,523,
  586,586,523,586,697,783,879,
  879,783,879,783,697,586,
  586,586,523,586,697,697,783,
  586,586,523,586,523,523,879,
  586,586,523,586,697,697,783,
  879,783,879,783,697,586,
  697,658,586,523,523,523,586,879,783,879,
  879,523,586,783,658,697,697,658,523,586,
  697,658,586,523,523,523,586,879,783,879,
  879,523,586,586,586,697,783,658,
  586,697,783,783,783,897,897,
  879,523,586,783,697,879,
  586,697,783,783,783,897,897,
  879,879,879,783,697,697,
  586,697,783,783,783,897,897,
  879,523,586,783,697,879,
  586,697,879,879,783,697,783,879,697,523,586,
  586,697,783,783,783,897,897,
  879,523,586,783,697,879,
  586,697,783,783,783,897,897,
  879,879,879,783,697,697,
  586,697,783,783,783,897,897,
  879,523,586,783,697,879,
  586,697,879,879,783,697,783,879,697,523,586,
  };
  static u16 u16counter = 0;
  
  u16counter++;
  
  if(u16counter == 80)
  {
    u16counter=0;
    PWMAudioSetFrequency(BUZZER1, u16buzzerrate[u8notecounter]);
    PWMAudioOn(BUZZER1);
    
    if(++u8notecounter == 192)
    {
      u8notecounter=0;
    }
  }
} /* end winnerMusic() */

//Loser Music
static void LoserMusic(void)
{
  static u8 u8notecounter = 0;
  static u16 u16buzzerrate[]  = {
  
  879,523,586,783,697,879,
  586,697,783,783,783,897,897,
  879,879,879,783,697,697,
  
  586,697,879,879,783,697,783,879,697,523,586,
  586,697,783,783,783,897,897,
  
  586,697,783,783,783,897,897,
  
  586,697,879,879,783,697,783,879,697,523,586,
  };
  static u16 u16counter = 0;
  
  u16counter++;
  
  if(u16counter == 80)
  {
    u16counter=0;
    PWMAudioSetFrequency(BUZZER1, u16buzzerrate[u8notecounter]);
    PWMAudioOn(BUZZER1);
    
    if(++u8notecounter == 45)
    {
      u8notecounter=0;
    }
  }
} /* end LoserMusic() */
//LED Flash
static void LedFlash(void)
{
  static u16 u16Count = 0;
  u16Count++;
  static bool flag =0;
  if(u16Count == 500 && flag ==1)
  {
    flag = !flag;
    u16Count = 0;
    LedOn(WHITE);
    LedOn(PURPLE);
    LedOn(BLUE);
    LedOn(CYAN);
    LedOff(GREEN);
    LedOff(YELLOW);
    LedOff(ORANGE);
    LedOff(RED);
  }
  else if(u16Count == 500 && flag ==0)
  {
    flag = !flag;
    u16Count = 0;
    LedOff(WHITE);
    LedOff(PURPLE);
    LedOff(BLUE);
    LedOff(CYAN);
    LedOn(GREEN);
    LedOn(YELLOW);
    LedOn(ORANGE);
    LedOn(RED);
  }
}

/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/

/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for a message to be queued */
static void UserAppSM_Idle(void)
{
  static u8 u8NextMole[]={0,6,13,19,19,13,6,0,
  13,19,6,0,13,13,13,19,
  19,0,0,6,19,13,0,6,19,
  13,19,19,19,19,13,13,0,
  13,19,19,0,0,6,6,6,
  0,0,6,6,0,6,13,19,6,
  0,0,6,13,13,13,19,6,
  0,19,13,13,6,6,13,13};
  static u8 *u8PointToNextMole=u8NextMole;
  static u8 u8CountForBit = 0;
  static u8 u8ClockDownForBegin[]="5";
  static u32 u32ClockDownForGame=0;
  static u16 u16ClockDownForBeginning=0;
  static u8 u8count=0;
  static u8 u8clockdown[3]="10";
  static bool flag = 0;
  static bool FlagForGame =0;
  static u8 u8tempfordisplay[3];
  static bool FlagW = 0;
  static bool FlagL = 0;
  static bool flagsend = 0;
  static u8 au8SendNumber[] = {0x41, 0x61, 0, 0, 0xA5, 0, 0, 0};
  static u8 au8Receive[] = "xxxxxxxxxxxxxxxx";
  static bool Flag_Button0 = 0;
  static bool Flag_Button1 = 0;
  static u8 u8LastState = 0xff;
  u8 u8return[]= {1,35,69,103,137,171,205,239};
  //AntQueueBroadcastMessage(u8return);
  
  //Game Part 
  if( FlagForGame == 1)
  {
    u16ClockDownForBeginning++;
    if(u16ClockDownForBeginning == 8500)
    {
      u16ClockDownForBeginning = 8000;
    }
    u32ClockDownForGame++;
    u8count++;
    //Clock Down For Beginning
    for(u8 i=1;i<8;i++)
    {
      if(u16ClockDownForBeginning == 1000*i)
      {
        LCDClearChars(LINE1_START_ADDR, 20);
          //LCDClearChars(LINE2_START_ADDR, 20);
          LCDMessage(LINE1_START_ADDR, "Clock Down:  s");
          LCDMessage(LINE1_START_ADDR+12, u8ClockDownForBegin);
          LCDMessage(LINE2_START_ADDR, "Player2");
          u8ClockDownForBegin[0]--;
          if(u8ClockDownForBegin[0] < '/')
          {
            flag = 1;
            u32ClockDownForGame=0;
            LCDCommand(LCD_CLEAR_CMD);
            LCDMessage(LINE1_START_ADDR, "Player2 T:");
            LCDMessage(LINE1_START_ADDR+10,u8clockdown);
            u8clockdown[0]='0';
            u8clockdown[1]='9';
            u8clockdown[2]='\0';
            ButtonAcknowledge(BUTTON0);
            ButtonAcknowledge(BUTTON1);
            ButtonAcknowledge(BUTTON2);
            ButtonAcknowledge(BUTTON3);
          }
        break;
      }
    }
    
    /*if(u16ClockDownForBeginning<8000 && (u16ClockDownForBeginning == 1000 ||
                                         u16ClockDownForBeginning == 2000 ||
                                         u16ClockDownForBeginning == 3000 ||
                                         u16ClockDownForBeginning == 4000 ||
                                         u16ClockDownForBeginning == 5000 ||
                                         u16ClockDownForBeginning == 6000 ||
                                         u16ClockDownForBeginning == 7000 ))
    {
      LCDClearChars(LINE1_START_ADDR, 20);
      //LCDClearChars(LINE2_START_ADDR, 20);
      LCDMessage(LINE1_START_ADDR, "Clock Down:  s");
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
        u8clockdown[2]='\0';
        ButtonAcknowledge(BUTTON0);
        ButtonAcknowledge(BUTTON1);
        ButtonAcknowledge(BUTTON2);
        ButtonAcknowledge(BUTTON3);
      }
    }
    if(u16ClockDownForBeginning == 2000)
    {
      u16ClockDownForBeginning == 1500;
      flag = 1;
      u8clockdown[0]='0';
        u8clockdown[1]='9';
        u8clockdown[2]='\0';
    }*/
    
    //Game Start
    if(flag == 1&&u8count == 100)
    {
      u8count = 0;
      LedOff(GREEN);
      LedOff(RED);
      LCDClearChars(LINE2_START_ADDR, 20);
      u8 u8Mole[]={'M','\0'};
      LCDMessage(LINE2_START_ADDR+*u8PointToNextMole, "M");
      if(WasButtonPressed(BUTTON0))
      {
         ButtonAcknowledge(BUTTON0);
         if(*u8PointToNextMole == 0)
         {
           u8CountForBit++;
           LedOn(GREEN);
           NumberToAscii(u8CountForBit,u8tempfordisplay);
           LCDClearChars(LINE1_START_ADDR+19,2);
           LCDMessage(LINE1_START_ADDR+14,"Num:");
           LCDMessage(LINE1_START_ADDR+18,u8tempfordisplay);
         }
         else
         {
           LedOn(RED);
         }
         LCDClearChars(LINE2_START_ADDR,20);
         
         u8PointToNextMole++;
      }
      if(WasButtonPressed(BUTTON1))
      {
         ButtonAcknowledge(BUTTON1);
         if(*u8PointToNextMole == 6)
         {
           u8CountForBit++;
           LedOn(GREEN);
           NumberToAscii(u8CountForBit,u8tempfordisplay);
           LCDClearChars(LINE1_START_ADDR+19,2);
           LCDMessage(LINE1_START_ADDR+14,"Num:");
           LCDMessage(LINE1_START_ADDR+18,u8tempfordisplay);
         }
         else
         {
           LedOn(RED);
         }
         LCDClearChars(LINE2_START_ADDR,20);
         
         u8PointToNextMole++;
      }
      if(WasButtonPressed(BUTTON2))
      {
         ButtonAcknowledge(BUTTON2);
         if(*u8PointToNextMole == 13)
         {
           u8CountForBit++;
           LedOn(GREEN);
           NumberToAscii(u8CountForBit,u8tempfordisplay);
           LCDClearChars(LINE1_START_ADDR+19,2);
           LCDMessage(LINE1_START_ADDR+14,"Num:");
           LCDMessage(LINE1_START_ADDR+18,u8tempfordisplay);
         }
         else
         {
           LedOn(RED);
         }
         LCDClearChars(LINE2_START_ADDR,20);
         
         u8PointToNextMole++;
      }
      if(WasButtonPressed(BUTTON3))
      {
         ButtonAcknowledge(BUTTON3);
         if(*u8PointToNextMole == 19)
         {
           u8CountForBit++;
           LedOn(GREEN);
           NumberToAscii(u8CountForBit,u8tempfordisplay);
           LCDClearChars(LINE1_START_ADDR+19,2);
           LCDMessage(LINE1_START_ADDR+14,"Num:");
           LCDMessage(LINE1_START_ADDR+18,u8tempfordisplay);
         }
         else
         {
           LedOn(RED);
         }
         LCDClearChars(LINE2_START_ADDR,20);
         
         u8PointToNextMole++;
      }
      
      
      
    }
   
    //Game Clock Down &&u8count == 50
    for(u8 i=1;i<11;i++)
    {
      if(u32ClockDownForGame == 1000*i)
      {
        if(flag == 1)
        {
          LCDClearChars(LINE1_START_ADDR+10,4);
          LCDMessage(LINE1_START_ADDR+10,u8clockdown);
          u8clockdown[1]=u8clockdown[1]-1;
        }
        break;
      }
    }
    
    //Game End
    if(u32ClockDownForGame == 10000)
    {
      flag = 0;
      LCDClearChars(LINE2_START_ADDR, 20);
      LCDMessage(LINE2_START_ADDR, "TIME OUT!");
      LedOff(GREEN);
      LedOff(RED);
      LedOff(BLUE);
      /*AntCloseChannel();*/
      LedOn(YELLOW);
    }
    if(u32ClockDownForGame > 11500)
    {
      u32ClockDownForGame = 10150;
      if(u8CountForBit > (au8Receive[0]-48)*16+au8Receive[1]-48)
      {
        LCDMessage(LINE1_START_ADDR, "You are the winner !");
        FlagW = 1;
      }
      else if(u8CountForBit < (au8Receive[0]-48)*16+au8Receive[1]-48)
      {
        LCDMessage(LINE1_START_ADDR, "You are the loser ! ");
        FlagL = 1;
      }
      else
      {
        LCDMessage(LINE1_START_ADDR, "Draw!               ");
      }
      LCDMessage(LINE2_START_ADDR, "Try again by B0      ");
    }
    //Game Again
    if(WasButtonPressed(BUTTON0)&&flag==0)
      {
        ButtonAcknowledge(BUTTON0);
        LCDCommand(LCD_CLEAR_CMD);
        u8PointToNextMole=u8NextMole;
        u32ClockDownForGame = 0;
        u16ClockDownForBeginning = 0;
        u8ClockDownForBegin[0]='5';
        u8clockdown[0]='1';
        u8clockdown[1]='0';
        FlagForGame =0;
        u8count=0;
        u8CountForBit = 0;
        FlagW = 0;
        FlagL = 0;
        flagsend = 0;
        PWMAudioOff(BUZZER1);
        LedOff(WHITE);
        LedOff(PURPLE);
        LedOff(BLUE);
        LedOff(CYAN);
        LedOff(GREEN);
        LedOff(YELLOW);
        LedOff(ORANGE);
        LedOff(RED);
        LCDMessage(LINE1_START_ADDR, "Wait for a player..");
        /*AntCloseChannel();
        LedOff(BLUE);
        LedOn(YELLOW);*/
        AntOpenChannel();
        
        u8LastState = EVENT_RX_FAIL_GO_TO_SEARCH;
      }
    if(FlagW == 1)
    {
      WinnerMusic();
      LedFlash();
    }
    if(FlagL == 1)
    {
      LoserMusic();
      LedFlash();
    }
  
  }

  
  //ANT Part
  //Get the count and send it to another Machine when the count has changed
  au8SendNumber[0] = u8CountForBit;
  if(au8SendNumber[0] != au8SendNumber[1])
  {
    AntQueueBroadcastMessage (au8SendNumber);
  }
  au8SendNumber[1] = u8CountForBit;
  /* Queue open channel and change LED0 from yellow to blinking green to indicate channel is opening */
  /*if(WasButtonPressed(BUTTON0)&&Flag_Button0 == 0)
  {
    ButtonAcknowledge(BUTTON0);
    Flag_Button0 = 1;
    
    AntOpenChannel();
    LedOff(YELLOW);
    LCDClearChars(LINE2_START_ADDR, 20);
    
  }*/
  LedOff(YELLOW);
  /* Always check for ANT messages */
  if( AntReadData() )
  {
     /* New data message: check what it is */
    if(G_eAntApiCurrentMessageClass == ANT_DATA)
    {
      /* We got some data: parse it into au8DataContent */
      for(u8 i = 0; i < ANT_DATA_BYTES; i++)
      {
        au8Receive[2 * i]     = HexToASCIICharUpper(G_au8AntApiCurrentData[i] / 16);
        au8Receive[2 * i + 1] = HexToASCIICharUpper(G_au8AntApiCurrentData[i] % 16);
      }
    } /* end if(G_eAntApiCurrentMessageClass == ANT_DATA) */

    else if(G_eAntApiCurrentMessageClass == ANT_TICK)
    {
      /* Look at the TICK contents to check the event code and respond only if it's different */
      if(u8LastState != G_au8AntApiCurrentData[ANT_TICK_MSG_EVENT_CODE_INDEX])
      {
        /* The state changed so update u8LastState and queue a debug message */
        u8LastState = G_au8AntApiCurrentData[ANT_TICK_MSG_EVENT_CODE_INDEX];

        /* Parse u8LastState to update LED status */
        switch (u8LastState)
        {
          /* If we are synced with a device, blue is solid */
          case RESPONSE_NO_ERROR:
          {
            LedOff(GREEN);
            LedOn(BLUE);
            FlagForGame =1;
            if(flagsend == 0)
            {
              flagsend = 1;
              u8 u8return[]= {0,35,69,103,137,171,205,239};
              AntQueueBroadcastMessage(u8return);
            }
            break;
          }

          /* If we are paired but missing messages, blue blinks */
          case EVENT_RX_FAIL:
          {
            LedOff(GREEN);
            LedBlink(BLUE, LED_2HZ);
            break;
          }

          /* If we drop to search, LED is green */
          case EVENT_RX_FAIL_GO_TO_SEARCH:
          {
            LedOff(BLUE);
            LedOn(GREEN);
            break;
          }

          /* If the search times out, the channel should automatically close */
          case EVENT_RX_SEARCH_TIMEOUT:
          {
            DebugPrintf("Search timeout\r\n");
            break;
          }

          default:
          {
            DebugPrintf("Unexpected Event\r\n");
            break;
          }
        } /* end switch (G_au8AntApiCurrentData) */
      } /* end if (u8LastState ...) */
    } /* end else if(G_eAntApiCurrentMessageClass == ANT_TICK) */
  } /* end AntReadData() */

   
  
  
  
   
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
