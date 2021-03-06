//===-- avrlit - AVR LLVM Integrated Tester - AVR Side --------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef AVRLIT_H
# define AVRLIT_H

# include <stdint.h>
# include <avr/pgmspace.h>

#include "serial/serial.h"

namespace avrlit {

// TODO: remove this
typedef bool    i1;
typedef int8_t  i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

extern "C" {
  void avrlit_initialize();

  void avrlit_send_byte(uint8_t byte);
  uint8_t avrlit_receive_byte();

  void avrlit_report_result_u8(const char *name, uint8_t result);
}

// namespace avrlit {
//
// struct dec {
//   explicit dec(int n);
//   explicit dec(unsigned int n);
//   operator char const*() { return buf_; }
// private:
//   char buf_[6];
// };
//
// class test {
//   public:
//     explicit test(ostream const& os) :
//       os_(os), planned_tests_(), passed_tests_(), failed_tests_() {}
//     ~test() { summarize(); }
//     void ok(bool result, char const* expression, char const* file, unsigned line);
//     void plan(unsigned count, char const * name = nullptr);
//
//     ostream & os() { return os_; }
//   private:
//     void summarize();
//     ostream os_;
//     unsigned planned_tests_;
//     unsigned passed_tests_;
//     unsigned failed_tests_;
// };
//
// void init();
//
// enum test_suite_state {
//   waiting,
//   delaying,
//   running,
//   done
// };
//
// extern bool is_terminal_ready;
//
// void do_tasks(bool performRead);
//
// ostream get_cdc_stream();
//
// int16_t cdc_receive_byte();
//
// template <typename F>
// inline
// int
// run(F f, ostream const& os) {
//   test t(os);
//   f(t);
//   return 0;
// }
//
// template  <typename... F>
// void
// run(F... tests) {
//   init();
//   ostream os(get_cdc_stream());
//   test_suite_state state = waiting;
//   int16_t delay = 10;
//   for (;;) {
//     char c = cdc_receive_byte();
//     switch (state) {
//       case waiting:
//         if (is_terminal_ready) {
//           delay = 10;
//           state = delaying;
//         }
//         break;
//       case delaying: 
//         if (--delay == 0) {
//           state = running; 
//         }
//         break;
//       case running:
//         [](...){ }(run(tests, os)...);
//         os << P("--\n");
//         state = done;
//         break;
//       case done:
//         if (not is_terminal_ready or c == ' ') state = waiting;
//         break;
//     }
//     do_tasks(false);
//   }
// }

} // end of namespace avrlit

// # define AVRLIT_TEST_SUITE()
// namespace { char const*const PROGMEM avrlit_test_suite_file = __FILE__; }
//
// # define AVRLIT_TEST_EXPRESSION(x) x, PSTR(#x), avrlit_test_suite_file, __LINE__
//
// # define _(x)       AVRLIT_TEST_EXPRESSION(x)

#endif // AVRLIT_H
