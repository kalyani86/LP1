import tkinter as tk
from tkinter import messagebox,ttk

def submit():
    label1=tk.Label(root,text=f"Enter data:{input.get()}",font=("Arial",20)).pack(padx=20,pady=25);

root=tk.Tk();
root.geometry("500x500");
root.title("demo");


label=tk.Label(root,text="enter text",font=("Arial",20));
label.pack(padx=20,pady=20);

input=tk.Entry(root,font=('Arial',20));
input.pack(padx=20,pady=22);

button=tk.Button(root,text="submit",command=submit,bg="red",fg="white",font="20").pack(padx=20,pady=23);



root.mainloop();