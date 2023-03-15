#include "user_api.h"
#include "sum_of_products.cpp"
using namespace std;



#include <utility>
#include <vector>
#include <algorithm>
#include <bitset>
#include <cmath>
#include <string>
#include <sstream>

using namespace std;

// Function to find the minimum terms in the function
vector<int> findMinTerms(vector<int> minTerms, vector<int> dontCares) {
    vector<int> allTerms = std::move(minTerms);
    allTerms.insert(allTerms.end(), dontCares.begin(), dontCares.end());
    sort(allTerms.begin(), allTerms.end());

    vector<int> uniqueTerms;
    unique_copy(allTerms.begin(), allTerms.end(), back_inserter(uniqueTerms));

    return uniqueTerms;
}

// Function to convert an integer to a binary string
string intToBin(int x, int numBits) {
    string bin = bitset<32>(x).to_string();
    return bin.substr(32 - numBits);
}

// Function to calculate the Hamming distance between two binary strings
int hammingDistance(string a, string b) {
    int distance = 0;
    for (int i = 0; i < a.size(); i++) {
        if (a[i] != b[i]) distance++;
    }
    return distance;
}

// Function to merge two binary strings, using '-' as a placeholder for differing bits
string merge(string a, string b) {
    string merged;
    for (int i = 0; i < a.size(); i++) {
        if (a[i] == b[i]) {
            merged += a[i];
        } else {
            merged += '-';
        }
    }
    return merged;
}

// Function to convert a binary string to LaTeX format
string toLaTeX(string bin) {
    string expr;
    for (int i = 0; i < bin.size(); i++) {
        if (bin[i] == '0') {
            expr += "\\overline{X_" + to_string(i + 1) + "}";
        } else if (bin[i] == '1') {
            expr += "X_" + to_string(i + 1);
        }
        if (i < bin.size() - 1 && *expr.end()!=';') {
            expr += " \\;";
        }
    }
    return expr;
}

// Main function to implement the Quine-McCluskey algorithm
string sum_of_products(string request) {

//    cout<< "request: " << request << endl;
    vector<int> minTerms = {};  // input minimum terms
    vector<int> dontCares = {};           // don't care terms

    //TODO:这里把m和d都当成了minTerms，需要改进
    stringstream ss(request.substr(1, request.size() - 2));
    string num;
    while (getline(ss, num, ',')) {
        minTerms.push_back(stoi(num));
    }

    int numVars = log2(*(max_element(minTerms.begin(), minTerms.end())))+1;
    vector<vector<string>> groups(numVars + 1);

    vector<int> uniqueTerms = findMinTerms(minTerms, dontCares);

    // Group the minimum terms by the number of 1's in their binary representation
    for (int uniqueTerm : uniqueTerms) {
        string s = intToBin(uniqueTerm, numVars);
        groups[count(s.begin(), s.end(), '1')].push_back(s);
    }

    vector<vector<string>> newGroups;
    while (true) {
        newGroups.resize(groups.size() - 1);
        bool foundNewGroups = false;
        for (int i = 0; i < groups.size() - 1; i++) {
            for (int j = 0; j < groups[i].size(); j++) {
                for (int k = 0; k < groups[i + 1].size(); k++) {
                    if (hammingDistance(groups[i][j], groups[i + 1][k]) == 1) {
                        newGroups[i].push_back(merge(groups[i][j], groups[i + 1][k]));
                        foundNewGroups = true;
                    }
                }
            }
            if (!newGroups[i].empty()) {
                sort(newGroups[i].begin(), newGroups[i].end());
                unique_copy(newGroups[i].begin(), newGroups[i].end(), back_inserter(groups[i + 1]));
                newGroups[i].clear();
                foundNewGroups = false;
            }
        }
        if (!foundNewGroups) {
            break;
        }
    }


// Identify prime implicants
    vector<string> primeImplicants;
    for (auto & group : groups) {
        for (auto & j : group) {
            int count_ = count(j.begin(), j.end(), '-');
            if (count_ == numVars - minTerms.size()) {
                primeImplicants.push_back(j);
            }
        }
    }

// Identify essential prime implicants
    vector<string> essentialPrimeImplicants;
    vector<bool> covered(minTerms.size(), false);
    for (int i = 0; i < minTerms.size(); i++) {
        int numCovered = 0;
        int idx = -1;
        for (int j = 0; j < primeImplicants.size(); j++) {
            if (primeImplicants[j].find(intToBin(minTerms[i], numVars)) != string::npos) {
                numCovered++;
                idx = j;
            }
        }
        if (numCovered == 1) {
            essentialPrimeImplicants.push_back(primeImplicants[idx]);
            for (auto & group : groups) {
                group.erase(remove(group.begin(), group.end(), primeImplicants[idx]), group.end());
            }
            for (int j = 0; j < minTerms.size(); j++) {
                if (primeImplicants[idx].find(intToBin(minTerms[j], numVars)) != string::npos) {
                    covered[j] = true;
                }
            }
        }
    }

// Determine non-essential prime implicants
    vector<string> nonEssentialPrimeImplicants;
    for (const auto & primeImplicant : primeImplicants) {
        if (find(essentialPrimeImplicants.begin(), essentialPrimeImplicants.end(), primeImplicant) == essentialPrimeImplicants.end()) {
            nonEssentialPrimeImplicants.push_back(primeImplicant);
        }
    }

// Determine the simplified expression
    string simplifiedExpr;
    for (int i = 0; i < essentialPrimeImplicants.size(); i++) {
        simplifiedExpr += toLaTeX(essentialPrimeImplicants[i]);
        if (i < essentialPrimeImplicants.size() - 1) {
            simplifiedExpr += " + ";
        }
    }
    if (!nonEssentialPrimeImplicants.empty()) {
        if (!simplifiedExpr.empty()) {
            simplifiedExpr += " + ";
        }
        simplifiedExpr += " ( ";
        for (int i = 0; i < nonEssentialPrimeImplicants.size(); i++) {
            simplifiedExpr += toLaTeX(nonEssentialPrimeImplicants[i]);
            if (i < nonEssentialPrimeImplicants.size() - 1) {
                simplifiedExpr += " + ";
            }
        }
        simplifiedExpr += " ) ";
    }

    return "$$"+simplifiedExpr+"$$";

}







bool handle_api(str &header,str &outbuf,str filename )
{
    header += "HTTP/1.1 200 OK\r\n"
              "Content-Type: application/text\r\n"
              "Connection: Close\r\n";
    if(filename.find("sum_of_products") != string::npos)
    {
        size_t pos = filename.find('&');
        if (pos != string::npos) {
            string request = filename.substr(pos);
            sum_of_products (request);
            string response = sum_of_products (request);
            header += "Content-Length:"+ to_string(response.length())+
                    "\r\n"
                    "Server: Lambert-API Server\r\n\r\n";
            outbuf += header;
            outbuf += response;
            return true;
        }
    }
    return false;
}