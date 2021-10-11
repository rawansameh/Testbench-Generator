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
        { outputs.push_back(IO[i]);
            i++;
        }
    }
    
    
    
    
        cout << "'timescale 1ns/1ps\n\n\n";
        cout << "module " + filename + "();\n\n";
    
   for(int i = 0; i < inputs.size(); i++)
   {    if(inputs[i] == "clk" || inputs[i] == "clock")
            cout << "parameter CLOCK_PERIOD = 10\n";
       
       if(inputs[i] == "rst" || inputs[i] == "reset")
       {    cout << "parameter RST_1 = 1'b1\n";
            cout << "parameter RST_DELAY = 20\n";
       }
       
   }
        cout << "parameter DELAY = 50 \n";
        cout << "parameter FINISH = 500 \n\n";
    
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
        
        cout << "\t" + moduleName + " UUT (";
        
        for(auto &it : inputs){
            cout << "." + it + "(" + it + "),";
        }
        for (int i = 0; i < outputs.size(); i++)
        {  if( i = outputs.size()-1)
            cout << "." + outputs[i] + "(" + outputs[i] + "));\n\n";
        
           else
               cout << "." + outputs[i] + "(" + outputs[i] + "), ";
        
        }
        
        for(int i = 0; i < inputs.size(); i++)
            if(inputs[i] == "clk" || inputs[i] == "clock")
            {   cout << "\tinitial begin\n";
                cout << "\tclk = 0;\n";
                cout << "\tforever #CLOCK_PERIOD clk=~clk;\n";
                cout << "\tend\n\n";
            }
        cout << "\tinitial begin\n";
        cout << "\t\t//Testcases\n\n";
    // inputs[i] = number
    // #DELAY
    // input[i] = number
    
    //IF THERES RESET
    //inputs[i] = number
    // reset = RST_1
    // RST_DELAY
    // reset = ~RST_1
    //#DELAY
    //inputs[i] = number
    // reset = RST_1
    // RST_DELAY
    // reset = ~RST_1
    
        cout << "\tend\n\n";
    
        cout << "\tinitial begin\n";
        cout << "\t$dumpfile (\"" + moduleName + ".vcd\");\n";
        cout << "\t$dumpvars\n";
        cout << "\tend\n\n";
    
        cout << "\tinitial begin\n";
        cout << "\t$montior(time=%d," ;
        for(auto &it : inputs){
            cout << it << "=%b,";
        }
    for (int i = 0; i < outputs.size(); i++)
    {  if( i = outputs.size()-1)
        cout << outputs[i] << "=%b\",";
        
    else
        cout << outputs[i] << "=%b,";
    }
        cout << "$time,";
        for(auto &it : inputs){
            cout << it << ",";
        }
        for (int i = 0; i < outputs.size(); i++)
            {  if( i = outputs.size()-1)
                   cout << outputs[i] << ");\n";
       
               else
                   cout << outputs[i] << ",";
            }

    cout << "\tend\n\n";
    
        cout << "\tintial\n";
        cout << "\t#FINISH $finish\n\n";
    
        
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
