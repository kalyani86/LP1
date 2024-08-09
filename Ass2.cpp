#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>

using namespace std;

map<string, pair<string, string>> MOT;
map<string, string> REG;
map<string, string> CONDITIONALS;
map<string, int> symbol_table;
vector<pair<string, int>> literal_table;

void makeMOT() {
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
    MOT["DC"] = {"01", "DL"};
    MOT["DS"] = {"02", "DL"};
}

void makeREG() {
    REG["AREG"] = "01";
    REG["BREG"] = "02";
    REG["CREG"] = "03";
    REG["DREG"] = "04";
}

void makeCONDITIONALS() {
    CONDITIONALS["LT"] = "01";
    CONDITIONALS["LE"] = "02";
    CONDITIONALS["EQ"] = "03";
    CONDITIONALS["GT"] = "04";
    CONDITIONALS["GE"] = "05";
    CONDITIONALS["ANY"] = "06";
}

class IntermediateCode {
    int lc;

public:
    IntermediateCode() {
        lc=0;
    }

    vector<string> split(const string& s) {
        vector<string> vect;
        stringstream ss(s);
        string word;
        while (ss >> word) {
            vect.push_back(word);
        }
        return vect;
    }

    void getData(const string& line, string& label, string& opcode, string& op1, string& op2) {
        vector<string> tokens = split(line);
        int index = 0;

        // Check if the first token is a label
        if (MOT.find(tokens[index]) == MOT.end()) {
            label = tokens[index];
            index++;
        }

        // Get opcode
        if (index < tokens.size()) {
            opcode = tokens[index];
            index++;
        }

        // Get first operand
        if (index < tokens.size()) {
            op1 = tokens[index];
            index++;
        }

        // Get second operand
        if (index < tokens.size()) {
            op2 = tokens[index];
        }
    }

    void generateCode(vector<string>& lines) {
        for (const auto& line : lines) {
            string label, opcode, op1, op2;
            getData(line, label, opcode, op1, op2);

            if (opcode == "START") {
                lc = stoi(op1);
                cout << lc << " (AD, 01) (C, " << op1 << ")" << endl;
            } else if (opcode == "END") {
                cout << lc << " (AD, 02)" << endl;
                // Process any pending literals
                processLiterals();
            } else if (opcode == "ORIGIN") {
                lc = evaluateExpression(op1);
                cout << lc << " (AD, 03) (C, " << op1 << ")" << endl;
            } else if (opcode == "EQU") {
                int value = evaluateExpression(op1);
                symbol_table[label] = value;
                cout << lc << " (AD, 04) (S, " << label << ")" << endl;
            } else if (opcode == "LTORG") {
                cout << lc << " (AD, 05)" << endl;
                processLiterals();
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
                        if (symbol_table.find(op1) == symbol_table.end()) {
                            symbol_table[op1] = -1;
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
                        if (symbol_table.find(op2) == symbol_table.end()) {
                            symbol_table[op2] = -1;
                        }
                        cout << " (S, " << op2 << ")";
                    }
                }
                cout << endl;
                lc++;
            } else if (opcode == "DC") {
                symbol_table[label] = lc;
                cout << lc << " (DL, 01) (C, " << op1 << ")" << endl;
                lc++;
            } else if (opcode == "DS") {
                symbol_table[label] = lc;
                cout << lc << " (DL, 02) (C, " << op1 << ")" << endl;
                lc += stoi(op1);
            }
        }
    }

private:
    void processLiterals() {
        for (auto& literal : literal_table) {
            if (literal.second == -1) {
                literal.second = lc;
                cout << lc << " (DL, 01) (C, " << literal.first.substr(1) << ")" << endl;
                lc++;
            }
        }
    }

    int evaluateExpression(const string& expr) {
        
        size_t pos = expr.find_first_of("+-");
        if (pos != string::npos) {
            string symbol = expr.substr(0, pos);
            int value = symbol_table[symbol];
            int constant = stoi(expr.substr(pos + 1));
            if (expr[pos] == '+') {
                return value + constant;
            } else {
                return value - constant;
            }
        }
        return symbol_table[expr];
    }
};

int main() {
    makeMOT();
    makeREG();
    makeCONDITIONALS();

    vector<string> lines = {
        "START 100",
        "MOVER AREG, ='5'",
        "ADD AREG, BREG",
        "MOVEM AREG, ALPHA",
        "LTORG",
        "ALPHA DS 1",
        "END"
    };

    IntermediateCode inter;
    inter.generateCode(lines);

    return 0;
}
