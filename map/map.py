#!/usr/bin/python

import socket, traceback

host = ''                               # Bind to all interfaces
port = 21567

s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
s.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)
s.bind((host, port))

while 1:
    try:
        message, address = s.recvfrom(8192)
        sp = message.split(",")
        lat = sp[1]
        lon = sp[2]
        
        f = open("latlong", "w")
        f.write(lat + ", " + lon)
        f.close()
        
    except (KeyboardInterrupt, SystemExit):
        raise
    except:
        traceback.print_exc()

