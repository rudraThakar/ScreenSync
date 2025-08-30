# server.py
import time
import argparse
from screen_capture import ScreenCapturer
from screen_share_net import NetworkWrapper

def main():
    parser = argparse.ArgumentParser(description='Screen Sharing Server')
    parser.add_argument('--port', type=int, default=8080, help='Port to listen on')
    parser.add_argument('--fps', type=int, default=15, help='Frames per second')
    parser.add_argument('--quality', type=int, default=70, help='Image quality (1-100)')
    args = parser.parse_args()
    
    print(f"Starting screen sharing server on port {args.port}")
    print("Press Ctrl+C to stop sharing")
    
    capturer = ScreenCapturer(fps=args.fps, quality=args.quality)
    network = NetworkWrapper()
    
    # Create server connection
    if not network.create_connection(None, args.port, 1):
        print("Failed to create server connection")
        return
    
    try:
        while True:
            # Capture and send frame
            frame = capturer.capture_frame()
            compressed_data = capturer.compress_frame(frame)
            
            if compressed_data:
                sent = network.send_data(compressed_data)
                if sent < 0:
                    print("Failed to send data")
                    break
            
            # Wait for next frame
            time.sleep(1 / args.fps)
            
    except KeyboardInterrupt:
        print("\nStopping server...")
    finally:
        network.close_connection()

if __name__ == '__main__':
    main()