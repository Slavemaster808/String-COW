#include <libcowstring/cowstring.hpp>

#include <gtest/gtest.h>
#include <iostream>
#include <string>

TEST(CowStringTest, CSTRConstructorTest) {
  const char* str = "test";
  const cowstring::CowString cowstr(str);
  //   std::cout << cowstr.c_str() << std::endl;
  EXPECT_EQ(cowstr.size(), 4);
}

TEST(CowStringTest, CSTRTest) {
  const char* str = "test";
  const cowstring::CowString cowstr(str);
  EXPECT_EQ(std::string(cowstr.c_str()), str);
}

TEST(CowStringTest, CntOwnersTest1) {
  const cowstring::CowString cowstr1("test");
  const cowstring::CowString cowstr2(cowstr1);
  EXPECT_EQ(cowstr1.use_cnt(), 2);
}

TEST(CowStringTest, CntOwnersTest2) {
  const cowstring::CowString cowstr1("test");
  cowstring::CowString cowstr2(cowstr1);
  cowstr2[0] = 'l';
  EXPECT_EQ(cowstr1.use_cnt(), 1);
}

TEST(CowStringTest, CntOwnersTest3) {
  const cowstring::CowString cowstr1("test");
  cowstring::CowString cowstr2(cowstr1);
  cowstr2[0] = 'l';
  EXPECT_EQ(cowstr1.use_cnt(), cowstr2.use_cnt());
}

TEST(CowStringTest, CntOwnersTest4) {
  const cowstring::CowString cowstr1("test");
  const cowstring::CowString cowstr2(cowstr1);
  [[maybe_unused]] const char ch = cowstr1[0];
  EXPECT_EQ(cowstr1.use_cnt(), 2);
}

TEST(CowStringTest, AtTest1) {
  cowstring::CowString cowstr("test");
  cowstr.at(0) = 'l';
  EXPECT_EQ(cowstr.c_str(), std::string("lest"));
}

TEST(CowStringTest, AtTest2) {
  cowstring::CowString cowstr("test");
  cowstr.at(0) = 'l';
  EXPECT_EQ(cowstr.use_cnt(), 1);
}

TEST(CowStringTest, CapTest1) {
  cowstring::CowString cowstr("test");
  cowstr.reserve(10);
  EXPECT_EQ(cowstr.capacity(), 10);
}

TEST(CowStringTest, CapTest2) {
  const cowstring::CowString cowstr("test");
  EXPECT_EQ(cowstr.capacity(), 8);
}

TEST(CowStringTest, ResizeTest1) {
  cowstring::CowString cowstr("test");
  cowstr.resize(10);
  EXPECT_EQ(cowstr.size(), 10);
}

TEST(CowStringTest, ResizeTest2) {
  cowstring::CowString cowstr("test");
  cowstr.resize(10, 's');
  EXPECT_EQ(cowstr.c_str(), std::string("testssssss"));
}

TEST(CowStringTest, ResizeTest3) {
  cowstring::CowString cowstr("test");
  cowstr.resize(3);
  EXPECT_EQ(cowstr.size(), 3);
}

TEST(CowStringTest, ResizeTest4) {
  cowstring::CowString cowstr("test");
  cowstr.resize(3);
  EXPECT_EQ(cowstr.c_str(), std::string("tes"));
}

TEST(CowStringTest, ShrinkToFitTest) {
  cowstring::CowString cowstr("test");
  cowstr.reserve(10);
  cowstr.shrinkToFit();
  EXPECT_EQ(cowstr.size(), 4);
}

TEST(CowStringTest, PushBackTest1) {
  cowstring::CowString cowstr("test");
  cowstr.pushBack('l');
  std::cout << cowstr.c_str() << std::endl;
  EXPECT_EQ(cowstr.c_str(), std::string("testl"));
}

TEST(CowStringTest, PushBackTest2) {
  cowstring::CowString cowstr("test");
  cowstr.pushBack('l');
  EXPECT_EQ(cowstr.capacity(), 8);
}

TEST(CowStringTest, PushBackTest3) {
  cowstring::CowString cowstr("te");
  cowstr.pushBack('s');
  cowstr.pushBack('s');
  cowstr.pushBack('s');
  // std::cout << cowstr.capacity() << std::endl;
  EXPECT_EQ(cowstr.capacity(), 6);
}

TEST(CowStringTest, STLTest1) {
  {
    cowstring::CowString cowstr("test");
    const auto it = std::find(cowstr.begin(), cowstr.end(), 't');
    EXPECT_EQ(*it, 't');
  }

  {
    cowstring::CowString cowstr("test");
    std::reverse(cowstr.begin(), cowstr.end());
    EXPECT_EQ(cowstr.c_str(), std::string("tset"));
  }

  {
    cowstring::CowString cowstr("test");
    auto it = std::remove(cowstr.begin(), cowstr.end(), 'e');
    cowstr.erase(it, cowstr.end());
    EXPECT_EQ(cowstr.c_str(), std::string("tst"));
  }
}

TEST(CowStringTest, ExprTest) {
  {
    cowstring::CowString cowstr("test");
    auto it = cowstr.begin();
    it += 2;
    EXPECT_EQ(*it, 's');
  }

  {
    cowstring::CowString cowstr("test");
    auto it = cowstr.begin() + 2;
    it -= 2;
    EXPECT_EQ(*it, 't');
  }

  {
    cowstring::CowString cowstr("test");
    auto it1 = cowstr.begin();
    auto it2 = cowstr.end() - 1;
    auto res = it2 - it1;
    EXPECT_EQ(res, 3);
  }
}

TEST(CowStringTest, EraseTest) {
  {
    cowstring::CowString cowstr("test");
    cowstr.erase(0, 1);
    EXPECT_EQ(cowstr.c_str(), std::string("est"));
  }

  {
    cowstring::CowString cowstr("test");
    cowstr.erase(0, 2);
    EXPECT_EQ(cowstr.c_str(), std::string("st"));
  }

  {
    cowstring::CowString cowstr("test");
    auto it = cowstr.begin();
    cowstr.erase(it);
    EXPECT_EQ(cowstr.c_str(), std::string("est"));
  }

  {
    cowstring::CowString cowstr("test");
    auto it = cowstr.end() - 1;
    cowstr.erase(it);
    EXPECT_EQ(cowstr.c_str(), std::string("tes"));
  }

  {
    cowstring::CowString cowstr("test");
    cowstr.erase(cowstr.begin(), cowstr.end() - 1);
    EXPECT_EQ(cowstr.c_str(), std::string("t"));
  }
}

TEST(CowStringTest, InsertTest1) {
  cowstring::CowString cowstr("test");
  cowstr.insert(0, 2, 'l');
  EXPECT_EQ(cowstr.c_str(), std::string("lltest"));
}

TEST(CowStringTest, InsertTest2) {
  cowstring::CowString cowstr("test");
  cowstr.insert(cowstr.begin(), 'l');
  EXPECT_EQ(cowstr.c_str(), std::string("ltest"));
}

TEST(CowStringTest, InsertTest3) {
  const cowstring::CowString cowstr("test");
  cowstring::CowString cowstr2(cowstr);
  cowstr2.insert(cowstr2.begin(), 'l');
  EXPECT_EQ(cowstr2.use_cnt(), 1);
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}