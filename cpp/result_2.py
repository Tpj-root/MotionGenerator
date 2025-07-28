import matplotlib
matplotlib.use('Agg')

import os
import re
import pandas as pd
import matplotlib.pyplot as plt

# Set input/output paths
input_dir = "FILES"
output_dir = "PLOTS"
os.makedirs(output_dir, exist_ok=True)

# Get sorted list of all CSVs
files = sorted([f for f in os.listdir(input_dir) if f.endswith(".csv")])

# Regex to extract V and A
pattern = re.compile(r"maxVelocity_(\d+)_maxAcceleration_(\d+)")

# Loop and plot
for file in files:
    match = pattern.search(file)
    if not match:
        continue
    vel = int(match.group(1))
    acc = int(match.group(2))

    df = pd.read_csv(os.path.join(input_dir, file))

    # Plot
    plt.figure(figsize=(5, 4))
    plt.plot(df["Time(ms)"], df["Position"])
    plt.title(f"Velocity={vel}, Acc={acc}")
    plt.xlabel("Time (ms)")
    plt.ylabel("Position")
    plt.grid(True)
    plt.tight_layout()

    # Save as image
    out_name = f"plot_V{vel}_A{acc}.png"
    plt.savefig(os.path.join(output_dir, out_name))
    plt.close()

print("✅ 100 plots saved in 'PLOTS' folder.")
