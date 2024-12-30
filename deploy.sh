USER=admin
IP=raspberrypi.local

scp -r * $USER@$IP:/home/$USER/sensehat
