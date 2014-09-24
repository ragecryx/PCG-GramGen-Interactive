#include <iostream>
#include <string>
#include <vector>

using namespace std;

#include <PCGGram/Ruleset.hpp>
#include <PCGGram/Generator.hpp>

using namespace PCGGrammar;

#include "gramgen_int_conf.h"

static const string RULE_DELIMETER = "->";
static const string SYMBOL_DELIMETER = ",";

int main (int argc, char** argv) {

    string buf;

    cout << ":: Interactive Rule-based Generator v" << VER_MAJOR << "." << VER_MINOR << " ::" << endl << endl;
    cout << "To exit type 'q'" << endl;
    cout << "Define a rule... RuleName->Symbol,AnotherSymbol,YetAnotherSymbol" << endl;
    cout << "Generate... !RuleName to generate an object of that type." << endl;
    
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
            string ruleToGenerate;
            ruleToGenerate = buf.substr(1);
            vector<string> genContent;
            genContent = generator.Generate(ruleToGenerate);
            for(vector<string>::iterator it = genContent.begin(); it!=genContent.end(); ++it)
                cout << *it << " ";
            cout << endl;
        } else {
            string rule;
            vector<string> listOfSymbols;
            
            // find rule delimeter
            size_t pos = 0;
            pos = buf.find(RULE_DELIMETER);
            if(pos == string::npos) {
                cout << "* Error adding generation rule!" << endl;
                continue;
            }

            rule = buf.substr(0,pos);
            string rest = buf.substr(pos + RULE_DELIMETER.length());

            // parse and add sybols to list
            string symbol;
            do {
                pos = rest.find(SYMBOL_DELIMETER);
                symbol = rest.substr(0, pos);
                listOfSymbols.push_back(symbol);
                rest.erase(0, pos + SYMBOL_DELIMETER.length());
            } while (pos != string::npos);

            #ifdef DEBUG
                cout << "* Added the following rules: " << endl;
                for(vector<string>::iterator it = listOfSymbols.begin(); it!=listOfSymbols.end(); ++it)
                    cout << " " << *it << endl;
            #endif

            rules.AddRule(rule, listOfSymbols, 1.0f);
        }
    }
    
    return 0;
}