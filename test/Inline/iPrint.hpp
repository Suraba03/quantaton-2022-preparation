//do not include this file
template <class T>
void test::print(T start, T end, const std::string &rName)
{
  std::string sM(rName);
  sM += std::string(":");
  std::cout << sM.c_str() << std::endl;

  for (T i = start; i < end; i++)
    {
      std::cout << "[" << (i - start) << "]"
		<< " = " << *i << std::endl;
    }
  std::cout << std::endl;
}
