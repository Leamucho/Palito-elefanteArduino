/* FOLLOWING LIBRARIES LICENSES, THIS WORK ALSO RELEASED WITH GNU GPL V3.0 
 * lIBRARY cREDITS TO iTS oWNERS
 * CODE dEVELOPED BY lEANDRO mUÑOZ http://leamucho.wixsite.com/portafolio
 * 2014-2020 
 * 3 analogs van a la esfera :D
 * ya ok **problema en 165 pegado ummm... #define MAPSIZE 32///importante na biblioteca para due
 * probando por un click si se puede apagar el trasterio
 */
 
/*
 * Obtain the documentation or find details of the phi_interfaces, phi_prompt TUI library, Phi-2 shield, 
 * and Phi-panel hardware or contact Dr. Liu at:
 * <a href="http://liudr.wordpress.com/phi_interfaces/">http://liudr.wordpress.com/phi_interfaces/</a>
 
*/
#include "ClickButton.h"
#include <phi_interfaces1-6.h>
#include <Scheduler.h>
#include <OSCBundle.h>
#include <OSCBoards.h>
#include <OSCTiming.h>
#include <PacketSerial.h>
#include <BufferPrinter.h>
#include <Entropy.h>
#include <Keypad_2.h> // Keypad modified to save int values instead of char

#define SAM3X8E

PacketSerial_<SLIP, SLIP::END, 8192> serial;

//#include <SLIPEncodedSerial.h>
// SLIPEncodedSerial SLIPSerial(Serial);

///////////////////////////////////////
//// TRASTES - MATRIX Strings Air/////
//////////////////////////////////////
    #define LIST_MAX 24
    #define MAPSIZE 32///importante na biblioteca para due
    //#define ROWS_AIR 2
    #define COL_CUERDAS 6
/*
    int MATRIXair_names [ROWS_AIR][COL_CUERDAS]= { // names on int format (fretString) 
          {68 ,58 ,48 ,38 ,28 ,18},
          {69 ,59 ,49 ,39 ,29 ,19}
    };
    //byte ROWSair_pins [ROWS_AIR]= {34,35}; //pick and somethingelse
    //                             6  5  4  3  2  1*/
    byte COLS_pins [COL_CUERDAS] ={8, 9,10,11,12,13};

   // Keypad aire = Keypad(makeKeymap(MATRIXair_names), ROWSair_pins, COLS_pins, ROWS_AIR, COL_CUERDAS );
    
/////////////////////////////////
//// TRASTES - MATRIX frets /////
//////////////////////////////////
    
    #define ROWS_TRASTES 26
    
    
    int MATRIX_names [ROWS_TRASTES][COL_CUERDAS]= { // names on int format (fretString)
          
          
          {68 ,58 ,48 ,38 ,28 ,18},
          {69 ,59 ,49 ,39 ,29 ,19},
          {16,15,14,13,12,11 }, 
          {26,25,24,23,22,21 }, 
          {36,35,34,33,32,31 }, 
          {46,45,44,43,42,41 },
          {56,55,54,53,52,51 }, 
          {66,65,64,63,62,61 }, 
          {76,75,74,73,72,71 }, 
          {86,85,84,83,82,81 },
          {96,95,94,93,92,91 }, 
          {106,105,104,103,102,101 },        
          {116,115,114,113,112,111 }, 
          {126,125,124,123,122,121 },
          {136,135,134,133,132,131 },         
          {146,145,144,143,142,141 }, 
          {156,155,154,153,152,151 }, 
          {166,165,164,163,162,161 },
          {176,175,174,173,172,171 }, 
          {186,185,184,183,182,181 }, 
          {196,195,194,193,192,191 },
          {206,205,204,203,202,201 },
          {216,215,214,213,212,211 }, 
          {226,225,224,223,222,221 }, 
          {236,235,234,233,232,231 }, 
          {246,245,244,243,242,241 }               
                        
    };
    ///
    //                             pick otro 1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19 20 21 22 23 24
    byte ROWS_pins [ROWS_TRASTES]={34,  35,  14,15,16,17,18,19,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53}; 
    //byte ROWS_pins [ROWS_TRASTES]={14,15,16,17,18,19,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39};
                  
    
    Keypad trasterio= Keypad(makeKeymap(MATRIX_names), ROWS_pins, COLS_pins, ROWS_TRASTES, COL_CUERDAS );
    //boolean trasteON = 
    int msg;
    int after;//random for aftertouch
    
