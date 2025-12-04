# AI-Powered Smart Cashier Machine

*(This README is fully formatted in Markdown and ready for GitHub use.)*

An AI-assisted cashier system using computer vision to classify
banknotes and an Arduino-based mechanism to validate, accept, or return
money. The system combines a lightweight AI model (deployed on PC or
edge device) with an Arduino Uno controlling actuators and sensors.

------------------------------------------------------------------------

## 🚀 Overview

This project implements an intelligent, automated cashier machine
capable of: - Recognizing **Vietnamese banknotes** using an AI
classification model. - Communicating between **AI model ↔ Arduino Uno**
via Serial. - Actuating a physical mechanism including **3× MG90S
servos**, **HC-SR04 ultrasonic sensor**, **IR sensor**, and **LCD I2C
16×2**. - Automatically accepting or returning money based on the
minimum required value (default: **20,000 VND**).

The system is designed for embedded automation, small vending systems,
and educational robotics/AI projects.

------------------------------------------------------------------------
## 🎥 Demo

[![Watch the video](https://img.youtube.com/vi/watch?v=eFbIRL3UvZc/maxresdefault.jpg)](https://www.youtube.com/shorts/pvQ0PyGk9uU)

## 🧠 System Logic

1.  **Default state:** Machine expects a minimum of **20,000 VND**.
2.  **User arrives:** Top lid automatically opens.
3.  **User inserts money:**
    -   The lid closes.
    -   The AI model classifies the banknote value.
4.  **Amount ≥ 20,000 VND:**
    -   The machine **accepts** the money.
    -   LCD displays: `Đủ tiền` or `Thừa X VND`.
5.  **Amount \< 20,000 VND:**
    -   The machine **returns** the money.
    -   LCD displays: `Không đủ tiền`.

------------------------------------------------------------------------

## 🛠 Hardware Components

  ------------------------------------------------------------------------
  Component                Quantity                 Purpose
  ------------------------ ------------------------ ----------------------
  **Arduino Uno**          1                        Controls servos,
                                                    sensors, LCD, and
                                                    Serial communication

  **MG90S Servo Motors**   3                        Lid motion, money
                                                    intake, money return
                                                    mechanism

  **HC-SR04 Ultrasonic     1                        Detect user presence
  Sensor**                                          

  **IR Sensor**            1                        Detect banknote
                                                    insertion

  **LCD I2C 16×2**         1                        User feedback display

  **External Camera        1                        Captures banknote
  (USB/Webcam)**                                    image for AI model

  **PC / Edge device**     1                        Runs AI model (e.g.,
                                                    CNN or
                                                    YOLO-Classifier)
  ------------------------------------------------------------------------

------------------------------------------------------------------------

## 📡 Software Architecture

    ┌──────────────┐     Serial     ┌──────────────┐
    │   AI Model    │ ◄────────────► │  Arduino Uno │
    │ (Python App)  │                │   Firmware   │
    └──────────────┘                └──────────────┘
            │                               │
            │ Camera Input                  │ Servos/LCD/Sensors
            ▼                               ▼
     Banknote Image                  Mechanical Control

### PC-Side (AI Model)

-   Captures image from webcam.
-   Runs AI classification to identify the banknote value.
-   Sends data to Arduino via Serial, e.g.: `VALUE:20000`.

### Arduino-Side (Firmware)

-   Receives classification results.
-   Controls servos to accept/return money.
-   Displays status on LCD.
-   Uses sensors to detect user and money.

------------------------------------------------------------------------

## 💻 Installation

### 1. Clone the repository

``` bash
git clone https://github.com/yourusername/ai-cashier-machine.git
cd ai-cashier-machine
```

### 2. Install Python requirements

``` bash
pip install -r requirements.txt
```

### 3. Flash Arduino firmware

Open the `.ino` file in Arduino IDE and upload to Arduino Uno.

------------------------------------------------------------------------

## ▶️ Usage

### Step 1: Start AI Model

``` bash
python run_model.py
```

### Step 2: Arduino Standby

-   Arduino waits for sensors.
-   When banknote inserted → captures → sends to AI.

### Step 3: System Decision

  Condition          Action
  ------------------ ------------------------------------------
  `value >= 20000`   Accept banknote, display "Đủ tiền"
  `value > 20000`    Accept & show "Thừa X VND"
  `value < 20000`    Return banknote, display "Không đủ tiền"

------------------------------------------------------------------------

## 📁 Folder Structure

    ai-cashier-machine/
    │
    ├── ai_model/           # Trained model, preprocessing, inference
    ├── arduino/            # Arduino Uno source code
    ├── utils/              # Utility scripts
    ├── data/               # Training data (banknote images)
    ├── README.md           # Project documentation
    └── run_model.py        # Main Python runner

------------------------------------------------------------------------

## 🔧 Customization

You can change the minimum required amount in both: - Python (model →
Arduino) - Arduino (decision logic)

Example:

``` cpp
int minimum_amount = 20000;
```

------------------------------------------------------------------------

## 📜 License

MIT License. Free for personal and educational use.

------------------------------------------------------------------------

## 🙌 Acknowledgements

Special thanks to open-source AI/embedded communities, and contributors
who provided datasets and feedback for improving model accuracy.
