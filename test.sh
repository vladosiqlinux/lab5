make >> log.txt
sudo insmod Timer.ko

echo 3000 >> /sys/kernel/timer/time
sleep 5
dmesg | tail -n 10

sudo rmmod Timer
make clean >> log.txt
