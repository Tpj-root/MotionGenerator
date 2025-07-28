import matplotlib
matplotlib.use('Agg')

import os
import pandas as pd
import matplotlib.pyplot as plt
from PIL import Image

# Folder with CSV files
folder = "FILES"
output_images = []

# Step 1: Generate individual PNG plots
for acc in range(1, 11):
    filename = f"{acc:03}_maxVelocity_1_maxAcceleration_{acc}_motion_output.csv"
    filepath = os.path.join(folder, filename)
    df = pd.read_csv(filepath)

    # Plot
    plt.figure(figsize=(5, 4))
    plt.plot(df["Time(ms)"], df["Position"], label=f"Acc={acc}")
    plt.xlabel("Time (ms)")
    plt.ylabel("Position")
    plt.title(f"Velocity=1, Acc={acc}")
    plt.grid(True)
    plt.tight_layout()

    # Save individual PNG
    img_name = f"plot_acc_{acc}.png"
    plt.savefig(img_name)
    output_images.append(img_name)
    plt.close()

print("✅ All individual plots saved.")

# Step 2: Combine into 5x2 grid using PIL
grid_width = 5
grid_height = 2
single_width, single_height = Image.open(output_images[0]).size

# Create new blank image
combined = Image.new("RGB", (grid_width * single_width, grid_height * single_height), "white")

# Paste each plot into grid
for idx, img_path in enumerate(output_images):
    img = Image.open(img_path)
    x = (idx % grid_width) * single_width
    y = (idx // grid_width) * single_height
    combined.paste(img, (x, y))

# Save final combined image
combined.save("motion_grid_5x2.png")
print("✅ Combined image saved as motion_grid_5x2.png")
