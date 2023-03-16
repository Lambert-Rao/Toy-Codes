//
// Created by lambert on 23-3-16.
//

#ifndef UNTITLED_QUINE_MCCLUSKEY_H
#define UNTITLED_QUINE_MCCLUSKEY_H

#endif //UNTITLED_QUINE_MCCLUSKEY_H#include <iostream>
#include <vector>
#include <set>
#include <bitset>
#include <algorithm>


using namespace std;

class Tabulation;
class Util{
    /*
    A class that contains utility functions required in the program.
    */
    friend class Tabulation;
private:
    static string intToBinString(int size, int val);

    static int get1s(string x) ;

    static bool compare(string a, string b) ;

    static string getDiff(string a, string b) ;

    static bool checkEmpty(vector< vector< string> > table);

    static bool primeIncludes(string imp, string minTerm);

    static int getVar(set<int> comb, vector<string> primeImp);

    static string toLaTeX(string bin) ;

    static vector<int> getSet(string bin);

    static string bin_to_latex(string x);
};

class Tabulation{
private:
    vector< int > minInt; // min terms in decimal
    vector< string > minBin; // min terms in binary
    int nBits;
    size_t nMin;  // no of variables
    vector< vector< string> > table;
    vector< string > primeImp;
    vector< set<int> > functions;
//    ostringstream details;


public:
    bool initialise(string request) ;

    void setPrimeImp() ;

    void minimise() ;

    void displayFunctions() ;

    void getPosComb(vector< set<int> > &patLogic, int level, set<int> prod, set< set<int> > &posComb) ;

    vector< vector< string> > combinePairs(vector< vector< string> > table, set<string>& primeImpTemp) ;

    void createTable() ;

    string getResult();
};

string  Quine_McCluskey(const string& request)  ;

