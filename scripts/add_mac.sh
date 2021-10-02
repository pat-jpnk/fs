#!/usr/bin/env bash

NAME="fs"

cd /etc/paths.d && touch $NAME && echo '/usr/local/$NAME/bin'

cd /usr/local && mkdir $NAME && cd $NAME && mkdir bin && cd bin && cd "${0%/*}"

cp fs /usr/local/$NAME/bin 

#touch test.txt && echo 'adad' >> test.txt || sudo cp test.txt ./test





#How to add to mac

#x = command name 

#/etc/paths.d 

#add ascii file "x" 
#	content: /usr/local/"x"/bin


#in /usr/local

#create /"x"/bin

#insert binary executable into /bin with name "x"  


