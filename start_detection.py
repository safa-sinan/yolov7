import subprocess

def start_detection():
    command = "python3 detect.py --weights best.pt --source 0 --device 0 --conf 0.25 --img-size 640"
    subprocess.run(command, shell=True)

if __name__ == "__main__":
    start_detection()
