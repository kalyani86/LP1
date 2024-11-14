#include<iostream> 
#include<map>
#include<vector>
#include<sstream>
#include<fstream>
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
        map<int,MDT>mdt;
        map<int,KDPTAB>kdptab;
        map<string,vector<PNTAB>>pntab;

        bool ismacro=false;
        int macroindex=0;
        string Cmacroname="";
        int kpdptr=1;
        int mdtptr=1;
        ifstream fin("input.txt");

        if(!fin.is_open())
        {
            cout<<"error";
        }
        string line;
        while(getline(fin,line))
        {
           
            // cout<<line<<endl;
            string word;
            stringstream ss(line);
            ss>>word;
            if(word=="MACRO")
            {
                ismacro=true;
            }
            else if(ismacro && word!="MEND")
            {
                MNT mne;
                mne.kdptr=kpdptr;
                mne.mdtptr=mdtptr;
                macroindex++;

                Cmacroname=word;
                int localkp=0;
                int localpn=0;
                while(ss>>word)
                {
                    //keyword
                    if(word.find('=')!=string::npos)
                    {
                        int pos=word.find('=');
                        string key=word.substr(0,pos);
                        string val=word.substr(pos+1);
                        key.erase(0,1);
                        if(val.back()==',')
                        {
                            val.pop_back();
                        }
                        KDPTAB kdp;
                        kdp.key=key;
                        kdp.dval=val;
                        kdptab[kpdptr]=kdp;
                        PNTAB pne;
                        pne.paraname=key;
                        pntab[Cmacroname].push_back(pne);
                        kpdptr++;
                        localkp++;
                    }
                    else
                    {
                        if(word.back()==',')
                        {
                            word.pop_back();
                        }
                        word.erase(0,1);
                        PNTAB pne;
                        pne.paraname=word;
                        pntab[Cmacroname].push_back(pne);
                        localpn++;
                    }
                }

                mne.macroname=Cmacroname;
                mne.index=macroindex;
                mne.kpcount=localkp;
                mne.ppcount=localpn;
                if(localkp==0)
                {
                    mne.kdptr=-1;
                }
                mnt[macroindex]=mne;
                ismacro=false;
            }
            else if(word=="MEND")
            {
                MDT entry;
                entry.index=mdtptr;
                entry.opcode=word;
                mdt[mdtptr]=entry;
                Cmacroname="";
                mdtptr++;
            }
            else
            {
                string opcode=word;
                MDT mde;
                mde.index=mdtptr;
                mde.opcode=opcode;

                string op1,op2;
                ss>>op1>>op2;

                if(op1[0]=='&')
                {
                    op1.erase(0,1);
                }
                if(op2[0]=='&')
                {
                    op2.erase(0,1);
                }

                vector<PNTAB>v=pntab[Cmacroname];
               // cout<<Cmacroname<<endl;
               
                int i=1;
                int i1=-1;
                int i2=-1;
                for(auto el:v)
                {
                    if(el.paraname==op1)
                    {
                        i1=i;
                    }
                    if(el.paraname==op2)
                    {
                        i2=i;
                    }
                    i++;
                }

                string op="";
                
                if(i1!=-1)
                {
                    if(i2!=-1)
                    {
                        op="(P,"+to_string(i1)+")   (P,"+to_string(i2)+")";
                        mde.operand=op;
                    }
                    else
                    {
                        op="(P,"+to_string(i1)+")";
                        mde.operand=op;
                    }
                    cout<<op<<endl;
                }
                mdt[mdtptr]=mde;
                mdtptr++;
            }

        }

        ofstream min("mne_1.txt");
        ofstream mdtin("mdt_1.txt");
        ofstream kpd("kdp_1.txt");
        ofstream pnt("pnt_1.txt");

        min<<"index macroname ppcount kpcount mdtptr kdptr"<<endl;
        for(auto el:mnt)
        {
            min<<el.first<<" "<<el.second.macroname<<" "<<el.second.ppcount<<" "<<el.second.kpcount<<" "<<el.second.mdtptr<<" "<<el.second.kdptr<<endl;
        }

        for(auto el:mdt)
        {
            mdtin<<el.first<<" "<<el.second.opcode<<" "<<el.second.operand<<endl;
        }

        for(auto el:kdptab)
        {
            kpd<<el.first<<" "<<el.second.key<<" "<<el.second.dval<<endl;
        }

        for(auto el:pntab)
        {
            pnt<<el.first<<endl;
            for(auto el1:el.second)
            {
                pnt<<el1.paraname<<endl;
            }
            pnt<<endl;
        }

   }

};
int main()
{
    generate g;
    g.generateCode();
    return 0;
}