#define BOOST_TEST_MODULE My Test 
#include <boost/test/included/unit_test.hpp> 

BOOST_AUTO_TEST_CASE(always_true)
{
  int i = 1;
  BOOST_TEST(i); 
  BOOST_TEST(i == 1); 
}

