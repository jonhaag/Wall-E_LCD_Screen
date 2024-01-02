
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//
//  WALL-E LCD Screen & Battery Monitor
//  Original code by Dan C.
//  http://wallebuilders.club
//
//--------------------------------------------------------------------
//--------------------------------------------------------------------


//--------------------------------------------------------------------
//                       Libraries
//--------------------------------------------------------------------
#include <SPI.h>
#include "Adafruit_GFX.h"
#include "Adafruit_RA8875.h"
#include "bmp.h"               // Image file


// RA8875 Library only supports hardware SPI at this time



// Connect SCK to UNO Digital #13 (Hardware SPI clock)
// Connect MISO to UNO Digital #12 (Hardware SPI MISO)
// Connect MOSI to UNO Digital #11 (Hardware SPI MOSI)
// Connect CS or SS to UNO Digital #10 (Hardware SPI SS)

// Connect SCK to Mega2560 Digital #52 (Hardware SPI clock)
// Connect MISO to Mega2560 Digital #50 (Hardware SPI MISO)
// Connect MOSI to Mega2560 Digital #51 (Hardware SPI MOSI)
// Connect CS or SS to Mega2560 Digital #53 (Hardware SPI SS)

// Connect SCK to Nano Digital #13 (Hardware SPI SCK)
// Connect MISO to Nano Digital #12 (Hardware SPI MISO)
// Connect MOSI to Nano Digital #11 (Hardware SPI MOSI)
// Connect CS or SS to Nano Digital #10 (Hardware SPI CS)



// Connect Signal wire from Voltage divider into pin A0



#define RA8875_CS 10     // 53 on Mega2560,  10 on Uno,  10 on Nano
#define RA8875_RESET 9  // any didgital pin

Adafruit_RA8875 tft = Adafruit_RA8875(RA8875_CS, RA8875_RESET);

#define RA8875_DARKERGRAY  0x333333   // change color of background boxes


//--------------------------------------------------------------------
//                            Variables/Constants
//--------------------------------------------------------------------
const unsigned long powercheckdelay = 1000;  // Delay for power level check once a second.
int voltageSensor1 = A0;
unsigned long currentMillis = 0;    
unsigned long powerMillis = 0;
float vOUT1 = 0.0;
float vIN1 = 0.0;
float R11 = 30000;  // 30000 adjust to exact value for more accurate reading
float R22 = 7500;   // 7500 adjust to exact value for more accurate reading
int value1 = 0;

byte booted = 0;

byte round_corner = 4;
byte start_x = 90;  //Moves LCD display Up or Down on display

#define Playbutton 6
#define Stopbutton 7
#define Recordbutton 8
#define Lightbutton 5



//--------------------------------------------------------------------
//                       SETUP
//--------------------------------------------------------------------

