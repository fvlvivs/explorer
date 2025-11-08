#ifndef CPP_UTILS_HPP
#define CPP_UTILS_HPP

#include <cstdio>

// Base struct to have CRTP.
template <typename Derived>
struct Base {

    Base() = default;

    void doSomething() {
        printf("doSomething()");
    }

    // Example of a method common to several structs, like PolicyA and PolicyB.
    void commonCall() {
        printf("commonCall()");
    }
};

struct PolicyA : public Base<PolicyA> {
    PolicyA() = default;

    // Redefinition.
    void doSomething() {
        printf("PolicyA::doSomething()");
    }
};


struct PolicyB : public Base<PolicyB> {
    PolicyB() = default;

    void doSomething() {
        printf("PolicyB::doSomething()");
    }
};

// Erasure class.
class PolicyHolderBase {
public:
    virtual ~PolicyHolderBase() = default;
    virtual void run() = 0;
};

// Actual class holding a Policy object.
template <typename Policy>
class PolicyHolder : public PolicyHolderBase {
public:
    PolicyHolder() = default;
    void run() override {
        // reinterpret_cast<Policy &>(policy_.run());
        policy_.doSomething();
    }

private:
    Policy policy_;

};



#endif //CPP_UTILS_HPP