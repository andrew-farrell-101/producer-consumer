#! /bin/sh

# usage: A3_donut_loop.sh  number_of_runs_integer
# so from the shell prompt:  $ ./A3_donut_loop.sh  10 
# will run the proddonuts program and the consdonuts
# programs (5 of them) 10 times and report how many
# of the 10 runs ended in deadlock

if [ -z $1 ]
then
echo " "
echo " "
echo "USAGE: A1_linux_donut_loop.sh  number_of_runs_integer"
echo "Try the command again"
echo " "
exit 1
fi

echo " "
echo " "
echo " "
echo "The configuration is for $1 LOOPS"
echo " "

local1=$1
lpcnt=1
count=0
while test $local1 -ne 0
do
    echo "___________________________________________________________"
    echo " "
    echo "Working on LOOP $lpcnt"
    echo " "
    ./proddonuts >  prod_out.txt & sleep 1 
    cmd=""
    local2="${2:-5}"
    while test $local2 -gt 0
    do
        cmd="$cmd./consdonuts $local2 > c$local2 & "
        local2=`expr $local2 - 1`
    done
    eval $cmd
    sleep 10
    pid=`ps | grep -v 'grep' | grep 'proddonuts' | awk '{print $1}'`
    if ps | grep -v 'grep' | grep -q 'consdonuts'
    then
        echo " "
        echo "DEADLOCK DETECTED ON LOOP $lpcnt"
        echo " "
        count=`expr $count + 1`
    else
        echo "LOOP $lpcnt COMPLETED SUCCESSFULLY"
        echo " "
    fi
    echo ">>>>>>>> NOW KILLING THE PRODUCER PROCESS PID: $pid"
    kill $pid
    local1=`expr $local1 - 1`
    lpcnt=`expr $lpcnt + 1`
    sleep 1
done
echo "___________________________________________________________"
echo " "
echo $1 loops and $count deadlocks
echo " "
echo " "
