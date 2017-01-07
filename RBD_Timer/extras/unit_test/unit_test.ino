// Arduino RBD Timer Library v1.3.0 - Unit test coverage.
// https://github.com/alextaujenis/RBD_Timer
// Copyright 2016 Alex Taujenis
// MIT License

#include <ArduinoUnit.h> // https://github.com/mmurdoch/arduinounit
#include <RBD_Timer.h>   // https://github.com/alextaujenis/RBD_Timer

RBD::Timer timer;
RBD::Timer timer_untouched;
RBD::Timer timer_timeout(100);
RBD::Timer timer_zero;

// constructor
  test(constructor_should_begin_expired) {
    assertTrue(timer_untouched.isExpired());
    assertFalse(timer_untouched.isActive());
  }

  test(constructor_should_set_the_default_timeout_to_zero_milliseconds) {
    assertEqual(timer_untouched.getTimeout(), 0);
  }

// overloaded constructor
  test(overloaded_constructor_should_set_the_timeout_in_milliseconds) {
    assertEqual(timer_timeout.getTimeout(), 100);
  }

// setTimeout
  test(setTimeout_should_set_the_timeout_in_milliseconds) {
    timer.setTimeout(100);

    assertEqual(timer.getTimeout(), 100);
  }

  test(setTimeout_should_set_the_timeout_in_long_milliseconds) {
    timer.setTimeout(100000L); // trailing 'L' is important for 'long' literal

    assertEqual(timer.getTimeout(), 100000L);
  }

  test(setTimeout_should_constrain_the_lower_bounds_to_one_millisecond) {
    timer.setTimeout(0);

    assertEqual(timer.getTimeout(), 1);
  }

// getTimeout
  test(getTimeout_should_return_the_timeout_in_milliseconds) {
    timer.setTimeout(42);

    assertEqual(timer.getTimeout(), 42);
  }

// setHertz
  test(setHertz_should_set_the_refresh_rate_per_second) {
    timer.setHertz(10);

    assertEqual(timer.getTimeout(), 100); // ten times per second
  }

  test(setHertz_should_constrain_the_lower_bounds_to_one_if_provided_zero) {
    timer.setHertz(0);

    assertEqual(timer.getHertz(), 1);
  }

  test(setHertz_should_constrain_the_lower_bounds_to_one_if_provided_a_negative_number) {
    timer.setHertz(-1);

    assertEqual(timer.getHertz(), 1);
  }

  test(setHertz_should_constrain_the_upper_bounds_to_one_thousand_if_provided_a_large_number) {
    timer.setHertz(1234);

    assertEqual(timer.getHertz(), 1000);
  }

  test(setHertz_should_properly_set_a_value_on_the_lower_bounds_of_the_threshold) {
    timer.setHertz(1);

    assertEqual(timer.getHertz(), 1);
  }

  test(setHertz_should_properly_set_a_value_on_the_upper_bounds_of_the_threshold) {
    timer.setHertz(1000);

    assertEqual(timer.getHertz(), 1000);
  }

// getHertz
  test(getHerts_should_return_the_hertz_value) {
    timer.setHertz(42);

    assertEqual(timer.getHertz(), 42);
  }

// restart
  test(restart_should_make_it_active) {
    timer.setTimeout(1);
    timer.restart();

    assertTrue(timer.isActive());
  }

  test(restart_should_make_it_not_expired) {
    timer.setTimeout(1);
    timer.restart();

    assertFalse(timer.isExpired());
  }

  test(restart_should_make_it_not_stopped) {
    timer.setTimeout(1);
    timer.stop();
    timer.restart();

    assertFalse(timer.isStopped());
  }

// isActive
  test(isActive_should_return_true_if_time_is_available) {
    timer.setTimeout(1);
    timer.restart();

    assertTrue(timer.isActive());
  }

  test(isActive_should_return_false_if_time_has_run_out) {
    timer.setTimeout(1);
    timer.restart();
    delay(2);

    assertFalse(timer.isActive());
  }

  test(isActive_should_remain_false_on_timer_rollover) {
    timer.setTimeout(10);
    timer.restart();

    delay(11);
    assertFalse(timer.isActive());

    timer.setTimeout(20); // make it active again without calling restart: almost like timer rollover

    assertFalse(timer.isActive());
  }

// isExpired
  test(isExpired_should_return_false_if_time_is_available) {
    timer.setTimeout(1);
    timer.restart();

    assertFalse(timer.isExpired());
  }

  test(isExpired_should_return_true_if_time_has_run_out) {
    timer.setTimeout(1);
    timer.restart();
    delay(1);

    assertTrue(timer.isExpired());
  }

  test(isExpired_should_remain_true_on_timer_rollover) {
    timer.setTimeout(10);
    timer.restart();

    delay(11);
    assertTrue(timer.isExpired());

    timer.setTimeout(20); // make it active again without calling restart: almost like timer rollover

    assertTrue(timer.isExpired());
  }

