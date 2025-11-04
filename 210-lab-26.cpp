#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>
#include <algorithm>
#include <set>
#include <list>
#include <iomanip>
using namespace std;

const int RUNS = 15; // number of test runs
const int OPERATIONS = 4; // read, sort, insert, delete
const int STRUCTURES = 3; // vector, list, set
const int W1 = 10; // column spacing


int main() {
    int results3D[RUNS][OPERATIONS][STRUCTURES]; // each run's results
    int accumulator[OPERATIONS][STRUCTURES] = {0}; // totals for averaging
    string cd;

    for (int run = 0; run < RUNS; run++) { // outerloop - repeat RUNS times
        cout << "\n--- Simulation " << run + 1 << " ---\n";

    vector<string> data_vector;
    list<string> data_list;
    set<string> data_set;

    // READ TEST - measure time to load data into each structure
    for (int i = 0; i < STRUCTURES; i++) {
        ifstream fin("codes.txt");
        if (!fin){
            cout << "File not found!" << run + 1 << endl;
            break;
        }
        auto start = chrono::high_resolution_clock::now();
        switch(i) {
            case 0: {  // read into a vector
                while (fin >> cd)
                        data_vector.push_back(cd);
                auto end = chrono::high_resolution_clock::now();
                auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
                results3D[run][0][i] = duration.count();
                accumulator[0][i] += duration.count();
                break;
            }
            case 1: {  // read into a list
                while (fin >> cd)
                        data_list.push_back(cd);
                auto end = chrono::high_resolution_clock::now();
                auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
                results3D[run][0][i] = duration.count();
                accumulator[0][i] += duration.count();
                break;
            }
            case 2: {  // read into a set
                while (fin >> cd)
                        data_set.insert(cd);
                auto end = chrono::high_resolution_clock::now();
                auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
                results3D[run][0][i] = duration.count();
                accumulator[0][i] += duration.count();
                break;
            }
        }
        fin.close();
    }

    // SORT TEST - sort vectore and list
    for (int i = 0; i < STRUCTURES; i++) {
        auto start = chrono::high_resolution_clock::now();
        switch(i) {
            case 0: {  // sort a vector
                sort(data_vector.begin(), data_vector.end());
                auto end = chrono::high_resolution_clock::now();
                auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
                results3D[run][1][i] = duration.count();
                accumulator[1][i] += duration.count();

                break;
            }
            case 1: {  // sort a list
                data_list.sort();
                auto end = chrono::high_resolution_clock::now();
                auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
                results3D[run][1][i] = duration.count();
                accumulator[1][i] += duration.count();

                break;
            }
            case 2: {  // can't sort a set, so set to -1
                results3D[run][1][i] = -1; // set cant be sorted
                accumulator[1][i] += -1;
                break;
            }
        }
    }

    // INSERT TEST - insert a value in the middle or into set
    for (int i = 0; i < STRUCTURES; i++) {
        int ind_v = data_vector.size() / 2;
        int ind_l = data_list.size() / 2;
        auto start = chrono::high_resolution_clock::now();
        switch(i) {
            case 0: {  // insert into a vector
                data_vector.insert(data_vector.begin() + ind_v, "TESTCODE");
                auto end = chrono::high_resolution_clock::now();
                auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
                results3D[run][2][i] = duration.count();
                accumulator[2][i] += duration.count();

                break;
            }
            case 1: {  // insert into a list
                auto it = data_list.begin();
                advance(it, ind_l);
                data_list.insert(it, "TESTCODE");
                auto end = chrono::high_resolution_clock::now();
                auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
                results3D[run][2][i] = duration.count();
                accumulator[2][i] += duration.count();

                break;
            }
            case 2: {  // insert into a set
                data_set.insert("TESTCODE");
                auto end = chrono::high_resolution_clock::now();
                auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
                results3D[run][2][i] = duration.count();
                accumulator[2][i] += duration.count();

                break;
            }
        }
    }

    // DELETE TEST - remove one element from each structure
    for (int i = 0; i < STRUCTURES; i++) {
        // select a target value in the vector 
        int ind = data_vector.size() / 2;
        string target_v = data_vector[ind];

        // select a target value in the list
        auto it1 = data_list.begin();
        advance(it1, ind);
        string target_l = *it1;

        // select a target value in the set
        auto it2 = data_set.begin();
        advance(it2, ind);
        string target_s = *it2;
        
        auto start = chrono::high_resolution_clock::now();
        switch(i) {
            case 0: {  // delete by value from vector
                data_vector.erase(remove(data_vector.begin(), data_vector.end(), target_v));
                auto end = chrono::high_resolution_clock::now();
                auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
                results3D[run][3][i] = duration.count();
                accumulator[3][i] += duration.count();

                break;
            }
            case 1: {  // delete by value from list
                data_list.remove(target_l);
                auto end = chrono::high_resolution_clock::now();
                auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
                results3D[run][3][i] = duration.count();
                accumulator[3][i] += duration.count();

                break;
            }
            case 2: {  // delete by value from set
                data_set.erase(target_s);    
                auto end = chrono::high_resolution_clock::now();
                auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
                results3D[run][3][i] = duration.count();
                accumulator[3][i] += duration.count();

                break;
            }
        }
    }
    // show results for current run
    string labels[] = {"Read", "Sort", "Insert", "Delete"};
    cout << setw(W1) << "Operation" << setw(W1) << "Vector" << setw(W1) << "List"
         << setw(W1) << "Set" << endl;
    for (int i = 0; i < 4; i++) {
        cout << setw(W1) << labels[i];
        for (int j = 0; j < STRUCTURES; j++) 
            cout << setw(W1) << accumulator[i][j];
        cout << endl;
    }

    // debug print to see if works
    cout << "Run " << run + 1 << " Vector read time: "
            << results3D[run][0][0] << " microseconds" << endl;
    
    } // closing new loop
    
    // after loops ends
    cout << "\nNumber of simulations: " << RUNS << endl;
    // compute averages
    double averages[OPERATIONS][STRUCTURES];
    for (int i = 0; i < OPERATIONS; i++) {
        for (int j = 0; j < STRUCTURES; j++) {
            averages[i][j] = static_cast<double>(accumulator[i][j]) / RUNS;
        }
    }

    // average output
    string labels[] = {"Read", "Sort", "Insert", "Delete"};
    cout << setw(W1) << "Operation" << setw(W1) << "Vector" << setw(W1) << "List"
         << setw(W1) << "Set" << endl;
    for (int i = 0; i < OPERATIONS; i++) {
        cout << setw(W1) << labels[i];
        for (int j = 0; j < STRUCTURES; j++) {
            cout << setw(W1) << fixed << setprecision(2) << averages[i][j];
        }
        cout << endl;
    }

    return 0;
}
