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
    mkdir -p $1/_includes/samples/$language
    cp $f $1/_includes/samples/$language/$p.md
    ed -s $1/_includes/samples/$language/$p.md <<EOF
0a
{% highlight $language %}
.
\$a
{% endhighlight %}
.
w
EOF
  done
done

