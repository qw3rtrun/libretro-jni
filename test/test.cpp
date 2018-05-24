//
// Created by qw3rtrun on 23.05.18.
//

#define BOOST_TEST_MODULE libretro-j-test

#include <boost/test/included/unit_test.hpp>

#include "../src/library.h"

BOOST_AUTO_TEST_CASE(FailTest) {
    BOOST_CHECK_EQUAL(5, f(4));
}

BOOST_AUTO_TEST_CASE(PassTest) {
    BOOST_CHECK_EQUAL(1, f(0));
}