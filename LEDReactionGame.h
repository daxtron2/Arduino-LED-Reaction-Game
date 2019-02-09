// PINS
const int buttonPin = A0;      // select the input pin
const int buzzerPin = 12;      // pin for Buzzer
const int greenLedPin = 5;     // select the pin for the green LED
const int redLedPin = 6;       // pin for red LED
const int greenButtonPin = A0; // pin for green button
const int redButtonPin = A1;   // pin for red button

// General Constants
const int goodToneHz = NOTE_D6; // Frequency of good tone in Hertz
const int badToneHz = NOTE_D3;  // Frequency of bad tone in Hertz
const int toneDuration = 100;   // Duration of tone in ms
const int minLedOn = 100;       // min time for LED in ms
const int maxLedOn = 900;       // max time for LED in ms
int numCorrectNeeded = 20;      // Score needed to "win", plays jingle when reached. Default 20

//Green LED stuff
unsigned long greenOnFor = random(minLedOn, maxLedOn); // Duration of LED
bool greenState = LOW;                                 // Current on/off state
bool greenCorrect = false;                             // Whether it was pressed correctly this frame
bool greenLastButtonState = 0;                         // Whether it was pressed or not last frame
unsigned long greenPreviousMillis = 0;                 // Last time its state was changed

//Red LED stuff
unsigned long redOnFor = random(minLedOn, maxLedOn); // Duration of LED
bool redState = LOW;                                 // Current on/off state
bool redCorrect = false;                             // Whether it was pressed correctly this frame
bool redLastButtonState = false;                     // Whether it was pressed or not last frame
unsigned long redPreviousMillis = 0;                 // Last time its state was changed

//to store how many have been correct
unsigned int numCorrect = 0;