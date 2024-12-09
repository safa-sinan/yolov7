[Unit]
Description=YOLOv7 Hand Detection Service
After=network.target

[Service]
ExecStart=/usr/bin/python3 /home/pi/yolov7/start_detection.py
WorkingDirectory=/home/pi/yolov7
StandardOutput=inherit
StandardError=inherit
Restart=always
User=pi

[Install]
WantedBy=multi-user.target
