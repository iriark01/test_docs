#!/usr/bin/env python
import socket
import signal
import select


def main(port):
    port = int(port)

    udp = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    udp.bind(('', port))
    udp.setblocking(0)
    udp.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

    tcp = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    tcp.bind(('', port))
    tcp.setblocking(0)
    tcp.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    tcp.listen(5)

    print "running on port %d" % port     
    sockets = [tcp, udp]
    clients = []

    while True:
        select.select(sockets, [], [])

        try:
            data, addr = udp.recvfrom(1 << 12)
            print 'udp %s:%d %d' % (addr[0], addr[1], len(data))
            udp.sendto(data, addr)
        except socket.error:
            pass

        try:
            client, addr = tcp.accept()
            print 'tcp %s:%d connect' % (addr[0], addr[1])
            client.setblocking(0)
            client.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
            sockets.append(client)
            clients.append((client, addr))
        except socket.error:
            pass

        for client, addr in clients:
            try:
                data = client.recv(1 << 12)
                if data:
                    print 'tcp %s:%d %d' % (addr[0], addr[1], len(data))
                    client.send(data)
                else:
                    print 'tcp %s:%d disconnect' % (addr[0], addr[1])
                    sockets.remove(client)
                    clients.remove((client, addr))
                    client.close()
            except socket.error:
                pass

    
if __name__ == "__main__":
    import sys
    main(*sys.argv[1:])
