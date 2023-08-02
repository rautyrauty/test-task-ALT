#include <iostream>
#include "lib.hpp"

int main()
{
    std::cout << GetBranchesALTDiff("p10","p9")["only_in_first"] << '\n';
}
