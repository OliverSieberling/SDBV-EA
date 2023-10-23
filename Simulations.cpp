#include <bits/stdc++.h>

using namespace std;

void sortIncreasingly(vector<char>& v);
void sortDecreasingly(vector<char>& v);
void permuteRandomly(vector<char>& v);
bool isFitterBV(vector<char>& v1, vector<char>& v2);
bool isFitter(vector<char>& v1, vector<char>& v2, string functionType, vector<int64_t> weights);
vector<char> generateChild(vector<char>& par);
vector<int64_t> initWeights(string functionType, int64_t n);

int main()
{
    // modify parameters here
    int64_t numIter =500;
    vector<string> functionTypeV = {"SDBV", "ADBV", "FDBV", "DBV", "NLF", "OM"}; // which functions to benchmark: SDBV, ADBV, FDBV, DBV, OM, NLF (uniform weights 1 to n)
    bool isElitary = false;
    bool selfAdjusting =  true;
    string lambda = "2log(n)"; // "1", "2log(n)", or "2sqrt(n)"
    long double F = 1.15;   // only for self adjusting
    long double s = 0.25;   // only for self adjusting
    int64_t from = 20; // smallest n to benchmark
    int64_t to = 420; // biggest n to benchmark
    int64_t stepSize =20; // step size of n
    

    // name output file after parameter choices
    stringstream filenameStream;

    string isElitaryString;
    if (isElitary) {
        isElitaryString = "+";
    } else {
        isElitaryString = ",";
    }
    if (selfAdjusting) {
        filenameStream << "SA-(1" << isElitaryString << lambda << ")-EA_F=" << F << "_s=" << s;
    } else {
        filenameStream << "(1" << isElitaryString << lambda << ")-EA";
    }
    filenameStream << "_tries=" << numIter << "_nStart=" << from << "_nEnd=" << to << "_stepSize=" << stepSize << ".txt";
    string filename = filenameStream.str();

    ofstream outFile(filename);
    streambuf* coutBuffer = cout.rdbuf();
    cout.rdbuf(outFile.rdbuf());

    cout << filename << endl;

    for (string functionType: functionTypeV) {
        cout << functionType << endl;

        vector<long double> avgGen, avgTotalChildren;
        for (int64_t n = from; n <= to; n += stepSize) { // change n

            long double genSum = 0;
            long double childrenSum = 0;

            long double numChildrenRaw;
            if (lambda == "1") {
                numChildrenRaw = 1;
            } else if (lambda == "2log(n)") {
                numChildrenRaw = 2*log(n);
            } else if (lambda == "2sqrt(n)") {
                numChildrenRaw = 2*sqrt(n);
            } else {
                cout << "Invalid lambda" << endl;
                return 0;
            }


            for (int64_t itter = 0; itter < numIter; itter++) {

                // initialize parent
                vector<char> parent(n);
                random_device rd2;
                mt19937 gen2(rd2());
                uniform_int_distribution<int64_t> dist2(0,1);
                int64_t parentZeroes = 0;
                for (int i = 0; i < n; i++) {
                    if (dist2(gen2) == 0) {
                        parentZeroes++;
                        parent[i] = 0;
                    } else {
                        parent[i] = 1;
                    }
                }

                int64_t generations = 0;
                int64_t totalChildren = 0;
                random_device rd;
                mt19937 gen(rd());
                uniform_int_distribution<int64_t> dist(0, n-1); // 1/n probability

                while (parentZeroes > 0) {
                    int64_t numChildren = round(numChildrenRaw);
                    totalChildren += numChildren;
                    generations++;

                    // sort the parent for the dynamic BinVal functions. You can then evaluate all of them using BinVal.
                    if (functionType.substr(functionType.size()-2, 2) == "BV") {
                        if (functionType == "SDBV") {
                            if (parentZeroes < (long double)n/2) {
                                sortIncreasingly(parent);
                            } else {
                                sortDecreasingly(parent);
                            }
                        }
                        if (functionType == "ADBV") {
                            sortIncreasingly(parent);
                        }
                        if (functionType == "FDBV") {
                            sortDecreasingly(parent);
                        }
                        if (functionType == "DBV") {
                            permuteRandomly(parent);
                        }
                    }

                    vector<char> fittestChild(n, 0);

                    vector<int64_t> weights = initWeights(functionType, n);
                    for (int64_t i = 0; i < numChildren; i++) {
                        vector<char> child = generateChild(parent);

                        if (isFitter(child, fittestChild, functionType, weights)) {
                            fittestChild = child;
                        }
                    }

                    if (selfAdjusting) {
                        if (isFitter(fittestChild, parent, functionType, weights)) {
                            numChildrenRaw *= 1/F;
                        } else {
                            numChildrenRaw *= pow(F, 1/s);
                        }
                    }

                    if (isElitary) {
                        if (isFitter(fittestChild, parent, functionType, weights)) {
                            parent = fittestChild;
                        }
                    } else {
                        parent = fittestChild;
                    }
                    parentZeroes = count(parent.begin(), parent.end(), false);
                }
                genSum += generations;
                childrenSum += totalChildren;
            }
            avgGen.push_back(genSum/numIter);
            avgTotalChildren.push_back(childrenSum/numIter);
        }
        cout << "average generations:" << endl;

        for (int i = 0; i < avgGen.size(); i++) {
            cout << avgGen[i];
            if (i < avgGen.size()-1) {
                cout << ", ";
            }
        }
        cout << endl;
        cout << "average total children: " << endl;
        for (int i = 0; i < avgTotalChildren.size(); i++) {
            cout << avgTotalChildren[i];
            if (i < avgTotalChildren.size()-1) {
                cout << ", ";
            }
        }
        cout << endl << endl << endl;
    }

    outFile.close();
    cout.rdbuf(coutBuffer);

    return 0;
}


