#include <model/Model.hpp>

#include <iostream>

int main() {
  openstudio::model::Model m;
  std::cout << m << std::endl;
  return 0;
}