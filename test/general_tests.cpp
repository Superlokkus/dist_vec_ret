#define BOOST_TEST_MODULE My Test 
#include <boost/test/included/unit_test.hpp>

#include <dist_vec_ret.hpp>



BOOST_AUTO_TEST_CASE(dist_vec_ret_init){
    information_retrieval::word_counter index1("This is a test query");
    BOOST_CHECK(!index1.isindexed());
    index1.update_index();
    BOOST_CHECK(index1.isindexed());

}

BOOST_AUTO_TEST_CASE(simple_query_test){
    information_retrieval::word_counter index1("This is a tasty Test query for tEst purposes");
    BOOST_WARN_EXCEPTION(index1.get_index(),std::logic_error,[](const std::exception &e){return true;});
    index1.update_index();
    auto index = index1.get_index();
    BOOST_CHECK_EQUAL(index.at("test"),2);
    BOOST_CHECK_EQUAL(index.at("tasty"),1);

}




