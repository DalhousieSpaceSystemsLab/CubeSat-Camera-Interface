#include "CubeSatCamera.hpp"
#include "CameraParameters.hpp"
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
      camera.init();
      testParams = {};
      // Code here will be called immediately after the constructor (right
      // before each test).
    }

    void TearDown() override {
      camera.release();
      // Code here will be called immediately after each test (right
      // before the destructor).
    }
    CubeSatCamera camera;
    cameraParams_t testParams;
    // Objects declared here can be used by all tests in the test case for Foo.
  };

  TEST_F(CubeSatBasic, BasicTestConstruct) {
    camera.release();
    CubeSatCamera camera2;
    long startTime = CURRENT_TIME;
    camera2.init();
    long runTime = CURRENT_TIME - startTime;
    camera2.release();
    EXPECT_LT(runTime, 1000);
  }

  TEST_F(CubeSatBasic, DoesCustName) {
    vector<std::string> vect; 
    vect.push_back("-n");
    vect.push_back("test");

    EXPECT_NO_THROW({
      camera.parseParams(vect, &testParams);
      camera.grab(&testParams);
    });
    EXPECT_EQ(testParams.fileName, "test");
  }

  TEST_F(CubeSatBasic, DoesCustFormat) {
    vector<std::string> vect;
    vect.push_back("-c");
    vect.push_back("png");
    EXPECT_NO_THROW({
      camera.parseParams(vect, &testParams);
      camera.grab(&testParams);
    });
    EXPECT_EQ(testParams.compression, "png");
  }

  TEST_F(CubeSatBasic, DoesCustImgQ) {
    vector<std::string> vect; 
    vect.push_back("-iq");
    vect.push_back("50");
    EXPECT_NO_THROW({
      camera.parseParams(vect, &testParams);
      camera.grab(&testParams);
    });
    EXPECT_EQ(testParams.quality, 50);
  }

  TEST_F(CubeSatBasic, CameraParamsTest) {
    testParams.compression = "sr";
    testParams.fileName = "test";
    testParams.quality = 30;

    cameraParams_t test;
    vector<std::string> vect;
    vect.push_back("-iq");
    vect.push_back("30");
    vect.push_back("-n");
    vect.push_back("test");
    vect.push_back("-c");
    vect.push_back("sr");
    
    camera.parseParams(vect, &test);

    testParams.filePath = test.filePath;
    testParams.date = test.date;
    EXPECT_EQ(test, testParams);
  }

  TEST_F(CubeSatBasic, Camera0Time) {
    vector<std::string> vect;
    camera.parseParams(vect, &testParams);
    for (int i = 0; i < 5; i++) {
      long startTime = CURRENT_TIME;
      camera.grab(&testParams);
      long runTime = CURRENT_TIME - startTime;
      EXPECT_LT(runTime, 100);
    }
  }

  TEST_F(CubeSatBasic, EmptyParams) {
    vector<string> vect;
    EXPECT_NO_THROW({
      camera.parseParams(vect, &testParams);
      camera.grab(&testParams);
    });
  }

  //TEST_F(CubeSatTest, DoesQ) {

}  // namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
