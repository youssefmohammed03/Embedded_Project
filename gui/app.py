import ctypes
from tkinter import Tk, Button, Label, messagebox

# Loading the shared library

alarm_lib = ctypes.CDLL("gui/alarm.dll") #don't remove .dll file it is the dile that combines .c , .h files and .py

# GUI Application
def main():
    # Create the main window
    root = Tk()
    root.title("Home Automation Control Panel")
    root.geometry("400x400")
    root.configure(bg="#f0f0f0")

    # Lamp ON/OFF Button
    lamp_status = [False]  # Use a mutable type to allow toggling
    def toggle_lamp():
        if lamp_status[0]:
            alarm_lib.DeactivateAlarm()  # Replace with the appropriate C function
            btn_lamp.config(text="Lamp OFF", bg="red")
        else:
            alarm_lib.ActivateAlarm()  # Replace with the appropriate C function
            btn_lamp.config(text="Lamp ON", bg="green")
        lamp_status[0] = not lamp_status[0]

    btn_lamp = Button(root, text="Lamp OFF", font=("Arial", 14), bg="red", fg="white", command=toggle_lamp)
    btn_lamp.pack(pady=10)

    # Plug ON/OFF Button
    plug_status = [False]  # Use a mutable type to allow toggling
    def toggle_plug():
        if plug_status[0]:
            alarm_lib.DeactivateAlarm()  # Replace with the appropriate C function
            btn_plug.config(text="Plug OFF", bg="red")
        else:
            alarm_lib.ActivateAlarm()  # Replace with the appropriate C function
            btn_plug.config(text="Plug ON", bg="green")
        plug_status[0] = not plug_status[0]

    btn_plug = Button(root, text="Plug OFF", font=("Arial", 14), bg="red", fg="white", command=toggle_plug)
    btn_plug.pack(pady=10)

    # View Door Status Button
    door_status = Label(root, text="Door is currently closed", font=("Arial", 12), bg="#f0f0f0")
    door_status.pack(pady=10)

    def view_door_status():
        # Placeholder: Replace with a call to your C function to get the real door status
        door_status.config(text="Door is currently open")

    btn_door_status = Button(root, text="View Door Status", font=("Arial", 14), bg="blue", fg="white", command=view_door_status)
    btn_door_status.pack(pady=10)

    # Save and Show Door Open/Close Timings
    door_timings = []

    def save_door_timing():
        # Placeholder: Replace with a call to your C function for saving door status timings
        from datetime import datetime
        current_time = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
        door_timings.append(f"Door status updated at {current_time}")
        messagebox.showinfo("Info", "Door timing saved!")

    def show_door_timings():
        if not door_timings:
            messagebox.showinfo("Door Timings", "No door timings recorded.")
            return
        timings = "\n".join(door_timings)
        messagebox.showinfo("Door Timings", f"Saved Timings:\n{timings}")

    btn_save_timing = Button(root, text="Save Door Timing", font=("Arial", 14), bg="orange", fg="white", command=save_door_timing)
    btn_save_timing.pack(pady=10)

    btn_show_timings = Button(root, text="Show Door Timings", font=("Arial", 14), bg="purple", fg="white", command=show_door_timings)
    btn_show_timings.pack(pady=10)

    # Display Room Temperature
    lbl_temperature = Label(root, text="Room Temperature: 25°C", font=("Arial", 12), bg="#f0f0f0")
    lbl_temperature.pack(pady=10)

    root.mainloop()

if __name__ == "__main__":
    main()

