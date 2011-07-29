#define BOOST_TEST_MODULE sync_access_test
#include <boost/test/unit_test.hpp>

#include <boost/thread.hpp>
#include <boost/thread/barrier.hpp>
#include <boost/bind.hpp>
#include <boost/ref.hpp>


using namespace riakboost;
namespace ut = riakboost::unit_test;

static riakboost::mutex m;

/// thread execution function
static void thread_function(riakboost::barrier& b)
{
    b.wait(); /// wait until memory barrier allows the execution
    riakboost::mutex::scoped_lock lock(m); /// lock mutex
    BOOST_CHECK_EQUAL(1,0); /// produce the fault
}

BOOST_AUTO_TEST_CASE_EXPECTED_FAILURES( test_multiple_assertion_faults, 100 )

/// test function which creates threads
BOOST_AUTO_TEST_CASE( test_multiple_assertion_faults )
{
    riakboost::thread_group tg;      // thread group to manage all threads
    riakboost::barrier      b(100);  // memory barrier, which should block all threads
                                 // until all 100 threads were created

    for(size_t i=0; i<100; ++i)
        tg.create_thread(riakboost::bind(thread_function, ref(b))); /// create a thread and pass it the barrier

    tg.join_all();
}

// EOF
