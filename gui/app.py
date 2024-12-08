import tkinter as tk
from tkinter import messagebox

# Create the main window
root = tk.Tk()
root.title("My Python GUI App")
root.geometry("400x400")  # Width x Height

# Add a label
label = tk.Label(root, text="Enter your name:", font=("Arial", 12))
label.pack(pady=10)

# Add a text entry box
entry = tk.Entry(root, font=("Arial", 12))
entry.pack(pady=5)

# Add a function to handle button click
def greet_user():
    name = entry.get()
    if name:
        messagebox.showinfo("Greeting", f"Hello, {name}!")
    else:
        messagebox.showwarning("Input Error", "Please enter your name.")

# Add a button
button = tk.Button(root, text="Greet Me", command=greet_user, font=("Arial", 12))
button.pack(pady=20)

# Run the application
root.mainloop()
