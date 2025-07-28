import matplotlib
matplotlib.use('Agg')

import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("motion_output.csv")
plt.plot(df["Time(ms)"], df["Position"])
plt.xlabel("Time (ms)")
plt.ylabel("Position")
plt.title("Motion Profile")
plt.grid(True)

plt.savefig("motion_plot.png")
print("Saved to motion_plot.png")
