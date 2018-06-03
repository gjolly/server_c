import socket
import pytest

def test_simple():
    TCP_IP = '127.0.0.1'
    TCP_PORT = 8080
    BUFFER_SIZE = 1024
    MESSAGE = str.encode('GET / HTTP/1.1; Host: www.google.test'.replace(';', '\n'))

    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect((TCP_IP, TCP_PORT))
    s.send(MESSAGE)
    data = s.recv(BUFFER_SIZE).decode()
    s.close()

    with open('tests/response.test') as f:
        read_data = f.read()
    f.close()

    assert data == read_data
