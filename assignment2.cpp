#include <iostream>
#include<map>
#include<vector>
#include<string>
#include<algorithm>
#include<sstream>
#include<fstream>
using namespace std;


map<string, pair<string, string>> MOT;


void makeMOT()
{
   MOT["STOP"] = {"00", "IS"};
   MOT["ADD"] = {"01", "IS"};
   MOT["SUB"] = {"02", "IS"};
   MOT["MULT"] = {"03", "IS"};
   MOT["MOVER"] = {"04", "IS"};
   MOT["MOVEM"] = {"05", "IS"};
   MOT["COMP"] = {"06", "IS"};
   MOT["BC"] = {"07", "IS"};
   MOT["DIV"] = {"08", "IS"};
   MOT["READ"] = {"09", "IS"};
   MOT["PRINT"] = {"10", "IS"};
   MOT["START"] = {"01", "AD"};
   MOT["END"] = {"02", "AD"};
   MOT["ORIGIN"] = {"03", "AD"};
   MOT["EQU"] = {"04", "AD"};
   MOT["LTORG"] = {"05", "AD"};
   MOT["DC"] = {"01", "DS"};
   MOT["DS"] = {"02", "DS"};
}


map<string, string> REG;


void makeREG()
{
   REG["AREG"] = "01";
   REG["BREG"] = "02";
   REG["CREG"] = "03";
   REG["DREG"] = "04";
}


map<string, string> CONDITIONALS;


void makeCONDITIONALS()
{
   CONDITIONALS["LT"] = "01";
   CONDITIONALS["LE"] = "02";
   CONDITIONALS["EQ"] = "03";
   CONDITIONALS["GT"] = "04";
   CONDITIONALS["GE"] = "05";
   CONDITIONALS["ANY"] = "06";
}




class IntermediateCode
{
   int lc;


   vector<pair<string, int>> literal_table;
   vector<pair<string, int>> symbol_table;


public:
   void printPair(pair<string, string> p)
   {
       cout << "(" << p.second << "," << p.first << ")" << endl;
   }
   vector<string> split(string s)
   {
       vector<string> vect;
       stringstream ss(s);
       string word;
       while (ss >> word)
       {
           vect.push_back(word);
       }
       return vect;
   }
   void getData(string line,string& label,string& opcode, string& op1,string & op2)
   {
       vector<string> tokens = split(line);
       int index = 0;

       if (MOT.find(tokens[index]) == MOT.end())
       {
          // cout << "label:" << tokens[0]<<endl;
           label = tokens[index];
           index++;
       }


       if (index < tokens.size())
       {
           opcode = tokens[index];
           index++;
       }

       if (index < tokens.size())
       {
           op1 = tokens[index];
           index++;
       }

       if (index < tokens.size())
       {
           op2 = tokens[index];
       }
   }
   void getLocationINsymbolTable()
   {
      
   }

    int find(vector<pair<string,int>>v,string op1)
    {
        for(auto i:v)
        {
            if(i.first==op1)
            {
                return true;
            }
        }
        return false;
    }
   void generateCode(string data)
   {
       string label, opcode, op1, op2;
       std:: transform(data.begin(), data.end(), data.begin(), ::toupper);
       getData(data, label, opcode, op1,op2);
      
       if (opcode == "START") {
                lc = stoi(op1);
                cout << lc << " (AD, 01) (C, " << op1 << ")" << endl;
            } else if (opcode == "END") {
                cout << lc << " (AD, 02)" << endl;
                // Process any pending literals
                processsLiteral();
            } else if (opcode == "ORIGIN") {
                lc = evaluateExpression(op1);
                cout << lc << " (AD, 03) (C, " << op1 << ")" << endl;
            } else if (opcode == "EQU") {
                int value = evaluateExpression(op1);
                symbol_table.push_back({label,value});
                cout << lc << " (AD, 04) (S, " << label << ")" << endl;
            } else if (opcode == "LTORG") {
                cout << lc << " (AD, 05)" << endl;
                processsLiteral();
            } else if (MOT.find(opcode) != MOT.end()) {
                pair<string, string> mot_entry = MOT[opcode];
                string opcode_num = mot_entry.first;
                string opcode_type = mot_entry.second;

                cout << lc << " (" << opcode_type << ", " << opcode_num << ")";
                if (!op1.empty()) {
                    if (REG.find(op1) != REG.end()) {
                        cout << " (R, " << REG[op1] << ")";
                    } else if (CONDITIONALS.find(op1) != CONDITIONALS.end()) {
                        cout << " (CC, " << CONDITIONALS[op1] << ")";
                    } else if (op1[0] == '=') {
                        literal_table.push_back({op1, -1});
                        cout << " (L, " << literal_table.size() - 1 << ")";
                    } else {
                        if (find(symbol_table,op1)) {
                            symbol_table.push_back({op2,-1});
                        }
                        cout << " (S, " << op1 << ")";
                    }
                }
                if (!op2.empty()) {
                    if (REG.find(op2) != REG.end()) {
                        cout << " (R, " << REG[op2] << ")";
                    } else if (CONDITIONALS.find(op2) != CONDITIONALS.end()) {
                        cout << " (CC, " << CONDITIONALS[op2] << ")";
                    } else if (op2[0] == '=') {
                        literal_table.push_back({op2, -1});
                        cout << " (L, " << literal_table.size() - 1 << ")";
                    } else {
                        if (find(symbol_table,op2)) {
                            symbol_table.push_back({op2,-1});
                        }
                        cout << " (S, " << op2 << ")";
                    }
                }
                cout << endl;
                lc++;
            } else if (opcode == "DC") {
                symbol_table.push_back({label,lc});
                cout << lc << " (DL, 01) (C, " << op1 << ")" << endl;
                lc++;
            } else if (opcode == "DS") {
                symbol_table.push_back({label,lc});
                cout << lc << " (DL, 02) (C, " << op1 << ")" << endl;
                lc += stoi(op1);
            }
   }


   void processsLiteral()
   {
       for(auto & literal:literal_table)
       {
           if(literal.second==-1)
           {
               literal.second = lc;
               lc++;
           }
       }
   }


   int evaluateExpression(string str)
   {
       string label = "";
       int i = 0;
       char op =' ';
       while (i < str.size())
       {
           char ch = str[i];
           if (ch == '+' || ch == '-')
           {
               op = ch;
               break;
           }
           label = label + ch;
           i++;
       }


       string c = "";
       while(i<str.size())
       {
           c = c + str[i];
           i++;
       }


       int inc = stoi(c);
       //cout << label << "  " << inc << endl;
       int lloc = lc;
       if (op == '+')
       {
           lloc = lloc + inc;
       }
       else if(op=='-')
       {
           lloc = lloc - inc;
       }
       return lloc;
   }
};


int main()
{
   makeMOT();
   makeREG();
   makeCONDITIONALS();
   IntermediateCode inter;
  
   fstream fobj;
   fobj.open("D://TE//LP1//pass1.txt");
string line;
   while (getline(fobj, line)) 
   {
        cout<<line<<"\t";
        inter.generateCode(line);
    }
 
   return 0;
}
