#include <iostream>
#include <utility>
#include <vector>
#include <set>
#include <bitset>
#include <algorithm>
#include <cmath>
#include <sstream>
#include "quine_mccluskey.h"

bool state = true;
string message = "$$\\mathbb{ERROR}$$";

using namespace std;

string Util::intToBinString(int size, int val){
    string bin;
    bin = bitset<100>(val).to_string();
    bin = bin.substr(100-size);
    return bin;
}

int Util::get1s(string x) {
    // returns the number of 1s in a binary string
    int count = 0;
    for (char i : x){
        if (i == '1')
            count++;
    }
    return count;
}

bool Util::compare(string a, string b) {
    // checks if two string differ at exactly one location or not
    int count = 0;
    for(int i = 0; i < a.length(); i++) {
        if (a[i]!=b[i])
            count++;
    }

    if(count == 1)
        return true;

    return false;
}

string Util::getDiff(string a, string b) {
    // returs a string that replaces the differ location of two strings with '-'
    for(int i = 0; i < a.length(); i++) {
        if (a[i]!=b[i])
            a[i]='-';
    }

    return a;
}

bool Util::checkEmpty(vector< vector< string> > table){
    for (int i = 0; i < table.size(); ++i){
        if (!table[i].empty()) {
            return false;
        }
    }
    return true;
}


bool Util::primeIncludes(string imp, string minTerm){
    // check is a prime implicant satisfies a min term or not
    for (int i = 0; i < imp.size(); ++i){
        if (imp[i]!='-'){
            if(imp[i]!=minTerm[i]){
                return false;
            }
        }
    }
    return true;
}

int Util::getVar(set<int> comb, vector<string> primeImp){
    // returns the number of variables in a petrick method combination
    int count = 0;
    set<int> :: iterator itr;
    for (itr = comb.begin(); itr != comb.end(); ++itr){
        int imp = *itr;
        for (int i = 0; i < primeImp[imp].size(); ++i){
            if (primeImp[imp][i]!='-')
                count ++;
        }
    }
    return count;

}
string Util::toLaTeX(string bin) {
    string expr;
    for (int i = bin.size() - 1; i >= 0; i--) {
        if (bin[i] == '0') {
            expr += "\\overline{X_{" + to_string(bin.size() - 1 - i) + "}}";
        } else if (bin[i] == '1') {
            expr += "X_{" + to_string(bin.size() - 1 - i)+ "}";
        }
        if (*expr.end()!=';') {
            expr += " \\;";
        }
    }
    return expr;
}
string Util::bin_to_latex(string x){
    // converts binary string to LaTeX expression
    string expr ;
    for (int i = 0; i < x.size(); ++i){
        if (x[i]=='1') {
            expr += "X_{" + to_string(i)+ "}";
        }
        else if (x[i]=='0') {
            expr += "\\overline{X_{" + to_string(i) + "}}";
        }
        if (*expr.end()!=';')
            expr += " \\;";
    }
    return expr;
}
vector<int> Util::getSet(string bin) {
    std::string numStr ;
    auto *set = new vector<int>;
    for (int i = 0; i < bin.length(); i++) {
        if (isdigit(bin[i])) {
            numStr = bin[i];
            while (isdigit(bin[i+1])) {
                numStr += bin[i+1];
                i++;
            }
            try {
                set->push_back(stoi(numStr));
            }
            catch (std::out_of_range& e) {
                state = false;
                message = "$$\\mathbb{ERROR}:\\text{Number out of range}$$";
                continue;
            }
        }
        else if(bin[i] == '&') {
            break;
        }
    }
    return *set;
}



