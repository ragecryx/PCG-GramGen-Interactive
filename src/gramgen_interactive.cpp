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
static const string WEIGHT_DELIMETER = "%";



int main (int argc, char** argv) {

    string buf;

    cout << ":: Interactive Rule-based Generator v" << VER_MAJOR << "." << VER_MINOR << " ::" << endl << endl;
    cout << "'q'   ...to exit" << endl;
    cout << "RuleName->Symbol,AnotherSymbol,YetAnotherSymbol%weight   ...to define a rule" << endl;
    cout << "!RuleName     ...to generate an object of that type." << endl;
    cout << "#filename     ...to load all rules from this file." << endl;
    cout << "$filename     ...to save all rules in this file." << endl;
    
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
        } else if(buf[0]=='#') {
            cout << "Not implemented yet! :(" << endl;
        } else if(buf[0]=='$') {
            cout << "Not implemented yet! :(" << endl;
        } else {

            // rule data
            string rule;
            vector<string> listOfSymbols;
            float weightValue = 1.0f;

            size_t pos = 0; // used to track spliting positions
            
            // find rule delimeter & rule string
            
            pos = buf.find(RULE_DELIMETER);
            if(pos == string::npos) {
                cout << "* Error adding generation rule!" << endl;
                continue;
            }

            rule = buf.substr(0,pos);
            string rest = buf.substr(pos + RULE_DELIMETER.length());



            // find weight delimeter & weight value
            pos = rest.find(WEIGHT_DELIMETER);
            if(pos == string::npos) {
                weightValue = 1.0f;
            }

            string weightString = rest.substr(pos + WEIGHT_DELIMETER.length(), string::npos);
            rest.erase(pos, string::npos);
            cout << weightString << endl;
            weightValue = atof(weightString.c_str());



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
                cout << "* Added " << rule << " of " << weightValue*100 << "% chance with symbols:" << endl;
                for(vector<string>::iterator it = listOfSymbols.begin(); it!=listOfSymbols.end(); ++it)
                    cout << " " << *it << endl;
            #endif
        }
    }
    
    return 0;
}