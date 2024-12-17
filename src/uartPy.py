import serial
import tkinter as tk
import threading

class HomeApp:
    def __init__(self, master):
        self.master = master
        master.title("Home")
        
        # Set the window size to 300x150 pixels
        master.geometry("300x150")
        
        # Disable resizing the window
        master.resizable(False, False)

        # Set up the serial connection
        self.ser = serial.Serial('COM4', 9600, timeout=1)
        self.plug_state = False  # False means Plug is off, True means on

        # Button to toggle Plug
        self.toggle_plug_button = tk.Button(master, text="Turn Plug On", command=self.toggle_plug)
        self.toggle_plug_button.pack(pady=10)  # Add some vertical padding

        # Label to show Plug status
        self.plug_status_label = tk.Label(master, text="Plug is Off")
        self.plug_status_label.pack(pady=5)  # Add some vertical padding

        # Button to toggle Lamp
        self.toggle_lamp_button = tk.Button(master, text="Lamp On/Off", command=self.toggle_lamp)
        self.toggle_lamp_button.pack(pady=10)  # Add some vertical padding

        # Label to show Door status
        self.door_status_label = tk.Label(master, text="Door status unknown")
        self.door_status_label.pack(pady=5)  # Add some vertical padding

        # Start a thread to read from the serial port
        self.read_thread = threading.Thread(target=self.read_serial)
        self.read_thread.daemon = True
        self.read_thread.start()

    def toggle_plug(self):
        # Toggle the Plug state
        self.plug_state = not self.plug_state
        if self.plug_state:
            self.ser.write(b'P')  # Send signal to turn Plug on
            self.toggle_plug_button.config(text="Turn Plug Off")
            self.plug_status_label.config(text="Plug is On")
        else:
            self.ser.write(b'p')  # Send signal to turn Plug off
            self.toggle_plug_button.config(text="Turn Plug On")
            self.plug_status_label.config(text="Plug is Off")

    def toggle_lamp(self):
        # Send signal to turn Lamp on
        self.ser.write(b'L')

    def read_serial(self):
        while True:
            data = self.ser.read()
            if data:
                self.handle_serial_data(data)

    def handle_serial_data(self, data):
        if data == b'D':
            self.door_status_label.config(text="Door is Open")
        elif data == b'd':
            self.door_status_label.config(text="Door is Closed")

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
