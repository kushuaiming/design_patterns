// Intent: Without violating encapsulation, capture and externalize an object's
// internal state so that the object can be restored to this state later.

#include <ctime>
#include <iostream>
#include <string>
#include <vector>

namespace {
static constexpr char alphanum[] =
    "0123456789"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz";
}

// stores internal state of the Originator object.
// protects against access by objects other than the originator.
class Memento {
 public:
  Memento(std::string state) : state_(state) {
    std::time_t now = std::time(0);
    date_ = std::ctime(&now);
  }

  std::string GetName() const {
    return date_ + " / (" + state_.substr(0, 9) + "...)";
  }

  std::string state() const { return state_; }
  std::string date() const { return date_; }

 private:
  std::string state_;
  std::string date_;
};

// creates a memento containing a snapshot of its current internal state.
// uses the memento to restore its internal state.
class Originator {
 private:
  std::string state_;

  std::string GenerateRandomString(int length = 10) {
    int stringLength = sizeof(alphanum) - 1;

    std::string random_string;
    for (int i = 0; i < length; i++) {
      random_string += alphanum[std::rand() % stringLength];
    }
    return random_string;
  }

 public:
  Originator(std::string state) : state_(state) {
    std::cout << "Originator: My initial state is: " << state_ << "\n";
  }

  void DoSomething() {
    std::cout << "Originator: I'm doing something important.\n";
    state_ = GenerateRandomString(30);
    std::cout << "Originator: and my state has changed to: " << state_ << "\n";
  }

  Memento *Save() { return new Memento(state_); }

  void Restore(Memento *memento) {
    state_ = memento->state();
    std::cout << "Originator: My state has changed to: " << state_ << "\n";
  }
};

// responsible for the memento's safekeeping.
// never operates on or examines the contents of a memento.
class Caretaker {
 private:
  std::vector<Memento *> mementos_;

  Originator *originator_;

 public:
  Caretaker(Originator *originator) : originator_(originator) {
    originator_ = originator;
  }

  void Backup() {
    std::cout << "\nCaretaker: Saving Originator's state...\n";
    mementos_.push_back(originator_->Save());
  }
  void Undo() {
    if (!mementos_.size()) {
      return;
    }
    Memento *memento = mementos_.back();
    mementos_.pop_back();
    std::cout << "Caretaker: Restoring state to: " << memento->GetName()
              << "\n";
    try {
      originator_->Restore(memento);
    } catch (...) {
      Undo();
    }
  }
  void ShowHistory() const {
    std::cout << "Caretaker: Here's the list of mementos:\n";
    for (Memento *memento : mementos_) {
      std::cout << memento->GetName() << "\n";
    }
  }
};

void ClientCode() {
  Originator *originator = new Originator("Super-duper-super-puper-super.");
  Caretaker *caretaker = new Caretaker(originator);
  caretaker->Backup();
  originator->DoSomething();
  caretaker->Backup();
  originator->DoSomething();
  caretaker->Backup();
  originator->DoSomething();
  std::cout << "\n";
  caretaker->ShowHistory();
  std::cout << "\nClient: Now, let's rollback!\n\n";
  caretaker->Undo();
  std::cout << "\nClient: Once more!\n\n";
  caretaker->Undo();

  delete originator;
  delete caretaker;
}

int main() {
  std::srand(static_cast<unsigned int>(std::time(NULL)));
  ClientCode();
  return 0;
}