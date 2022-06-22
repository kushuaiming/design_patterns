#include <cxxabi.h>

#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>
#include <typeinfo>

std::string demangle(const char* mangled) {
  int status;
  std::unique_ptr<char[], void (*)(void*)> result(
      abi::__cxa_demangle(mangled, 0, 0, &status), std::free);
  return result.get() ? std::string(result.get()) : "error occurred";
}

template <typename T>
std::string name(T t) {
  return demangle(typeid(t).name());
}

template <typename T>
std::string name(T* t) {
  return demangle(typeid(*t).name());
}