#include<iostream>
#include<vector>
#include<unordered_set>
#include<queue>
#include<algorithm>
#include<sstream>
#include<deque>
using namespace std;


void fifo(vector<int>v,int cap)
{
    unordered_set<int>s;
    queue<int>q;

    int miss=0;
    int n=v.size();
    for(int i=0;i<v.size();i++)
    {
        if(s.size()<cap)
        {
            if(find(s.begin(),s.end(),v[i])==s.end())
            { 
                miss++;
                s.insert(v[i]);
                q.push(v[i]);
                cout<<"\npage miss "<<v[i]<<" frame:";
            }
            else
            {
                cout<<"psge hit "<<v[i]<<" frame:";
            }
        }
        else
        {
            if(find(s.begin(),s.end(),v[i])==s.end())
            {
                cout<<"\npage miss "<<v[i]<<" frame:";
                int el=q.front();
                q.pop();
                s.erase(el);
                s.insert(v[i]);
                q.push(v[i]);
                miss++;
            }
            else
            {
                cout<<"\npage hit "<<v[i]<<" frame:";
            }

        }

        for(auto el:s)
        {
            cout<<el<<" ";
        }
        cout<<endl;
    }

    cout<<"\nmiss:"<<miss<<endl;
    cout<<"\nhit:"<<n-miss<<endl;

}
void LRU(vector<int>v,int cap)
{
    deque<int>d;
    int miss=0;
    int n=v.size();
    for(int i=0;i<n;i++)
    {
        if(d.size()<cap)
        {
            if(find(d.begin(),d.end(),v[i])==d.end())
            {
                d.push_back(v[i]);
                miss++;
                cout<<"\npage miss "<<v[i]<<" frame:";
            }
            else
            {
                cout<<"\npage hit "<<v[i]<<" frame:";            }
        }
        else
        {
            if(find(d.begin(),d.end(),v[i])==d.end())
            {
                d.pop_front();
                d.push_back(v[i]);
                miss++;
                cout<<"\npage miss "<<v[i]<<" frame:";
            }
            else
            {
                auto pos=find(d.begin(),d.end(),v[i]);
                d.erase(pos);
                d.push_back(v[i]);
                cout<<"\npage hit "<<v[i]<<" frame:";   
            }
        }

        for(auto el:d)
        {
            cout<<el<<" ";
        }
    }

    cout<<"\nmiss:"<<miss<<endl;
    cout<<"hit:"<<n-miss<<endl;
}
int predict(vector<int>v,int ind,vector<int>f)
{
    int  res=-1;
    int far=ind;
    for(int i=0;i<f.size();i++)
    {
        int j;
        for(j=ind;j<v.size();j++)
        {
            if(f[i]==v[j])
            {
                if(j>far){
                res=i;
                far=j;}
                break;
            }
        }
        if(j==v.size())
        {
            res=i;
            return res;
        }
    }

    if(res==-1)
    {
        return  0;
    }
    return res;
}
void optimal(vector<int>v,int cap)
{
    vector<int>f;
    int n=v.size();
    int miss=0;
    for(int i=0;i<n;i++)
    {
        if(f.size()<cap)
        {
            if(find(f.begin(),f.end(),v[i])==f.end())
            {
                f.push_back(v[i]);
                miss++;
                cout<<"\npage miss "<<v[i]<<" frame:";
            }
            else
            {   
                cout<<"\npage hit "<<v[i]<<" frame:";
            }
        }
        else
        {
            if(find(f.begin(),f.end(),v[i])==f.end())
            {
                int pos=predict(v,cap,f);
                f[pos]=v[i];
                miss++;
                cout<<"\npage miss "<<v[i]<<" frame:";
            }
            else
            {
                cout<<"\npage hit "<<v[i]<<" frame:";
            }
        }

        for(auto el:f)
        {
            cout<<el<<" ";
        }
        
    }

    cout<<"\nmiss:"<<miss<<endl;
    cout<<"\nhit:"<<n-miss<<endl;
}
int main()
{
    // string pages="3 1 2 1 6 5 1 3";
   // string pages="1 3 0 3 5 6 3";
   string pages="7 0 1 2 0 3 0 4 2 3 0 3 2 3";
    // cout<<"\nenter pages:";
    // cin>>pages;

    stringstream ss(pages);
    vector<int>v;
    int el;
    while(ss>>el)
    {
        v.push_back(el);
    }

    int fs;
    cout<<"\nfarme size:";
    cin>>fs;

    //fifo(v,fs);
    //LRU(v,fs);
    optimal(v,fs);

}