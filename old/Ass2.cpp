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


fstream fobj;
fstream fsymbol;
fstream fliterals;
 vector<pair<string, int>> literal_table;
 vector<pair<string, int>> symbol_table;
public:
IntermediateCode()
{
   fobj.open("output.txt");

}
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
 int getLocationINsymbolTable(string label)
 {
    //  cout<<label;
      for(int i=0;i<symbol_table.size();i++)
      {
          if(symbol_table[i].first==label)
          {
           
              return symbol_table[i].second;
          }
      }
      return -1;
 }




  void updateSymbolTable(string label,int l)
  {
      for(int i=0;i<symbol_table.size();i++)
      {
          if(symbol_table[i].first==label)
          {
              symbol_table[i].second=l;
              return;
          }
      }
      symbol_table.push_back({label,l});
  }








  void addSymboltable()
  { fsymbol.open("symbol.txt");
      for(int i=0;i<symbol_table.size();i++)
      {
          
          fsymbol<<symbol_table[i].first<<"  "<<symbol_table[i].second<<endl;
      }
      fsymbol.close();
  }


  void addLiteraltable()
  {
    fliterals.open("literal.txt");
      for(int i=0;i<literal_table.size();i++)
      {
          fliterals<<literal_table[i].first<<" "<<literal_table[i].second<<endl;
      }
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
      if(label!="")
      {
         
          updateSymbolTable(label,lc);
      }
         if (opcode == "START") {
              lc = stoi(op1);
              fobj<<" (AD,01) (C," << op1 << ")" << endl;
              return;
          } else if (opcode == "END") {
              fobj << " (AD,02)" << endl;
             
              processsLiteral();
          } else if (opcode == "ORIGIN") {
              //cout<<"before:"<<lc<<endl;
              lc = evaluateExpression(op1);
             // cout<<"after:"<<lc<<endl;
              fobj << " (AD,03) (C," << lc << ")" << endl;
          } else if (opcode == "EQU") {
              int value = evaluateExpression(op1);
              updateSymbolTable(label,value);
             // cout << lc << " (AD, 04) (c, " << value << ")" << endl;
          } else if (opcode == "LTORG") {
             
              processsLiteral();
           
          } else if (opcode == "DC") {
             // symbol_table.push_back({label,lc});
             updateSymbolTable(label,lc);
              fobj<< lc << " (DL,01) (C," << op1 << ")" << endl;
              lc++;
          } else if (opcode == "DS") {
              //symbol_table.push_back({label,lc});
              fobj<< lc << " (DL,02) (C," << op1 << ")" << endl;
              lc += stoi(op1);               
          }
          else if(MOT.find(opcode) != MOT.end()) {
              pair<string, string> mot_entry = MOT[opcode];
              string opcode_num = mot_entry.first;
              string opcode_type = mot_entry.second;
              fobj << lc << " (" << opcode_type << "," << opcode_num << ")";
       
          if (!op1.empty()) {
                  if (REG.find(op1) != REG.end()) {
                      fobj << " (R," << REG[op1] << ")";
                  } else if (CONDITIONALS.find(op1) != CONDITIONALS.end()) {
                      fobj << " (C," << CONDITIONALS[op1] << ")";
                  } else if (op1[0] == '=') {
                      literal_table.push_back({op1, -1});
                      if(literal_table.size()<10){
                         fobj << " (L,0" << literal_table.size() - 1 << ")";
                      }else{
                      fobj << " (L," << literal_table.size() - 1 << ")";}
                  } else {
                      int loc=getLocationINsymbolTable(op1);
                      if(loc==-1)
                      {
                          updateSymbolTable(op1,-1);
                      }
                      if(symbol_table.size()<10)
                      {
                        fobj << " (S,0" << symbol_table.size()-1 << ")";
                      }else{
                      fobj << " (S," << symbol_table.size()-1 << ")";}
                      //cout<<"op1:"<<op1<<endl;
                  }
              }
              if (!op2.empty()) {
                  if (REG.find(op2) != REG.end()) {
                      fobj << " (R," << REG[op2] << ")";
                  } else if (CONDITIONALS.find(op2) != CONDITIONALS.end()) {
                      fobj << " (CC," << CONDITIONALS[op2] << ")";
                  } else if (op2[0] == '=') {
                      literal_table.push_back({op2, -1});
                      if(literal_table.size()<10){
                         fobj << " (L,0" << literal_table.size() - 1 << ")";
                      }else{
                      fobj << " (L," << literal_table.size() - 1 << ")";}
                  } else {
                     int loc=getLocationINsymbolTable(op2);
                      if(loc==-1)
                      {
                          updateSymbolTable(op2,-1);
                      }
                       if(symbol_table.size()<10)
                      {
                        fobj << " (S,0" << symbol_table.size()-1 << ")";
                      }else{
                      fobj << " (S," << symbol_table.size()-1 << ")";}
                     // cout<<"op2:"<<op2<<endl;
                  }
              }
              lc++;
              fobj<<endl;
          }
          
           
 }


 void processsLiteral()
 {
     int i=0;
     for(auto & literal:literal_table)
     {
         if(literal.second==-1)
         {
             fobj << lc << " (AD,05)  (DL,01) (L ," <<i<<")"<< endl;
             literal.second = lc;
             lc++;
         }
         i++;
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
         if(ch!=' '){
         label = label + ch;}
         i++;
     }
     string c = "";
     while(i<str.size())
     {if(str[i]!=' '){
         c = c + str[i];}
         i++;
     }


     int inc = stoi(c);
    //cout << label << "  " << inc << endl;
     int lloc = getLocationINsymbolTable(label);
    //cout<<lloc<<endl;
     if (op == '+')
     {
         lloc = lloc + inc;
     }
     else if(op=='-')
     {
         lloc = lloc + inc;
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
 fobj.open("pass1.txt");
   string line;
 while (getline(fobj, line))
 {
      //cout<<line<<"\t";
      inter.generateCode(line);
  }
   inter.addSymboltable();
   inter.addLiteraltable();
 return 0;
}





























