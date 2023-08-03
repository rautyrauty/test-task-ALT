#include <iostream>
#include <fstream>
#include "lib.hpp"

int main()
{
    using namespace std;
    cout << "Enter the name of the two altlinux branches you want to compare:" << '\n'
         << "allowed package set names are : ['sisyphus', 'p9', 'p10']. Example input: p10 p9." << '\n';

    string first, second;
    cin >> first >> second;

    ofstream file(first + "_and_" + second + "_branches_diff.json");
    if(file.is_open()) file << GetBranchesAltDiff(first,second).dump(4);

    cout << "You're welcome!" << '\n';

    return 0;
}
