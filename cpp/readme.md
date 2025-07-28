

csv to image





```
install

git clone 'https://github.com/Tpj-root/miniconda.git'



```





ALL_IN_ONE

```
convert -delay 20 -loop 0 plot_V*_A*.png output.gif

```


for v in {1..10}; do
    convert -delay 20 -loop 0 plot_V${v}_A*.png plot_V${v}_output.gif
done


