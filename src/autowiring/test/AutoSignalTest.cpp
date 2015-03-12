// Copyright (C) 2012-2015 Leap Motion, Inc. All rights reserved.
#include "stdafx.h"
#include <autowiring/autowiring.h>

class AutoSignalTest:
  public testing::Test
{};

class RaisesASignal {
public:
  autowiring::signal<void(int)> signal;
};

TEST_F(AutoSignalTest, SimpleSignalTest) {
  autowiring::signal<void(int)> signal;

  bool handler_called = false;
  int val = 0;

  // Register a handler directly on the signal:
  auto* registration =
    signal += [&](int v) {
      handler_called = true;
      val = v;
    };

  // Trivially raise the signal:
  signal(101);
  ASSERT_TRUE(handler_called) << "Handler was not called on a stack-allocated signal";
  ASSERT_EQ(101, val) << "Handler did not correctly copy a passed value";

  // Unregister and verify we can still raise the signal:
  signal -= registration;

  handler_called = false;
  signal(102);
  ASSERT_FALSE(handler_called) << "Handler was called after being unregistered";
}

TEST_F(AutoSignalTest, SignalWithAutowiring) {
  bool handler_called = false;
  int val = 202;

  {
    Autowired<RaisesASignal> ras;

    // Register a signal handler:
    ras(&RaisesASignal::signal) += [&](int v) {
      handler_called = true;
      val = v;
    };

    // Inject type type after the signal has been registered
    AutoRequired<RaisesASignal>();

    // Now raise the signal, see what happens:
    ras->signal(55);

    // Verify that the handler got called with the correct value:
    ASSERT_TRUE(handler_called) << "Signal handler was not invoked";
    ASSERT_EQ(55, val) << "Signal handler not called with the correct parameter as expected";
  }

  // Raise the signal again, this should not cause anything to break:
  Autowired<RaisesASignal> ras;
  handler_called = false;
  ras->signal(99);
  ASSERT_FALSE(handler_called) << "A handler was unexpectedly called after it should have been destroyed";
}
