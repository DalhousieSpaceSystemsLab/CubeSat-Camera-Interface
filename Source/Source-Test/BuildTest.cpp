#include "CubeSatCamera.hpp"
#include "gtest/gtest.h"

namespace {

// The fixture for testing class CubeSatTest
class CubeSatBasic : public ::testing::Test {
 protected:
  // You can remove any or all of the following functions if its body
  // is empty.
  CubeSatBasic() {
     // You can do set-up work for each test here.
  }

  ~CubeSatBasic() override {
     // You can do clean-up work that doesn't throw exceptions here.

  }

  // If the constructor and destructor are not enough for setting up
  // and cleaning up each test, you can define the following methods:

  void SetUp() override {
     // Code here will be called immediately after the constructor (right
     // before each test).
  }

  void TearDown() override {
     // Code here will be called immediately after each test (right
     // before the destructor).
  }

  // Objects declared here can be used by all tests in the test case for Foo.
};

TEST_F(CubeSatBasic, BasicTestConstruct) {
  //printf("HELLO\n");
  string argv[1];
  argv[0] = "CubeSatCamera";
  long startTime = CURRENT_TIME;
  CubeSatCamera camera(argv, 1);
  long runTime = CURRENT_TIME - startTime;
  EXPECT_LT(runTime, 800);
}




// Tests that Foo does Xyz.
TEST_F(CubeSatBasic, DoesNC0) {
  string argv[] = {"CubeSatCamera", "-nc0"};
  EXPECT_NO_THROW({
    CubeSatCamera camera(argv, 2);
    camera.grab(0);
  });
}

TEST_F(CubeSatBasic, DoesNC1) {
  string argv[] = {"CubeSatCamera", "-nc1"};
  EXPECT_NO_THROW({
    CubeSatCamera camera(argv, 2);
    camera.grab(0);
  });
}

TEST_F(CubeSatBasic, DoesCustName) {
  string argv[] = {"CubeSatCamera", "-n", "test"};
  EXPECT_NO_THROW({
    CubeSatCamera camera(argv, 2);
    camera.grab(0);
  });
}

TEST_F(CubeSatBasic, DoesCustFormat) {
  string argv[] = {"CubeSatCamera", "-c", "png"};
  EXPECT_NO_THROW({
    CubeSatCamera camera(argv, 2);
    camera.grab(0);
  });
}

TEST_F(CubeSatBasic, DoesCustImgQ) {
  string argv[] = {"CubeSatCamera", "-iq", "50"};
  EXPECT_NO_THROW({
    CubeSatCamera camera(argv, 2);
    camera.grab(0);
  });
}

TEST_F(CubeSatBasic, Camera0Time) {
  string argv[1];
  argv[0] = "CubeSatCamera";
  CubeSatCamera camera(argv, 1);
  for (int i = 0; i < 5; i++) {
    long startTime = CURRENT_TIME;
    camera.grab(0);
    long runTime = CURRENT_TIME - startTime;
    EXPECT_LT(runTime, 90);
  }

}

//TEST_F(CubeSatTest, DoesQ) {

}  // namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
