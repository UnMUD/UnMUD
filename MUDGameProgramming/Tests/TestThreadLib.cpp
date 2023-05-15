#include <gtest/gtest.h>
#include <pthread.h>

#include "BasicLib/BasicLibTime.h"
#include "ThreadLib/ThreadLib.h"

const BasicLib::sint64 TOLERANCE_MS = 10;

void TestFunction(void* data)
{
    int* value = static_cast<int*>(data);
    *value = 42;
}


TEST(ThreadLibFunctions, Create)
{
    int value = 0;
    ThreadLib::ThreadID threadId = ThreadLib::Create(TestFunction, &value);
    ThreadLib::WaitForFinish(threadId);
    ASSERT_EQ(42, value);

    // try {
    //     threadId = ThreadLib::Create(nullptr, &value);
    //     FAIL() << "Exception not thrown";
    // } catch (const ThreadLib::Exception& e) {
    //     EXPECT_EQ(ThreadLib::Error::CreationFailure, e.GetError());
    // } catch (...) {
    //     FAIL() << "Unexpected exception thrown";
    // }

    FAIL() << "Thread creation with null function results in"
              " Segmentation Fault (core dumped) error";
}

TEST(ThreadLibFunctions, GetThreadID) {
    ThreadLib::ThreadID threadID = ThreadLib::GetID();
    EXPECT_NE(threadID, 0);
}

TEST(ThreadLibFunctions, YieldThread) {
    int milliseconds = 100;
    BasicLib::Timer t;
    t.Reset(0);
    ThreadLib::YieldThread(milliseconds);
    EXPECT_NEAR(t.GetMS(), milliseconds, TOLERANCE_MS);
}

void TestExitFunction(void *data)
{
    sleep(5);
    int* value = static_cast<int*>(data);
    *value = 42;
}

TEST(ThreadLibFunctions, KillThread) {
    int value = 0;
    ThreadLib::ThreadID thread = ThreadLib::Create(TestExitFunction, &value);
    ThreadLib::Kill(thread);
    ThreadLib::WaitForFinish(thread);
    EXPECT_EQ(value, 0);
}

TEST(ThreadLibExceptions, GetError) {
    ThreadLib::Exception excp;
    EXPECT_EQ(excp.GetError(), 0);

    ThreadLib::Error e = ThreadLib::Error::InitFailure;
    excp = ThreadLib::Exception(e);
    EXPECT_EQ(excp.GetError(), e);
}