/*

FIX THE LOG BUG

This is a maze solving robot utilizing the Left Hand Rule and a simplifying algorithm.

*Might need to add a delay on a direction change
in order to stop false sensing


LEFT HAND RULE
--------------

1. If at INTERSECTION or WALL turn LEFT
2. If CANNOT go LEFT go FORWARDS
3. If CANNOT go FORWARDS go RIGHT
4. If CANNOT go RIGHT go BACKWARDS

LOGGING
-------

Directions will be logged in the foloowing Symbols:

L: Left Shift
R: Right Shift
S: Straight
U: U-Turn

Simplifying Algorithm
---------------------
LUR = U
LUS = R
RUL = U
SUL = R
SUS = U
LUL = S

*/

#include <Arduino.h>
#include <NewPing.h>

#define SONAR_NUM 4      // Number of sensors.
#define MAX_DISTANCE 200 // Maximum distance (in cm) to ping.


NewPing sonar[SONAR_NUM] = {   // Sensor object array.
  NewPing(10, 11, MAX_DISTANCE), //Sensor 0
  NewPing(12, 13, MAX_DISTANCE), //Sensor 1
  NewPing(A0, A1, MAX_DISTANCE), //Sensor 2
  NewPing(A2, A3, MAX_DISTANCE)//Sensor 3

};

int temporary[4][9];

int directions [5][9] = { // Row 1-4 Hold Data on each Direction. //Column 1-8: High and Low Data // Column 9: Ultrasonic Sensor
// Direction Can be Manipulated by Changing the Row. No need for an extra column
    {1,0,1,0,1,0,1,0,0}, // 0| Default Direction Forward
    {0,1,1,0,1,0,0,1,1}, // 1| Default Direction Left {0,1,1,0,1,0,0,1,3}
    {0,1,0,1,0,1,0,1,2}, // 2| Default Direction Back
    {1,0,0,1,0,1,1,0,3}, // 3| Default Direction Right {1,0,0,1,0,1,1,0,1}
    {0,0,0,0,0,0,0,0,0}  // 4| STOPS EVERYTHING
};

int pinsNumbers [8] = {2,3,4,5,6,7,8,9}; //Holds what Pins is Associated with Column Number

void driveMotor(int driveDirection) { // Runs Four Motors of Chosen Direction. 0: Front, 1: Right, 2: Back, 3: Left, 4: STOP
    Serial.println(driveDirection);
    for (int i = 0; i < 8; i++ ) {
        /*
        Serial.print("Pin: ");
        Serial.println(pinsNumbers[i]);
        Serial.print("Direction: ");
        Serial.println(directions[driveDirection][i]);
        */
        digitalWrite(pinsNumbers[i], directions[driveDirection][i]);
    }
}


void setup() {
  pinMode(pinsNumbers[0], OUTPUT);
  pinMode(pinsNumbers[1], OUTPUT);
  pinMode(pinsNumbers[2], OUTPUT);
  pinMode(pinsNumbers[3], OUTPUT);
  pinMode(pinsNumbers[4], OUTPUT);
  pinMode(pinsNumbers[5], OUTPUT);
  pinMode(pinsNumbers[6], OUTPUT);
  pinMode(pinsNumbers[7], OUTPUT);
    driveMotor(4);
    delay(20);
    Serial.begin(115200); //Initiates Serial Monitor
    delay(800);
}


void leftShift() {


    for (int i = 0; i < 4; i++) { // Copies Array Data to a Temporary Array
        for (int j = 0; j < 9; j++) {
            temporary[i][j] = directions[i][j];
        }
    }
// Assigning New Rows. Below we have 3 -> 0, 0 -> 1, 1 -> 2, 2 -> 3
    for (int i = 0; i < 9; i++) {
        directions[0][i] = temporary[3][i];
    }
    for (int i = 0; i < 9; i++) {
        directions[1][i] = temporary[0][i];
    }
    for (int i = 0; i < 9; i++) {
        directions[2][i] = temporary[1][i];
    }
    for (int i = 0; i < 9; i++) {
        directions[3][i] = temporary[2][i];
    }

    driveMotor(0); // Running the New Forwards Configuration
    delay(20);

} 

void rightShift() {

    for (int i = 0; i < 4; i++) { // Copies Array Data to a Temporary Array
        for (int j = 0; j < 9; j++) {
            temporary[i][j] = directions[i][j];
        }
    }

    for (int i = 0; i < 9; i++) {
        directions[0][i] = temporary[1][i];
    }
    for (int i = 0; i < 9; i++) {
        directions[1][i] = temporary[2][i];
    }
    for (int i = 0; i < 9; i++) {
        directions[2][i] = temporary[3][i];
    }
    for (int i = 0; i < 9; i++) {
        directions[3][i] = temporary[0][i];
    }

    driveMotor(0);
    delay(20);

}

void uTurn() {

    for (int i = 0; i < 4; i++) { // Copies Array Data to a Temporary Array
        for (int j = 0; j < 9; j++) {
            temporary[i][j] = directions[i][j];
        }
    }

    for (int i = 0; i < 9; i++) {
        directions[0][i] = temporary[2][i];
    }
    for (int i = 0; i < 9; i++) {
        directions[1][i] = temporary[3][i];
    }
    for (int i = 0; i < 9; i++) {
        directions[2][i] = temporary[0][i];
    }
    for (int i = 0; i < 9; i++) {
        directions[3][i] = temporary[1][i];
    }

    driveMotor(0);
    delay(20);


}

/*
Simplifying Algorithm
---------------------
LUR = U
LUS = R
RUL = U
SUL = R
SUS = U
LUL = S
*/

int emptyDistance = 10; // Set Distance for Detecting Emptiness
int objectDistance = 3; // Set Distance for Detecting a Barrier

void loop() {

    driveMotor(0); // Makes Robot Run Forwards
    delay(20); // Michael

    if (sonar[directions[0][9]].ping_cm() >= objectDistance || sonar[directions[1][9]].ping_cm() >= emptyDistance // Addresses all scenarios where a decision needs to be made
    || sonar[directions[3][9]].ping_cm() >= emptyDistance) {

        driveMotor(4); // Stops and enabling arduino to analyze environment
        delay(20);

        if (sonar[directions[0][9]].ping_cm() >= emptyDistance && sonar[directions[1][9]].ping_cm() >= emptyDistance && sonar[directions[3][9]].ping_cm() >= emptyDistance) { // Detects whether the maze is complete 
        Serial.print("FINISH!");
        // ADD MORE CODE (MAYBE A METHOD) TO WAIT FOR A BUTTON PRESS
    }

        else if (sonar[directions[3][9]].ping_cm() >= emptyDistance) { //Scenario 1: Left Turn Available
            leftShift();
        }

        else if (sonar[directions[0][9]].ping_cm() >= emptyDistance) { //Scenario 2: Left Turn Unavailable but Forwards Available
            driveMotor(0);
            delay(20);
        }

        else if (sonar[directions[1][9]].ping_cm() >= emptyDistance) { //Scenario 3: Left Turn Unavailable, Forwards Unavailable, but Right Available
            rightShift();
        }

        else if (sonar[directions[2][9]].ping_cm() >= emptyDistance) { //Scenario 4: NOTHING AVAILABLE UTURN. (IF this Doesn't work this is broken)
            uTurn();
        }
}

}