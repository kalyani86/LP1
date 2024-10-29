#include <iostream>
#include <vector>
#include <fstream>
#include <string>
using namespace std;

class macroProcess {
    vector<pair<string, vector<int>>> mnt;            // Macro Name Table
    vector<string> mdt;                              // Macro Definition Table
    vector<pair<string, string>> kpdtab;             // Keyword Parameter Default Table
    vector<vector<string>> pntab;                    // Parameter Name Table
    int pp, kp, mdtp, kpdtp;                         // Positional params, keyword params, mdt pointer, kpdt pointer

public:
    macroProcess() : pp(0), kp(0), mdtp(0), kpdtp(0) {}

    // Helper function to split a string by a delimiter
    vector<string> split(string &line, char del) {
        vector<string> ans;
        string temp = "";
        for (int i = 0; i < line.length(); i++) {
            char ch = line[i];
            if (ch == del) {
                ans.push_back(temp);
                temp = "";
                while (i < line.length() && line[i] == del) {
                    i++;
                }
                i--;
            } else {
                temp += (ch);
            }
        }
        if (!temp.empty()) {
            ans.push_back(temp);
        }
        return ans;
    }

    // Function to open and process the file
    void open_file(string file_name) {
        fstream obj(file_name);
        if (!obj.is_open()) {
            cerr << "Failed to open the file: " << file_name << endl;
            return;
        }
        process_code(obj);
    }

    // Function to process the macro code line by line
    void process_code(fstream &obj) {
        string line;
        bool macro = false;  // True when processing macro, false otherwise
        bool name = false;   // True when the macro name has been processed

        while (getline(obj, line)) {
            if (line.empty()) continue;  // Skip empty lines

            vector<string> ans = split(line, ' ');
            int size = ans.size();

            if (!macro) {
                if (size == 1 && ans[0] == "MACRO") {
                    macro = true;
                    name = false;
                    pp = 0;
                    kp = 0;
                    continue;
                } else {
                    // Not a macro, so continue with the next line
                    continue;
                }
            } else {
                if (!name) {
                    // Process the macro name and parameters
                    string n = ans[0];
                    vector<string> positional;

                    for (int i = 1; i < ans.size(); i++) {
                        int s = ans[i].length();
                        string x = "";
                        bool isKeyword = false;

                        for (int j = 1; j < s; j++) {
                            x += ans[i][j];

                            if (ans[i][j] == '=') {  // Keyword parameter
                                isKeyword = true;
                                string paramName = x.substr(0, j - 1);
                                string defaultValue = "";

                                if (j < s - 1) {
                                    defaultValue = ans[i].substr(j + 1);
                                }
                                kpdtab.push_back({paramName, defaultValue});
                                kp++;
                                break;
                            }
                        }

                        if (!isKeyword) {  // Positional parameter
                            pp++;
                            positional.push_back(x);
                        }
                    }

                    // Add macro to the MNT with parameter info
                    mdtp = mdt.size() + 1;
                    kpdtp = kpdtab.size() - kp + 1;  // KPDTP starts from current position
                    vector<int> param = {pp, kp, mdtp, kpdtp};
                    mnt.push_back({n, param});
                    pntab.push_back(positional);
                    name = true;
                } else {
                    // If MEND is encountered, mark the end of macro definition
                    if (size == 1 && ans[0] == "MEND") {
                        mdt.push_back("MEND");
                        macro = false;  // End macro processing
                    } else {
                        // Macro definition lines need to be processed here
                        string defLine = ans[0];

                        // Replace parameters with positional indices from PNTAB
                        for (int i = 1; i < ans.size(); i++) {
                            if (ans[i][0] == '&') {  // Check if it’s a parameter
                                // Search for positional and keyword parameter
                                string paramName = ans[i].substr(1);
                                int found = -1;

                                // Search in PNTAB for positional parameter
                                for (int j = 0; j < pntab.back().size(); j++) {
                                    if (pntab.back()[j] == paramName) {
                                        found = j;
                                        break;
                                    }
                                }

                                // If not found in PNTAB, search in KPD Table
                                if (found == -1) {
                                    for (int j = kpdtp - 1; j < kpdtab.size(); j++) {
                                        if (kpdtab[j].first == paramName) {
                                            found = j + pp;  // Keyword index
                                            break;
                                        }
                                    }
                                }

                                // Replace with index
                                if (found != -1) {
                                    defLine += " (P" + to_string(found + 1) + ")";
                                } else {
                                    defLine += " " + ans[i];  // Unchanged
                                }
                            } else {
                                defLine += " " + ans[i];
                            }
                        }

                        // Add the processed macro definition line to MDT
                        mdt.push_back(defLine);
                    }
                }
            }
        }
    }

    // Function to print the tables in the correct format
    void printTables() {
        // Print MNT
        cout << "MNT (Macro Name Table):" << endl;
        cout << "MNT Index | Name of Macro | #PP | #KP | MDTP | KPDTP" << endl;
        for (int i = 0; i < mnt.size(); i++) {
            cout << i + 1 << " | " << mnt[i].first << " | " << mnt[i].second[0] << " | " << mnt[i].second[1] << " | " << mnt[i].second[2] << " | " << mnt[i].second[3] << endl;
        }

        // Print KPD Table
        cout << "\nKPD Table (Keyword Parameter Default Table):" << endl;
        for (int i = 0; i < kpdtab.size(); i++) {
            cout << i + 1 << " | " << kpdtab[i].first << " = " << (kpdtab[i].second.empty() ? "(no default)" : kpdtab[i].second) << endl;
        }

        // Print MDT
        cout << "\nMDT (Macro Definition Table):" << endl;
        for (int i = 0; i < mdt.size(); i++) {
            cout << i + 1 << ": " << mdt[i] << endl;
        }

        // Print PNTAB
        cout << "\nPNTAB (Parameter Name Table):" << endl;
        for (int i = 0; i < pntab.size(); i++) {
            cout << "PNTAB for " << mnt[i].first << " Macro" << endl;
            for (int j = 0; j < pntab[i].size(); j++) {
                cout << j + 1 << ": " << pntab[i][j] << endl;
            }
        }
    }
};

int main() {
    macroProcess mp;
    mp.open_file("macro_code.txt");  // Provide the correct file path
    mp.printTables();  // To print the MNT, MDT, KPDTAB, and PNTAB tables

    return 0;
}
