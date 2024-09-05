import cv2 
import requests
import time
from threading import Timer


# Load the pre-trained Haar Cascade for face detection from OpenCV
face_cascade = cv2.CascadeClassifier(cv2.data.haarcascades + 'haarcascade_frontalface_default.xml')

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

    # Convert the frame to grayscale for face detection
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

    # Detect faces in the frame
    faces = face_cascade.detectMultiScale(gray,
                                          scaleFactor=1.1,
                                          minNeighbors=5,
                                          minSize=(80, 80))

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
