#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <iomanip>
#include <stdexcept>

using namespace std;

class MachineCode {
    vector<pair<string, int>> symbol;
    vector<pair<string, int>> literal;

public:
    MachineCode() {
        processSymbol();
        processLiteral();
    }

    vector<string> split(string s) {
        vector<string> vect;
        stringstream ss(s);
        string word;
        while (ss >> word) {
            vect.push_back(word);
        }
        return vect;
    }

    int safeStringToInt(const string& str) {
        try {
            return stoi(str);
        } catch (const invalid_argument& e) {
            cerr << "Error: Invalid integer conversion for string: " << str << endl;
            return -1;
        }
    }

    void processSymbol() {
        ifstream fs("symbol.txt");
        string line;
        while (getline(fs, line)) {
            vector<string> s = split(line);
            if (s.size() == 2) {
                string sym = s[0];
                int lc = safeStringToInt(s[1]);
                if (lc != -1) {
                    symbol.push_back({sym, lc});
                }
            } else {
                cerr << "Error: Invalid data in symbol.txt: " << line << endl;
            }
        }
    }

    void processLiteral() {
        ifstream fs("literal.txt");
        string line;
        while (getline(fs, line)) {
            vector<string> s = split(line);
            if (s.size() == 2) {
                string lit = s[0];
                int lc = safeStringToInt(s[1]);
                if (lc != -1 && lit[0] == '=' && lit[1] == '\'' && lit[lit.length() - 1] == '\'') {
                    lit = lit.substr(2, lit.length() - 3); // Remove = and '
                    literal.push_back({lit, lc});
                } else {
                    cerr << "Error: Invalid literal format: " << line << endl;
                }
            } else {
                cerr << "Error: Invalid data in literal.txt: " << line << endl;
            }
        }
    }

    void getData(const string& line, string& lc, string& opcode, string& op1, string& op2) {
        stringstream ss(line);
        string part1, part2, part3, part4;

        ss >> part1;
        if (ss >> part2) part2 = part2;
        if (ss >> part3) part3 = part3;
        if (ss >> part4) part4 = part4;

        if (part1[0] == '(') {
            lc = "";
            opcode = part1;
            op1 = part2;
            op2 = part3;
        } else {
            lc = part1;
            opcode = part2;
            op1 = part3;
            op2 = part4;
        }
    }

    void generateMachineCode(const string& line, ofstream& outputFile) {
        string lc, opcode, op1, op2;
        getData(line, lc, opcode, op1, op2);

        if (opcode.substr(1, 2) == "IS") {
            string code = opcode.substr(4, 2);
            string codeOp1 = (op1 != "") ? op1.substr(3, 2) : "00";
            string codeOp2 = (op2 != "") ? op2.substr(4, 1) : "0";

            // Output the formatted machine code
            outputFile << setw(2) << setfill('0') << code << " "
                       << setw(2) << setfill('0') << codeOp1 << " "
                       << setw(1) << setfill('0') << codeOp2 << endl;
        }
    }

    void processIntermediateCode(const string& filename, ofstream& outputFile) {
        ifstream fs(filename);
        string line;
        while (getline(fs, line)) {
            generateMachineCode(line, outputFile);
        }
    }
};

int main() {
    MachineCode mc;

    ofstream outputFile("output2.txt"); // Output file to store machine code
    if (!outputFile.is_open()) {
        cerr << "Error: Unable to open output file." << endl;
        return 1;
    }

    mc.processIntermediateCode("output.txt", outputFile);

    outputFile.close();
    return 0;
}
