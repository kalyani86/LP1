import tkinter as tk
from tkinter import messagebox

class RailwayReservation:
    def __init__(self, root):  # Corrected __init__ method
        self.root = root
        self.root.title("Railway Reservation System")

        # Reservation details storage
        self.reservations = []

        # Heading
        tk.Label(root, text="Railway Reservation System", font=("Arial", 16)).pack(pady=10)

        # Name
        tk.Label(root, text="Name:").pack(anchor="w", padx=20)
        self.name_entry = tk.Entry(root, width=30)
        self.name_entry.pack(padx=20, pady=5)

        # Age
        tk.Label(root, text="Age:").pack(anchor="w", padx=20)
        self.age_entry = tk.Entry(root, width=30)
        self.age_entry.pack(padx=20, pady=5)

        # Destination
        tk.Label(root, text="Destination:").pack(anchor="w", padx=20)
        self.destination_var = tk.StringVar()
        self.destination_var.set("Select Destination")

        self.destination_menu = tk.OptionMenu(root, self.destination_var, "New York", "Chicago", "Los Angeles", "San Francisco")
        self.destination_menu.config(width=25)
        self.destination_menu.pack(padx=20, pady=5)

        # Train Selection
        tk.Label(root, text="Select Train:").pack(anchor="w", padx=20)
        self.train_var = tk.StringVar()
        self.train_menu = tk.OptionMenu(root, self.train_var, "Express 101", "Intercity 202", "Regional 303", "Fastline 404")
        self.train_menu.config(width=25)
        self.train_var.set("Select Train")
        self.train_menu.pack(padx=20, pady=5)

        # Book Button
        tk.Button(root, text="Book Seat", command=self.book_seat).pack(pady=15)

        # Display Reservations Button
        tk.Button(root, text="View All Reservations", command=self.view_reservations).pack(pady=5)

    def book_seat(self):
        # Get user input
        name = self.name_entry.get()
        age = self.age_entry.get()
        destination = self.destination_var.get()
        train = self.train_var.get()

        # Input validation
        if not name or not age or destination == "Select Destination" or train == "Select Train":
            messagebox.showwarning("Input Error", "Please fill in all fields")
            return

        try:
            age = int(age)
            if age <= 0:
                raise ValueError("Invalid age")
        except ValueError:
            messagebox.showwarning("Input Error", "Please enter a valid age")
            return

        # Add reservation to the list
        reservation = {"Name": name, "Age": age, "Destination": destination, "Train": train}
        self.reservations.append(reservation)

        # Show confirmation
        messagebox.showinfo("Booking Confirmed", f"Seat booked for {name} on train {train} to {destination}.")

        # Clear fields
        self.name_entry.delete(0, tk.END)
        self.age_entry.delete(0, tk.END)
        self.destination_var.set("Select Destination")
        self.train_var.set("Select Train")

    def view_reservations(self):
        # Display all reservations
        if not self.reservations:
            messagebox.showinfo("No Reservations", "No reservations have been made yet.")
            return
        
        reservations_list = "\n".join(
            [f"{r['Name']}, Age: {r['Age']}, Train: {r['Train']}, Destination: {r['Destination']}" for r in self.reservations]
        )
        
        # Show reservations in a new window
        reservation_window = tk.Toplevel(self.root)
        
        reservation_window.title("All Reservations")
        tk.Label(reservation_window, text="All Reservations", font=("Arial", 14)).pack(pady=10)
        tk.Label(reservation_window, text=reservations_list, justify="left").pack(padx=20, pady=5)

# Create main window
root = tk.Tk()
app = RailwayReservation(root)
root.mainloop()
