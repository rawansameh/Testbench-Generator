#include <iostream>
#include <list>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>
#include <regex>

using namespace std;

void emitError()
{
    cout << "cannot open file\n";
    exit(0);
}

void generate_tb(string moduleName, string Arguments)
{
    string filename = moduleName + "_tb.v";
    
    vector<string> inputs, outputs;
    string s = "";
    vector<string> IO;
    for (auto x: Arguments){
        if (x == '(' || x == ' ' || x == ',' || x == ')'){
            if (s != "") IO.push_back(s);
            s = "";
        } else {
            s = s + x;
        }
    }
    for(int i = 1; i < IO.size(); i++)
    {
        while(IO[i] != "output")
        { inputs.push_back(IO[i]);
            i++;
        }
        i++;
        while(i != IO.size())
        { outputs.push_back(IO[i]);
            i++;
        }
    }
    
    
        cout << "'timescale 1ns/1ps\n\n\n";
        cout << "module " + filename + "();\n\n";
        for(auto &it : inputs){
            cout << "\treg "+ it + "_i = 0;\n";
        }
        for(auto &it : outputs){
            cout << "\twire "+ it + "_o;\n";
        }
        
        cout << "\t" + moduleName + " UUT (";
        
        for(auto &it : inputs){
            cout << "." + it + "(" + it + "),";
        }
        for(auto &it : outputs){
            cout << "." + it + "(" + it + "),";
        }
        cout << ");\n\n";
        
        for(int i = 0; i < inputs.size(); i++)
            if(inputs[i] == "clk" || inputs[i] == "clock")
            {   cout << "\tinitial begin\n";
                cout << "\tclk = 0\n;";
                cout << "\tforever #1 clk=~clk;\n;";
                cout << "\tend\n\n;";
            }
        cout << "\tinitial begin\n";
        cout << "\t\t//Testcases\n\n";
        cout << "\tend\n\n";
    
        cout << "\tinitial begin\n";
        cout << "\tdumpfile (\"" + moduleName + ".vcd\");\n";
        cout << "\tdumpvars\n";
        cout << "\tend\n\n";
        
        cout << "endmodule\n";
    
    
}




int main(int argc, const char * argv[]) {
    ifstream inFile;
   // ofstream outFile;
    string moduleName, Arguments, moduleCode;
    string buffer = "";
    
    if(argc<2) emitError();
    
    
    string filename, word;
    filename = argv[1];
    inFile.open(filename.c_str());
    
    while (getline(inFile, word)){
        buffer += word;
        if (word=="endmodule") break;
        buffer += " ";
    }
    
    smatch match;
    regex re("(module )([A-Za-z0-9_]+)(.*?);(.*)");
    if (regex_search(buffer, match, re) == true) {
        moduleName = match.str(2);
        Arguments = match.str(3);
        moduleCode = match.str(4);
    }
    
    
    generate_tb(moduleName, Arguments);
    
    
    return 0;
}

