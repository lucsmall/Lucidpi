#!/bin/bash
cd /home/pi/projects/Lucidpi/simple_client
sleep 10
./text2screen.rb > /tmp/ip-31415.dat
./lcd_client.rb localhost 1234 /tmp/ip-31415.dat

sleep 20
./text2screen.rb > /tmp/ip-31415.dat
./lcd_client.rb localhost 1234 /tmp/ip-31415.dat

sleep 40
./text2screen.rb > /tmp/ip-31415.dat
./lcd_client.rb localhost 1234 /tmp/ip-31415.dat
