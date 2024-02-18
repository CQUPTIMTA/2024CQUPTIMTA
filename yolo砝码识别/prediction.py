from ultralytics import YOLO
import cv2
model = YOLO('best.pt')
img=cv2.imread('./test.jpg')
img=cv2.resize(img,(1280,720))
model.predict(source=img,show=True)
input()