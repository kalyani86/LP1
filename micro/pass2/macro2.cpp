#include<iostream>
#include<map>
#include<sstream>
#include<fstream>
#include<vector>

using namespace std;


class MNT
{
    public:
    int index;
    string macroname;
    int kpcount;
    int ppcount;
    int mdtptr;
    int kdptr;

    MNT()
    {
        index=0;
        macroname="";
        kpcount=0;
        ppcount=0;
        mdtptr=1;
        kdptr=1;
    }

};

class MDT
{
    public:
    int index;
    string opcode;
    string operand;
};

class KDPTAB
{
    public:
    string key;
    string dval;
    KDPTAB()
    {
        key="";
        dval="";
    }
};

class PNTAB
{
    public:
    string paraname;
    PNTAB()
    {
        paraname="";
    }
};

class generate
{
    public:
    void generateCode()
    {
        map<int,MNT>mnt;
        map<int,KDPTAB>kdptab;
        map<string,vector<PNTAB>>pntab;
        map<string,int>macro;


        //load

        ifstream min("mnt.txt");
        ifstream mdtin("mdt.txt");
        ifstream kdpin("kpdtab.txt");
        ifstream pnin("pntab.txt");

        string line;
        getline(min,line);
        while(getline(min,line))
        {
            int index,kpcount,ppcount,mdtptr,kpdtr;
            string macroname;
            stringstream ss(line);
            ss>>index>>macroname>>ppcount>>kpcount>>mdtptr>>kpdtr;
            MNT mne;
            mne.index=index;
            mne.macroname=macroname;
            mne.ppcount=ppcount;
            mne.kpcount=kpcount;
            mne.kdptr=kpdtr;
            mne.mdtptr=mdtptr;
            mnt[index]=mne;
            macro[macroname]=index;
        }

        getline(kdpin,line);
        while(getline(kdpin,line))
        {
            int index;
            string key,val;
            stringstream ss(line);
            ss>>index>>key>>val;
            KDPTAB entry;
            entry.key=key;
            entry.dval=val;
            kdptab[index]=entry;
        }

        getline(pnin,line);
        int mcount=mnt.size();
        int i=0;
        while(i<mcount)
        {
            getline(pnin,line);
            if(!line.empty())
            {
                string macroname=line;
                int ind=macro[line];
                int ppcount=mnt[ind].ppcount+mnt[ind].kpcount;
                int j=0;
                vector<PNTAB>v;
                while(j<ppcount)
                {
                    getline(pnin,line);
                    if(!line.empty())
                    {
                        PNTAB p;
                        p.paraname=line;
                        v.push_back(p);
                        j++;
                    }
                }
                pntab[macroname]=v;
                i++;
            }
            
        }

        map<int,pair<string,string>>aptab;

        string macrocall="INCR_D INCR_VAL=B MEM_VAL=A REG=BREG";
        stringstream breakmacro(macrocall);
        string macroname;
        breakmacro>>macroname;

        vector<PNTAB>v=pntab[macroname];
        int ind=1;
        for(auto el:v)
        {
            aptab[ind].first=el.paraname;
            //cout<<el.paraname<<endl;
            ind++;
        }

        if(macrocall.find('=')!=string::npos)
        {
            int ind=macro[macroname];
            int kpcount=mnt[ind].kpcount;
            int kpptr=mnt[ind].kdptr;

            int i=0;
            while(i<kpcount)
            {
                string key=kdptab[kpptr].key;
                for(auto& el:aptab)
                {
                    if(el.second.first==key)
                    {
                        el.second.second=kdptab[kpptr].dval;
                    }
                }
                kpptr++;
                i++;
            }
            
            string parameter;
            ind=1;
            while(breakmacro>>parameter)
            {
                cout<<parameter<<endl;
                if(parameter.find('=')!=string::npos)
                {
                    int pos=parameter.find('=');
                    string key=parameter.substr(0,pos);
                    string val=parameter.substr(pos+1);
                    
                    for(auto& el:aptab)
                    {
                        cout<<"ap:"<<el.second.first<<" "<<key<<endl;
                        if(el.second.first==key)
                        {
                            el.second.second=val;
                        }
                    }
                }
                else
                {
                    aptab[ind].second=parameter;
                    ind++;
                }
            }
        }
        else
        {
            int ind=1;
            string parameter;
            while(breakmacro>>parameter)
            {
                aptab[ind].second=parameter;
                ind++;
            }
        }

        ofstream fo("aptab_1.txt");

        for(auto el:aptab)
        {
            fo<<el.first<<" "<<el.second.first<<" "<<el.second.second<<endl;
        }
        ind=macro[macroname];
        int mdtptr=mnt[ind].mdtptr;
        ofstream fout("expan.txt");
        while(getline(mdtin,line))
        {
            stringstream ss(line);
            int index;
            string opcode,op1,op2;
            ss>>index>>opcode>>op1>>op2;
            if(index<mdtptr)
            {
                continue;
            }
            else if(opcode=="MEND")
            {
                fout<<index<<" "<<opcode;
                break;
            }
            else
            {
                cout<<"op:"<<op1<<" "<<op2<<endl;
                
                    int in=op1[3]-'0';
                   
                    op1=aptab[in].second;
                
                    in=op2[3]-'0';
                    op2=aptab[in].second;
                
                cout<<"op:"<<op1<<" "<<op2<<endl;
                fout<<index<<" "<<opcode<<" "<<op1<<" "<<op2<<endl;
            }
        }

        
    }
};
int main()
{
    generate g;
    g.generateCode();
    return 0;
}