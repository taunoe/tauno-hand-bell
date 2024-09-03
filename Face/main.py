import cv2
import requests
import time

# Load the pre-trained Haar Cascade for face detection from OpenCV
face_cascade = cv2.CascadeClassifier(cv2.data.haarcascades + 'haarcascade_frontalface_default.xml')

# Start capturing video from the webcam
cap = cv2.VideoCapture(1)

# Blynk API URL
auth_token = "MdlHJUkkWEBwuTIjYy4yQd6dY_Krrp5m"  # Replace with your Blynk Auth Token
pin = "V1"  # Replace with the virtual pin you want to trigger
url = f"http://blynk-cloud.com/{auth_token}/update/{pin}?value=1"

request_sent = False

while True:
    # Capture frame-by-frame
    ret, frame = cap.read()

    # Convert the frame to grayscale for face detection
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

    # Detect faces in the frame
    faces = face_cascade.detectMultiScale(gray, scaleFactor=1.1, minNeighbors=5, minSize=(30, 30))

    # If a face is detected and the request hasn't been sent yet, send the GET request to Blynk API
    if len(faces) > 0 and not request_sent:
        requests.get(url)
        request_sent = True
        time.sleep(5)  # Prevent sending multiple requests too quickly

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
