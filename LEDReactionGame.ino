/*
A short reaction speed game for Arduino Nano
More information @ https://github.com/daxtron2/Arduino-LED-Reaction-Game
Hardware & Software by TJ Wolschon
PlayJingle() from https://www.arduino.cc/en/Tutorial/toneMelody
*/

#include "pitches.h"
#include "LEDReactionGame.h"

void setup()
{
    // declare the led pins as an OUTPUT:
    pinMode(greenLedPin, OUTPUT);
    pinMode(redLedPin, OUTPUT);
    Serial.begin(9600);

    //if both buttons are held down at power on, let user choose score
    //otherwise, defaults to 20
    if (analogRead(redButtonPin) != 0 && analogRead(greenButtonPin) != 0)
    {
        //minimum winning score is 1
        numCorrectNeeded = 1;

        //wait for user to release both buttons, otherwise it just skips through
        while (analogRead(redButtonPin) != 0 || analogRead(greenButtonPin) != 0)
        {
            Serial.println("CHOOSE WINNING SCORE MODE ACTIVATED. RELEASE BOTH BUTTONS.");
            digitalWrite(redLedPin, HIGH);
            digitalWrite(greenLedPin, HIGH);
        }

        digitalWrite(redLedPin, LOW);

        //wait for red button to be pressed before starting
        while (analogRead(redButtonPin) == 0)
        {
            digitalWrite(greenLedPin, LOW);

            //instructions
            Serial.print("Press the green LED button to set the score needed to win. ");
            Serial.print("Current Score Needed: ");
            Serial.println(numCorrectNeeded);

            //if green button is pressed
            if (analogRead(greenButtonPin) != 0)
            {
                //add one to the winning score
                numCorrectNeeded++;
                digitalWrite(greenLedPin, HIGH);
                delay(250);
            }
        }
        delay(1500);
    }
    randomSeed(analogRead(A5));
}

void loop()
{
    //Serial.println(analogRead(A0), DEC);

    bool greenButtonState = analogRead(greenButtonPin) != 0;
    bool redButtonState = analogRead(redButtonPin) != 0;

    //handles button presses, sounds, and losing score
    ButtonHandling(greenState, &greenCorrect, greenButtonState, greenLastButtonState);
    ButtonHandling(redState, &redCorrect, redButtonState, redLastButtonState);

    //handles LED pulses and checks if it was correctly pressed this frame for each individually
    //also chooses a new random time to be on for
    //adds a point if pressed correctly
    CheckReady(&greenOnFor, &greenCorrect, &greenState, greenLedPin, &greenPreviousMillis);
    CheckReady(&redOnFor, &redCorrect, &redState, redLedPin, &redPreviousMillis);

    //if we've gotten the amount of points we need to win
    if (numCorrect >= numCorrectNeeded)
    {
        //play a "You Won!" jingle
        PlayJingle();

        //reset numCorrect
        numCorrect = 0;
    }

    //store last button states for next iteration
    greenLastButtonState = greenButtonState;
    redLastButtonState = redButtonState;

    //print out score
    PrintScore();
}

//from the example sketch toneMelody
void PlayJingle()
{
    // notes in the melody:
    int melody[] = {
        NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4};

    // note durations: 4 = quarter note, 8 = eighth note, etc.:
    int noteDurations[] = {
        4, 8, 8, 4, 4, 4, 4, 4};

    for (int thisNote = 0; thisNote < 8; thisNote++)
    {
        // to calculate the note duration, take one second divided by the note type.
        //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
        int noteDuration = 1000 / noteDurations[thisNote];
        tone(buzzerPin, melody[thisNote], noteDuration);

        // to distinguish the notes, set a minimum time between them.
        // the note's duration + 30% seems to work well:
        int pauseBetweenNotes = noteDuration * 1.30;
        delay(pauseBetweenNotes);
        // stop the tone playing:
        noTone(buzzerPin);
    }
}

void CheckReady(unsigned long *ledOnFor, bool *ledCorrect, bool *ledState, int ledPin, unsigned long *previousMillis)
{

    //get current milliseconds since power on
    unsigned long currentMillis = millis();

    //if the LED has been in its power state for the correct amt of time
    if (currentMillis - *previousMillis >= *ledOnFor)
    {

        //store current ms for next frame
        *previousMillis = currentMillis;

        //if LED was correct last flash, add to numCorrect
        if (*ledCorrect)
        {
            //plus 1 score
            numCorrect++;

            //reset state
            *ledCorrect = false;
        }

        //random amount of time for LED to be on
        *ledOnFor = random(minLedOn, maxLedOn);

        //swap power state of LED
        *ledState = !*ledState;

        //push power state of LED
        digitalWrite(ledPin, *ledState);
    }
}

void ButtonHandling(bool ledState, bool *ledCorrect, bool buttonState, bool lastButtonState)
{
    //if button pushed this frame
    if (buttonState && lastButtonState != buttonState)
    {
        //and the LED is on
        if (ledState && !*ledCorrect)
        {
            //play a higher pitched, "correct" noise
            tone(buzzerPin, goodToneHz, toneDuration);

            //correctly pressed this frame
            *ledCorrect = true;
        }
        else
        {
            //play a lower pitched, "wrong" noise
            tone(buzzerPin, badToneHz, toneDuration);

            //didn't correctly press
            *ledCorrect = false;

            //if we have any points
            if (numCorrect > 0)
            {
                //lose a point
                numCorrect--;
            }
        }
    }
}

void PrintScore()
{
    Serial.print("Current Score: ");
    Serial.print(numCorrect);
    Serial.print(" | Score to Win: ");
    Serial.println(numCorrectNeeded);
}