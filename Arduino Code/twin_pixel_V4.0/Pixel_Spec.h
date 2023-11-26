/*  added by Mark Donners, The Electronic Engineer
 *  For implementation of pixelleds spectrum analyzer
 *  Assuming a matrix of pixelleds. no more than 16 channels 
 *  256 leds ( 16x16 )
 */

// matrix 16x16 leds
#define kMatrixHeight 16
#define kMatrixWidth 52//52
#define numBands 52//52 // should match the number of bins below 16,25,52,69
#define Startbin binStart52[i] // options binStart16[i] binStart25[i],binStart52[i] binStart69[i]
#define Stopbin   binEnd52[i] // options binEnd16[i] binEnd25[i], binEnd42[i] , binEnd69[i]
#define TOP kMatrixHeight

#define SENSEPOT 10 //Pot 1
#define BRIGHTPOT 11 //pot 2
#define PEAKDELAYPOT 1 //= ledpin 

// what pin are the leds connected to?
#define LED_PIN 16

float FreqBinsNew[numBands];
float FreqBins[numBands];
float FreqBinsOld[numBands];
float peak[numBands];
char PeakFlag[numBands];                           // the top peak delay needs to have a flag because it has different timing while floating compared to falling to the stack
int PeakTimer[numBands];                           // counter how many loops to stay floating before falling to stack
int Fallingspeed=10;
int Peakdelay=30;
int LedMode=1; // the decide on the patterns for the leds
int LedModeMax=7;

int BRIGHTNESS=50;
int BRIGHTNEW=0;
int BRIGHTOLD=50;
// Colors mode 1 These are the colors from the TRIBAR 
#define TriBar_Color_Top      0 , 255, 255    // Red CHSV
#define TriBar_Color_Bottom   95 , 255, 255   // Green CHSV
#define TriBar_Color_Middle   45, 255, 255    // Yellow CHSV
