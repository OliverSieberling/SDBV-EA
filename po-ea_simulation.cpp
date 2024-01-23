#include <bits/stdc++.h>

using namespace std;

vector<char> generateChild(vector<char>& par, bool isPess);

int main()
{
    // modify parameters here
    int64_t numIter = 500;
    int64_t from = 20;
    int64_t to = 420;
    int64_t stepSize =20;

    // name output file after parameter choices
    stringstream filenameStream;

    filenameStream << "PO-EA_tries=" << numIter << "_nStart=" << from << "_nEnd=" << to << "_stepSize=" << stepSize << ".txt";
    string filename = filenameStream.str();

    ofstream outFile(filename);
    streambuf* coutBuffer = cout.rdbuf();
    cout.rdbuf(outFile.rdbuf());

    cout << filename << endl;

    for (int j = 0; j < 2; j++) { // first iteration PO-EA, second iteration PO-EA^- (pessimistic)
        if (j == 0) {
            cout << "PO-EA" << endl;
        } else {
            cout << "PO-EA^- (pessimistic)" << endl;
        }
        for (int64_t n = from; n <= to; n += stepSize) {
            cout << "n: " << n << endl;
            for (int64_t itter = 0; itter < numIter; itter++) {

                // initialize parent
                vector<char> parent(n);
                random_device rd2;
                mt19937 gen2(rd2());
                uniform_int_distribution<int64_t> dist2(0,1);
                for (int i = 0; i < n; i++) {
                    parent[i] = dist2(gen2);
                }
                int64_t parentZeroes = count(parent.begin(), parent.end(), 0);

                int64_t generations = 1;

                while (parentZeroes > 0) {
                    generations++;
                    vector<char> child = generateChild(parent, j); // j=1 true, j=0 false
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
                        parentZeroes = count(parent.begin(), parent.end(), 0);
                    }
                }
                cout << generations << "\n";
            }
        }
    }

    outFile.close();
    cout.rdbuf(coutBuffer);

    return 0;
}

// returns a copy of the parent where each bit is flipped with probability 1/n
vector<char> generateChild(vector<char>& par, bool isPess) {
    vector<char> child(par);
    random_device rd;
    mt19937 gen(rd());
    bool zeroFlipSeen = false;
    uniform_int_distribution<int64_t> dist(0,par.size()-1);
    for (int64_t i = 0; i < par.size(); i++) {
        if (isPess) {
            if (par[i] == 1) {
                if (dist(gen) == 0) {
                    child[i] = 1-par[i];
                }
            } else {
                if (!zeroFlipSeen && dist(gen) == 0) {
                    zeroFlipSeen = true;
                    child[i] = 1-par[i];
                }
            }
        } else {
            if (dist(gen) == 0) {
                child[i] = 1-par[i];
            }
        }
    }
    return child;
}
