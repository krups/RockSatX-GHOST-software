import serial

# Configure the serial port
ser = serial.Serial(
    port='/dev/ttyACM0',  # Replace with your serial port
    baudrate=9600,      # Replace with your baud rate
    timeout=1            # Timeout in seconds
)

try:
    while True:
        # Read a line of data from the serial port
        line = ser.readline().decode('utf-8').strip()

        # Process the received data
        if line:  # Check if the line is not empty
            print(line)

except KeyboardInterrupt:
    print("Exiting...")
finally:
    ser.close()  # Close the serial port