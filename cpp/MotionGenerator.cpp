#include "MotionGenerator.h"
#include <chrono>  // Required for custom millis() implementation
#include <cmath>   // Required for abs(), sqrt(), etc.
#include <algorithm> // For min(), max() if needed



// Constructor
// Initializes motion generator with max velocity, acceleration, and initial position.
// Also calls init() to set initial state.
MotionGenerator::MotionGenerator(float aMaxVel, float aMaxAcc, float aInitPos)
    : maxVel(aMaxVel), maxAcc(aMaxAcc), initPos(aInitPos)
{
    init();  // Setup internal state
}




// Replacement for Arduino's millis() function — returns time since program started in milliseconds
unsigned long millis() {
    static auto start = std::chrono::steady_clock::now();  // Initial time at first call
    auto now = std::chrono::steady_clock::now();           // Current time
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count(); // Difference in ms
    return static_cast<unsigned long>(ms);
}




// Initialize internal variables
void MotionGenerator::init() {
    oldTime = millis();    // Save starting time
    lastTime = oldTime;    // Set last update time
    deltaTime = 0;         // Time step between updates

    reset();               // Clear position, velocity, etc.

    signM = 1;             // Direction of motion: +1 or -1
    shape = true;          // true = trapezoidal, false = triangular profile
    isFinished = false;    // Flag to indicate whether motion is complete
}


// Main update function — call this repeatedly to move toward posRef
float MotionGenerator::update(float posRef) {

    // Check if target position has changed since last update
    if (oldPosRef != posRef)
    {
        isFinished = false;

        // Save current state before recomputing motion
        oldPosRef = posRef;
        oldPos = pos;
        oldVel = vel;
        oldTime = lastTime;

        // Calculate braking time and distance (if deceleration is needed)
        tBrk = abs(oldVel) / maxAcc;
        dBrk = tBrk * abs(oldVel) / 2;

        // Determine direction of movement (+1 or -1)
        signM = sign(posRef - (oldPos + sign(oldVel) * dBrk));

        // If direction changes, braking is needed
        if (signM != sign(oldVel)) {
            tAcc = maxVel / maxAcc;
            dAcc = tAcc * (maxVel / 2);
        } else {
            tBrk = 0;
            dBrk = 0;
            tAcc = (maxVel - abs(oldVel)) / maxAcc;
            dAcc = tAcc * (maxVel + abs(oldVel)) / 2;
        }

        // Total distance after braking
        dTot = abs(posRef - oldPos + signM * dBrk);

        // Deceleration phase
        tDec = maxVel / maxAcc;
        dDec = tDec * (maxVel) / 2;

        // Constant speed phase
        dVel = dTot - (dAcc + dDec);
        tVel = dVel / maxVel;

        // Decide whether it's a trapezoid or triangle profile
        if (tVel > 0) {
            shape = true;  // Enough space for full speed
        } else {
            shape = false; // Not enough space → triangular profile

            if (signM != sign(oldVel)) {
                // Start from rest or reverse → use triangle calculation
                velSt = sqrt(maxAcc * dTot);
                tAcc = velSt / maxAcc;
                dAcc = tAcc * (velSt / 2);
            } else {
                // Already moving in same direction
                tBrk = 0;
                dBrk = 0;
                dTot = abs(posRef - oldPos);
                velSt = sqrt(0.5 * oldVel * oldVel + maxAcc * dTot);
                tAcc = (velSt - abs(oldVel)) / maxAcc;
                dAcc = tAcc * (velSt + abs(oldVel)) / 2;
            }

            // Deceleration for triangle
            tDec = velSt / maxAcc;
            dDec = tDec * velSt / 2;
        }
    }

	// Get current time and calculate how much time has passed since last update
	unsigned long time = millis();		// current time
	// Calculate time since last set-point change
	deltaTime = (time - oldTime);
	// Calculate new setpoint
	// Generate new position/velocity/acceleration
	calculateTrapezoidalProfile(posRef);
	// Update last time
	    // Save current time for next update
	lastTime = time;
	
	//calculateTrapezoidalProfile(setpoint);
	    // Return current position to caller
	return pos;
}