// sorts the bit string non-decreasingly
void sortIncreasingly(vector<char>& v) {
    int64_t numZeroes = count(v.begin(), v.end(), false);
    for (int i = 0; i < numZeroes; i++) {
        v[i] = 0;
    }
    for (int i = numZeroes; i < v.size(); i++) {
        v[i] = 1;
    }
}

// sorts the bit string non-increasingly
void sortDecreasingly(vector<char>& v) {
    int64_t numZeroes = count(v.begin(), v.end(), false);
    for (int i = 0; i < v.size()-numZeroes; i++) {
        v[i] = 1;
    }
    for (int i = v.size()-numZeroes; i < v.size(); i++) {
        v[i] = 0;
    }
}

// randomly permutes the bit string
void permuteRandomly(vector<char>& v) {
    // improve: use better random generator than rand()
    random_shuffle(v.begin(), v.end());
}

// returns true if and only if bit string v2 is lexicographically smaller than bit string v1
bool isFitterBV(vector<char>& v1, vector<char>& v2) {
    return lexicographical_compare(v2.begin(), v2.end(), v1.begin(), v1.end());
}

// checks if bit string v1 is fitter than bit string v2
bool isFitter(vector<char>& v1, vector<char>& v2, string functionType, vector<int64_t> weights) {
    if (functionType.substr(functionType.size()-2, 2) == "BV") {
        return isFitterBV(v1, v2);
    }
    if (weights.size() == 0) {
        cout << "Error: Invalid function type" << endl;
        return false;
    }

    int64_t fitness1 = 0;
    int64_t fitness2 = 0;
    for (int64_t i = 0; i < v1.size(); i++) {
        fitness1 += v1[i]*weights[i];
        fitness2 += v2[i]*weights[i];
    }
    return fitness1 > fitness2;
}


// returns a copy of the parent where each bit is flipped with probability 1/n
vector<char> generateChild(vector<char>& par) {
    vector<char> child(par.size());
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int64_t> dist(0,par.size()-1);
    for (int64_t i = 0; i < par.size(); i++) {
        if (dist(gen) == 0) {
            child[i] = 1-par[i];
        } else {
            child[i] = par[i];
        }
    }
    return child;
}


// returns initialized weights depending on the function type; not for BinVal as weights would overflow
vector<int64_t> initWeights(string functionType, int64_t n) {
    vector<int64_t> weights;
    if (functionType == "NLF") {
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<int64_t> dist(1, n);
        for (int64_t i = 0; i < n; i++) {
            weights.push_back(dist(gen));
        }
    }
    if (functionType == "OM") {
        for (int64_t i = 0; i < n; i++) {
            weights.push_back(1);
        }
    }
    return weights;
}
