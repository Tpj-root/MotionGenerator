#!/bin/bash

# Create temporary directory
mkdir -p temp_frames

# Extract all frames from each GIF
for gif in plot_V*_output.gif; do
    convert "$gif" -coalesce temp_frames/"${gif%.gif}"_%03d.gif
done

# Combine frames from all into grid per frame
frame_count=$(ls temp_frames/plot_V1_output_*.gif | wc -l)

mkdir -p temp_combined

for i in $(seq -f "%03g" 0 $((frame_count - 1))); do
    convert +append temp_frames/*_$i.gif temp_combined/row_$i.gif
done

# Combine rows vertically to form the final frames
for i in $(seq -f "%03g" 0 $((frame_count - 1))); do
    convert -append $(ls temp_combined/row_$i.gif | sort) combined_$i.gif
done

# Create final animated GIF
convert -delay 20 -loop 0 combined_*.gif final_grid_animation.gif

# Cleanup
rm -r temp_frames temp_combined combined_*.gif
