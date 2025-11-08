// This example shows a sort of compile time polymorphism.
// There is still the run time cost on the pointer, but not for the rest.

#include <memory>
#include "utils.hpp"

int main() {

    std::unique_ptr<PolicyHolderBase> ptr;

    ptr = std::make_unique<PolicyHolder<PolicyA>>();
    ptr->run();

    ptr = std::make_unique<PolicyHolder<PolicyB>>();
    ptr->run();

    return 0;
}