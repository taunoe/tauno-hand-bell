import cv2 
import requests
import time
from threading import Timer
from ultralytics import YOLO
from ultralytics.utils.plotting import Annotator, colors
# https://github.com/ultralytics/ultralytics
# https://colab.research.google.com/github/ultralytics/ultralytics/blob/main/examples/object_tracking.ipynb#scrollTo=Cx-u59HQdu2o


# Load a model
model = YOLO("yolov8n.pt")  # load a pretrained model
model.conf = 0.8

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

    if not ret:
        print("Video frame is empty")
        break

    # Create an annotator object to draw on the frame
    annotator = Annotator(frame, line_width=2)

    # Use the model
    results = model.track(frame, persist=True)

    # Extract class labels
    for box in results[0].boxes:
        class_id = int(box.cls)  # Get class ID
        class_label = results[0].names[class_id]  # Get class label from class ID
        print(f'Detected class: {class_label}')  # Print class label

        if class_label == 'person' and not request_sent:
            requests.get(url)
            request_sent = True
            print("Blynk request_sent")
            Timer(2, reset_request_flag).start()

        #elif class_label == 'paper':
        #    arduino.write(b'2')

    # Check if tracking IDs and masks are present in the results
    #if results[0].boxes.id is not None and results[0].masks is not None:
        # Extract masks and tracking IDs
        #masks = results[0].masks.xy
        #track_ids = results[0].boxes.id.int().cpu().tolist()

        # Annotate each mask with its corresponding tracking ID and color
        #for mask, track_id in zip(masks, track_ids):
            #annotator.seg_bbox(mask=mask, mask_color=colors(track_id, True), track_label=str(track_id))
            #print("mask" + mask )

    # Display the annotated frame
    #cv2.imshow("instance-segmentation-object-tracking", frame)

    # Exit the loop if 'q' is pressed
    if cv2.waitKey(1) & 0xFF == ord("q"):
        break

# Release the video writer and capture objects, and close all OpenCV windows
cap.release()
cv2.destroyAllWindows()