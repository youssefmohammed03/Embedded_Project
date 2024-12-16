import serial
import tkinter as tk

class LEDControlApp:
    def __init__(self, master):
        self.master = master
        master.title("LED Control")
        
        # Set the window size to 300x150 pixels
        master.geometry("300x150")
        
        # Disable resizing the window
        master.resizable(False, False)

        # Set up the serial connection
        self.ser = serial.Serial('COM4', 9600, timeout=1)
        self.led_state = False  # False means LED is off, True means on

        # Button to toggle LED
        self.toggle_button = tk.Button(master, text="Turn LED On", command=self.toggle_led)
        self.toggle_button.pack(pady=10)  # Add some vertical padding

        # Label to show LED status
        self.status_label = tk.Label(master, text="LED is Off")
        self.status_label.pack(pady=5)  # Add some vertical padding

    def toggle_led(self):
        # Toggle the LED state
        self.led_state = not self.led_state

        # Send the new state to the microcontroller
        if self.led_state:
            self.ser.write(b'1')  # Turn on LED
            self.toggle_button.config(text="Turn LED Off")
            self.status_label.config(text="LED is On")
        else:
            self.ser.write(b'0')  # Turn off LED
            self.toggle_button.config(text="Turn LED On")
            self.status_label.config(text="LED is Off")

    def close_connection(self):
        self.ser.close()

def main():
    root = tk.Tk()
    app = LEDControlApp(root)

    # Ensure the serial connection is closed cleanly
    root.protocol("WM_DELETE_WINDOW", app.close_connection)

    root.mainloop()

if __name__ == "__main__":
    main()
