#ifndef RUBIK_INCLUDED_BIT_RUBIK_HPP_
#define RUBIK_INCLUDED_BIT_RUBIK_HPP_

#include <iostream>
#include <string>


/*{'f', 0}, {'b', 1}
  {'r', 2}, {'l', 3}
  {'u', 4}, {'d', 5}*/

/*
     uuu
    _uuu_
   0 uuu 0
ff|f lll b|bb
ff|f lll b|bb
ff|f lll b|bb
   0_ddd_0
     ddd
     ddd
*/

namespace rubik{
  class BitRubik;
}

std::ostream& operator<<(std::ostream& os, const rubik::BitRubik& solver);

namespace rubik{

  enum FaceId{
    F = 0, B = 1, R = 2, L = 3, U = 4, D = 5
  };

  const char mov_coding[6] = {'F', 'B', 'R', 'L', 'U', 'D'};

  class BitRubik{
  private:
    struct FaceData{
      FaceId id;
      char m[3][3] = {0};
      FaceData* nb[4];

      FaceData(FaceId face_id);

      void setUp(FaceData* up, FaceData* right, FaceData* down, FaceData* left);
      void rotate(bool clockwise);
      bool areSame();
    };

    FaceData* faces3x3[6];

    std::string getString(FaceId face_id) const;

  public:
    BitRubik();
    ~BitRubik();

    bool isSolved();
    void rotateFace(FaceId mov_id, bool clockwise);
    void rotateFace(const char& mov_code);

    friend std::ostream& ::operator<<(std::ostream& os, const BitRubik& solver);
  };

}

#endif