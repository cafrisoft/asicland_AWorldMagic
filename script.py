import sys

print("Python script started")

while True:
    line = sys.stdin.readline().strip()  # Read line from stdin
    if line == "exit":
        break
    print(f"Python received 1111: {line}")
