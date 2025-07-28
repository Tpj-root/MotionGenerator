import matplotlib
matplotlib.use('Agg')  # Use non-GUI backend for image saving

import os
import pandas as pd
import matplotlib.pyplot as plt

# Folder where CSV files are stored
folder = "FILES"

# Create a plot
plt.figure(figsize=(10, 6))

# Loop through acceleration values from 1 to 10
for acc in range(1, 11):
    # Construct filename with zero-padding
    filename = f"{acc:03}_maxVelocity_1_maxAcceleration_{acc}_motion_output.csv"
    filepath = os.path.join(folder, filename)

    # Read the CSV data
    df = pd.read_csv(filepath)

    # Plot the curve
    plt.plot(df["Time(ms)"], df["Position"], label=f"Acc={acc}")

# Add labels and title
plt.xlabel("Time (ms)")
plt.ylabel("Position")
plt.title("Motion Profile (maxVelocity = 1, Acceleration = 1 to 10)")
plt.legend(title="Acceleration")
plt.grid(True)

# Save the plot
plt.tight_layout()
plt.savefig("plot_maxVel_1.png")
print("Saved plot as plot_maxVel_1.png")
