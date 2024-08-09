#include <iostream>
#include <iomanip>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;
class Task 
{
    int no;
    int AT;
    int BT;
    int TN;
    int CT;
    int TAT;
    int WT;
    
    public:
    Task()
    {
       no = 0;
       AT = 0;
       BT = 0;
       CT = 0;
       TAT = 0;
       WT = 0;
       TN=0;
    }
   
   void inputTask()
   {
       cout << "\nEnter no of task:";
       cin >> TN;
       cout << "Enter the arrival time:";
       cin >> AT;
       cout << "\nEnter Brust time:";
       cin >> BT;
   }
   
    /*void displayTask()
    {
        cout << no << setw(5) << arrival_time << setw(5) << Brust_time
        << setw(5) << comp_time << setw(5) << TAT << setw(5) << waiting_time <<
        endl;
    }*/
   friend class Schedule;
};

class Schedule
{
    vector<Task> v;
    
    public:
    void input()
    {
        int n;
        cout << "\nEnter number of task:";
        cin >> n;
        Task t;
        for (int i = 0; i < n; i++)
        {
            cout << "\nEnter task " << i + 1<<" : " << endl;
            t.inputTask();
            t.TN=i+1;
            v.push_back(t);
        } 
    }
    
    void sort_According_to_arrival()
    {
        sort(v.begin(), v.end(), [](Task &T1, Task &T2)
        { 
            return T1.AT < T2.AT; 
        });
    }

    void FCFS()
    {
        sort_According_to_arrival();
        int t = 0;
        for (int i = 0; i < v.size(); i++)
        {
            if (t < v[i].AT)
            {
                t = v[i].AT;
                cout<<" -TimeGap- ";
            }
            v[i].CT = t + v[i].BT;
            v[i].TAT = v[i].CT - v[i].AT;
            v[i].WT = v[i].TAT - v[i].BT;
            t = v[i].CT;
            cout<<" -"<<v[i].TN<<"- ";
        }

        cout << "\n----------------------------------------------------------------------------------------------";
        cout << "\n"
             << "PROCESS" << "\t | \t" << "AT" << "\t  |  \t" << "BT" << "\t | \t" << "CT" << "\t | \t" << "TAT" << "\t | \t" << "WT" << "\t | \t";
        cout << "\n----------------------------------------------------------------------------------------------";
        for (int i = 0; i < v.size(); i++)
        {
            cout << "\n"
                 << "P" << v[i].TN << "\t | \t" << v[i].AT << "\t  |  \t" << v[i].BT << "\t | \t" << v[i].CT << "\t | \t" << v[i].TAT << "\t | \t" << v[i].WT << "\t | \t";
        }
        cout << "\n----------------------------------------------------------------------------------------------";        
    }    
};

int main()
{
    Schedule s;
    s.input();
    s.FCFS();
}