import serial
import tkinter as tk
import threading
from datetime import datetime
import winsound

class HomeApp:
    def __init__(self, master):
        self.master = master
        master.title("Home")
        
        # Set the window size to 300x300 pixels
        master.geometry("350x400")
        master.resizable(False, False)

        # Set background color and fonts
        master.config(bg="#f0f0f0")

        # Set up the serial connection
        self.ser = serial.Serial('COM4', 9600, timeout=1)
        self.plug_state = False  # False means Plug is off, True means on
        self.last_door_status = None  # Initialize last door status to None

        # Button to toggle Plug
        self.toggle_plug_button = tk.Button(master, text="Turn Plug On", command=self.toggle_plug, 
                                             bg="#4CAF50", fg="white", font=("Arial", 12, "bold"), height=2)
        self.toggle_plug_button.pack(pady=10, fill="x")  # Add some vertical padding

        # Label to show Plug status
        self.plug_status_label = tk.Label(master, text="Plug is Off", bg="#f0f0f0", fg="black", font=("Arial", 14))
        self.plug_status_label.pack(pady=5)  # Add some vertical padding

        # Button to toggle Lamp
        self.toggle_lamp_button = tk.Button(master, text="Lamp On/Off", command=self.toggle_lamp, 
                                             bg="#2196F3", fg="white", font=("Arial", 12, "bold"), height=2)
        self.toggle_lamp_button.pack(pady=10, fill="x")  # Add some vertical padding

        # Label to show Door status
        self.door_status_label = tk.Label(master, text="Door status unknown", bg="#f0f0f0", fg="black", font=("Arial", 14))
        self.door_status_label.pack(pady=5)  # Add some vertical padding

        # Label to show Temperature
        self.temperature_label = tk.Label(master, text="Temperature: --", bg="#f0f0f0", fg="black", font=("Arial", 14))
        self.temperature_label.pack(pady=5)

        # Label to display the latest four logs
        self.log_label = tk.Label(master, text="Latest Logs:\n(No logs yet)", bg="#f0f0f0", fg="black", font=("Arial", 12))
        self.log_label.pack(pady=10)

        # Start a thread to read from the serial port
        self.read_thread = threading.Thread(target=self.read_serial)
        self.read_thread.daemon = True
        self.read_thread.start()

        # Open the log file in append mode
        self.log_file = open("door_logs.txt", "a")

        self.alarm_active = False

    def toggle_plug(self):
        # Toggle the Plug state
        self.plug_state = not self.plug_state
        if self.plug_state:
            self.ser.write(b'P')  # Send signal to turn Plug on
            self.toggle_plug_button.config(text="Turn Plug Off", bg="#f44336")
            self.plug_status_label.config(text="Plug is On", fg="#4CAF50")
        else:
            self.ser.write(b'p')  # Send signal to turn Plug off
            self.toggle_plug_button.config(text="Turn Plug On", bg="#4CAF50")
            self.plug_status_label.config(text="Plug is Off", fg="#f44336")

    def toggle_lamp(self):
        # Send signal to turn Lamp on
        self.ser.write(b'L')

    def read_serial(self):
        buffer = ""  # Temporary buffer to store characters
        while True:
            data = self.ser.read().decode('utf-8')  # Read one byte, decode as UTF-8
            if data:
                if data == 'T':  # Start of ADC data
                    buffer = ""  # Clear the buffer
                elif data == '\n':  # End of ADC data (assuming newline termination)
                    self.handle_adc_data(buffer)  # Process the complete ADC value
                elif data == 'D' or data == 'd':  # Door status
                    self.handle_serial_data(data.encode('utf-8'))
                else:
                    buffer += data  # Append to the buffer

    def start_beeping(self):
        """Start a continuous beep."""
        while self.alarm_active:
            winsound.Beep(1000, 500)  # Beep at 1000 Hz for 500 ms

    def handle_adc_data(self, adc_value_str):
        try:
            # Convert ADC value from string to integer
            adc_value = int(adc_value_str)

            # Calculate the temperature using the formula (adcValue * 330) / 4096
            temperature = (adc_value * 330) / 4096

            # Update the GUI with the temperature
            self.temperature_label.config(text=f"Temperature: {temperature:.2f}°C")

            # Handle the alarm logic
            if temperature > 27:
                if not self.alarm_active:  # Start the alarm if it's not already active
                    self.alarm_active = True
                    threading.Thread(target=self.start_beeping, daemon=True).start()
            else:
                if self.alarm_active:  # Stop the alarm if it's active
                    self.alarm_active = False

        except ValueError:
            # Handle invalid ADC data (if any)
            print("Invalid ADC value received:", adc_value_str)

    def handle_serial_data(self, data):
        timestamp = datetime.now().strftime("%Y-%m-%d %H:%M:%S")  # Get the current timestamp
        if data == b'D':  # Door is open
            self.door_status_label.config(text="Door is Open", fg="green")
            if self.last_door_status != "open":
                self.log_event(f"{timestamp} - Door is Open")
                self.last_door_status = "open"
        elif data == b'd':  # Door is closed
            self.door_status_label.config(text="Door is Closed", fg="red")
            if self.last_door_status != "closed":
                self.log_event(f"{timestamp} - Door is Closed")
                self.last_door_status = "closed"

    def log_event(self, event):
        # Write the log to the file
        self.log_file.write(event + "\n")
        self.log_file.flush()

        # Update the log display in the GUI
        self.update_logs()

    def update_logs(self):
        try:
            # Read the last four lines from the log file
            with open("door_logs.txt", "r") as file:
                lines = file.readlines()
                last_logs = lines[-4:] if len(lines) >= 4 else lines

            # Update the log label in the GUI
            self.log_label.config(text="Latest Logs:\n" + "".join(last_logs))
        except FileNotFoundError:
            self.log_label.config(text="Latest Logs:\n(No logs yet)")

    def close_connection(self):
        self.ser.close()

def main():
    root = tk.Tk()
    app = HomeApp(root)

    # Ensure the serial connection is closed cleanly
    root.protocol("WM_DELETE_WINDOW", app.close_connection)

    root.mainloop()

if __name__ == "__main__":
    main()
