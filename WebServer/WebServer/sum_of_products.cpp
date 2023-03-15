////
//// Created by lambert on 23-3-15.
////
//#include <iostream>
//#include <vector>
//#include <algorithm>
//#include <bitset>
//#include <cmath>
//
//using namespace std;
//
//// Function to find the minimum terms in the function
//vector<int> findMinTerms(vector<int> minTerms, vector<int> dontCares)
//{
//    vector<int> allTerms = minTerms;
//    allTerms.insert(allTerms.end(), dontCares.begin(), dontCares.end());
//    sort(allTerms.begin(), allTerms.end());
//
//    vector<int> uniqueTerms;
//    unique_copy(allTerms.begin(), allTerms.end(), back_inserter(uniqueTerms));
//
//    return uniqueTerms;
//}
//
//// Function to convert an integer to a binary string
//string intToBin(int x, int numBits)
//{
//    string bin = bitset<32>(x).to_string();
//    return bin.substr(32 - numBits);
//}
//
//// Function to calculate the Hamming distance between two binary strings
//int hammingDistance(string a, string b)
//{
//    int distance = 0;
//    for (int i = 0; i < a.size(); i++)
//    {
//        if (a[i] != b[i]) distance++;
//    }
//    return distance;
//}
//
//// Function to merge two binary strings, using '-' as a placeholder for differing bits
//string merge(string a, string b)
//{
//    string merged = "";
//    for (int i = 0; i < a.size(); i++)
//    {
//        if (a[i] == b[i])
//        {
//            merged += a[i];
//        } else
//        {
//            merged += '-';
//        }
//    }
//    return merged;
//}
//
//// Function to convert a binary string to LaTeX format
//string toLaTeX(string bin)
//{
//    string expr = "";
//    for (int i = 0; i < bin.size(); i++)
//    {
//        if (bin[i] == '0')
//        {
//            expr += "\\overline{X_" + to_string(i + 1) + "}";
//        } else if (bin[i] == '1')
//        {
//            expr += "X_" + to_string(i + 1);
//        }
//    }
//    return expr;
//}
////convert string of numbers to vector
//vector<int> extractNumbers(string request) {
//    vector<int> numbers;
//    string currentNum;
//    for (auto it = request.begin(); it != request.end(); it++) {
//        if (*it >= '0' && *it <= '9') {
//            currentNum += *it;
//        } else if (*it == ',' || *it == '&') {
//            if (!currentNum.empty()) {
//                numbers.push_back(stoi(currentNum));
//                currentNum = "";
//            }
//        }
//    }
//    if (!currentNum.empty()) {
//        numbers.push_back(stoi(currentNum));
//    }
//    return numbers;
//}
//// Main function to implement the Quine-McCluskey algorithm
//string sum_of_products (string request)
//{
//    vector<int> minTerms = {};  // input minimum terms
//    vector<int> dontCares = {};           // don't care terms
//
//    minTerms = extractNumbers(request);
//
//
//    int numVars = log2(*(max_element(minTerms.begin(), minTerms.end()))) + 1;
//    vector<vector<string>> groups(numVars + 1);
//
//    vector<int> uniqueTerms = findMinTerms(minTerms, dontCares);
//
//    // Group the minimum terms by the number of 1's in their binary representation
//    for (int i = 0; i < uniqueTerms.size(); i++)
//    {
//        groups[intToBin(uniqueTerms[i], numVars).find_first_of('1')].push_back(intToBin(uniqueTerms[i], numVars));
//    }
//
//    vector<vector<string>> newGroups;
//    while (true)
//    {
//        newGroups.clear();
//        for (int i = 0; i < groups.size() - 1; i++)
//        {
//            for (int j = 0; j < groups[i].size(); j++)
//            {
//                for (int k = 0; k < groups[i + 1].size(); k++)
//                {
//                    if (hammingDistance(groups[i][j], groups[i + 1][k]) == 1)
//                    {
//                        newGroups[i].push_back(merge(groups[i][j], groups[i + 1][k]));
//                        newGroups[i].push_back(merge(groups[i][j], groups[i + 1][k]));
//                    }
//                }
//            }
//            if (!newGroups[i].empty())
//            {
//                sort(newGroups[i].begin(), newGroups[i].end());
//                unique_copy(newGroups[i].begin(), newGroups[i].end(), back_inserter(groups[i + 1]));
//            }
//        }
//        if (newGroups.empty())
//        {
//            break;
//        }
//    }
//
//// Identify prime implicants
//    vector<string> primeImplicants;
//    for (int i = 0; i < groups.size(); i++)
//    {
//        for (int j = 0; j < groups[i].size(); j++)
//        {
//            int count_ = count (groups[i][j].begin(), groups[i][j].end(), '-');
//            if (count_ == numVars - minTerms.size())
//            {
//                primeImplicants.push_back(groups[i][j]);
//            }
//        }
//    }
//
//// Identify essential prime implicants
//    vector<string> essentialPrimeImplicants;
//    vector<bool> covered(minTerms.size(), false);
//    for (int i = 0; i < minTerms.size(); i++)
//    {
//        int numCovered = 0;
//        int idx = -1;
//        for (int j = 0; j < primeImplicants.size(); j++)
//        {
//            if (primeImplicants[j].find(intToBin(minTerms[i], numVars)) != string::npos)
//            {
//                numCovered++;
//                idx = j;
//            }
//        }
//        if (numCovered == 1)
//        {
//            essentialPrimeImplicants.push_back(primeImplicants[idx]);
//            for (int j = 0; j < groups.size(); j++)
//            {
//                groups[j].erase(remove(groups[j].begin(), groups[j].end(), primeImplicants[idx]), groups[j].end());
//            }
//            for (int j = 0; j < minTerms.size(); j++)
//            {
//                if (primeImplicants[idx].find(intToBin(minTerms[j], numVars)) != string::npos)
//                {
//                    covered[j] = true;
//                }
//            }
//        }
//    }
//
//// Determine non-essential prime implicants
//    vector<string> nonEssentialPrimeImplicants;
//    for (int i = 0; i < primeImplicants.size(); i++)
//    {
//        if (find(essentialPrimeImplicants.begin(), essentialPrimeImplicants.end(), primeImplicants[i]) ==
//            essentialPrimeImplicants.end())
//        {
//            nonEssentialPrimeImplicants.push_back(primeImplicants[i]);
//        }
//    }
//
//// Determine the simplified expression
//    string simplifiedExpr = "";
//    for (int i = 0; i < essentialPrimeImplicants.size(); i++)
//    {
//        simplifiedExpr += toLaTeX(essentialPrimeImplicants[i]);
//        if (i < essentialPrimeImplicants.size() - 1)
//        {
//            simplifiedExpr += " + ";
//        }
//    }
//    if (!nonEssentialPrimeImplicants.empty())
//    {
//        if (!simplifiedExpr.empty())
//        {
//            simplifiedExpr += " + ";
//        }
//        simplifiedExpr += " ( ";
//        for (int i = 0; i < nonEssentialPrimeImplicants.size(); i++)
//        {
//            simplifiedExpr += toLaTeX(nonEssentialPrimeImplicants[i]);
//            if (i < nonEssentialPrimeImplicants.size() - 1)
//            {
//                simplifiedExpr += " + ";
//            }
//        }
//        simplifiedExpr += " ) ";
//    }
//
//    cout << simplifiedExpr << endl;
//
//    return 0;
//
//}