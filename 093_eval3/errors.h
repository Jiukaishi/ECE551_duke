#ifndef ERROR_H
#define ERROR_H
#include <cstdlib>
#include <exception>
#include <iostream>
class NumberError : public std::exception {
  const char * what() { return "Not a valid size_t number"; }
};
class Step2Error : public std::exception {
  const char * what() { return "Not fulfill Step2's requirment"; }
};
class Step4Error : public std::exception {
  const char * what() { return "Step4 Error"; }
};
class TypeError : public std::exception {
  const char * what() { return "unknown page type"; }
};
class FopenError : public std::exception {
  const char * what() { return "open file failed"; }
};
class WrongVirtualError : public std::exception {
  const char * what() { return "Dynamic Dispatched Error"; }
};
class ContentError : public std::exception {
  const char * what() { return "Content not matched the template"; }
};
#endif
