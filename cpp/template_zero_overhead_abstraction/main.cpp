#include <memory>
#include <stdexcept>
#include <string>

#include "structs.hpp"



int main() {

    std::unique_ptr<PolicyHolderBase> ptr = createPolicyHolder<PolicyA>(0.0);
    float x = 5;
    printf("A: %.2f\n", ptr->run(x));

    float z = 2.0;
    ptr = createPolicyHolder<PolicyB>(1.0, z);
    printf("B: %.2f\n", ptr->run(x));

    return 0;
}