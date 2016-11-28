#define BOOST_TEST_MODULE My Test 
#include <boost/test/included/unit_test.hpp>

#include <dist_vec_ret.hpp>



BOOST_AUTO_TEST_CASE(dist_vec_ret_init){
    information_retrieval::indexer index1("This is a test query");
    BOOST_CHECK(!index1.isindexed());
    index1.index();
    BOOST_CHECK(index1.isindexed());

}



