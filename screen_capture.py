# screen_capture.py
import numpy as np
import cv2
from PIL import ImageGrab
import io

class ScreenCapturer:
    def __init__(self, fps=15, quality=70):
        self.fps = fps
        self.quality = quality
    
    def capture_frame(self):
        # Capture screen
        screen = ImageGrab.grab()
        
        # Convert to OpenCV format
        frame = np.array(screen)
        frame = cv2.cvtColor(frame, cv2.COLOR_RGB2BGR)
        
        return frame
    
    def compress_frame(self, frame):
        # Compress frame as JPEG
        encode_param = [int(cv2.IMWRITE_JPEG_QUALITY), self.quality]
        result, encoded_frame = cv2.imencode('.jpg', frame, encode_param)
        
        if result:
            return encoded_frame.tobytes()
        return None
    
    def decompress_frame(self, data):
        # Decompress frame from bytes
        np_array = np.frombuffer(data, np.uint8)
        frame = cv2.imdecode(np_array, cv2.IMREAD_COLOR)
        return frame
    
    def display_frame(self, frame, window_name="Screen Share"):
        # Display the frame
        cv2.imshow(window_name, frame)
        
        # Wait for a short time and check for exit key
        if cv2.waitKey(1) & 0xFF == ord('q'):
            return False
        return True