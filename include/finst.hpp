#include <tuple>
#include <functional>
#include <memory>

namespace finst
{

namespace helper
{

// Base case: If T is not a function, return T
template<typename T>
struct return_type {
    using type = T;
};

// Recursive case: If T is a function, apply return_type to its return type
template<typename R, typename... Args>
struct return_type<R(Args...)> {
    using type = typename return_type<R>::type;
};

// Helper function to deduce the return type of a lambda
template<typename F, typename... Args>
auto get_return_type(F&& f, Args&&... args) -> typename return_type<decltype(f(std::forward<Args>(args)...))>::type {
    return f(std::forward<Args>(args)...);
}

// Calls your func with tuple element.
template <class Func, class Tuple, size_t N = 0>
void runtime_get(Func func, Tuple& tup, size_t idx) {
    if (N == idx) {
        std::invoke(func, std::get<N>(tup));
        return;
    }

    if constexpr (N + 1 < std::tuple_size_v<Tuple>) {
        return runtime_get<Func, Tuple, N + 1>(func, tup, idx);
    }
} // https://stackoverflow.com/a/58674921

// Returns lambda
template <class Tuple, size_t N = 0>
auto runtime_get_lambda(Tuple& tup, size_t idx) {
    if (N == idx) {
        return std::get<N>(tup);
    }

    if constexpr (N + 1 < std::tuple_size_v<Tuple>) {
        return runtime_get<Tuple, N + 1>(tup, idx);
    }
} // https://stackoverflow.com/a/58674921 (MODIFIED)

} // namespace helper

// ------------------------------------------------------------------------------------------

template<typename Lambda>
class State {
private:
    Lambda _callable;

public:
    // Constructor
    State() {}

    // Method to execute the lambda
    template<typename... Args>
    auto execute(Args... args) -> decltype(_callable(args...)) {
        return _callable(std::forward(args...));
    }
};

template <typename Enum_States, typename... States>
class State_machine;

template <typename Enum_States, typename... States>
class State_machine {
private:
    std::tuple<States...> _state_ops;
    Enum_States _current_state;
public:
    State_machine() {};
    ~State_machine();

    void set_state(Enum_States arg_state) { _current_state = arg_state; };
    Enum_States get_state() { return _current_state; };

    template <typename... Args>
    auto process(Args... args);

};

template <typename Class, typename... States>
State_machine<Class, States...>::~State_machine() = default;

template <typename Enum_States, typename... States>
template <typename... Args>
inline auto State_machine<Enum_States, States...>::process(Args... args) {
    auto result = helper::get_return_type(helper::runtime_get_lambda(_state_ops, _current_state), args...);
}

}; // namespace finst
