#define BOOST_TEST_MODULE My Test

#include <boost/test/included/unit_test.hpp>


#include <dist_vec_ret.hpp>
#include <type_traits>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>


BOOST_AUTO_TEST_CASE(dist_vec_ret_init) {
    information_retrieval::word_counter index1("This is a test query");
    BOOST_CHECK(!index1.isindexed());
    index1.update_index();
    BOOST_CHECK(index1.isindexed());

}

BOOST_AUTO_TEST_CASE(simple_query_test) {
    information_retrieval::word_counter index1("This is a tasty Test query for tEst purposes");
    BOOST_WARN_EXCEPTION(index1.get_index(), std::logic_error, [](const std::exception &e) { return true; });
    index1.update_index();
    auto index = index1.get_index();
    BOOST_CHECK_EQUAL(index->at("test"), 2);
    BOOST_CHECK_EQUAL(index->at("tasty"), 1);

}

struct weight_fixture {
    weight_fixture() : weight_fixture("This is a tasty test query for test purposes, it is.") {

    }

    weight_fixture(std::string query) : query(std::move(query)) {
        information_retrieval::word_counter index1(this->query);
        index1.update_index();
        this->word_index = index1.get_index();
    }

    const std::string query;
    decltype(std::declval<information_retrieval::word_counter>().get_index()) word_index;
};

BOOST_AUTO_TEST_CASE(local_weight) {
    weight_fixture wf;
    auto global_weight = std::make_shared<information_retrieval::global_weight_state_t>();
    information_retrieval::weighter w{global_weight, wf.word_index};
    information_retrieval::weight_index_t local_weights;
    w.local_weighting(local_weights);
    BOOST_CHECK_EQUAL(local_weights["test"], 2.0 / 11.0);
    BOOST_CHECK_EQUAL(local_weights["woot"], 0);
    BOOST_CHECK_EQUAL(local_weights["tasty"], 1.0 / 11.0);
}

BOOST_AUTO_TEST_CASE(global_weight) {
    weight_fixture index1{}, index2{"This is a total different test query"};
    using namespace information_retrieval;
    auto global_weight_state_1 = std::make_shared<information_retrieval::global_weight_state_t>();

    boost::uuids::uuid uuid1 = boost::uuids::random_generator()(), uuid2 = boost::uuids::random_generator()();
    global_weight_state_1->update_document(uuid1, *index1.word_index);
    global_weight_state_1->update_document(uuid2, *index2.word_index);

    weight_index_t global_weight1, global_weight2;
    weighter w1{global_weight_state_1, index1.word_index}, w2{global_weight_state_1, index2.word_index};
    w1.global_weighting(global_weight1);
    w2.global_weighting(global_weight2);


    BOOST_CHECK_EQUAL(global_weight1["test"], std::log(2.0 / 2.0));
    BOOST_CHECK_EQUAL(global_weight1["test"], global_weight2["test"]);
    BOOST_CHECK_EQUAL(global_weight2["different"], std::log(2.0 / 1.0));

}

BOOST_AUTO_TEST_CASE(global_weight_delete_doc) {
    weight_fixture index1{}, index2{"This is a total different test query"};
    using namespace information_retrieval;
    auto global_weight_state_1 = std::make_shared<information_retrieval::global_weight_state_t>();

    boost::uuids::uuid uuid1 = boost::uuids::random_generator()(), uuid2 = boost::uuids::random_generator()();
    global_weight_state_1->update_document(uuid1, *index1.word_index);
    global_weight_state_1->update_document(uuid1, *index1.word_index);
    global_weight_state_1->update_document(uuid2, *index2.word_index);

    {
        weight_index_t global_weight1, global_weight2;
        weighter w1{global_weight_state_1, index1.word_index}, w2{global_weight_state_1, index2.word_index};
        w1.global_weighting(global_weight1);
        w2.global_weighting(global_weight2);


        BOOST_CHECK_EQUAL(global_weight1["test"], std::log(2.0 / 2.0));
        BOOST_CHECK_EQUAL(global_weight1["test"], global_weight2["test"]);
        BOOST_CHECK_EQUAL(global_weight2["different"], std::log(2.0 / 1.0));
    }

    global_weight_state_1->remove_document(uuid2);

    {
        weight_index_t global_weight1, global_weight2;
        weighter w1{global_weight_state_1, index1.word_index}, w2{global_weight_state_1, index2.word_index};
        w1.global_weighting(global_weight1);
        w2.global_weighting(global_weight2);


        BOOST_CHECK_EQUAL(global_weight1["test"], std::log(1.0 / 1.0));
        BOOST_CHECK_EQUAL(global_weight1["test"], global_weight2["test"]);
        BOOST_CHECK_EQUAL(global_weight2["different"], 0);
    }
}

struct weight_fixture2 {
    weight_fixture2() : weight_fixture2(
            "This is a tasty test query for test purposes, it is.",
            "This is a total different test query"
    ) {}

    weight_fixture2(std::string string1, std::string string2,
                    std::shared_ptr<information_retrieval::global_weight_state_t> shared_weight_state
                    = std::make_shared<information_retrieval::global_weight_state_t>()) {
        using namespace information_retrieval;

        word_counter index1{string1}, index2{string2};
        index1.update_index();
        index2.update_index();


        boost::uuids::uuid uuid1 = boost::uuids::random_generator()(), uuid2 = boost::uuids::random_generator()();
        shared_weight_state->update_document(uuid1, *index1.get_index());
        shared_weight_state->update_document(uuid2, *index2.get_index());


        w1.reset(new information_retrieval::weighter{shared_weight_state, index1.get_index()});
        w2.reset(new information_retrieval::weighter{shared_weight_state, index2.get_index()});

    }

    std::shared_ptr<information_retrieval::weight_index_t> weight1() {
        return w1->get_weight();
    }

    std::shared_ptr<information_retrieval::weight_index_t> weight2() {
        return w2->get_weight();
    }

private:
    std::unique_ptr<information_retrieval::weighter> w1, w2;

};

BOOST_AUTO_TEST_CASE(final_weight) {
    using namespace information_retrieval;
    weight_fixture2 wf{};
    {
        weighter empty_weighter{std::shared_ptr<global_weight_state_t>{}, std::shared_ptr<count_index_t>{}};
        BOOST_CHECK_EQUAL(empty_weighter.ischached(), false);
    }
    BOOST_CHECK_CLOSE(wf.weight1()->at("test"), 0.0, 0.1);
    BOOST_CHECK_CLOSE(wf.weight1()->at("for"), 0.063, 1);
}

BOOST_AUTO_TEST_CASE(distance) {
    using namespace information_retrieval;
    weight_fixture2 wf{};
    BOOST_CHECK_CLOSE(calc_distance(*wf.weight1(), *wf.weight1()), 1, 1);

    weight_fixture2 wf2{"These strings", "have nothing in common"};
    BOOST_CHECK_CLOSE(calc_distance(*wf2.weight1(), *wf2.weight2()), 0, 1);

    auto shared_state_3_4 = std::make_shared<information_retrieval::global_weight_state_t>();
    weight_fixture2 wf3{"One word common", "in this one this one example", shared_state_3_4};
    weight_fixture2 wf4{"but", "not here", shared_state_3_4};
    BOOST_CHECK_CLOSE(calc_distance(*wf3.weight1(), *wf3.weight2()), 0.1259, 1);

}


