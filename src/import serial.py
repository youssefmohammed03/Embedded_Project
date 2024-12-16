import serial
import time

def send_button_state(port, baudrate, lamp_state, plug_state):
    
    try:
        # Initialize serial connection
        with serial.Serial(port, baudrate, timeout=1) as ser:
            # Construct the message
            message = f"{lamp_state},{plug_state}\n"
            
            # Send the message
            ser.write(message.encode())
            print(f"Sent: {message.strip()}")
            
            # Read the response (optional, depends on MCU implementation)
            response = ser.readline().decode().strip()
            if response:
                print(f"Received: {response}")
            else:
                print("No response from MCU.")
    except serial.SerialException as e:
        print(f"Error: {e}")

if __name__ == "__main__":
   
    COM_PORT = "COM4" # first plug in right side in yusuf laptop  
    BAUD_RATE = 9600

    while True:
        # Get input from the user for lamp and plug states
        lamp_state = int(input("Enter 1 or 0 for appButtonForLamp: "))
        plug_state = int(input("Enter 1 or 0 for appButtonForPlug: "))
        
        # Send the states to the microcontroller
        send_button_state(COM_PORT, BAUD_RATE, lamp_state, plug_state)
