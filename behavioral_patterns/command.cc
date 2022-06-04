/**
 * Command Design Pattern
 *
 * Encapsulate a request as an object, thereby letting you parameterize clients
 * with different requests, queue or log requests, and support undoable
 * operations.
 */

#include <iostream>
#include <memory>
#include <string>
/**
 * The Command interface declares a method for executing a command.
 */
class Command {
 public:
  virtual ~Command() {}
  virtual void Execute() const = 0;
};
/**
 * Some commands can implement simple operations on their own.
 */
class SimpleCommand : public Command {
 private:
  std::string pay_load_;

 public:
  explicit SimpleCommand(std::string pay_load) : pay_load_(pay_load) {}
  void Execute() const override {
    std::cout << "SimpleCommand: See, I can do simple things like printing ("
              << pay_load_ << ")\n";
  }
};

/**
 * The Receiver classes contain some important business logic. They know how to
 * perform all kinds of operations, associated with carrying out a request. In
 * fact, any class may serve as a Receiver.
 */
class Receiver {
 public:
  void DoSomething(const std::string &a) {
    std::cout << "Receiver: Working on (" << a << ".)\n";
  }
  void DoSomethingElse(const std::string &b) {
    std::cout << "Receiver: Also working on (" << b << ".)\n";
  }
};

/**
 * However, some commands can delegate more complex operations to other objects,
 * called "receivers."
 */
class ComplexCommand : public Command {
  /**
   * @var Receiver
   */
 private:
  std::shared_ptr<Receiver> receiver_;
  /**
   * Context data, required for launching the receiver's methods.
   */
  std::string a_;
  std::string b_;
  /**
   * Complex commands can accept one or several receiver objects along with any
   * context data via the constructor.
   */
 public:
  ComplexCommand(std::shared_ptr<Receiver> receiver, std::string a,
                 std::string b)
      : receiver_(receiver), a_(a), b_(b) {}
  /**
   * Commands can delegate to any methods of a receiver.
   */
  void Execute() const override {
    std::cout << "ComplexCommand: Complex stuff should be done by a receiver "
                 "object.\n";
    receiver_->DoSomething(a_);
    receiver_->DoSomethingElse(b_);
  }
};

/**
 * The Invoker is associated with one or several commands. It sends a request to
 * the command.
 */
class Invoker {
  /**
   * @var Command
   */
 private:
  std::shared_ptr<Command> on_start_;
  /**
   * @var Command
   */
  std::shared_ptr<Command> on_finish_;
  /**
   * Initialize commands.
   */
 public:
  ~Invoker() {}

  void SetOnStart(std::shared_ptr<Command> command) { on_start_ = command; }
  void SetOnFinish(std::shared_ptr<Command> command) { on_finish_ = command; }
  /**
   * The Invoker does not depend on concrete command or receiver classes. The
   * Invoker passes a request to a receiver indirectly, by executing a command.
   */
  void DoSomethingImportant() {
    std::cout << "Invoker: Does anybody want something done before I begin?\n";
    if (on_start_) {
      on_start_->Execute();
    }
    std::cout << "Invoker: ...doing something really important...\n";
    std::cout << "Invoker: Does anybody want something done after I finish?\n";
    if (on_finish_) {
      on_finish_->Execute();
    }
  }
};
/**
 * The client code can parameterize an invoker with any commands.
 */

int main() {
  std::unique_ptr<Invoker> invoker(new Invoker());
  invoker->SetOnStart(std::make_shared<SimpleCommand>("Say Hi!"));
  std::shared_ptr<Receiver> receiver(new Receiver());
  invoker->SetOnFinish(
      std::make_shared<ComplexCommand>(receiver, "Send email", "Save report"));
  invoker->DoSomethingImportant();
  return 0;
}