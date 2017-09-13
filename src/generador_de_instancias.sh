#!/bin/bash
rm -R casos_de_prueba
mkdir casos_de_prueba
for i in 20 30 40 45 50 60
do
  for j in 0.1 0.3 0.5 0.7 0.9
  do
    for k in 1 2 3
    do
      echo "0 $k 1 $i 1 $j" >> casos_de_prueba/temp
      ./Generator/generator casos_de_prueba/$i"_"$j"_"$k < casos_de_prueba/temp
      rm casos_de_prueba/temp
    done
  done
done