void setup()
{
  Serial.begin(9600);   // Debug Console

     // initialize the Electroncis Panel pushbutton pins as an input:
  pinMode(Playbutton, INPUT_PULLUP);
  pinMode(Stopbutton, INPUT_PULLUP);
  pinMode(Recordbutton, INPUT_PULLUP);
  pinMode(Lightbutton, INPUT_PULLUP);

  pinMode(A0, INPUT);   
  
    /* Initialize the display using 'RA8875_480x80', 'RA8875_480x128', 'RA8875_480x272' or 'RA8875_800x480' */
  if (!tft.begin(RA8875_800x480)) {
    Serial.println("RA8875 Not Found!");
    while (1);
  }
  
  tft.displayOn(true);
  tft.GPIOX(true);                              // Enable TFT - display enable tied to GPIOX
  tft.PWM1config(true, RA8875_PWM_CLK_DIV1024); // PWM output for backlight
  tft.PWM1out(255);                             // Set Display Brightness to maximum
  tft.setRotation(3);                           // 0,1,2 or 3  Changes screen rotation
  tft.fillScreen(RA8875_BLACK);                 // Clear the Screen


  
  tft.drawBitmap(start_x + 0, 0, sclbmp, 32, 480,RA8875_YELLOW);
  tft.drawBitmap(start_x + 77, 337, sun, 144, 144,RA8875_YELLOW);
  
  tft.fillRoundRect(start_x + 483, 0, 71, 294, round_corner, RA8875_DARKERGRAY);
  tft.fillRoundRect(start_x + 438, 0, 20, 294, round_corner, RA8875_DARKERGRAY);
  tft.fillRoundRect(start_x + 393, 0, 20, 294, round_corner, RA8875_DARKERGRAY);
  tft.fillRoundRect(start_x + 348, 0, 20, 294, round_corner, RA8875_DARKERGRAY);
  tft.fillRoundRect(start_x + 303, 0, 20, 294, round_corner, RA8875_DARKERGRAY);
  tft.fillRoundRect(start_x + 258, 0, 20, 294, round_corner, RA8875_DARKERGRAY);
  tft.fillRoundRect(start_x + 213, 0, 20, 294, round_corner, RA8875_DARKERGRAY);
  tft.fillRoundRect(start_x + 168, 0, 20, 294, round_corner, RA8875_DARKERGRAY);
  tft.fillRoundRect(start_x + 123, 0, 20, 294, round_corner, RA8875_DARKERGRAY);
  tft.fillRoundRect(start_x + 78, 0, 20, 294, round_corner, RA8875_DARKERGRAY);

  
  delay(2000);              // adjust delay as needed to adjust sound timing
  boot_sequence();
}
  
  //********************************** END SETUP ***********************************//

void(* resetFunc) (void) = 0;//declare reset function at address 0

//--------------------------------------------------------------------
//                       LOOP START
//--------------------------------------------------------------------
void loop()
{
  currentMillis = millis();
 

  //--------------------------------------------------------------------
  //                       Battery Monitoring
  //--------------------------------------------------------------------

  if (currentMillis - powerMillis >= powercheckdelay)  // Runs power check once a second.
  {
    value1 = analogRead(voltageSensor1);  // onboard lipo battery monitor
    vOUT1 = (value1 * 5.0) / 1024.0;
    vIN1 = vOUT1/(R22/(R11+R22));
    powerMillis = currentMillis;
  }

// Note:  Battery voltage monitoring setup for a 60AH LifeP04 Battery.  Adjust accordingly.


  if(vIN1 >= 11){                                             // Line 1
    tft.fillRoundRect(start_x + 483, 0, 71, 294, round_corner, RA8875_YELLOW);
    } else {
      tft.fillRoundRect(start_x + 483, 0, 71, 294, round_corner, RA8875_DARKERGRAY);
    }
  if(vIN1 >= 11.25){                                           // Line 2
    tft.fillRoundRect(start_x + 438, 0, 20, 294, round_corner, RA8875_YELLOW);
    } else {
      tft.fillRoundRect(start_x + 438, 0, 20, 294, round_corner, RA8875_DARKERGRAY);
    }
  if(vIN1 >= 11.5){                                            // Line 3
    tft.fillRoundRect(start_x + 393, 0, 20, 294, round_corner, RA8875_YELLOW);
    } else {
      tft.fillRoundRect(start_x + 393, 0, 20, 294, round_corner, RA8875_DARKERGRAY);
    }
  if(vIN1 >= 11.75){                                          // Line 4
    tft.fillRoundRect(start_x + 348, 0, 20, 294, round_corner, RA8875_YELLOW);
    } else {
      tft.fillRoundRect(start_x + 348, 0, 20, 294, round_corner, RA8875_DARKERGRAY);
    }
  if(vIN1 >= 12){                                            // Line 5
    tft.fillRoundRect(start_x + 303, 0, 20, 294, round_corner, RA8875_YELLOW);
    } else {
      tft.fillRoundRect(start_x + 303, 0, 20, 294, round_corner, RA8875_DARKERGRAY);
    }
  if(vIN1 >= 12.25){                                          // Line 6
    tft.fillRoundRect(start_x + 258, 0, 20, 294, round_corner, RA8875_YELLOW);
    } else {
      tft.fillRoundRect(start_x + 258, 0, 20, 294, round_corner, RA8875_DARKERGRAY);
    }
  if(vIN1 >= 12.5){                                            // Line 7
    tft.fillRoundRect(start_x + 213, 0, 20, 294, round_corner, RA8875_YELLOW);
    } else {
      tft.fillRoundRect(start_x + 213, 0, 20, 294, round_corner, RA8875_DARKERGRAY);;
    }
  if(vIN1 >= 12.75){                                          // Line 8
    tft.fillRoundRect(start_x + 168, 0, 20, 294, round_corner, RA8875_YELLOW);
    } else {
      tft.fillRoundRect(start_x + 168, 0, 20, 294, round_corner, RA8875_DARKERGRAY);
    }
  if(vIN1 >= 13){                                         // Line 9
    tft.fillRoundRect(start_x + 123, 0, 20, 294, round_corner, RA8875_YELLOW);
    } else {
      tft.fillRoundRect(start_x + 123, 0, 20, 294, round_corner, RA8875_DARKERGRAY);
    }
  if(vIN1 >= 13.25){                                         // Line 10
    tft.fillRoundRect(start_x + 78, 0, 20, 294, round_corner, RA8875_YELLOW);
    } else {
      tft.fillRoundRect(start_x + 78, 0, 20, 294, round_corner, RA8875_DARKERGRAY);
    }

 Serial.print(vIN1);


  //--------------------------------------------------------------------
  //                    Electroncis Panel buttons
  //--------------------------------------------------------------------

  // read the state of the electroncis panel pushbutton values:

int recordbuttonState = digitalRead(Recordbutton);   
int playbuttonState = digitalRead(Playbutton);     
int stopbuttonState = digitalRead(Stopbutton);  
int lightbuttonState = digitalRead(Lightbutton);    

  if (recordbuttonState == LOW) 
  {
    Serial.print("v, Record on, ");
        // Place code here for action
  }
  else
  {
    Serial.print("v Record off, ");
        // Place code here for action
  }


  if (playbuttonState == LOW) 
  {
    Serial.print("Play on, ");
    // Place code here for action
  }
  else
  {
    Serial.print("Play off, ");
        // Place code here for action
  }

    if (stopbuttonState == LOW) 
  {
    Serial.print("Stop on, ");
        resetFunc(); //call reset 
  }
  else
  {
    Serial.print("Stop off, ");
        // Place code here for action
  }

    if (lightbuttonState == LOW) 
  {
    Serial.println("Light on");
        // Place code here for action
  }
  else
  {
    Serial.println("Light off");
        // Place code here for action
  }

 
}   //=================End Loop========================


