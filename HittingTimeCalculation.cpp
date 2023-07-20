#include <bits/stdc++.h>
#include <gmp.h>
#include <gmpxx.h>
using namespace std; 


int main()
{
    uint64_t n;
    long double cutoff;
    cin >> n >> cutoff;
    cutoff = min((long double)n+1, cutoff);
   
    vector<vector<mpz_class>> probN(n+1, vector<mpz_class>(n+1));
    vector<vector<mpz_class>> probD(n+1, vector<mpz_class>(n+1));
    
    for (uint64_t i = 0; i <= n; i++) {
        for (uint64_t k = 0; k <=n; k++) {
            mpz_class numer;
            mpz_class denom;
            mpz_init(numer.get_mpz_t());
            mpz_init(denom.get_mpz_t());
            mpz_set_ui(numer.get_mpz_t(), (uint64_t) 0);
            mpz_set_ui(denom.get_mpz_t(), (uint64_t) 1);
            probN[i][k] = numer;
            probD[i][k] = denom;
        }
    }

    // ADBV selection, change state
    for (uint64_t s = 1; s < cutoff; s++) {
        for (uint64_t k = 0; k <= n; k++) {
            if (s == k) continue;
            for (uint64_t i = 1; i <= s; i++) {
                if (i-s+k < 0 || i-s+k > n-s) continue;
                mpz_t numer;
                mpz_t denom;
                mpz_init(numer);
                mpz_init(denom);
                mpz_set_ui(numer, (uint64_t) 1);
                mpz_set_ui(denom, (uint64_t) 1);


                // s choose i
                for (uint64_t g = s; g >= s-i+1; g--) {
                    mpz_mul_ui(numer, numer, g);
                }
                for (uint64_t g = i; g >= 1; g--) {
                    mpz_mul_ui(denom, denom, g);
                }

                // (n-s) choose (i-s+k)
                for (uint64_t g = n-s; g >= n-s-(i-s+k)+1; g--) {
                    mpz_mul_ui(numer, numer, g);
                }
                for (uint64_t g = i-s+k; g >= 1; g--) {
                    mpz_mul_ui(denom, denom, g);
                }

                // (1/n)^(2i-s+k)
                for (uint64_t g = 0; g < 2*i-s+k; g++) {
                    mpz_mul_ui(denom, denom, n);
                }

                // ((n-1)/n)^(n-2*i+s-k)
                for (uint64_t g = 0; g < n-2*i+s-k; g++) {
                    mpz_mul_ui(numer, numer, n-1);
                }
                for (uint64_t g = 0; g < n-2*i+s-k; g++) {
                    mpz_mul_ui(denom, denom, n);
                }
                
                // add to p_{s,k}
                mpz_mul(numer, numer, probD[s][k].get_mpz_t());
                mpz_mul(probD[s][k].get_mpz_t(), probD[s][k].get_mpz_t(), denom);
                mpz_mul(probN[s][k].get_mpz_t(), probN[s][k].get_mpz_t(), denom);
                mpz_add(probN[s][k].get_mpz_t(), probN[s][k].get_mpz_t(), numer);
                
                // make numerator and denominator of p_{s,k} coprime
                mpz_t commonFac;
                mpz_init(commonFac);
                mpz_gcd(commonFac, probN[s][k].get_mpz_t(),  probD[s][k].get_mpz_t());
                mpz_div(probN[s][k].get_mpz_t(),  probN[s][k].get_mpz_t(), commonFac);
                mpz_div(probD[s][k].get_mpz_t(),  probD[s][k].get_mpz_t(), commonFac);
            }
        }
    }




    // FDBV selection, change state
    for (uint64_t s = (uint64_t) ceil(cutoff); s <= n; s++) {
        for (uint64_t k = 0; k <= n; k++) {
            if (s == k) continue;
            if (s-k < 0 || s-k > s) continue;
            mpz_t numer;
            mpz_t denom;
            mpz_init(numer);
            mpz_init(denom);
            mpz_set_ui(numer, (uint64_t) 1);
            mpz_set_ui(denom, (uint64_t) 1);

            // s choose (s-k)
            for (uint64_t g = s; g >= s-(s-k)+1; g--) {
                mpz_mul_ui(numer, numer, g);
            }
            for (uint64_t g = s-k; g >= 1; g--) {
                mpz_mul_ui(denom, denom, g);
            }

            // (1/n)^(s-k)
            for (uint64_t g = 0; g < s-k; g++) {
                mpz_mul_ui(denom, denom, n);
            }

            // ((n-1)/n)^(n-s+k)
            for (uint64_t g = 0; g < n-s+k; g++) {
                mpz_mul_ui(numer, numer, n-1);
            }
            for (uint64_t g = 0; g < n-s+k; g++) {
                mpz_mul_ui(denom, denom, n);
            }

            mpz_set(probN[s][k].get_mpz_t(), numer);
            mpz_set(probD[s][k].get_mpz_t(), denom);
        }
    }


    // remain in same state
    for (uint64_t s = 1; s <= n; s++) {
        for (uint64_t k = 0; k <= n; k++) {
            if (k == s) continue;
            mpz_t numer;
            mpz_t denom;
            mpz_init(numer);
            mpz_init(denom);
            mpz_set(numer, probN[s][k].get_mpz_t());
            mpz_set(denom, probD[s][k].get_mpz_t());

            // add to p_{s,s}
            mpz_mul(numer, numer, probD[s][s].get_mpz_t());
            mpz_mul(probD[s][s].get_mpz_t(), probD[s][s].get_mpz_t(), denom);
            mpz_mul(probN[s][s].get_mpz_t(), probN[s][s].get_mpz_t(), denom);
            mpz_add(probN[s][s].get_mpz_t(), probN[s][s].get_mpz_t(), numer);
            
            // make numerator and denominator of p_{s,s} coprime
            mpz_t commonFac;
            mpz_init(commonFac);
            mpz_gcd(commonFac, probN[s][s].get_mpz_t(),  probD[s][s].get_mpz_t());
            mpz_div(probN[s][s].get_mpz_t(),  probN[s][s].get_mpz_t(), commonFac);
            mpz_div(probD[s][s].get_mpz_t(),  probD[s][s].get_mpz_t(), commonFac);
        }
        mpz_mul_si(probN[s][s].get_mpz_t(), probN[s][s].get_mpz_t(), -1);
    }



    // output as input for Wolfram Mathematica
    cout <<  "A = {";
    for (uint64_t s = 1; s <= n; s++) {
        cout << "{";
        for (uint64_t k = 1; k <= n; k++) {
            gmp_printf("%Zd/%Zd", probN[s][k].get_mpz_t(), probD[s][k].get_mpz_t());
            if (k != n) {
                cout << ",";
            }
        }
        cout << "}";
        if (s != n) {
            cout << ",";
        }
    }
    cout << "};" << endl;
    
    cout << "b = {";
    for (uint64_t i = 1; i <= n; i++) {
        cout << "-1";
        if (i != n) {
            cout << ",";
        }
    }
    cout << "};" << endl;

    
    cout << "solution = LinearSolve[A, b];" << endl;
    cout << "totalSol = Sum[solution[[i]]*Binomial[Length[solution], i]/(2^Length[solution]), {i, 1, Length[solution]}];" << endl;
    cout << "Round[solution, 0.00001];" << endl;
    cout << "Round[totalSol, 0.00001]" << endl;

        
	
	return EXIT_SUCCESS;
}

