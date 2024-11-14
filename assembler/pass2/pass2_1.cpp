#include<iostream>
#include<map>
#include<vector>
#include<fstream>
#include<sstream>
using namespace std;


class generate
{
    vector<pair<string,int>>sym;
    vector<pair<string,int>>literal;

    public:

    void load()
    {
        ifstream sin("sym.txt");
        ifstream lin("lit.txt");
        ifstream fin("output.txt");
        ofstream fout("mc.txt");

        string line;
        while(getline(sin,line))
        {
            stringstream ss(line);
            string ind,symbol;
            int lc;
            ss>>ind>>symbol>>lc;
            sym.push_back({symbol,lc});
        }

        while(getline(lin,line))
        {
            stringstream ss(line);
            string lit;
            int lc;
            ss>>lit>>lc;
            literal.push_back({lit,lc});
        }


        while(getline(fin,line))
        {
            stringstream ss(line);
            int loc;
            string opcode,op;
            ss>>loc>>opcode;
            if(opcode=="(IS,")
            {
                fout<<loc<<" ";
                string str;
                ss>>str;

                str.pop_back();
                fout<<str<<" ";
                while(ss>>op)
                {
                    if(op=="(S,")
                    {
                        string str;
                        ss>>str;
                        str.pop_back();
                        fout<<sym[stoi(str)-1].second<<" ";
                    }
                    else if(op=="(C")
                    {   
                        string str;
                        ss>>str;
                        str.pop_back();
                        fout<<str<<" ";
                    }
                    else if(op=="(L,")
                    {
                        string str;
                        ss>>str;
                        str.pop_back();
                        fout<<literal[stoi(str)-1].second<<" ";
                    }
                    else
                    {
                        fout<<op<<" ";
                    }

                }
                fout<<endl;
            }
            else if(opcode=="(DL,")
            {
                fout<<loc<<" ";
                string word;
                ss>>word;
                if(word=="01)")
                {
                    fout<<"00 00 ";
                    string str;
                    ss>>str>>str;
                    str.pop_back();
                    fout<<str<<endl;
                }
                else
                {
                    fout<<"no mc code"<<endl;
                }
            }
            else
            {
                fout<<"\nno mc code"<<endl;
            }
        }
    }
};


int main()
{
    generate g;
    g.load();
    return 0;
}