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

    vector<string> split(const string& s) {
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
        if (!fs.is_open()) {
            cerr << "Error: Unable to open symbol.txt file." << endl;
            exit(1);
        }

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
        if (!fs.is_open()) {
            cerr << "Error: Unable to open literal.txt file." << endl;
            exit(1);
        }

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
        ss >> lc >> opcode >> op1 >> op2;
    }

    // Function to decode symbol and literal based on (S, x) or (L, x)
    int decodeOperand(const string& operand) {
        if (operand.empty()) return 0;

        // Check if the operand is a symbol or a literal
        char type = operand[1]; // Get the type (S or L)
        int index = safeStringToInt(operand.substr(3, operand.length() - 4)); // Get the index

        if (type == 'S' && index < symbol.size()) {
            return symbol[index].second; // Get symbol location counter
        }
        else if (type == 'L' && index < literal.size()) {
            return literal[index].second; // Get literal location counter
        }

        return 0;
    }

    void generateMachineCode(const string& line, ofstream& outputFile) {
        string lc, opcode, op1, op2;
        getData(line, lc, opcode, op1, op2);

        if (opcode.substr(1, 2) == "IS") { // Imperative Statement
            string code = opcode.substr(4, 2);  // Extract operation code
            int operand1 = decodeOperand(op1);  // Decode first operand
            int operand2 = decodeOperand(op2);  // Decode second operand

            // Output the formatted machine code
            outputFile << setw(2) << setfill('0') << code << " "
                       << setw(2) << setfill('0') << operand1 << " "
                       << setw(1) << setfill('0') << operand2 << endl;
        }
        else if (opcode.substr(1, 2) == "DL") { // Declarative Statement
            // Handle DL statements, like DC (Define Constant) or DS (Define Storage)
            if (op1 == "(C") {
                int constantValue = safeStringToInt(op2.substr(0, op2.length() - 1)); // Extract constant
                outputFile << "00 00 " << setw(2) << setfill('0') << constantValue << endl;
            }
        }
        else if (opcode.substr(1, 2) == "AD") {
            // Assembler Directives are typically ignored in machine code generation
            return;
        }
        else {
            cerr << "Error: Unsupported opcode format: " << opcode << " in line: " << line << endl;
        }
    }

    void processIntermediateCode(const string& filename, ofstream& outputFile) {
        ifstream fs(filename);
        if (!fs.is_open()) {
            cerr << "Error: Unable to open intermediate code file: " << filename << endl;
            exit(1);
        }

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
