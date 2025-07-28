#ifndef __MOTIONGENERATOR_H__
#define __MOTIONGENERATOR_H__

// We don't need Arduino headers for desktop use
// So comment out or remove these for Linux/PC
// #if defined(ARDUINO) && ARDUINO >= 100
// #include "Arduino.h"
// #else
// #include "WProgram.h"
// #endif

#include <cmath>       // For math functions like abs(), sqrt()
#include <algorithm>   // For min/max, clamp, etc.




/**
 * Generates the analytical solution for the trapezoidal motion.
 *
 * <p>
 * Usage:
 * // Includes
 * #include "MotionGenerator.h"
 *
 * Initialization
 *
 * @param int aVelMax maximum velocity (units/s)
 * @param int aAccMax maximum acceleration (units/s^2)
 * @param int aInitPos initial position (units)
 *

 // Define the MotionGenerator object
 MotionGenerator *trapezoidalProfile = new MotionGenerator(100, 400, 0);

 // Retrieve calculated position
 float positionRef = 1000;
 float position = trapezoidalProfile->update(positionRef)

 // Retrieve current velocity
 float velocity = trapezoidalProfile->getVelocity();

 // Retrieve current acceleration
 float acceleration = trapezoidalProfile->getAcceleration();

 // Check if profile is finished
 if (trapezoidalProfile->getFinished()) {};

 // Reset internal state
 trapezoidalProfile->reset();

 *
 * @author      AerDronix <aerdronix@gmail.com>
 * @web		https://aerdronix.wordpress.com/
 * @version     1.0 
 * @since       2016-12-22
 */


/**
 * Class to generate trapezoidal motion profiles.
 * This is useful in robotics or CNC to move from one position to another 
 * smoothly, respecting maximum speed (velocity) and acceleration limits.
 */

/*
Example Usage:

#include "MotionGenerator.h"

// Create profile with maxVel=100, maxAcc=400, startPos=0
MotionGenerator *trapezoidalProfile = new MotionGenerator(100, 400, 0);

// Call update() repeatedly to move toward desired position (e.g., 1000)
float positionRef = 1000;
float position = trapezoidalProfile->update(positionRef);

// Get current velocity and acceleration
float velocity = trapezoidalProfile->getVelocity();
float acceleration = trapezoidalProfile->getAcceleration();

// Check if motion has reached the target
if (trapezoidalProfile->getFinished()) {
   // Done
}

// You can reset the profile to reuse it
trapezoidalProfile->reset();
*/


class MotionGenerator {		
		/**	
		 * Constructor
		 * 
		 * @param int aVelocityMax maximum velocity
		 * @param int aAccelerationMax maximum acceleration
		 */
	public:
	    // Constructor: takes max velocity, acceleration, and initial position
	    MotionGenerator(float aMaxVel, float aMaxAcc, float aInitPos);
	
	    // Internal initialization — sets timing, state, etc.
	    void init();
	
		/**	
		 * Updates the state, generating new setpoints
		 *
		 * @param aSetpoint The current setpoint.
		 */
	    // Main function to call regularly — updates motion
	    float update(float aPosRef);
	
	    // Accessors
	    float getVelocity();
	    float getAcceleration();
	    bool getFinished();  // True if target reached
	
	    // Setters — change motion limits or reset start position
	    void setMaxVelocity(float aMaxVel);
	    void setMaxAcceleration(float aMaxAcc);
	    void setInitPosition(float aInitPos);
	
	    // Reset state: position, velocity, time, etc.
	    void reset();


	private:
		/** 	
		 * Increments the state number.
		 * 
		 * @see
		  currentState
		 */		
	    // Computes trapezoidal motion based on target
	    void calculateTrapezoidalProfile(float target);
	
	    // Helper: returns sign of value (-1, 0, or 1)
	    short int sign(float aVal);
	
	    // Motion parameters
	    float maxVel;     // Max allowed velocity
	    float maxAcc;     // Max allowed acceleration
	    float initPos;    // Initial start position
	
	    // Current motion state
	    float pos;        // Current position
	    float vel;        // Current velocity
	    float acc;        // Current acceleration
	    float oldPos;     // Last position
	    float oldPosRef;  // Previous target
	    float oldVel;     // Previous velocity
	
	    // Distance calculations
	    float dBrk;       // Distance to stop
	    float dAcc;       // Distance to accelerate
	    float dVel;       // Constant speed distance
	    float dDec;       // Distance to decelerate
	    float dTot;       // Total distance
	
	    // Time durations for each phase
	    float tBrk;       // Time to brake
	    float tAcc;       // Time to accelerate
	    float tVel;       // Time at constant velocity
	    float tDec;       // Time to decelerate
	
	    float velSt;      // Velocity at start of motion
	
	    // Timing
	    unsigned long oldTime;    // Time at last update
	    unsigned long lastTime;   // For delta calculation
	    unsigned long deltaTime;  // Time difference
	

        // short int signM;      	// 1 = positive change, -1 = negative change
        // bool shape;      	// true = trapezoidal, false = triangular

	    short int signM;   // Direction of motion (+1 or -1)
	    bool shape;        // true = trapezoid, false = triangle
	    bool isFinished;   // Has the target been reached
	};


#endif