bool Tabulation::initialise(string request)
{
    minInt = Util::getSet(std::move(request));
    if(!state)
        return false;
    nMin = minInt.size();
    {
        int max = *(max_element(minInt.begin(), minInt.end()));
        if (max == 0)
            nBits = 1;
        else if(max > 1073741824)
            return false;
        else
            nBits = static_cast<int>(log2(max) + 1);
    }
    if(nMin == static_cast<size_t>(pow(2, nBits)))
    {
        message = "$$1$$";
        return false;
    }
    for (int j = 0; j < nMin; ++j){
        minBin.push_back(Util::intToBinString(nBits, minInt[j]));
    }
    table = vector< vector< string> >(nBits);

//    cout << "\nBinary representation of provided min terms:" << endl;
//    for (int i = 0; i < nMin; ++i){
//        cout << i << ") " <<  (minBin[i]) << endl;
//    }

    table = vector< vector< string> >(nBits+1);
    return true;
}

void Tabulation::setPrimeImp() {
    set< string > primeImpTemp;
    createTable();
    //create a table storage the binary representation of min terms
//        cout << "\nGetting Prime Implicants.." << endl;

    // Combine consecutive terms in the table until its empty
    while (!Util::checkEmpty(table)){
        table = combinePairs(table, primeImpTemp);
    }

    set<string > :: iterator itr;  // convert set to vector
    for (itr = primeImpTemp.begin(); itr != primeImpTemp.end(); ++itr){
        string x = *itr;
        primeImp.push_back(x);
    }

//    cout << "\nThe Prime Implicants are:" << endl;
//    for (int i = 0; i < primeImp.size(); ++i){
//        cout  << i << ") "<< (primeImp[i]) << endl;
//    }

}

void Tabulation::minimise() {
    // prepare primeImp chart
    bool** primeImpChart = new bool*[primeImp.size()];
    for (int i = 0; i < primeImp.size(); ++i) {
        primeImpChart[i] = new bool[nMin];
    }

    for (int i = 0; i < primeImp.size(); ++i){
        for (int j = 0; j < nMin; ++j){
            primeImpChart[i][j] = Util::primeIncludes(primeImp[i], minBin[j]);
        }
    }

//    cout << "\nPrime implicants chart (rows: prime implicant no., col: minterm no.):" << endl;
//    for (int i = 0; i < primeImp.size(); ++i){
//        for (int j = 0; j < nMin; ++j){
//            if (primeImpChart[i][j] == true){
//                cout << "1\t";
//            }
//            else {
//                cout << "0\t";
//            }
//        }
//        cout << endl;
//    }

    // petric logic
    vector< set<int> > patLogic;
    for (int j = 0; j < nMin; ++j){ // column iteration
        set<int> x;
        for (int i = 0; i < primeImp.size(); ++i){ // row iteration
            if (primeImpChart[i][j] == true) {
                x.insert(i);
            }
        }
        patLogic.push_back(x);
    }
//    cout << "\nPetric logic is (row: minterms no., col: prime implicants no.): " << endl;
    for (int i = 0; i < patLogic.size(); ++i){
        set<int > :: iterator itr;  // convert set to vector
        for (itr = patLogic[i].begin(); itr != patLogic[i].end(); ++itr){
            int x = *itr;
//            cout << x << " ";
        }
//        cout << endl;
    }

    // get all possible combinations
    set< set<int> > posComb;
    set<int> prod;
    getPosComb(patLogic, 0, prod, posComb); // recursively multiply set elements
    int min = 9999;

//    cout << "\nPossible combinations that satisfy all minterms:" << endl;
    set< set<int> > :: iterator itr1;
    for (itr1 = posComb.begin(); itr1 != posComb.end(); ++itr1){
        set<int> comb = *itr1;
        if (comb.size() < min){
            min = comb.size();
        }
        set<int > :: iterator itr;
        for (itr = comb.begin(); itr != comb.end(); ++itr){
            int x = *itr;
//            cout << x << " ";
        }
//        cout << endl;
    }

//    cout << "\nGetting the combinations with min terms and min variables ..." << endl;
    //Combinations with minimum terms
    vector< set<int> > minComb;
    set< set<int> > :: iterator itr;
    for (itr = posComb.begin(); itr != posComb.end(); ++itr){
        set<int> comb = *itr;
        if (comb.size() == min) {
            minComb.push_back(comb);
        }
    }

    //Combinations with minimum variables
    min = 9999;
    for (int i = 0; i < minComb.size(); ++i){
        if(Util::getVar(minComb[i], primeImp) < min){
            min = Util::getVar(minComb[i], primeImp);
        }
    }

    for (int i = 0; i < minComb.size(); ++i){
        if(Util::getVar(minComb[i], primeImp) == min){
            functions.push_back(minComb[i]);
        }
    }
}

