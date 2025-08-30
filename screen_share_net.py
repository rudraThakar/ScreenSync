# screen_share_net.py
import ctypes
import os
from ctypes import c_int, c_char_p, c_void_p, c_size_t, POINTER, Structure

# Load the shared library
lib_path = os.path.join(os.path.dirname(__file__), 'libnetshare.so')
lib = ctypes.CDLL(lib_path)

# Define argument and return types
lib.create_connection.argtypes = [c_char_p, c_int, c_int]
lib.create_connection.restype = c_void_p

lib.send_data.argtypes = [c_void_p, c_char_p, c_size_t]
lib.send_data.restype = c_int

lib.receive_data.argtypes = [c_void_p, c_char_p, c_size_t]
lib.receive_data.restype = c_int

lib.close_connection.argtypes = [c_void_p]
lib.close_connection.restype = None

class NetworkWrapper:
    def __init__(self):
        self.conn = None
    
    def create_connection(self, ip, port, is_server):
        ip_bytes = ip.encode('utf-8') if ip else None
        self.conn = lib.create_connection(ip_bytes, port, is_server)
        return self.conn is not None
    
    def send_data(self, data):
        if not self.conn:
            return -1
        return lib.send_data(self.conn, data, len(data))
    
    def receive_data(self, buffer_size=65507):
        if not self.conn:
            return None
        buffer = ctypes.create_string_buffer(buffer_size)
        received = lib.receive_data(self.conn, buffer, buffer_size)
        if received > 0:
            return buffer.raw[:received]
        return None
    
    def close_connection(self):
        if self.conn:
            lib.close_connection(self.conn)
            self.conn = None