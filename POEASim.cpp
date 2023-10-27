#include <bits/stdc++.h>

using namespace std;

vector<char> generateChild(vector<char>& par);

int main()
{
    // modify parameters here
    int64_t numIter =5;
    int64_t from = 20;
    int64_t to = 420;
    int64_t stepSize =20;

    // name output file after parameter choices
    stringstream filenameStream;

    filenameStream << "PO-EA_tries=" << numIter << "_nStart=" << from << "_nEnd=" << to << "_stepSize=" << stepSize << ".txt";
    string filename = filenameStream.str();

    ofstream outFile(filename);
    streambuf* coutBuffer = cout.rdbuf();
   // cout.rdbuf(outFile.rdbuf());

    cout << filename << endl;

    for (int j = 0; j < 2; j++) { // first iteration PO-EA, second iteration PO-EA^- (pessimistic)
        vector<long double> avgGen, avgTotalChildren;
        for (int64_t n = from; n <= to; n += stepSize) { // change n
            //cout << n << endl;
            long double genSum = 0;
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

                while (parentZeroes > 0) {
                    generations++;
                    vector<char> child = generateChild(parent);
                    int64_t childZeroes = count(child.begin(), child.end(), 0);
                    bool childDominates = true;
                    bool parentDominates = true;
                    for (int i = 0; i < n; i++) {
                        if (parent[i] > child[i]) {
                            childDominates = false;
                        }
                        if (child[i] > parent[i]) {
                            parentDominates = false;
                        }
                    }

                    if (childDominates || (!parentDominates && childZeroes > parentZeroes)) {
                        parent = child;
                        if (j == 0) { // PO-EA
                            parentZeroes = count(parent.begin(), parent.end(), 0);
                        }
                        if (j == 1) { // pessimistic PO-EA
                            parentZeroes = max(parentZeroes-1, count(parent.begin(), parent.end(), 0));
                        }
                    }
                }
                genSum += generations;
            }
            avgGen.push_back(genSum/numIter);
        }
        if (j == 0) {
            cout << "PO-EA" << endl;
        }
        if (j == 1) {
            cout << "PO-EA^- (pessimistic)" << endl;
        }
        cout << "average generations:" << endl;

        for (int i = 0; i < avgGen.size(); i++) {
            cout << avgGen[i];
            if (i < avgGen.size()-1) {
                cout << ", ";
            }
        }
        cout << endl << endl << endl;

    }



    outFile.close();
    cout.rdbuf(coutBuffer);

    return 0;
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