void Tabulation::displayFunctions() {
    // prints output
//    cout << "\n\nThe possible functions are-\n" << endl;
    for (int i = 0; i < functions.size(); ++i){
        set<int> function = functions[i];
        set<int> :: iterator itr;
//        cout << "Function " << i+1 << ":"<< endl;
        for (itr = function.begin(); itr != function.end(); ++itr){
            int x = *itr;
//            cout << Util::bin_to_latex(primeImp[x]) << " + ";
        }
//        cout << "\b\b  \n" << endl;
    }
}

string Tabulation::getResult() {
    set<int> :: iterator itr;
    ostringstream oss;
    bool first = true;  // 标志变量，用于判断是否是第一次迭代
    for (itr = functions[0].begin(); itr != functions[0].end(); ++itr) {
        int x = *itr;
        if (first) {
            oss << Util::bin_to_latex(primeImp[x]);
            first = false;
        } else {
            oss << "+" << Util::bin_to_latex(primeImp[x]);
        }
    }
    return oss.str();
}


void Tabulation::getPosComb(vector< set<int> > &patLogic, int level, set<int> prod, set< set<int> > &posComb) {
    // a recursive function to multiple elements of set patLogic and store it in set posComb
    if (level == patLogic.size()){
        set<int> x = prod;
        posComb.insert(x);
        return;
    }
    else{
        set<int > :: iterator itr;
        for (itr = patLogic[level].begin(); itr != patLogic[level].end(); ++itr){

            int x = *itr;
            bool inserted = prod.insert(x).second;
            getPosComb(patLogic, level+1, prod, posComb);
            if (inserted){
                prod.erase(x);
            }
        }
    }
}

vector< vector< string> > Tabulation::combinePairs(vector< vector< string> > table, set<string>& primeImpTemp) {
    bool **checked = new bool*[table.size()];
    for(int i = 0; i < table.size(); ++i)
        checked[i] = new bool[nMin];
    vector< vector< string> > newTable(table.size()-1);


    for (int i = 0; i < table.size() -1; ++i){
        for (int j = 0; j < table[i].size(); ++j){
            for (int k = 0; k < table[i+1].size(); k++){
                if (Util::compare(table[i][j], table[i+1][k])){
                    newTable[i].push_back(Util::getDiff(table[i][j], table[i+1][k]));
                    checked[i][j] = true;
                    checked[i+1][k] = true;
                }
            }
        }
    }

    for (int i = 0; i < table.size(); ++i){
        for (int j = 0; j < table[i].size(); ++j){
            if (!checked[i][j]) {
                primeImpTemp.insert(table[i][j]);
            }
        }
    }

    return newTable;
}

void Tabulation::createTable() {
    for (int i = 0; i < nMin; ++i){
        int num1s = Util::get1s(minBin[i]);
        table[num1s].push_back(minBin[i]);
    }

//        cout << "\nTable:" << endl;
//        for (int i = 0; i < nBits+1; ++i){
//            cout << i << ")  ";
//            for (int j = 0; j < table[i].size(); ++j){
//                cout << table[i][j] << ", ";
//            }
//            cout << endl;
//        }
}

string  Quine_McCluskey(const string& request)  {
//        cout << request << endl;
    state = true;
    Tabulation tab;
    if(!tab.initialise(request))
        return message;
    tab.initialise(request);
    tab.setPrimeImp();
    tab.minimise();
    tab.displayFunctions();
    if(state)
        return "$$"+tab.getResult()+"$$";
    else
        return message;
}