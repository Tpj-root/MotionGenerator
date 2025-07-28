#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <iomanip>      // For leading zeros in file index
#include "MotionGenerator.h"
#include <sstream>


int main() {
    int fileIndex = 1;  // Start index for filenames (001 to 100)

    // Outer loop: test maxVelocity from 1 to 10
    for (int maxVel = 1; maxVel <= 10; ++maxVel) {

        // Inner loop: test maxAcceleration from 1 to 10
        for (int maxAcc = 1; maxAcc <= 10; ++maxAcc) {

            // Create MotionGenerator instance with current velocity & acceleration
            // Initial position is set to 0.0
            MotionGenerator motionGen(static_cast<float>(maxVel), static_cast<float>(maxAcc), 0.0f);

            // Create filename with leading zeros and current parameters
            std::ostringstream filename;
            filename << std::setw(3) << std::setfill('0') << fileIndex
                     << "_maxVelocity_" << maxVel
                     << "_maxAcceleration_" << maxAcc
                     << "_motion_output.csv";

            // Open CSV file to save results
            std::ofstream outFile(filename.str());

            // CSV Header: Time and Position
            outFile << "Time(ms),Position\n";

            // Simulate motion over 100 steps (each step = 100ms)
            for (int i = 0; i < 100; ++i) {
                // Target reference position is always 10.0 units
                float output = motionGen.update(10.0f);

                // Simulated time in milliseconds
                unsigned long time = i * 100;

                // Print to terminal (optional)
                std::cout << "File " << fileIndex
                          << " | V=" << maxVel << ", A=" << maxAcc
                          << " | Time: " << time
                          << " ms, Position: " << output << std::endl;

                // Write data to CSV
                outFile << time << "," << output << "\n";

                // Wait for 100 ms to simulate real-time step
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }

            // Close file after 100 steps
            outFile.close();

            // Confirm saved
            std::cout << "Saved: " << filename.str() << "\n\n";

            // Move to next file
            fileIndex++;
        }
    }

    // All done
    std::cout << "All 100 simulations complete. Check the CSV files.\n";

    return 0;
}
