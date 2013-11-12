#!/bin/bash
for i in 10
do 
touch userData/newFile.txt
./backup -s /home/adminuser/CS/CS240/project3/userData
done