// isStopped
  test(isStopped_should_return_true_if_stopped) {
    timer.setTimeout(1);
    timer.restart();
    timer.stop();

    assertTrue(timer.isStopped());
  }

  test(isStopped_should_return_false_if_active) {
    timer.setTimeout(5);
    timer.restart();

    assertTrue(timer.isActive());
    assertFalse(timer.isStopped());
  }

  test(isStopped_should_return_false_if_expired) {
    timer.setTimeout(1);
    timer.restart();
    delay(1);

    assertTrue(timer.isExpired());
    assertFalse(timer.isStopped());
  }

  test(isStopped_should_remain_true_on_timer_rollover) {
    timer.setTimeout(10);
    timer.restart();

    delay(11);
    assertTrue(timer.isExpired());
    timer.stop();

    timer.setTimeout(20); // make it active again without calling restart: almost like timer rollover

    assertTrue(timer.isStopped());
  }

// onRestart
  test(onRestart_should_return_true_if_the_timer_expires) {
    timer.setTimeout(1);
    timer.restart();
    delay(1);

    assertTrue(timer.onRestart());
  }

  test(onRestart_should_return_false_the_second_time_after_the_timer_expires) {
    timer.setTimeout(1);
    timer.restart();
    delay(1);
    timer.onRestart();

    assertFalse(timer.onRestart());
  }

// onActive
  test(onActive_should_return_false_if_the_timer_has_not_been_restarted) {
    timer.setTimeout(1);

    assertFalse(timer.onActive());
  }

  test(onActive_should_return_true_after_the_timer_is_restarted) {
    timer.setTimeout(1);
    timer.restart();

    assertTrue(timer.onActive());
  }

  test(onActive_should_return_false_the_second_time_after_the_timer_is_restarted) {
    timer.setTimeout(1);
    timer.restart();
    timer.onActive();

    assertFalse(timer.onActive());
  }

  test(onActive_should_return_false_on_timer_rollover) {
    timer.setTimeout(1);
    timer.restart();

    assertTrue(timer.onActive());
    assertFalse(timer.onActive());

    timer.setTimeout(5); // make it active again without calling restart: almost like timer rollover

    assertFalse(timer.onActive());
  }

// onExpired
  test(onExpired_should_return_true_after_the_timer_expires) {
    timer.setTimeout(1);
    timer.restart();
    delay(1);

    assertTrue(timer.onExpired());
  }

  test(onExpired_should_return_false_the_second_time_after_the_timer_is_restarted) {
    timer.setTimeout(1);
    timer.restart();
    delay(1);
    timer.onExpired();

    assertFalse(timer.onExpired());
  }

  test(onExpired_should_return_false_on_timer_rollover) {
    timer.setTimeout(1);
    timer.restart();
    delay(1);

    assertTrue(timer.onExpired());
    assertFalse(timer.onExpired());

    timer.setTimeout(5); // make it active again without calling restart, then wait for it to expire: almost like timer rollover
    delay(5);

    assertFalse(timer.onExpired());
  }

// stop
  test(isActive_and_onActive_should_return_false_after_stop) {
    timer.setTimeout(1);
    timer.restart();
    timer.stop();

    assertFalse(timer.isActive());
    assertFalse(timer.onActive());
  }

  test(isExpired_and_onExpired_should_return_false_after_stop) {
    timer.setTimeout(1);
    timer.restart();
    timer.stop();
    delay(1);

    assertFalse(timer.isExpired());
    assertFalse(timer.onExpired());
  }

  test(onRestart_should_return_false_after_stop) {
    timer.setTimeout(1);
    timer.restart();
    timer.stop();
    delay(1);

    assertFalse(timer.onRestart());
  }

// getValue
  test(getValue_should_return_the_time_passed_since_restart) {
    timer.setTimeout(5);
    timer.restart();
    delay(2);

    assertEqual(timer.getValue(), 2);
  }

// getInverseValue
  test(getInverseValue_should_return_the_time_left_until_timeout) {
    timer.setTimeout(5);
    timer.restart();
    delay(2);

    assertEqual(timer.getInverseValue(), 3);
  }

// getPercentValue
  test(getPercentValue_should_return_the_percentage_of_time_passed) {
    timer.setTimeout(100);
    timer.restart();
    delay(40);

    assertEqual(timer.getPercentValue(), 40);
  }

  test(getPercentValue_should_not_divide_by_zero) {
    timer_zero.restart();

    assertEqual(timer_zero.getPercentValue(), 0);
  }

// getInversePercentValue
  test(getInversePercentValue_should_return_the_percentage_of_time_remaining) {
    timer.setTimeout(100);
    timer.restart();
    delay(40);

    assertEqual(timer.getInversePercentValue(), 60);
  }

  test(getInversePercentValue_should_not_divide_by_zero) {
    timer_zero.restart();

    assertEqual(timer_zero.getInversePercentValue(), 100);
  }


void setup() {
  Serial.begin(115200);
  while(!Serial);
}

void loop() {
  Test::run();
}
