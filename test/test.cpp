#include "catch_almagamated.hpp"
#include <iostream>

// change if you choose to use a different header name
#include "graph.h"

using namespace std;

// // the syntax for defining a test is below. It is important for the name to be
// // unique, but you can group multiple tests with [tags]. A test can have
// // [multiple][tags] using that syntax.
// TEST_CASE("Example Test Name - Change me!", "[tag]") {
//   // instantiate any class members that you need to test here
//   int one = 1;

//   // anything that evaluates to false in a REQUIRE block will result in a
//   // failing test
//   REQUIRE(one == 0); // fix me!

//   // all REQUIRE blocks must evaluate to true for the whole test to pass
//   REQUIRE(false); // also fix me!
// }

// TEST_CASE("Test 2", "[tag]") {
//   // you can also use "sections" to share setup code between tests, for example:
//   int one = 1;

//   SECTION("num is 2") {
//     int num = one + 1;
//     REQUIRE(num == 2);
//   };

//   SECTION("num is 3") {
//     int num = one + 2;
//     REQUIRE(num == 3);
//   };

//   // each section runs the setup code independently to ensure that they don't
//   // affect each other
// }

// // Refer to Canvas for a list of required tests. 
// // We encourage you to write more than required to ensure proper functionality, but only the ones on Canvas will be graded.

// // See the following for an example of how to easily test your output.
// // Note that while this works, I recommend also creating plenty of unit tests for particular functions within your code.
// // This pattern should only be used for final, end-to-end testing.

// // This uses C++ "raw strings" and assumes your CampusCompass outputs a string with
// //   the same thing you print.
// TEST_CASE("Example CampusCompass Output Test", "[flag]") {
//   // the following is a "raw string" - you can write the exact input (without
//   //   any indentation!) and it should work as expected
//   // this is based on the input and output of the first public test case
//   string input = R"(6
// insert "Student A" 10000001 1 1 COP3502
// insert "Student B" 10000002 1 1 COP3502
// insert "Student C" 10000003 1 2 COP3502 MAC2311
// dropClass 10000001 COP3502
// remove 10000001
// removeClass COP3502
// )";

//   string expectedOutput = R"(successful
// successful
// successful
// successful
// unsuccessful
// 2
// )";

//   string actualOutput;

//   // somehow pass your input into your CampusCompass and parse it to call the
//   // correct functions, for example:
//   /*
//   CampusCompass c;
//   c.parseInput(input)
//   // this would be some function that sends the output from your class into a string for use in testing
//   actualOutput = c.getStringRepresentation()
//   */

//   REQUIRE(actualOutput == expectedOutput);
// }

TEST_CASE("Test insertion func"){
  Graph guh = makeGraph(); //lol
  REQUIRE(guh.insert("Diya", 12341234, 19, {"COP3530", "MAC2311"}) == "successful");
  REQUIRE(guh.insert("Diya", 12341234, 19, {"COP3530", "MAC2311"}) == "unsuccessful");
  REQUIRE(guh.insert("Anne with an E123", 45679999, 15, {"MAC2311"}) == "unsuccessful");
  REQUIRE(guh.insert("Gilber", 4567999, 15, {"MAC2311"}) == "unsuccessful");
  REQUIRE(guh.insert("Diana", 45679999, 15, {"mac2311"}) == "unsuccessful");
}

TEST_CASE("Test remove, drop, and replace funcs"){
  Graph guh = makeGraph(); //lol again
  guh.insert("Diya", 12341234, 19, {"COP3530", "MAC2311"});
  
  REQUIRE(guh.remove(12341234) == "successful");
  REQUIRE(guh.remove(12341234) == "unsuccessful");

  guh.insert("Jerry", 12341234, 19, {"COP3530", "MAC2311"});
  REQUIRE(guh.dropClass(12341234, "COP3530") == "successful");
  REQUIRE(guh.dropClass(12341234, "CDA3101") == "unsuccessful");
  REQUIRE(guh.dropClass(12341234, "MAC2311") == "successful");
  REQUIRE(guh.remove(12341234) == "unsuccessful");

  guh.insert("Matthew", 12341234, 19, {"COP3530", "MAC2311"});
  REQUIRE(guh.replaceClass(12341234, "COP3530", "CDA3101") == "successful");

}

TEST_CASE("Test removeClass func"){
  Graph guh = makeGraph(); //lol again again
  guh.insert("Diya", 12341234, 19, {"COP3530", "MAC2311"});
  guh.insert("Jerry", 12341235, 20, {"COP3530", "CDA3101"});
  guh.insert("Matthew", 12341236, 55, {"CDA3101", "MAC2311"});

  REQUIRE(guh.removeClass("COP3530") == "2");
  REQUIRE(guh.removeClass("CDA3101") == "2");
  REQUIRE(guh.removeClass("MAC2312") == "unsuccessful");
  REQUIRE(guh.removeClass("ZOO1234") == "unsuccessful");
}

TEST_CASE("Test the edges"){
  Graph guh = makeGraph(); //lol again again again
  REQUIRE(guh.checkEdgeStatus(1, 2) == "open");
  REQUIRE(guh.checkEdgeStatus(1, 999) == "DNE");
  g.toggleEdgesClosure({{1, 2}});
  REQUIRE(guh.checkEdgeStatus(1, 2) == "closed");
  g.toggleEdgesClosure({{1, 2}});
  REQUIRE(guh.checkEdgeStatus(1, 2) == "open");
  REQUIRE(guh.isConnected(1, 2) == "successful");
  REQUIRE(guh.isConnected(1, 1) == "successful");
}
 
TEST_CASE("Test stuff from assignment"){
  Graph gub = makeGraph(); //lol 
  REQUIRE(gub.insert("Anne", 45679999, 20, {"COP3530","MAC2311"}) == "successful");
  REQUIRE(gub.insert("Maria", 35459999, 21, {"COP3530","CDA3101","MAC2311"}) == "successful");
  REQUIRE(gub.insert("Matthew", 87879999, 22, {"CDA3101","MAC2311","EEL3701"}) == "successful");

  REQUIRE(gub.removeClass("COP3530") == "2");
  REQUIRE(gub.remove(87879999) == "successful");

  REQUIRE(gub.removeClass("CDA3101") == "1");

  REQUIRE(gub.remove(35459999) == "successful");
  REQUIRE(gub.remove(35459999) == "unsuccessful");
}