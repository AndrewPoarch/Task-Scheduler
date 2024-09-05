#include <gtest/gtest.h>

#include "../lib/TaskShudler.hpp"
#include <cmath>

TEST(SchedulerTests, AnyTrivial) {
    Any A;
    A.Put(1);
    EXPECT_EQ(A.Get<int>(), 1);
}

TEST(SchedulerTests, AnyVector) {
    Any A;
    A.Put(std::vector<std::string>(1, "hello"));
    EXPECT_EQ(A.Get<std::vector<std::string>>(), std::vector<std::string>{"hello"});
}

TEST(SchedulerTests, AnyTrivial1) {
    Any A;
    A.Put(52.0);
    EXPECT_EQ(A.Get<double>(), 52.0);
}

TEST(SchedulerTests, AnyFloat) {
    Any A;
    float c = 1;
    A.Put(c);
    EXPECT_EQ(A.Get<float>(), 1.0f);
}

TEST(SchedulerTests, AnyFloat1) {
    Any A;
    A.Put(2.0f);
    EXPECT_EQ(A.Get<float>(), 2.0f);
}

TEST(SchedulerTests, HasCycle) {
    Graph g;
    g.AddEdge(0, 1);
    g.AddEdge(1, 2);
    g.AddEdge(2, 3);
    g.AddEdge(3, 4);
    g.AddEdge(3, 5);
    g.AddEdge(3, 0);

    ASSERT_TRUE(g.HasCycleOneVertex(0));
    ASSERT_TRUE(g.HasCycleOneVertex(1));
    ASSERT_TRUE(g.HasCycleOneVertex(2));
    ASSERT_TRUE(g.HasCycleOneVertex(3));
    ASSERT_FALSE(g.HasCycleOneVertex(4));
    ASSERT_FALSE(g.HasCycleOneVertex(5));
    ASSERT_TRUE(g.HasCycle());
}



TEST(SchedulerTests, ZeroArg) {
    TTaskScheduler scheduler;

    auto id1 = scheduler.add([]() { return "52!"; });
    EXPECT_EQ(scheduler.getResult<const char*>(id1), "52!");
}

TEST(SchedulerTests, OneArg) {
    TTaskScheduler scheduler;

    std::vector<int> v{1, 2, 4, 5};

    auto id1 = scheduler.add([](const std::vector<int>& v) { return v[2]; }, v);
    scheduler.getResult<int>(id1);
    scheduler.executeAll();
    EXPECT_EQ(scheduler.getResult<int>(id1), 4);
}

TEST(SchedulerTests, GetResString) {
    std::string x = "Hello";

    TTaskScheduler scheduler;

    auto id1 = scheduler.add([](const std::string& x) { return x + ", world!"; }, x);
    EXPECT_EQ(scheduler.getResult<std::string>(id1), "Hello, world!");
}

TEST(SchedulerTests, GetRes) {
    int x = 1;
    int y = 2;

    TTaskScheduler scheduler;

    auto id1 = scheduler.add([](int x, int y) { return x + y; }, x, y);
    EXPECT_EQ(scheduler.getResult<int>(id1), 3);
}


TEST(SchedulerTests, Example) {
    float a = 2;
    float b = -4;
    float c = 0;

    TTaskScheduler scheduler;

    auto id1 = scheduler.add([](float a, float c) { return -4 * a * c; }, a, c);

    auto id2 = scheduler.add([](float b, float v) { return b * b + v; }, b, scheduler.getFutureResult<float>(id1));

    auto id3 =
        scheduler.add([](float b, float d) { return -b + std::sqrt(d); }, b, scheduler.getFutureResult<float>(id2));

    auto id4 =
        scheduler.add([](float b, float d) { return -b - std::sqrt(d); }, b, scheduler.getFutureResult<float>(id2));

    auto id5 = scheduler.add([](float a, float v) { return v / (2 * a); }, a, scheduler.getFutureResult<float>(id3));

    auto id6 = scheduler.add([](float a, float v) { return v / (2 * a); }, a, scheduler.getFutureResult<float>(id4));

    scheduler.executeAll();

    EXPECT_EQ(scheduler.getResult<float>(id5), 2);
    EXPECT_EQ(scheduler.getResult<float>(id6), 0);
    EXPECT_EQ(scheduler.getResult<float>(id4), 0);
    EXPECT_EQ(scheduler.getResult<float>(id3), 8);
    EXPECT_EQ(scheduler.getResult<float>(id2), 16);
    EXPECT_EQ(scheduler.getResult<float>(id1), 0);
}


TEST(SchedulerUnitTestSuite, SchedulerTest2) {
    float a = 1;
    float b = -2;
    float c = 0;

    TTaskScheduler scheduler;

    auto id1 = scheduler.add([](float a, float c) { return -4 * a * c; }, a, c);
    auto fr1 = scheduler.getFutureResult<float>(id1);

    auto id2 = scheduler.add([](float b, float v) { return b * b + v; }, b, fr1);

    auto id3 =
        scheduler.add([](float b, float d) { return -b + std::sqrt(d); }, b, scheduler.getFutureResult<float>(id2));

    auto id4 =
        scheduler.add([](float b, float d) { return -b - std::sqrt(d); }, b, scheduler.getFutureResult<float>(id2));

    auto id5 = scheduler.add([](float a, float v) { return v / (2 * a); }, a, scheduler.getFutureResult<float>(id3));

    auto id6 = scheduler.add([](float a, float v) { return v / (2 * a); }, a, scheduler.getFutureResult<float>(id4));

    scheduler.executeAll();

    ASSERT_EQ(scheduler.getResult<float>(id4), 0);
    ASSERT_EQ(scheduler.getResult<float>(id5), 2);
    ASSERT_EQ(scheduler.getResult<float>(id2), 4);
    ASSERT_EQ(scheduler.getResult<float>(id6), 0);
    ASSERT_EQ(scheduler.getResult<float>(id3), 4);
    ASSERT_EQ(scheduler.getResult<float>(id1), 0);
}

TEST(SchedulerTests, GetResagag) {
    int x = 1;
    int y = 2;

    TTaskScheduler scheduler;
    auto id1 = scheduler.add([](int x, int y) { return x + y; }, x, y);
    id1.id_ = 10;
    auto id2 = scheduler.add([](int x){return x;}, scheduler.getFutureResult<int>(id1));
    EXPECT_EQ(scheduler.getResult<int>(id1), 3);

}
