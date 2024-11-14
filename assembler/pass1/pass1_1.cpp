#include<iostream>
#include<map>
#include<vector>
#include<fstream>
#include<sstream>
using namespace std;

class generate
{
    map<string,pair<string,string>>opcode;
    map<string,pair<int,string>>symbol;
    vector<pair<string,int>>literal;
    vector<string>pooltab;
    int lc;
    int litindex;
    public:

    void opcodeg()
    {
        opcode={
            {"STOP",{"IS","00"}},
            {"ADD",{"IS","01"}},
            {"SUB",{"Is","02"}},
            {"MULT",{"IS","03"}},
            {"MOVER",{"IS","04"}},
            {"MOVEM",{"IS","05"}},
            {"COMP",{"IS","06"}},
            {"BC",{"IS","07"}},
            {"DIV",{"IS","08"}},
            {"PRINT",{"IS","09"}},
            {"READ",{"IS","10"}},
            {"START",{"AD","01"}},
            {"END",{"AD","02"}},
            {"ORIGIN",{"AD","03"}},
            {"EQU",{"AD","04"}},
            {"LTORG",{"AD","05"}},
            {"DC",{"DL","01"}},
            {"DS",{"DL","02"}},
            {"AREG",{"01",""}},
            {"BREG",{"02",""}},
            {"CREG",{"03",""}},
            {"DREG",{"04",""}},
            {"LT",{"01",""}},
            {"LG",{"02",""}},
            {"EQ",{"03",""}},
            {"GT",{"04",""}},
            {"GQ",{"05",""}},
            {"ANY",{"06",""}}
        };
    }

    void generateCode()
    {
        ifstream fin("input.txt");
        ofstream sout("symbol1_1.txt");
        ofstream lout("literal1_1.txt");
        ofstream pout("poo1_1.txt");
        ofstream fout("output1_1.txt");
        litindex=0;
        lc=-1;
        symbol.clear();
        literal.clear();
        pooltab.clear();

        if(fin.fail() || sout.fail())
        {
            cout<<"error";
            exit(0);
        }

        string line;
        while(getline(fin,line))
        {
            stringstream ss(line);

            string word;
            ss>>word;

            string label;
            //label
            if(opcode.count(word)==0)
            {
                if(symbol.find(word)==symbol.end())
                {
                    symbol[word]={lc,to_string(symbol.size()+1)};
                }
                else
                {
                    symbol[word].first=lc;
                }
                label=word;
                ss>>word;
            }

            string opeartion=word;
            if(opeartion=="START")
            {
                int Loc;
                ss>>Loc;
                lc=Loc;
                fout<<"(AD,01) "<<" (C, "<<lc<<")"<<endl;
            }
            else if(opeartion=="END")
            {
                fout<<"("<<opcode[opeartion].first<<", "<<opcode[opeartion].second<<")"<<endl;
                //literal process
                pooltab.push_back("#"+to_string(litindex+1));
                for(;litindex<literal.size();litindex++)
                {
                    fout<<lc<<" ";
                    fout<<"(DL,01)"<<" ";
                    literal[litindex].second=lc;
                    string lit=literal[litindex].first;
                    string sublit=lit.substr(2,lit.size()-1);
                    fout<<"(C, "<<sublit<<")"<<endl;
                    lc++;
                }
            }
            else if(opeartion=="LTORG")
            {
                pooltab.push_back("#"+to_string(litindex+1));
                for(;litindex<literal.size();litindex++)
                {
                    fout<<lc<<" ";
                    fout<<"(DL,01)"<<" ";
                    literal[litindex].second=lc;
                    string lit=literal[litindex].first;
                    string sublit=lit.substr(2,lit.size()-1);
                    fout<<"(C, "<<sublit<<")"<<endl;
                    lc++;
                }
            }
            else if(opeartion=="EQU")
            {
                fout<<"("<<opcode[opeartion].first<<", "<<opcode[opeartion].second<<")";

                string equation;
                ss>>equation;

                int pmloc=0;
                if(equation.find('+'))
                {
                    pmloc=equation.find('+');
                }
                else if(equation.find('-'))
                {
                    pmloc=equation.find('-');
                }

                string be;
                string af="0";
                string sign="0";
                if(pmloc==0)
                {
                    be=equation;
                }
                else
                {
                    be=equation.substr(0,pmloc);
                    sign=equation[pmloc];
                    af=equation.substr(pmloc+1);
                }

                symbol[label].first=symbol[be].first;
                fout<<"(S,"<<symbol[be].second<<")";
                if(sign!="0")
                {
                    if(sign=="+")
                    {
                        symbol[label].first+=stoi(af);
                        fout<<"+"<<af<<endl;
                    }
                    if(sign=="-")
                    {
                        symbol[label].first =symbol[label].first-stoi(af);
                        fout<<"-"<<af<<endl;
                    }
                }
               

            }
            else if(opeartion=="ORIGIN")
            {
                fout<<"("<<opcode[opeartion].first<<", "<<opcode[opeartion].second<<")";

                string equation;
                ss>>equation;

                int pmloc=0;
                if(equation.find('+'))
                {
                    pmloc=equation.find('+');
                }
                else if(equation.find('-'))
                {
                    pmloc=equation.find('-');
                }

                string be;
                string af="0";
                string sign="0";
                if(pmloc==0)
                {
                    be=equation;
                }
                else
                {
                    be=equation.substr(0,pmloc);
                    sign=equation[pmloc];
                    af=equation.substr(pmloc+1);
                }

                lc=symbol[be].first;
                if(sign!="0")
                {
                    if(sign=="+")
                    {
                        lc+=stoi(af);
                        
                    }
                    if(sign=="-")
                    {
                        lc-=stoi(af);
                    }
                }
                fout<<"(C,"<<lc<<")"<<endl;
               
            }
            else
            {
                fout<<lc<<" ";
                fout<<"("<<opcode[opeartion].first<<", "<<opcode[opeartion].second<<") ";

                while(ss>>word)
                {
                    if(opeartion=="DC")
                    {
                        if(word.length() == 3)
                        {
                            word = word.substr(1,word.length()-2);

                        }
                        fout<<"(C,"<<word<<")";
                    }
                    else if(opeartion=="DS")
                    {
                        if(word.length() == 3)
                        {
                            word = word.substr(1,word.length()-2);

                        }
                        fout<<"(C,"<<word<<")";
                        lc+=stoi(word)-1;

                    }
                    else if(word.find("=")!=string::npos)
                    {
                        literal.push_back({word,lc});
                        fout<<"(L,"<<literal.size()<<")";
                    }
                    else if(opcode.count(word))
                    {
                        fout<<"("<<opcode[word].first<<")";
                    }
                    else
                    {
                        if(symbol.find(word)==symbol.end())
                        {
                            symbol[word]={lc,to_string(symbol.size()+1)};
                        }
                       fout<<"(S, "<<symbol[word].second<<")";
                        
                    }
                    
                }
                lc++;
                fout<<endl;
            }
        }

        fout.close();

        for(auto el :symbol)
        {
            sout<<el.second.second<<" "<<el.first<<" "<<el.second.first<<endl;
        }

        for(auto el:literal)
        {
            lout<<el.first<<" "<<el.second<<endl;
        }

        for(auto el:pooltab)
        {
            pout<<el<<endl;
        }
       
        
    }
    
};

int main()
{
    generate g;
    g.opcodeg();
    g.generateCode();
    return 0;
}