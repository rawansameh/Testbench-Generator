#include <iostream>
#include <list>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>
#include <regex>
#include <string>
#include <cmath>

using namespace std;
vector<string> inputs, outputs;

void emitError()
{
    cout << "cannot open file\n";
    exit(0);
}

bool rst_check()
{   for(int i = 0; i < inputs.size(); i++)
    if(inputs[i] == "rst" || inputs[i] == "reset")
        return true;
    
    return false;
}

bool clk_check()
{
    for(int i = 0; i < inputs.size(); i++)
        if(inputs[i] == "clk" || inputs[i] == "clock")
            return true;
    
    return false;
    
}

void generate_tb(string moduleName, string Arguments)
{
    string filename = moduleName + "_tb";
    
    
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
        {
            if (IO[i][0] == '[' )
            {
                inputs.push_back(IO[i]+IO[i+1]);
                i++;
            }
            else {
                inputs.push_back(IO[i]);
            }
            i++;
        }
        i++;
        while(i != IO.size())
        { if (IO[i][0] == '[' )
        {
            outputs.push_back(IO[i]+IO[i+1]);
            i++;
        }
        else
        {
            outputs.push_back(IO[i]);
        }
            i++;
        }
        
    }
    
    vector <int> no_of_bits;
    
    for(int i = 0; i < inputs.size(); i++)
    {     if(inputs[i][0] == '[')
    {    int k,m;
        string str = inputs[i];
        k = str.find(':');
        m = str.find(']');
        str.erase(0,1);
        str.erase(k, m);
        int num;
        stringstream ss;
        ss << str;
        ss >> num;
        no_of_bits.push_back(num);
    }
    else
        no_of_bits.push_back(1);
        
    }
    
    for(int i = 0; i < outputs.size(); i++)
    {     if(outputs[i][0] == '[')
    {    int k,m;
        string str = outputs[i];
        k = str.find(':');
        m = str.find(']');
        str.erase(0,1);
        str.erase(k, m);
        int num;
        stringstream ss;
        ss << str;
        ss >> num;
        no_of_bits.push_back(num);
    }
    else
        no_of_bits.push_back(1);
        
    }
    
    
    cout << "`timescale 1ns/1ps\n\n\n";
    cout << "module " + filename + "();\n\n";
    
    if(clk_check())
        cout << "parameter CLOCK_PERIOD = 10;\n";
    
    if(rst_check())
    {    cout << "parameter RST_1 = 1'b1;\n";
        cout << "parameter RST_DELAY = 20;\n";
    }
    
    
    cout << "parameter DELAY = 50; \n";
    cout << "parameter FINISH = 700; \n\n";
    
    for(auto &it : inputs){
        cout << "\treg "+ it + "_i = 0;\n";
    }
    int pos1, pos2;
    for(int i = 0; i < inputs.size(); i++)
        for(int j = 0; j < inputs[i].length(); j++)
            if(inputs[i][j] == '[')
                pos1 = j;
    
    for(int i = 0; i < inputs.size(); i++)
        for(int j = 0; j < inputs[i].length(); j++)
            if(inputs[i][j] == ']')
            {   pos2 = j;
                inputs[i].erase(pos1,pos2+1);
            }
    
    for(auto &it : outputs){
        cout << "\twire "+ it + "_o;\n\n";
    }
    int pos3, pos4;
    for(int i = 0; i < outputs.size(); i++)
        for(int j = 0; j < outputs[i].length(); j++)
            if(outputs[i][j] == '[')
                pos3 = j;
    
    for(int i = 0; i < outputs.size(); i++)
        for(int j = 0; j < outputs[i].length(); j++)
            if(outputs[i][j] == ']')
            {   pos4 = j;
                outputs[i].erase(pos3,pos4+1);
            }
    
    cout << "\t" + moduleName + " UUT (";
    
    for(auto &it : inputs){
        cout << "." + it + "(" + it + "_i " + "),";
    }
    for (int i = 0; i < outputs.size(); i++)
    {  if( i == outputs.size()-1)
        cout << "." + outputs[i] + "(" + outputs[i] + "_o " + "));\n\n";
        
    else
        cout << "." + outputs[i] + "(" + outputs[i] + "_o " + "), ";
        
    }
    
    if(clk_check())
    {   cout << "\tinitial begin\n";
        cout << "\tclk = 0;\n";
        cout << "\tforever #CLOCK_PERIOD clk=~clk;\n";
        cout << "\tend\n\n";
    }
    cout << "\tinitial begin\n";
    for(int j = 0; j < 5; j++)
    {  for(int i = 0; i < inputs.size(); i++)
    {
        if(inputs[i] != "clk" && inputs[i] != "clock" && inputs[i] != "reset" && inputs[i] != "rst" )
        { int n = pow(2,no_of_bits[i]);
            cout << "\t" + inputs[i] + "_i" + " = " << no_of_bits[i] << "'d" << rand() % n << ";\n";
        }
        
        if(inputs[i] == "reset" || inputs[i] == "rst")
        { cout << "\t" + inputs[i] + "_i" << " = RST_1;\n";
            cout << "\t#RST_DELAY;\n";
            cout << "\t" + inputs[i] + "_o" << " = ~RST_1;\n";
        }
        
        
    }
        if(j != 4)
            cout << "\t#DELAY\n\n";
    }
    
    cout << "\tend\n\n";
    
    cout << "\tinitial begin\n";
    cout << "\t$dumpfile (\"" + moduleName + ".vcd\");\n";
    cout << "\t$dumpvars;\n";
    cout << "\tend\n\n";
    
    cout << "\tinitial begin\n";
    cout << "\t$monitor(\"time=%d,";
    for(auto &it : inputs){
        cout << it  + "_i" << "=%b,";
    }
    for (int i = 0; i < outputs.size(); i++)
    {  if( i == outputs.size()-1)
        cout << outputs[i]  + "_o" << "=%b,\",";
        
    else
        cout << outputs[i]  + "_o" << "=%b,";
    }
    cout << "$time,";
    for(auto &it : inputs){
        cout << it  + "_i " << ",";
    }
    for (int i = 0; i < outputs.size(); i++)
    {  if( i == outputs.size()-1)
        cout << outputs[i] + "_o" << ");\n";
        
    else
        cout << outputs[i] + "_o" << ",";
    }
    
    cout << "\tend\n\n";
    
    cout << "\tinitial\n";
    cout << "\t#FINISH $finish;\n\n";
    
    
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
