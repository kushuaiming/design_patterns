// Intent: Define a one-to-many dependency between objects so that when one
// object changes state, all its dependents are notified and updated
// automatically.

#include <iostream>
#include <list>
#include <memory>
#include <string>

// defines an updating interface for objects that should be notified of changes
// in a subject
class Observer {
 public:
  virtual ~Observer(){};
  virtual void Update(const std::string &message_from_subject) = 0;
};

// konws its observers. Any number of Observer objects may observe a subject.
class Subject {
 public:
  virtual ~Subject(){};
  virtual void Attach(Observer *observer) = 0;
  virtual void Detach(Observer *observer) = 0;
  virtual void Notify() = 0;
};

class ConcreteSubject : public Subject {
 public:
  virtual ~ConcreteSubject() { std::cout << "Goodbye, I was the Subject.\n"; }

  void Attach(Observer *observer) override {
    list_observer_.push_back(observer);
  }
  void Detach(Observer *observer) override { list_observer_.remove(observer); }
  void Notify() override {
    std::list<Observer*>::iterator iterator = list_observer_.begin();
    HowManyObserver();
    while (iterator != list_observer_.end()) {
      (*iterator)->Update(message_);
      ++iterator;
    }
  }

  void CreateMessage(std::string message = "Empty") {
    message_ = message;
    Notify();
  }
  void HowManyObserver() {
    std::cout << "There are " << list_observer_.size()
              << " observers in the list.\n";
  }

  void SomeBusinessLogic() {
    message_ = "change message message";
    Notify();
    std::cout << "I'm about to do some thing important\n";
  }

 private:
  std::list<Observer*> list_observer_;
  std::string message_;
};

class ConcreteObserver : public Observer {
 public:
  ConcreteObserver(ConcreteSubject &subject) : subject_(subject) {
    subject_.Attach(this);
    std::cout << "Hi, I'm the Observer \"" << ++ConcreteObserver::static_number_
              << "\".\n";
    number_ = ConcreteObserver::static_number_;
  }
  virtual ~ConcreteObserver() {
    std::cout << "Goodbye, I was the Observer \"" << number_ << "\".\n";
  }

  void Update(const std::string &message_from_subject) override {
    message_from_subject_ = message_from_subject;
    PrintInfo();
  }
  void RemoveMeFromTheList() {
    subject_.Detach(this);
    std::cout << "Observer \"" << number_ << "\" removed from the list.\n";
  }
  void PrintInfo() {
    std::cout << "Observer \"" << number_
              << "\": a new message is available --> " << message_from_subject_
              << "\n";
  }

 private:
  std::string message_from_subject_;
  ConcreteSubject& subject_;
  static int static_number_;
  int number_;
};

int ConcreteObserver::static_number_ = 0;

void ClientCode() {
  std::unique_ptr<ConcreteSubject> subject(new ConcreteSubject);
  std::unique_ptr<ConcreteObserver> observer1(new ConcreteObserver(*subject));
  std::unique_ptr<ConcreteObserver> observer2(new ConcreteObserver(*subject));
  std::unique_ptr<ConcreteObserver> observer3(new ConcreteObserver(*subject));
  std::unique_ptr<ConcreteObserver> observer4;
  std::unique_ptr<ConcreteObserver> observer5;

  subject->CreateMessage("Hello World! :D");
  observer3->RemoveMeFromTheList();

  subject->CreateMessage("The weather is hot today! :p");
  observer4 = std::make_unique<ConcreteObserver>(*subject);

  observer2->RemoveMeFromTheList();
  observer5 = std::make_unique<ConcreteObserver>(*subject);

  subject->CreateMessage("My new car is great!)");
  observer5->RemoveMeFromTheList();

  observer4->RemoveMeFromTheList();
  observer1->RemoveMeFromTheList();
}

int main() {
  ClientCode();
  return 0;
}