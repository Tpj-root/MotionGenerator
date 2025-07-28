#!/bin/bash

# Input GIFs
GIF1="plot_V1_output.gif"
GIF2="plot_V2_output.gif"
GIF3="plot_V3_output.gif"
GIF4="plot_V4_output.gif"

# Temp folder for frames
mkdir -p tmp_frames
cd tmp_frames

# Split each gif into frames
convert ../$GIF1 gif1_%02d.png
convert ../$GIF2 gif2_%02d.png
convert ../$GIF3 gif3_%02d.png
convert ../$GIF4 gif4_%02d.png

# Number of frames (assumes all gifs have same frame count)
frames=$(ls gif1_*.png | wc -l)

# Combine corresponding frames into 2x2 grid
mkdir -p combined
for ((i=0; i<frames; i++)); do
    frame=$(printf "%02d" $i)
    convert \( gif1_${frame}.png gif2_${frame}.png +append \) \
            \( gif3_${frame}.png gif4_${frame}.png +append \) \
            -append combined/frame_${frame}.png
done

# Create final animated gif
convert -delay 20 -loop 0 combined/frame_*.png ../final_2x2.gif

# Clean up
cd ..
rm -rf tmp_frames

echo "✅ Output: final_2x2.gif"
