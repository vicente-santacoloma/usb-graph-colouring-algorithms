#!/bin/bash
# Generador de tabla en latex para los resultados obtenidos
# de los algoritmos brown, brown-korman y brelaz

if [ -e GraphColouringResult.tex ] 
then
  rm GraphColouringResult.tex
fi
touch GraphColouringResult.tex

FILE='GraphColouringResult.tex'

echo '\documentclass{article}' >> $FILE
echo '' >> $FILE
echo '\begin{document}' >> $FILE
echo '\begin{tabular}{|c|c|c|c|c|c|c|c|}' >> $FILE
echo '\hline' >> $FILE
echo '\multicolumn{1}{|c|}{} &' >> $FILE
echo '\multicolumn{1}{|c|}{} &' >> $FILE
echo '\multicolumn{3}{|c|}{Coloracion Promedio} &' >> $FILE
echo '\multicolumn{3}{|c|}{Tiempo Promedio} \\' >> $FILE
echo '\hline' >> $FILE
echo '\multicolumn{1}{|c|}{n} &' >> $FILE
echo '\multicolumn{1}{|c|}{d} &' >> $FILE
echo '\multicolumn{1}{|c|}{Brown} &' >> $FILE
echo '\multicolumn{1}{|c|}{Brown-Korman} &' >> $FILE
echo '\multicolumn{1}{|c|}{Brelaz} &' >> $FILE
echo '\multicolumn{1}{|c|}{Brown} &' >> $FILE
echo '\multicolumn{1}{|c|}{Brown-Korman} &' >> $FILE
echo '\multicolumn{1}{|c|}{Brelaz} \\' >> $FILE
echo '\hline' >> $FILE

for i in 20 30 40 45 50 60
do
  ROW="$i & "
  for j in 10 30 50 70 90
  do
    ROW=$ROW"$j & "
    m = 0
    for k in out_brown out_brown-korman out_brelaz
    do
      for l in 1 2 3
      do
        C[$l]=`cut -d " " -f 1 ./$k/$i_$j_$l`
        T[$l]=`cut -d " " -f 2 ./$k/$i_$j_$l`
      done
      CP[$m]=`echo "scale=5;(${C[1]}+${T[2]}+${C[3]})/3"|bc`
      TP[$m]=`echo "scale=5;(${T[1]}+${T[2]}+${T[3]})/3"|bc`
      m=$(($m+1))
    done
    ROW=$ROW"${CP[0]} & ${CP[1]} & ${CP[2]} & ${TP[0]} & ${TP[1]} & ${TP[2]} \\"
  done
  echo $ROW" \hline" >> $FILE 
done

echo '\end{tabular}' >> $FILE
echo '\end{document}' >> $FILE