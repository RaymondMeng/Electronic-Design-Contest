import time
import cv2

# Cam properties
fps = 20
frame_width = 640
frame_height = 480
# Create capture
cap = cv2.VideoCapture(0)

# Set camera properties
cap.set(cv2.CAP_PROP_FRAME_WIDTH, frame_width)
cap.set(cv2.CAP_PROP_FRAME_HEIGHT, frame_height)
cap.set(cv2.CAP_PROP_FPS, fps)

# Define the gstreamer sink
gst_str_rtp = "appsrc ! video/x-raw,framerate=20/1 ! videoscale ! videoconvert ! x264enc tune=zerolatency bitrate=500 speed-preset=superfast ! rtph264pay ! udpsink host=127.0.0.1 port=5000"


# Check if cap is open
if cap.isOpened() is not True:
    print( "Cannot open camera. Exiting.")
    quit()

# Create videowriter as a SHM sink
out = cv2.VideoWriter(gst_str_rtp, 0, fps, (frame_width, frame_height), True)

# Loop it
while True:
    # Get the frame
    ret, frame = cap.read()
    # Check
    if ret is True:
        # Flip frame
        frame = cv2.flip(frame, 1)
        # Write to SHM
        cv2.imshow("test",frame)
        # out.write(frame)

    else:
        print( "Camera error.")
        time.sleep(10)

cap.release()