void MotionGenerator::calculateTrapezoidalProfile(float posRef) {
    // Convert deltaTime from milliseconds to seconds for motion math
    float t = static_cast<float>(deltaTime) / 1000.0f;

    // ----- CASE 1: Trapezoidal profile -----
    if (shape) {
        // === Phase 1: Acceleration (including braking if needed) ===
        if (t <= (tBrk + tAcc)) {
            pos = oldPos + oldVel * t + signM * 0.5f * maxAcc * t * t;  // position using kinematic equation
            vel = oldVel + signM * maxAcc * t;                          // velocity = v0 + at
            acc = signM * maxAcc;                                       // constant acceleration
        }
        // === Phase 2: Constant velocity ===
        else if (t > (tBrk + tAcc) && t < (tBrk + tAcc + tVel)) {
            pos = oldPos + signM * (-dBrk + dAcc + maxVel * (t - tBrk - tAcc));
            vel = signM * maxVel;                                       // max velocity
            acc = 0;                                                    // no acceleration
        }
        // === Phase 3: Deceleration ===
        else if (t >= (tBrk + tAcc + tVel) && t < (tBrk + tAcc + tVel + tDec)) {
            float tDecPhase = t - tBrk - tAcc - tVel;
            pos = oldPos + signM * (
                -dBrk + dAcc + dVel + maxVel * tDecPhase - 0.5f * maxAcc * tDecPhase * tDecPhase
            );
            vel = signM * (maxVel - maxAcc * tDecPhase);
            acc = -signM * maxAcc;  // negative acceleration
        }
        // === Phase 4: Motion complete ===
        else {
            pos = posRef;
            vel = 0;
            acc = 0;
            isFinished = true;
        }
    }

    // ----- CASE 2: Triangular profile (shorter distances) -----
    else {
        // === Phase 1: Acceleration (possibly from brake) ===
        if (t <= (tBrk + tAcc)) {
            pos = oldPos + oldVel * t + signM * 0.5f * maxAcc * t * t;
            vel = oldVel + signM * maxAcc * t;
            acc = signM * maxAcc;
        }
        // === Phase 2: Deceleration ===
        else if (t > (tBrk + tAcc) && t < (tBrk + tAcc + tDec)) {
            float tDecPhase = t - tBrk - tAcc;
            pos = oldPos + signM * (
                -dBrk + dAcc + velSt * tDecPhase - 0.5f * maxAcc * tDecPhase * tDecPhase
            );
            vel = signM * (velSt - maxAcc * tDecPhase);
            acc = -signM * maxAcc;
        }
        // === Phase 3: Motion complete ===
        else {
            pos = posRef;
            vel = 0;
            acc = 0;
            isFinished = true;
        }
    }
}


bool MotionGenerator::getFinished() {
	return isFinished;  // Returns true if motion is completed
}

float MotionGenerator::getVelocity() {
	return vel;         // Returns current velocity
}

float MotionGenerator::getAcceleration() {
	return acc;         // Returns current acceleration
}



void MotionGenerator::setMaxVelocity(float aMaxVel) {
	maxVel = aMaxVel;   // Update the maximum velocity
}

void MotionGenerator::setMaxAcceleration(float aMaxAcc) {
	maxAcc = aMaxAcc;   // Update the maximum acceleration
}

void MotionGenerator::setInitPosition(float aInitPos) {
	initPos 	= aInitPos;   // Set starting position
	pos 		= aInitPos;   // Reset current position
	oldPos 		= aInitPos;   // Reset old position reference
}



// Used to determine direction of motion.
// Returns -1 for negative, 1 for positive, and 0 for zero.
short int MotionGenerator::sign(float aVal) {
	if (aVal < 0)
		return -1;
	else if (aVal > 0)
		return 1;
	else
		return 0;
}


// This method is automatically called by init() when the motion generator is created.
// You can also call reset() manually if you want to clear the state and start fresh.

void MotionGenerator::reset() {
	// Reset all state variables to initial/default values

	// Position-related
	pos       = initPos;   // Current position set to initial position
	oldPos    = initPos;   // Last position reference also reset
	oldPosRef = 0;         // Last position command (reference)

	// Motion-related
	vel       = 0;         // Velocity reset
	acc       = 0;         // Acceleration reset
	oldVel    = 0;         // Previous velocity

	// Distance variables for motion profile
	dBrk      = 0;         // Braking distance
	dAcc      = 0;         // Acceleration distance
	dVel      = 0;         // Constant velocity distance
	dDec      = 0;         // Deceleration distance
	dTot      = 0;         // Total distance to target

	// Time durations for motion profile
	tBrk      = 0;         // Braking time
	tAcc      = 0;         // Acceleration time
	tVel      = 0;         // Constant velocity time
	tDec      = 0;         // Deceleration time

	// Velocity used in triangular profile
	velSt     = 0;         // Velocity at the peak of triangle (when no cruise phase)
}

