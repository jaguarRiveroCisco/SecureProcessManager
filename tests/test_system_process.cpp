#include <gtest/gtest.h>
#include <thread>
#include "system_process.h"
#include "process_controller.h"
#include "communicator.h"

namespace process {

class SystemProcessTest : public ::testing::Test {
protected:
    void SetUp() override
    {
        // Set up necessary configurations or mocks
        ProcessController::readConfigFile("../exampleConfigFile.conf");
    }

    void TearDown() override
    {
        // Clean up after each test
    }
};


TEST_F(SystemProcessTest, ArgumentsPopulate)
{
    ASSERT_NO_THROW(Arguments::populate());
    ASSERT_FALSE(Arguments::args.empty());
}

} // namespace process