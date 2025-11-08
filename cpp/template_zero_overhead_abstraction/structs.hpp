#ifndef CPP_STRUCTS_HPP
#define CPP_STRUCTS_HPP

// Example of how to use a modular approach to add different objects by leveraging
// template zero overhead abstraction.

// Design principle:
// This pattern separates compile-time polymorphism (templates) from runtime polymorphism
// (virtual functions). The decision about which Policy to use happens once at object
// creation time (runtime), but after that, all method calls within that policy are
// optimized at compile time with zero overhead through template instantiation and inlining.
//
// Performance characteristics:
// - One-time cost: Virtual function call overhead when calling run() through PolicyHolderBase*
// - Zero overhead: All calls from run() -> policy_method_.run() -> policy_.run() can be inlined
// - The compiler generates separate, optimized code for each Policy type (PolicyA, PolicyB, etc.)


// A struct which accepts two arguments.
struct PolicyA {
    PolicyA() = default;

    explicit PolicyA(const float a) : a_(a) {
    };

    float run(const float x) const {
        return x * a_;
    }

    float a_;
};

// A struct which accepts one argument.
struct PolicyB {
    PolicyB() = default;

    explicit PolicyB(const float a, const float b) : a_(a), b_(b) {
    };

    float run(const float x) const {
        return x * a_ - b_;
    }

    float a_;
    float b_;
};

// Here PolicyMethod is just a pass-through wrapper, but it might own extra functionality
// or variables that should not be contained in structs A and B.
template<typename Policy>
struct PolicyMethod {
    Policy policy_;

    // Apply perfect forwarding to a generic number of arguments.
    template<typename... PolicyArgs>
    explicit PolicyMethod(PolicyArgs &&... policy_args) {
        policy_ = Policy(std::forward<PolicyArgs>(policy_args)...);
    }

    float run(float x) {
        return policy_.run(x);
    }
};

// Type erase interface.
class PolicyHolderBase {
public:
    virtual ~PolicyHolderBase() = default;

    virtual float run(float x) = 0;
};

// Class which owns a generic templated struct object.
template<typename Policy>
class PolicyHolder final : public PolicyHolderBase {
    PolicyMethod<Policy> policy_method_;

public:
    // Apply perfect forwarding in case different structs accepts different arguments.
    template<typename... Args>
    explicit PolicyHolder(Args &&... args) {
        policy_method_ = PolicyMethod<Policy>(std::forward<Args>(args)...);
    }

    // NOTE: here there is a small overhead due to runtime polymorphism. Calling run with the right argument
    // can be crucial.
    float run(float x) override {
        return policy_method_.run(x);
    }
};

// The compiler generates separate, fully optimized code for PolicyHolder<PolicyA>
// and PolicyHolder<PolicyB>.
template<typename Policy, typename... Args>
std::unique_ptr<PolicyHolderBase> createPolicyHolder(Args &&... args) {
    return std::make_unique<PolicyHolder<Policy> >(std::forward<Args>(args)...);
}


#endif //CPP_STRUCTS_HPP
