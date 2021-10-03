#!/usr/bin/env bash

P=${pwd}
NAME="fs"

cd /etc/paths.d && touch $NAME && echo '/usr/local/$NAME/bin' >> $NAME

cd /usr/local && mkdir $NAME && cd $NAME && mkdir bin 

cd $P && cp $NAME /usr/local/$NAME/bin
