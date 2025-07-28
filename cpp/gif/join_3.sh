#!/bin/bash

# Define 10 input GIFs (ordered row-wise for 4x3 layout)
GIFS=(
    "plot_V1_output.gif"  "plot_V4_output.gif"  "plot_V7_output.gif"
    "plot_V2_output.gif"  "plot_V5_output.gif"  "plot_V8_output.gif"
    "plot_V10_output.gif" "plot_V3_output.gif"  "plot_V6_output.gif"
    "plot_V9_output.gif"
)

# Create working folder
mkdir -p tmp_4x3
cd tmp_4x3

# Extract frames for each GIF
for i in "${!GIFS[@]}"; do
    convert ../"${GIFS[$i]}" "gif${i}_%02d.png"
done

# Get number of frames (assuming all GIFs have same count)
frame_count=$(ls gif0_*.png | wc -l)

# Create combined frames
mkdir -p combined
for ((f=0; f<frame_count; f++)); do
    frame=$(printf "%02d" $f)

    # Rows: combine each row horizontally
    convert +append gif0_${frame}.png gif1_${frame}.png gif2_${frame}.png row1.png
    convert +append gif3_${frame}.png gif4_${frame}.png gif5_${frame}.png row2.png
    convert +append gif6_${frame}.png gif7_${frame}.png gif8_${frame}.png row3.png
    convert +append gif9_${frame}.png null: null: row4.png

    # Stack rows vertically
    convert -append row1.png row2.png row3.png row4.png combined/frame_${frame}.png
done

# Make final animated GIF
convert -delay 20 -loop 0 combined/frame_*.png ../final_4x3.gif

# Clean up
cd ..
rm -rf tmp_4x3

echo "✅ Output saved: final_4x3.gif"
