#include "bit_rubik.hpp"

#include <algorithm>
#include <cmath>

rubik::BitRubik::BitRubik(){
  for (int f = 0; f < 6; ++f){
    faces3x3[f] = new FaceData(FaceId(f));
  }
  faces3x3[F]->setUp(faces3x3[U], faces3x3[R], faces3x3[D], faces3x3[L]);
  faces3x3[B]->setUp(faces3x3[U], faces3x3[L], faces3x3[D], faces3x3[R]);
  faces3x3[L]->setUp(faces3x3[U], faces3x3[F], faces3x3[D], faces3x3[B]);
  faces3x3[R]->setUp(faces3x3[U], faces3x3[B], faces3x3[D], faces3x3[F]);
  faces3x3[U]->setUp(faces3x3[B], faces3x3[R], faces3x3[F], faces3x3[L]);
  faces3x3[D]->setUp(faces3x3[F], faces3x3[R], faces3x3[B], faces3x3[L]);
}

rubik::BitRubik::~BitRubik(){
  for (int f = 0; f < 6; ++f){
    delete faces3x3[f];
  }
}

rubik::BitRubik::FaceData::FaceData(FaceId face_id){
  id = face_id;
  char fill_e = mov_coding[id];
  for (int i = 0; i < 3; ++i){
    for (int j = 0; j < 3; ++j){
      m[i][j] = fill_e;
    }
  }
}

void rubik::BitRubik::FaceData::setUp(FaceData* up, FaceData* right, FaceData* down, FaceData* left){
  nb[0] = up;
  nb[1] = right;
  nb[2] = down;
  nb[3] = left;
}

template<unsigned int N>
void transposeNxN(char mat[N][N]){
  for (int i = 0; i < N; ++i) {
    for (int j = i; j < N; ++j)
      std::swap(mat[i][j], mat[j][i]);
  }
}

template<unsigned int N>
void reverseRowsNxN(char mat[N][N]){
  for (int i = 0; i < N; ++i)
    std::reverse(mat[i], mat[i] + N);
}

template<unsigned int N>
void clockwiseRotNxN(char mat[N][N]){
  transposeNxN<N>(mat);
  reverseRowsNxN<N>(mat);
}

template<unsigned int N>
void anticlockwiseRotNxN(char mat[N][N]){
  reverseRowsNxN<N>(mat);
  transposeNxN<N>(mat);
}

void rubik::BitRubik::FaceData::rotate(bool clockwise){
  //Fix orientation to current face
  if (id == FaceId::R){
    clockwiseRotNxN<3>(nb[0]->m);
    anticlockwiseRotNxN<3>(nb[2]->m);
  }
  else if (id == FaceId::L){
    anticlockwiseRotNxN<3>(nb[0]->m);
    clockwiseRotNxN<3>(nb[2]->m);
  }
  else if (id == FaceId::B){
    clockwiseRotNxN<3>(nb[0]->m); clockwiseRotNxN<3>(nb[0]->m);
    clockwiseRotNxN<3>(nb[2]->m); clockwiseRotNxN<3>(nb[2]->m);
  }
  else if (id == FaceId::D){
    anticlockwiseRotNxN<3>(nb[3]->m);
    clockwiseRotNxN<3>(nb[1]->m);
    clockwiseRotNxN<3>(nb[2]->m); clockwiseRotNxN<3>(nb[2]->m);
  }
  else if (id == FaceId::U){
    clockwiseRotNxN<3>(nb[3]->m);
    anticlockwiseRotNxN<3>(nb[1]->m);
    clockwiseRotNxN<3>(nb[0]->m); clockwiseRotNxN<3>(nb[0]->m);
  }
  //Rotation matrix 5x5
  char gquad5x5[5][5] = {{0, nb[0]->m[2][0], nb[0]->m[2][1], nb[0]->m[2][2], 0},
                         {nb[3]->m[0][2], m[0][0], m[0][1], m[0][2], nb[1]->m[0][0]},
                         {nb[3]->m[1][2], m[1][0], m[1][1], m[1][2], nb[1]->m[1][0]},
                         {nb[3]->m[2][2], m[2][0], m[2][1], m[2][2], nb[1]->m[2][0]},
                         {0, nb[2]->m[0][0], nb[2]->m[0][1], nb[2]->m[0][2], 0}};
  //Rotation cases
  if (clockwise) {//upper
    transposeNxN<5>(gquad5x5);
    reverseRowsNxN<5>(gquad5x5);
  } 
  else {//lower
    reverseRowsNxN<5>(gquad5x5);
    transposeNxN<5>(gquad5x5);
  }
  //Write changes
  //self
  m[0][0] = gquad5x5[1][1], m[0][1] = gquad5x5[1][2], m[0][2] = gquad5x5[1][3];
  m[1][0] = gquad5x5[2][1], m[1][1] = gquad5x5[2][2], m[1][2] = gquad5x5[2][3];
  m[2][0] = gquad5x5[3][1], m[2][1] = gquad5x5[3][2], m[2][2] = gquad5x5[3][3];
  //up
  nb[0]->m[2][0] = gquad5x5[0][1];
  nb[0]->m[2][1] = gquad5x5[0][2];
  nb[0]->m[2][2] = gquad5x5[0][3];
  //right
  nb[1]->m[0][0] = gquad5x5[1][4];
  nb[1]->m[1][0] = gquad5x5[2][4];
  nb[1]->m[2][0] = gquad5x5[3][4];
  //down
  nb[2]->m[0][0] = gquad5x5[4][1];
  nb[2]->m[0][1] = gquad5x5[4][2];
  nb[2]->m[0][2] = gquad5x5[4][3];
  //left
  nb[3]->m[0][2] = gquad5x5[1][0];
  nb[3]->m[1][2] = gquad5x5[2][0];
  nb[3]->m[2][2] = gquad5x5[3][0];

  //Fix orientation to original
  if (id == FaceId::R){
    anticlockwiseRotNxN<3>(nb[0]->m);
    clockwiseRotNxN<3>(nb[2]->m);
  }
  else if (id == FaceId::L){
    clockwiseRotNxN<3>(nb[0]->m);
    anticlockwiseRotNxN<3>(nb[2]->m);
  }
  else if (id == FaceId::B){
    anticlockwiseRotNxN<3>(nb[0]->m); anticlockwiseRotNxN<3>(nb[0]->m);
    anticlockwiseRotNxN<3>(nb[2]->m); anticlockwiseRotNxN<3>(nb[2]->m);
  }
  else if (id == FaceId::D){
    clockwiseRotNxN<3>(nb[3]->m);
    anticlockwiseRotNxN<3>(nb[1]->m);
    anticlockwiseRotNxN<3>(nb[2]->m); anticlockwiseRotNxN<3>(nb[2]->m);
  }
  else if (id == FaceId::U){
    anticlockwiseRotNxN<3>(nb[3]->m);
    clockwiseRotNxN<3>(nb[1]->m);
    anticlockwiseRotNxN<3>(nb[0]->m); anticlockwiseRotNxN<3>(nb[0]->m);
  }
}

