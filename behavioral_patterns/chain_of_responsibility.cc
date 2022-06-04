/**
 * Chain of Responsibility Design Pattern
 *
 * Avoid coupling the sender of a request to its receiver by giving more than
 * one object a chance to handle the request. Chain the receiving objects and
 * pass the request along the chain until an object handles it.
 */

#include <iostream>
#include <memory>
#include <string>
#include <vector>
/**
 * The Handler interface declares a method for building the chain of handlers.
 * It also declares a method for executing a request.
 */
class Handler {
 private:
  std::shared_ptr<Handler> successor_;

 public:
  virtual std::shared_ptr<Handler> SetSuccessor(
      std::shared_ptr<Handler> handler) {
    successor_ = handler;
    return handler;
  }
  virtual std::string HandleRequest(std::string request) {
    if (successor_) {
      return successor_->HandleRequest(request);
    } else return "";
  }
};

/**
 * All Concrete Handlers either handle a request or pass it to the next handler
 * in the chain.
 */
class MonkeyHandler : public Handler {
 public:
  std::string HandleRequest(std::string request) override {
    if (request == "Banana") {
      return "Monkey: I'll eat the " + request + ".\n";
    } else {
      return Handler::HandleRequest(request);
    }
  }
};
class SquirrelHandler : public Handler {
 public:
  std::string HandleRequest(std::string request) override {
    if (request == "Nut") {
      return "Squirrel: I'll eat the " + request + ".\n";
    } else {
      return Handler::HandleRequest(request);
    }
  }
};
class DogHandler : public Handler {
 public:
  std::string HandleRequest(std::string request) override {
    if (request == "MeatBall") {
      return "Dog: I'll eat the " + request + ".\n";
    } else {
      return Handler::HandleRequest(request);
    }
  }
};
/**
 * The client code is usually suited to work with a single handler. In most
 * cases, it is not even aware that the handler is part of a chain.
 */
void ClientCode(std::shared_ptr<Handler> handler) {
  std::vector<std::string> food = {"Nut", "Banana", "Cup of coffee"};
  for (const std::string &f : food) {
    std::cout << "Client: Who wants a " << f << "?\n";
    const std::string result = handler->HandleRequest(f);
    if (!result.empty()) {
      std::cout << "  " << result;
    } else {
      std::cout << "  " << f << " was left untouched.\n";
    }
  }
}
/**
 * The other part of the client code constructs the actual chain.
 */
int main() {
  std::shared_ptr<Handler> monkey(new MonkeyHandler);
  std::shared_ptr<Handler> squirrel(new SquirrelHandler);
  std::shared_ptr<Handler> dog(new DogHandler);
  monkey->SetSuccessor(squirrel)->SetSuccessor(dog);

  /**
   * The client should be able to send a request to any handler, not just the
   * first one in the chain.
   */
  std::cout << "Chain: Monkey > Squirrel > Dog\n\n";
  ClientCode(monkey);
  std::cout << "\n";
  std::cout << "Subchain: Squirrel > Dog\n\n";
  ClientCode(squirrel);
  return 0;
}