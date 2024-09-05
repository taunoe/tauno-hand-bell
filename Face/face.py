import cv2 
import requests
import time
from threading import Timer
from ultralytics import YOLO
# https://github.com/ultralytics/ultralytics

# Load a model
model = YOLO("yolov8n.pt")  # load a pretrained model

# Start capturing video from the webcam
cap = cv2.VideoCapture(0)

# Blynk API URL
auth_token = "MdlHJUkkWEBwuTIjYy4yQd6dY_Krrp5m"
pin = "V0"
region = "fra1"
url = f"https://{region}.blynk.cloud/external/api/update?token={auth_token}&{pin}=1"


request_sent = False

def reset_request_flag():
    global request_sent
    request_sent = False

while True:
    # Capture frame-by-frame
    ret, frame = cap.read()

    # Use the model
    results = model(frame)  # predict on an image

    # https://docs.ultralytics.com/modes/predict/#key-features-of-predict-mode
    # TODO:

    # If a face is detected and the request hasn't been sent yet, send the GET request to Blynk API
    if len(faces) > 0 and not request_sent:
        requests.get(url)
        request_sent = True
        print("request_sent")
        Timer(2, reset_request_flag).start()

    # Draw rectangles around detected faces
    for (x, y, w, h) in faces:
        cv2.rectangle(frame, (x, y), (x+w, y+h), (255, 0, 0), 2)

    # Display the resulting frame
    cv2.imshow('Face Detection', frame)

    # Break the loop if the user presses the 'q' key
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# Release the capture and close windows
cap.release()
cv2.destroyAllWindows()

