#include "test/Output.hpp"
#include "test/Main.hpp"

using namespace std;
using namespace test;


void printAtStart(const std::string &sMessage)
{
  std::string sOut(sMessage);
  sOut.append(" by ");
  sOut.append(STUDENT_ID);
  test::print(sOut);
}

void printAtEnd(const std::string &sFileName)
{
  std::string sM("The output is written to the file ");
  sM += sFileName;
  print(sM);
}

std::string fileName(const std::string &sDir1, const std::string &sDir2, const std::string &sFile)
{
  std::string a(sDir1);
  a += std::string("/");
  a += sDir2;
  a += std::string("/");
  a += sFile;
  a += std::string(".txt");
  return a;
}

void test::project(const std::function<void()> &rF, const std::string &sProjectDir,
                   const std::string &sFileName, const std::string &sTitle)
{
  std::string sFile = fileName(OUTPUT_DIR, sProjectDir, sFileName);
  std::ofstream fOut(sFile.c_str());
  std::streambuf *strmBuffer = std::cout.rdbuf();
  std::cout.rdbuf(fOut.rdbuf());
  printAtStart(sTitle);
  rF();
  std::cout.rdbuf(strmBuffer);
  printAtEnd(sFile);
}

