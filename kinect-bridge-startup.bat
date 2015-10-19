cd bin\Release
:: Wait a few seconds for us to get an IP address
sleep 10
kinect_server.exe --listen-ip 192.168.1.102
