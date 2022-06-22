// Intent: Define a family of algorithms, encapsulate each one, and make them
// interchangeable. Strategy lets the algorithm vary independently from clients
// that use it.

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
class Strategy {
 public:
  virtual ~Strategy() {}
  virtual std::string DoAlgorithm(
      const std::vector<std::string> &data) const = 0;
};

// is configured with a ConcreteStrategy object.
// maintains a reference to a Strategy object.
// may define an interface that lets Strategy access its data.
class Context {
 private:
  Strategy *strategy_;
 public:
  Context(Strategy *strategy = nullptr) : strategy_(strategy) {}
  ~Context() { delete strategy_; }
  void set_strategy(Strategy *strategy) {
    delete strategy_;
    strategy_ = strategy;
  }
  void DoSomeBusinessLogic() const {
    // ...
    std::cout << "Context: Sorting data using the strategy (not sure how it'll "
                 "do it)\n";
    std::string result = strategy_->DoAlgorithm(
        std::vector<std::string>{"a", "e", "c", "b", "d"});
    std::cout << result << "\n";
    // ...
  }
};

// implements the algorithm using the Strategy interface.
class ConcreteStrategyA : public Strategy {
 public:
  std::string DoAlgorithm(const std::vector<std::string> &data) const override {
    std::string result;
    std::for_each(std::begin(data), std::end(data),
                  [&result](const std::string &letter) { result += letter; });
    std::sort(std::begin(result), std::end(result));

    return result;
  }
};
class ConcreteStrategyB : public Strategy {
  std::string DoAlgorithm(const std::vector<std::string> &data) const override {
    std::string result;
    std::for_each(std::begin(data), std::end(data),
                  [&result](const std::string &letter) { result += letter; });
    std::sort(std::begin(result), std::end(result));
    for (int i = 0; i < result.size() / 2; i++) {
      std::swap(result[i], result[result.size() - i - 1]);
    }

    return result;
  }
};

void ClientCode() {
  Context *context = new Context(new ConcreteStrategyA);
  std::cout << "Client: Strategy is set to normal sorting.\n";
  context->DoSomeBusinessLogic();
  std::cout << "\n";
  std::cout << "Client: Strategy is set to reverse sorting.\n";
  context->set_strategy(new ConcreteStrategyB);
  context->DoSomeBusinessLogic();
  delete context;
}

int main() {
  ClientCode();
  return 0;
}