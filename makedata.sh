#!/bin/bash

dir=$1

for f in ${dir}/post*.xml
do
    b1=$(xmlstarlet sel --template --value-of "/item/wp:postmeta[wp:meta_key='blogger_permalink']/wp:meta_value" $f)
    b2=http://anomalyuk.blogspot.com${b1}
    w1=$(xmlstarlet sel --template --value-of "/item/link" $f)
    w2=${w1/http:\/\/blog.anomalyuk.party\/blog/https:\/\/www.anomalyblog.co.uk}

    if [[ -n $b1 ]]
    then
        echo $b2 $w2
    fi
done

   
