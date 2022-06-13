/**
 * Mediator Design Pattern
 *
 * Define an object that encapsulates how a set of objects interact.
 */

#include <iostream>
#include <string>
/**
 * The Mediator interface declares a method used by components to notify the
 * mediator about various events. The Mediator may react to these events and
 * pass the execution to other components.
 */
class Colleague;
class Mediator {
 public:
  virtual void Notify(Colleague *sender, std::string event) const = 0;
};

/**
 * The Colleague provides the basic functionality of storing a mediator's
 * instance inside ConcreteColleague objects.
 */
class Colleague {
 protected:
  Mediator *mediator_;

 public:
  Colleague(Mediator *mediator = nullptr) : mediator_(mediator) {}
  void set_mediator(Mediator *mediator) { mediator_ = mediator; }
};

/**
 * Concrete Colleague implement various functionality. They don't depend on
 * other Colleagues. They also don't depend on any concrete mediator classes.
 */
class ConcreteColleague1 : public Colleague {
 public:
  void DoA() {
    std::cout << "Colleague 1 does A.\n";
    mediator_->Notify(this, "A");
  }
  void DoB() {
    std::cout << "Colleague 1 does B.\n";
    mediator_->Notify(this, "B");
  }
};

class ConcreteColleague2 : public Colleague {
 public:
  void DoC() {
    std::cout << "Colleague 2 does C.\n";
    mediator_->Notify(this, "C");
  }
  void DoD() {
    std::cout << "Colleague 2 does D.\n";
    mediator_->Notify(this, "D");
  }
};

/**
 * Concrete Mediators implement cooperative behavior by coordinating several
 * colleague.
 */
class ConcreteMediator : public Mediator {
 private:
  ConcreteColleague1 *colleague1_;
  ConcreteColleague2 *colleague2_;

 public:
  ConcreteMediator(ConcreteColleague1 *c1, ConcreteColleague2 *c2)
      : colleague1_(c1), colleague2_(c2) {
    colleague1_->set_mediator(this);
    colleague2_->set_mediator(this);
  }
  void Notify(Colleague *sender, std::string event) const override {
    if (event == "A") {
      std::cout << "Mediator reacts on A and triggers following operations:\n";
      colleague2_->DoC();
    }
    if (event == "D") {
      std::cout << "Mediator reacts on D and triggers following operations:\n";
      colleague1_->DoB();
      colleague2_->DoC();
    }
  }
};

void ClientCode() {
  ConcreteColleague1 *c1 = new ConcreteColleague1;
  ConcreteColleague2 *c2 = new ConcreteColleague2;
  ConcreteMediator *mediator = new ConcreteMediator(c1, c2);
  std::cout << "Client triggers operation A.\n";
  c1->DoA();
  std::cout << "\n";
  std::cout << "Client triggers operation D.\n";
  c2->DoD();

  delete c1;
  delete c2;
  delete mediator;
}

int main() {
  ClientCode();
  return 0;
}