#!/bin/bash
if [ -z "$1" ]
  then
    echo "I need one argument: the root folder of the MQWeb GitHub pages"
    exit
fi

declare -A languages=( ["php"]="*.php" ["perl"]="*.pl" ["ruby"]="*.rb" ["js"]="*.js" ["python"]="*.py" )

shopt -s nullglob

for language in "${!languages[@]}"
do
  for f in ./$language/${languages["$language"]}
  do 
    p=`basename $f | cut -f1 -d.`
    echo "processing $f"
    pygmentize -O tabsize=2 -o $1/api/samples/$language/$p.html $f
  done
done

