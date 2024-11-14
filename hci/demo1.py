import tkinter as tk;
from tkinter import messagebox ,ttk

def login():
    print("hi")
    user=username_entry.get();
    passw=password_entry.get();
    
    if not user or not passw:
        messagebox.showwarning("Input error","Please eneter field")
        return;
    if user=="1" and passw=="1":
        messagebox.showinfo("login","login")
        tab_control.tab(2,state="normal")
        tab_control.forget(login_frame);
        tab_control.forget(signin_frame)

def signup():
    user=username_entry1.get();
    passw=password_entry1.get();

    if(user and passw):
        messagebox.showinfo("signup","signup")
    else:
        messagebox.showerror("eror","eri")

resevation=[]
def book():
    name=name_entry.get();
    age=age_entry.get();
    dest=dest_entry.get();
    src=src_entru.get();

    
    if not name or not age or not src or not dest:
        messagebox.showerror("ser","no")
    else:
        try:
            age=int(age)
            if age<=0:
                raise ValueError
        except ValueError:
            messagebox.showerror("age","age")

    res={"name":name,"age":age,"src":src,"dest":dest};
    resevation.append(res);



def show():

    reservation_list="\n".join(
        [f"name:{r['name']} ,age:{r['age']},src:{r['src']},dest:{r['dest']}" for r in resevation]
    )

    win=tk.Toplevel(root);
    tk.Label(win,text=reservation_list,font="20").pack(pady=2)

root=tk.Tk()
root.title("Railway reservation system")
root.geometry("500x500")

tab_control=ttk.Notebook(root)
tab_control.pack(pady=2)

login_frame=ttk.Frame(tab_control)
signin_frame=ttk.Frame(tab_control)
reservation_frame=ttk.Frame(tab_control)

tab_control.add(login_frame,text="login")
tab_control.add(signin_frame,text="signin")
tab_control.add(reservation_frame,text="reservation",state="disabled")


tk.Label(login_frame,text="username" ,font="30").pack(pady=10)
username_entry=tk.Entry(login_frame,font="20")
username_entry.pack(pady=20)

tk.Label(login_frame,text="password" ,font="30").pack(pady=10)
password_entry=tk.Entry(login_frame,font="20")
password_entry.pack(pady=20)

button=tk.Button(login_frame,text="login",command=login).pack(pady=2)

tk.Label(signin_frame,text="username" ,font="30").pack(pady=10)
username_entry1=tk.Entry(signin_frame,font="20")
username_entry1.pack(pady=20)

tk.Label(signin_frame,text="password" ,font="30").pack(pady=10)
password_entry1=tk.Entry(signin_frame,font="20")
password_entry1.pack(pady=20)

signup_button=tk.Button(signin_frame,command=signup,text="signup").pack(pady=2)

tk.Label(reservation_frame,text="name",font="20").pack(pady=2)
name_entry=tk.Entry(reservation_frame,font="20")
name_entry.pack(pady=2);

tk.Label(reservation_frame,text="age",font=20).pack(pady=2);
age_entry=tk.Entry(reservation_frame,font="20")
age_entry.pack(pady=2)

src_entru=tk.StringVar();
src_entru.set("select src");
src_option=tk.OptionMenu(reservation_frame,src_entru,"pune","mumbai").pack(pady=2)

dest_entry=tk.StringVar();
dest_entry.set("select dest");
dest_option=tk.OptionMenu(reservation_frame,dest_entry,"pune","mumbai").pack(pady=2)

book_button=tk.Button(reservation_frame,command=book,font="20").pack(pady=2)
show=tk.Button(reservation_frame,command=show,font="20").pack(pady=2)
root.mainloop()