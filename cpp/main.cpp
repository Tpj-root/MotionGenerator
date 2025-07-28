#include <iostream>     // For printing to terminal using std::cout
#include <fstream>      // For writing data to a file
#include <chrono>       // For working with time (delay, timestamp)
#include <thread>       // For using sleep (to simulate time intervals)
#include "MotionGenerator.h"  // Your custom class for trapezoidal motion


int main() {
    // Create a MotionGenerator object
    // Parameters: maxVelocity = 2.0 units/s, maxAcceleration = 1.0 units/s^2, initialPosition = 0.0
    MotionGenerator motionGen(2.0, 1.0, 0.0);


    // Create a CSV file to log position values over time
    std::ofstream outFile("motion_output.csv");

    // Write the CSV header: column names
    outFile << "Time(ms),Position\n";


    // Run a simulation loop for 100 steps (i = 0 to 99)
    for (int i = 0; i < 100; ++i) {
        // Call the motion profile update function
        // It moves the object toward the target position (10.0 units)
        float output = motionGen.update(10.0);

        // Calculate simulated time in milliseconds (100 ms per loop)
        unsigned long time = i * 100;


        // Print current time and position to terminal
        std::cout << "Time: " << time << " ms, Position: " << output << std::endl;

        // Also write the same data to the CSV file for plotting later
        outFile << time << "," << output << "\n";


        // Pause the loop for 100 milliseconds to simulate real-time movement
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    // Close the CSV file once the loop is done
    outFile.close();

    // Notify user that file is saved
    std::cout << "Output saved to motion_output.csv\n";

    // End of program
    return 0;
}
