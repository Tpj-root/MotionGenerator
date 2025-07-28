#!/bin/bash

# Input GIFs
gifs=(plot_V1_output.gif plot_V2_output.gif plot_V3_output.gif plot_V4_output.gif plot_V5_output.gif \
      plot_V6_output.gif plot_V7_output.gif plot_V8_output.gif plot_V9_output.gif plot_V10_output.gif)

# Temporary files for each row
convert -delay 20 -loop 0 ${gifs[0]} ${gifs[1]} ${gifs[2]} ${gifs[3]} +append row1.gif
convert -delay 20 -loop 0 ${gifs[4]} ${gifs[5]} ${gifs[6]} ${gifs[7]} +append row2.gif
convert -delay 20 -loop 0 ${gifs[8]} ${gifs[9]} null: null: +append row3.gif

# Combine rows vertically
convert -delay 20 -loop 0 -append row1.gif row2.gif row3.gif final_4x3.gif

# Clean up
rm row1.gif row2.gif row3.gif
