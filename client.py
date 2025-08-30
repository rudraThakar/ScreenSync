# client.py
import time
import argparse
import cv2
from screen_capture import ScreenCapturer
from screen_share_net import NetworkWrapper

def main():
    parser = argparse.ArgumentParser(description='Screen Sharing Client')
    parser.add_argument('server_ip', help='Server IP address')
    parser.add_argument('--port', type=int, default=8080, help='Server port')
    parser.add_argument('--fps', type=int, default=15, help='Frames per second')
    args = parser.parse_args()
    
    print(f"Connecting to server at {args.server_ip}:{args.port}")
    print("Press 'q' to quit")
    
    capturer = ScreenCapturer(fps=args.fps)
    network = NetworkWrapper()
    
    # Create client connection
    if not network.create_connection(args.server_ip, args.port, 0):
        print("Failed to connect to server")
        return
    
    try:
        while True:
            # Receive and display frame
            data = network.receive_data()
            if data:
                frame = capturer.decompress_frame(data)
                if frame is not None:
                    # Display the frame
                    if not capturer.display_frame(frame, f"Screen Share - {args.server_ip}"):
                        break
            
            # Small delay to prevent high CPU usage
            time.sleep(0.001)
            
    except KeyboardInterrupt:
        print("\nDisconnecting from server...")
    finally:
        network.close_connection()
        cv2.destroyAllWindows()

if __name__ == '__main__':
    main()