////////////////////////////
///// ROTADOR - ROTARY /////
////////////////////////////
#define Encoder1ChnA 20
#define Encoder1ChnB 21
#define EncoderDetent 24
#define EncoderType_NO 0

char mapping1[]={3,2}; // This is a rotary encoder so it returns 3 char for up and 2 char for down on the dial.
phi_rotary_encoders_d rotary(mapping1, Encoder1ChnA, Encoder1ChnB, EncoderDetent, EncoderType_NO );
multiple_button_input* dial1=&rotary;
int temp_rota;//se guarda rotary temporal
//int temp_rota_dec; // temprota en decimales
//byte t_ang_rota; // rotary angle temp
///////////////////////////
const int but_pin = 22; // BOTON DEL ROTARY
ClickButton but(but_pin,1);//pin y funcionamiento(high o low)
long pre_Mil_but, pre_Mil_baj, pre_Mil_sub; // temporales de millis para but y rota
long interval = 400;           // interval at which to blink (milliseconds)
 
//////////////////////////////////
////Rotary Pins used for PWM//////
const byte redPin = 2;
const byte greenPin = 3;
const byte bluePin = 4;
//////////////////////////////////////
//// variables para rotary button /////
int butState = 0; // current state of the button
int modo_but = 1; //modo del boton (col_rotador,trastes, pick, calib o val_subaj)

const int col_rotador = 1; // modo_but 1 // inicia en col_rotador
int color = 0;//contador de color iniciando

int roj_actual = 0; // inician en vermellon 
int ver_actual = 130; //
int zul_actual = 255; //

float est_col[3]; // array de los estados de cada col_rotador
char direccionp[15];
/////////////////////////
const int col_deep = 2; // modo_but 2
int color_deep = 0; //contador de colores largo iniciando
boolean deep_st= false;

int co_parse; // parsear o col_rotador o color_deep en subaj


float est_col_deep[6];// array de estados de cada col_deep
//////////////////////////
const int afterM = 3; // modo_but 3
boolean after_ON = true;
int afterHold = 10000;

const int pick = 4; // modo_but 4
boolean pick_ON = true;
///////////////////////////
const int val_subaj = 5; // modo_but 5
//int subaj [9] = {0.03125,0.03125,0.03125,0.03125,0.03125,0.03125,0.03125,0.03125,0.03125}; // var de def de cada col_rotador
int subaj [9] = {32,32,32,64,64,64,64,64,64}; // var de def de cada col_rotador
//float subaj_col_deep [6] = {0.0625,0.0625,0.0625,64,64,64}; //var de cada col_rot_deep
//ahora cada color su n'umero //int ver_valsubaj = 72; // verde inicial con subaj 64
//int sig_colsb= 1; // signo para + o - color verde

///////////////////////////////////////
/////ANALOG STUFF///////////////
//////////////////////// 
int last_analog[12];
int actual_analog[12];
const int numReadings = 10;         //Number of reading to do to each pot
int potReadings [12][numReadings];  //smoothdouble array for pots 
int pinJitter[]={5,3,5,3,3,3,3,8,3,3,3,3}; //array of jitterness to read not moving
int readIndex[12];                  // the index of the current reading POT
int total[12];                      // the running total for all POTs
int average[12] ;                   // the average of each pin


int piezos_ref[12];//la resta val de referencia guardada para sacarle valor absoluto
//float anlg_val [12];         // valores de analogs para calibrar


//////////////////////////////////////////////////////
//////PACKET AND BUFFER-PRINTER (OpFr STUFF)///////
static uint8_t b[256];
BufferPrinter printer(b, 256);
///////////////////////////////////////////////////////

//converts the pin to an osc address
char * numToOSCAddress( int pin){
  static char s[10];
  int i = 9;
	
  s[i--]= '\0';
	do
  {
		s[i] = "0123456789"[pin % 10];
              --i;
              pin /= 10;
  }
  while(pin && i);
  s[i] = '/';
  return &s[i];
}

void setup()
{
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  
  analogWriteResolution(8);
  Entropy.initialize();
  
  trasterio.setDebounceTime(15);
  trasterio.setHoldTime(afterHold);

  //SLIPSerial.begin(460800);
  
  serial.begin(115200);
  Scheduler.startLoop(loop2);
  Scheduler.startLoop(loop3);
  //Scheduler.startLoop(loop4); 
}
