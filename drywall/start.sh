sudo node app.js &
sudo ffmpeg -s 320x240 -f video4linux2 -i /dev/video0 -f mpeg1video -b 800k -r 30 http://audrone.eng.auburn.edu:8082/austream/320/240
