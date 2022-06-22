// Intent: Allow an object to alter its behavior when its internal state
// changes. The object will appear to change its class.

#include <iostream>
#include <typeinfo>

#include "type.h"

class Context;

class State {
 protected:
  Context *context_;

 public:
  virtual ~State() {}

  void set_context(Context *context) { context_ = context; }

  virtual void Handle1() = 0;
  virtual void Handle2() = 0;
};

// define the interface of interest to clients
class Context {
 private:
  State *state_;

 public:
  Context(State *state) : state_(nullptr) { TransitionTo(state); }
  ~Context() { delete state_; }

  void TransitionTo(State *state) {
    std::cout << "Context: Transition to " << name(state) << ".\n";
    if (state_ != nullptr) delete state_;
    state_ = state;
    state_->set_context(this);
  }

  void Request1() { state_->Handle1(); }
  void Request2() { state_->Handle2(); }
};

// each subclass implements a behavior associated with a state of the Contex.
class ConcreteStateA : public State {
 public:
  void Handle1() override;

  void Handle2() override { std::cout << "ConcreteStateA handles request2.\n"; }
};

class ConcreteStateB : public State {
 public:
  void Handle1() override { std::cout << "ConcreteStateB handles request1.\n"; }
  void Handle2() override {
    std::cout << "ConcreteStateB handles request2.\n";
    std::cout << "ConcreteStateB wants to change the state of the context.\n";
    context_->TransitionTo(new ConcreteStateA);
  }
};

void ConcreteStateA::Handle1() {
  {
    std::cout << "ConcreteStateA handles request1.\n";
    std::cout << "ConcreteStateA wants to change the state of the context.\n";

    context_->TransitionTo(new ConcreteStateB);
  }
}

void ClientCode() {
  Context *context = new Context(new ConcreteStateA);
  context->Request1();
  context->Request2();
  delete context;
}

int main() {
  ClientCode();
  return 0;
}