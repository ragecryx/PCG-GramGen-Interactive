#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

#include <PCGGram/Ruleset.hpp>
#include <PCGGram/Generator.hpp>

using namespace PCGGrammar;

#include "gramgen_int_conf.h"



static const string RULE_DELIMETER = "->";
static const string SYMBOL_DELIMETER = ",";
static const string WEIGHT_DELIMETER = "%";


bool ParseAddRule(string input, Ruleset &ruleset);


int main (int argc, char** argv) {

    string buf;

    cout << ":: Interactive Rule-based Generator v" << VER_MAJOR << "." << VER_MINOR << " ::" << endl << endl;
    cout << "Type 'q'      ...to exit" << endl;
    cout << "RuleName->Symbol,AnotherSymbol%Weight   ...to define a rule" << endl;
    cout << "!RuleName     ...to generate an object of that type." << endl;
    cout << "#filename     ...to import all rules from this file." << endl;
    cout << "*             ...to clear all current rules." << endl;
    
    cout << endl;
    cout << "* Enter a seed: ";
    string seed;
    cin >> seed;

    Ruleset rules;
    Generator generator(seed);
    generator.SetRuleset(&rules);
    
    cout << " = = = = =" << endl;
    while (true) {
        cout << ":> ";
        cin >> buf;

        if(buf.size()<1)
            continue;
        if(buf[0]=='q' && buf.length() == 1) {
            break;
        } else if(buf[0]=='!') {
            // generate
            {
                string ruleToGenerate;
                ruleToGenerate = buf.substr(1);
                vector<string> genContent = generator.Generate(ruleToGenerate);
                for(vector<string>::iterator it = genContent.begin(); it!=genContent.end(); ++it)
                    cout << *it << " ";
                cout << endl;
            }
        } else if(buf[0]=='#') {
            // load rules from file
            {
                string filename;
                filename = buf.substr(1);

                ifstream filestream(filename.c_str());

                string line;
                while(std::getline(filestream, line)){
                    ParseAddRule(line, rules);
                }

                filestream.close();
            }
        } else if(buf[0]=='*') {
            rules = Ruleset();
        } else {
            ParseAddRule(buf, rules);
        }
    }
    
    return 0;
}



bool ParseAddRule(string input, Ruleset &rules) {
    // rule data
    string rule;
    vector<string> listOfSymbols;
    float weightValue = 1.0f;

    size_t pos = 0; // used to track spliting positions
            
    // find rule delimeter & rule string
            
    pos = input.find(RULE_DELIMETER);
    if(pos == string::npos) {
        cout << "* Error adding generation rule!" << endl;
        return false;
    }

    rule = input.substr(0,pos);
    string rest = input.substr(pos + RULE_DELIMETER.length());



    // find weight delimeter & weight value
    pos = rest.find(WEIGHT_DELIMETER);
    if(pos == string::npos) {
        weightValue = 1.0f;
    } else {
        string weightString = rest.substr(pos + WEIGHT_DELIMETER.length(), string::npos);
        rest.erase(pos, string::npos);
        weightValue = atof(weightString.c_str());
    }


    // parse and add sybols to list
    string symbol;
    do {
        pos = rest.find(SYMBOL_DELIMETER);
        symbol = rest.substr(0, pos);
        listOfSymbols.push_back(symbol);
        rest.erase(0, pos + SYMBOL_DELIMETER.length());
    } while (pos != string::npos);


    // add parsed data into the ruleset
    rules.AddRule(rule, listOfSymbols, weightValue);

    #ifdef DEBUG
        cout << "* Added " << rule << " with " << weightValue << " weight and with symbols: ";
        for(vector<string>::iterator it = listOfSymbols.begin(); it!=listOfSymbols.end(); ++it)
            cout << " " << *it;
        cout << endl;
    #endif

    return true;
}
