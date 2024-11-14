#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <string>
#include <sstream>
using namespace std;


class MNT
{
    int index;
    string macroName;
    int ppCount;
    int kpCount;
    int mdtPointer;
    int kpdtPointer;

public:
    MNT()
    {
        index = 0;
        macroName = "";
        ppCount = 0;
        kpCount = 0;
        mdtPointer = 1;
        kpdtPointer = 1;
    }
    friend class Generate;
};

class MDT
{
    int index;
    string opcode;
    string operand;

public:
    MDT(){};
    MDT(int i, string opc, string opr)
    {
        index = i;
        opcode = opc;
        operand = opr;
    }
    friend class Generate;
};

class KDPTAB
{
    string keyword;
    string defaultValue;

public:
    KDPTAB(){};
    KDPTAB(string key, string value)
    {
        keyword = key;
        defaultValue = value;
    }
    friend class Generate;
};

class PNTAB
{
    string paramName;

public:
    PNTAB(){};
    PNTAB(string pn)
    {
        paramName = pn;
    }
    friend class Generate;
};

class Generate
{
public:
    void inout();
};

void Generate ::inout()
{
    ifstream mntFile("mnt.txt");
    ifstream pntabFile("pntab.txt");
    ifstream kpdtabFile("kpdtab.txt");
    ifstream mdtFile("mdt.txt");
    ofstream aptabFile("aptab.txt");
    ofstream expandedCodeFile("expanded_code.txt");

    if (!mntFile.is_open() || !pntabFile.is_open() || !kpdtabFile.is_open() || !mdtFile.is_open() || !aptabFile.is_open() || !expandedCodeFile.is_open())
    {
        cerr << "Error opening one or more input/output files." << endl;
        return;
    }

    // Load data structures from files
    map<int, MNT> mnt;
    map<int, MDT> mdt;
    map<int, KDPTAB> kpdtabMap;
    map<string, vector<PNTAB>> pntab;
    map<string,int>nameofmacro;

    // Load MNT
    string line;
    getline(mntFile, line); // Skip the header line
    while (getline(mntFile, line))
    {
        stringstream ss(line);
        int index, ppCount, kpCount, mdtPointer, kpdtPointer;
        string macroName;
        //cout<<"line:"<<line<<endl;
        ss >> index >> macroName >> ppCount >> kpCount >> mdtPointer >> kpdtPointer;
        MNT mntEntry;
        mntEntry.index = index;
        mntEntry.macroName = macroName;
        mntEntry.ppCount = ppCount;
        mntEntry.kpCount = kpCount;
        mntEntry.mdtPointer = mdtPointer;
        mntEntry.kpdtPointer = kpdtPointer;
        mnt[index] = mntEntry;
        nameofmacro[macroName]=index;
    }

    // Load PNTAB
    getline(pntabFile, line);
    string currentMacro = "";
    int macrocount=mnt.size();
    //cout<<"count:"<<macrocount<<endl;
    int i=0;
    while(i<macrocount)
    {
        getline(pntabFile,line);
        if(!line.empty())
        {
            currentMacro=line;
            int index=nameofmacro[currentMacro];
            int pcount=mnt[index].ppCount+mnt[index].kpCount;
            int j=0;
            while(j<pcount)
            {
                getline(pntabFile,line);
                if(!line.empty())
                {
                    PNTAB pn(line);
                    pntab[currentMacro].push_back(pn);
                    j++;
                }
            }
            currentMacro="";
            i++;
        }
    }

    // cout<<"pntab:\n";
    // for(auto i:pntab)
    // {
    //     cout<<"name:"<<i.first<<endl;
    //     for(auto j:i.second)
    //     {
    //         cout<<j.paramName<<" ";
    //     }
    //     cout<<endl;
    // }

    // Load KPDTAB
    getline(kpdtabFile, line);
    while (getline(kpdtabFile, line))
    {
        //cout<<"line1:"<<line<<endl;
        stringstream ss(line);
        int index;
        string keyword, defaultValue;
        ss >> index >> keyword >> defaultValue;
        KDPTAB kpdtabEntry;
        kpdtabEntry.keyword = keyword;
        kpdtabEntry.defaultValue = defaultValue;
        kpdtabMap[index] = kpdtabEntry;
    }

    // cout<<"kpd:"<<endl;
    // for(auto i:kpdtabMap)
    // {
    //     cout<<i.first<<" "<<i.second.keyword<<"-"<<i.second.defaultValue<<endl; 
    // }

    map<int,pair<string,string>> aptab;
    // call macro
    string Macrocall = " INCR_D INCR_VAL=B MEM_VAL=A REG=BREG";
    stringstream breakmacrocall(Macrocall);
    string macroname;
    breakmacrocall>>macroname;
    //cout<<"\nb= "<<macroname;
    string arg;

    vector<PNTAB>plist=pntab[macroname];
    int index=1;
    for(auto i:plist)
    {
        aptab[index].first=i.paramName;
        index++;
    }
    //2 posibilities 1-keyword 2-pos

    if(Macrocall.find('=')!=string::npos)
    {
        //keyword

        //1-load default values
        int ind=nameofmacro[macroname];
        int kpdind=mnt[ind].kpdtPointer;
        int kcount=mnt[ind].kpCount;
        int i=0;
        while(i<kcount)
        {
            string key,value;
            auto el=kpdtabMap[kpdind];
            key=el.keyword;
            value=el.defaultValue;
            for(auto& i:aptab)
            {
                if(i.second.first==key)
                {
                    i.second.second=value;
                }
            }
            kpdind++;
            i++;
        }

        //read the macro call and assign values accordingly;
        string parameter="";
        ind=1;
        while(breakmacrocall>>parameter)
        {
            cout<<"para:"<<parameter<<endl;
            if(parameter.find('=')!=string::npos)
            {
                int pos=parameter.find('=');
                string key=parameter.substr(0,pos);
                string value=parameter.substr(pos+1);
                for(auto& i:aptab)
                {
                    if(i.second.first==key)
                    {
                        i.second.second=value;
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
        //pos
        int index=1;
        string parameter;
        while(breakmacrocall>>parameter)
        {
            aptab[index].second=parameter;
            index++;
        }
    }
   
    
    cout<<"\nAPTAB"<<endl;
    aptabFile<<"\nAPTAB"<<endl;
    for(auto i:aptab){
    	cout<<i.first<<" "<<i.second.first<<" "<<i.second.second<<endl;
    	aptabFile<<i.first<<" "<<i.second.second<<endl;
    }
    cout<<endl;

//finding last
    int ind=nameofmacro[macroname];
    int sind=mnt[ind].mdtPointer;

    index=1;
    getline(mdtFile, line);
    while (getline(mdtFile, line))
    {
            //cout<<"\nline = "<<line;
            stringstream ss(line);
            string opcode, operand1, operand2;
            ss >> index >> opcode >> operand1 >> operand2;
            
            if(index<sind)
            {
                continue;
            }
            if(opcode=="MEND")
            {
                expandedCodeFile<<index<<"\t"<<opcode<<"\t"<<operand1<<"\t"<<operand2<<endl;
                //cout<<"\nDone";
                break;
            }
            if(operand1[1]=='P'){
            	int in=(operand1[3]-'0');
            	operand1=aptab[in].second;
            }
            if(operand2[1]=='P'){
                int in=(operand2[3]-'0');
                operand2=aptab[in].second;
            }
           // cout<<"\nhere = "<<index<<" "<<opcode<<" "<<operand1<<" "<<operand2<<endl;
            expandedCodeFile<<index<<"\t"<<opcode<<"\t"<<operand1<<"\t"<<operand2<<endl;
     }

    mntFile.close();
    pntabFile.close();
    kpdtabFile.close();
    mdtFile.close();
    aptabFile.close();
    expandedCodeFile.close();

    cout << "APTAB and expanded code generated successfully." << endl;
}

int main()
{
    Generate obj;
    obj.inout();
    return 0;
}