bool rubik::BitRubik::FaceData::areSame(){
  for (int i = 0; i < 3; ++i){
    for (int j = 0; j < 3; ++j){
      if (m[i][j] != mov_coding[id]){
        return false;
      }
    }
  }
  return true;
}

std::string rubik::BitRubik::getString(FaceId face_id) const{
  std::string m_s;
  for (int i = 0; i < 3; ++i){
    for (int j = 0; j < 3; ++j){
      m_s.push_back(faces3x3[face_id]->m[i][j]);
    }
    m_s.append(" \n");
  }
  return m_s;
}

bool rubik::BitRubik::isSolved(){
  for (int i = 0; i < 6; ++i){
    if (!faces3x3[i]->areSame()){
      return false;
    }
  }
  return true;
}

void rubik::BitRubik::rotateFace(FaceId mov_id, bool clockwise){
  faces3x3[mov_id]->rotate(clockwise);
}

void rubik::BitRubik::rotateFace(const char& mov_code){
  char lower = tolower(mov_code);
  bool clockwise_rot = isupper(mov_code);
  if (lower == 'f'){
    rotateFace(F, clockwise_rot);
  }
  else if (lower == 'b'){
    rotateFace(B, clockwise_rot);
  }
  else if (lower == 'l'){
    rotateFace(L, clockwise_rot);
  }
  else if (lower == 'r'){
    rotateFace(R, clockwise_rot);
  }
  else if (lower == 'u'){
    rotateFace(U, clockwise_rot);
  }
  else if (lower == 'd'){
    rotateFace(D, clockwise_rot);
  }
}

std::string concatFaceString(const std::string& first, const std::string& second){
  std::string concatenated;
  int fll = first.length() / 3 - 1;
  int sll = second.length() / 3 - 1;
  concatenated.append(first.substr(0, fll) + second.substr(0, sll) + "\n");
  concatenated.append(first.substr((fll + 1), fll) + second.substr((sll + 1), sll) + "\n");
  concatenated.append(first.substr((fll + 1) * 2, fll) + second.substr((sll + 1) * 2 , sll) + "\n");
  return concatenated;
}

std::ostream& operator<<(std::ostream& os, const rubik::BitRubik& solver){
  std::string void_space = "    \n    \n    \n";
  os << concatFaceString(void_space, solver.getString(rubik::U));
  os << concatFaceString(solver.getString(rubik::L), concatFaceString(solver.getString(rubik::F), concatFaceString(solver.getString(rubik::R), solver.getString(rubik::B))));
  os << concatFaceString(void_space, solver.getString(rubik::D));
  return os;
}