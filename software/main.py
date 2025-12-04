import serial
import cv2
import tensorflow as tf
import numpy as np
import time

model_path = r"C:\Users\ASUS\Downloads\moneyvndetection.h5"
labels_path = {'100000':0, '10000':1, '1000':2, '200000':3, '20000':4, '2000':5, '500000':6, '50000':7, '5000':8}
image_size = (224, 224)
serial_port = "COM10"

#load model và nhãn
model = tf.keras.models.load_model(model_path)
labels = [None] * len(labels_path)
for key, val in labels_path.items():
    labels[val] = key

#kết nối Serial
ser = serial.Serial(serial_port, 115200, timeout=1)

#trích xuất đặc trưng ảnh
feature_extractor = tf.keras.applications.MobileNetV2(
    include_top=False,
    pooling='avg',
    input_shape=(224, 224, 3),
    weights='imagenet'
)

#thêm tầng chuyển đặc trưng từ 1280 → 512
feature_dense = tf.keras.Sequential([
    tf.keras.layers.Input(shape=(1280,)),
    tf.keras.layers.Dense(512, activation='relu')
])

#hàm chụp ảnh bằng webcam
def capture_image(filename="capture.jpg"):
    cap = cv2.VideoCapture(0)
    time.sleep(2)
    ret, frame = cap.read()
    if ret:
        cv2.imwrite(filename, frame)
        print("Ảnh đã được chụp.")
    cap.release()

# Tiền xử lý ảnh
def preprocess_image(img_path):
    img = cv2.imread(img_path)
    img = cv2.resize(img, image_size)
    img = img / 255.0
    img = np.expand_dims(img, axis=0)
    return img

# Dự đoán mệnh giá tiền
def predict(img):
    image = preprocess_image(img)
    raw_features = feature_extractor.predict(image,verbose=0)
    features = feature_dense.predict(raw_features,verbose=0)  # Giảm còn (1, 512)
    predictions = model.predict([image, features],verbose=0)[0]
    class_idx = np.argmax(predictions)
    label = labels[class_idx]
    confidence = predictions[class_idx]
    return label, confidence

#lắng nghe tín hiệu từ Arduino
while True:
    if ser.in_waiting:
        command = ser.readline().decode().strip()
        if command == "capture":
            print("Nhận lệnh từ Arduino: capture")
            capture_image()
            label, confidence = predict("capture.jpg")
            print(f"Kết quả: {label} ({confidence:.2f})")
            label_int = int(label)
            ser.write((f"{label_int}\n").encode())