void boot_sequence(){
  // Start Boot Sequence by turning the display ON
  tft.displayOn(true);
  delay(1000);   
  tft.fillRoundRect(start_x + 483, 0, 71, 294, round_corner, RA8875_YELLOW);  // Line 1
  delay(1000);
  tft.fillRoundRect(start_x + 438, 0, 20, 294, round_corner, RA8875_YELLOW);  // Line 2
  delay(700);
  tft.fillRoundRect(start_x + 393, 0, 20, 294, round_corner, RA8875_YELLOW);  // Line 3
  delay(650);
  tft.fillRoundRect(start_x + 348, 0, 20, 294, round_corner, RA8875_YELLOW);  // Line 4
  delay(600);
  tft.fillRoundRect(start_x + 303, 0, 20, 294, round_corner, RA8875_YELLOW);  // Line 5
  delay(550);
  tft.fillRoundRect(start_x + 258, 0, 20, 294, round_corner, RA8875_YELLOW);  // Line 6
  delay(450);
  tft.fillRoundRect(start_x + 213, 0, 20, 294, round_corner, RA8875_YELLOW);  // Line 7
  delay(400);
  tft.fillRoundRect(start_x + 168, 0, 20, 294, round_corner, RA8875_YELLOW);  // Line 8
  delay(350);
  tft.fillRoundRect(start_x + 123, 0, 20, 294, round_corner, RA8875_YELLOW);  // Line 9
  delay(250);
  tft.fillRoundRect(start_x + 78, 0, 20, 294, round_corner, RA8875_YELLOW);   // Line 10
  delay(5000);
}
