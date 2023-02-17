#include "bit_rubik.hpp"
//F red
//R yellow
//U blue
//B orange
//L white
//D green
int main(){
  rubik::BitRubik solver;

  std::cout << solver << std::endl;

  solver.rotateFace(rubik::F, true);

  std::cout << solver << std::endl;

  solver.rotateFace(rubik::R, false);

  std::cout << solver << std::endl;

  solver.rotateFace(rubik::B, true);

  std::cout << solver << std::endl;

  solver.rotateFace(rubik::D, true);

  std::cout << solver << std::endl;

  solver.rotateFace(rubik::U, false);

  std::cout << solver << std::endl;
  return 0;
}