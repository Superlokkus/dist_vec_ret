#define BOOST_TEST_MODULE My Test 
#include <boost/test/included/unit_test.hpp>

#include <dist_vec_ret.hpp>
#include <type_traits>


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
    BOOST_CHECK_EQUAL(index->at("test"), 2);
    BOOST_CHECK_EQUAL(index->at("tasty"), 1);

}

struct weight_fixture {
    weight_fixture() : index1(query) {
        index1.update_index();
        this->word_index = index1.get_index();

    }

    const std::string query = "This is a tasty test query for test purposes, it is.";
    information_retrieval::word_counter index1;
    decltype(index1.get_index()) word_index;
};

BOOST_AUTO_TEST_CASE(local_weight_test) {
    weight_fixture wf;
    information_retrieval::global_weight_t global_weight;
    information_retrieval::weighter w{global_weight, wf.word_index};
    information_retrieval::weight_index_t local_weights;
    w.local_weighting(local_weights);
    BOOST_CHECK_EQUAL(local_weights["test"], 2.0 / 11.0);
    BOOST_CHECK_EQUAL(local_weights["woot"], 0);
    BOOST_CHECK_EQUAL(local_weights["tasty"], 1.0 / 11.0);
}



