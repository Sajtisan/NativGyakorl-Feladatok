#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <map>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace solution {
// -----  Ide dolgozz!  -----



// -----  Ez alatt mar ne legyen implementacio!  -----
} // namespace solution
using namespace solution;

#ifndef TEST_BIRO

namespace testing {

#define ASSERT_EQ(ACTUAL_EXPR, EXPECTED_EXPR, message)                                                                 \
  do {                                                                                                                 \
    const auto& _actual = (ACTUAL_EXPR);                                                                               \
    const auto& _expected = (EXPECTED_EXPR);                                                                           \
    if ((_actual) != (_expected)) {                                                                                    \
      testing::FAIL() << __FILE_NAME__ << ":" << __LINE__ << ": " << __PRETTY_FUNCTION__ << ": " << (message)          \
                      << std::endl;                                                                                    \
      testing::FAIL() << "returned: " << (_actual) << std::endl;                                                       \
      testing::FAIL() << "expected: " << (_expected) << std::endl;                                                     \
      abort();                                                                                                         \
    } else {                                                                                                           \
      testing::OK() << __FILE_NAME__ << ":" << __LINE__ << ": " << (message) << std::endl;                             \
      testing::OK() << "returned: " << (_actual) << std::endl;                                                         \
    }                                                                          \
  } while (0)

#define ASSERT_EQ_BOOL(ACTUAL_EXPR, EXPECTED_EXPR, message)                 \
  do {                                                                      \
    const auto& _actualStr   = (ACTUAL_EXPR == true) ? "true" : "false";    \
    const auto& _expectedStr = (EXPECTED_EXPR == true) ? "true" : "false";  \
    ASSERT_EQ(_actualStr, _expectedStr, message);                           \
  } while (0)

#define ASSERT_FAIL(message)                                                                                           \
  do {                                                                                                                 \
      testing::FAIL() << __FILE_NAME__ << ":" << __LINE__ << ": " << __PRETTY_FUNCTION__ << ": " << (message)          \
                      << std::endl;                                                                                    \
      abort();                                                                                                         \
  } while (0)

#define ASSERT_OK(message)                                                    \
  do {                                                                        \
    testing::OK() << __FILE_NAME__ << ":" << __LINE__ << ": " << (message)  \
                    << std::endl;                                           \
  } while (0)


std::ostream& initialize(int, char**) { return std::cout; }

std::ostream& REPORT() { return std::cout; }

static unsigned s_compile = 0u;
static unsigned s_output = 0u;

std::ostream& CASE(int compile, int output) {
  static unsigned testIdx = 0u;
  s_compile += compile;
  s_output += output;

  REPORT() << "Test #" << ++testIdx
    << " (points: compile: " << compile << ", exec: " << output << ")"
    << std::endl;
  return REPORT();
}

std::ostream& CASE_END() {
  REPORT() << "Sum points: " << s_compile + s_output << std::endl;
  return REPORT();
}

std::ostream& FAIL() {
  std::ostream& out = REPORT();
  out << "FAIL: ";
  return out;
}

std::ostream& OK() {
  std::ostream& out = REPORT();
  out << "OK: ";
  return out;
}

} // namespace testing


#define CHECK(...) ASSERT_EQ_BOOL((__VA_ARGS__), true, #__VA_ARGS__)

int main(int argc, char** argv) {
  [[maybe_unused]] std::ostream& report = testing::initialize(argc, argv);

#if 1
  {
    testing::CASE(1, 1) << "# Oszlopok es ures mezok" << std::endl;
        CHECK(column("a;;c;", 0) == "a"); CHECK(column("a;;c;", 1).empty());
        CHECK(column("a;;c;", 3).empty()); CHECK(column("", 0).empty());
        {
      try { (void)column("a;b", 2);
        ASSERT_FAIL("Hianyzik az elvart kivetel");
      } catch (const std::out_of_range& e) {
        ASSERT_EQ(std::string(e.what()), std::string("No column"), "Kivetel uzenete");
      }
    }
  }
#endif
#if 1
  {
    testing::CASE(1, 1) << "# Szigoru szamkonverzio" << std::endl;
        CHECK(toNumber("0") == 0); CHECK(toNumber("0012") == 12);
        CHECK(toNumber("2147483647") == 2147483647);
        for (const auto& s : {"", "12x", " 12", "12 ", "+12", "-1", "1.5", "999999999999x"}) CHECK(toNumber(s) == -1);
        CHECK(toNumber("2147483648") == -2);
  }
#endif
#if 1
  {
    testing::CASE(1, 1) << "# Konyv, const getterek es telites" << std::endl;
        Book b("Dune", "Herbert", 100); const Book& c = b;
        CHECK(c.getTitle() == "Dune" && c.getAuthor() == "Herbert");
        CHECK(c.getPages() == 100 && c.getRead() == 0 && c.remaining() == 100);
        b.read(30); CHECK(c.info() == "Book[Dune; Herbert; 30/100]");
        b.read(std::numeric_limits<unsigned>::max()); b.read(1); CHECK(c.getRead() == 100);
        {
      try { Book x("", "A", 10);
        ASSERT_FAIL("Hianyzik az elvart kivetel");
      } catch (const std::invalid_argument& e) {
        ASSERT_EQ(std::string(e.what()), std::string("Invalid book"), "Kivetel uzenete");
      }
    }
        {
      try { Book x("X", "A", 0);
        ASSERT_FAIL("Hianyzik az elvart kivetel");
      } catch (const std::invalid_argument& e) {
        ASSERT_EQ(std::string(e.what()), std::string("Invalid book"), "Kivetel uzenete");
      }
    }
  }
#endif
#if 1
  {
    testing::CASE(1, 1) << "# Teljes sor feldolgozasa" << std::endl;
        CHECK(parseBook("The Hobbit;Tolkien;310").getPages() == 310);
        for (const auto& s : {"A;B", "A;B;1;", ";B;2", "A;;2", "A;B;0", "A;B;1000001", "A;B;3x", "A;B;999999999999"})
            {
      try { (void)parseBook(s);
        ASSERT_FAIL("Hianyzik az elvart kivetel");
      } catch (const std::invalid_argument& e) {
        ASSERT_EQ(std::string(e.what()), std::string("Invalid book"), "Kivetel uzenete");
      }
    }
  }
#endif
#if 1
  {
    testing::CASE(1, 1) << "# Masolat, egyediseg, referenciak" << std::endl;
        ReadingList l("Hetvege"); CHECK(l.count() == 0 && l.totalRemaining() == 0);
        Book b("Dune", "Herbert", 100); CHECK(l.add(b)); b.read(90);
        CHECK(!l.add(Book("Dune", "Mas", 4))); CHECK(l.add(Book("dune", "Mas", 5)));
        l.find("Dune").read(20); const ReadingList& c = l;

        CHECK(c.find("Dune").getRead() == 20 && c.totalRemaining() == 85);
        CHECK(&l.find("Dune") == &c.find("Dune"));
        {
      try { (void)c.find("X");
        ASSERT_FAIL("Hianyzik az elvart kivetel");
      } catch (const std::out_of_range& e) {
        ASSERT_EQ(std::string(e.what()), std::string("No book"), "Kivetel uzenete");
      }
    }
  }
#endif
#if 1
  {
    testing::CASE(1, 1) << "# Egymas melletti torles es ures prefix" << std::endl;
        ReadingList l("X"); l.add(Book("AA", "A", 1)); l.add(Book("AB", "A", 2)); l.add(Book("B", "A", 3));
        CHECK(l.erasePrefix("A") == 2); CHECK(l.find("B").getPages() == 3);
        CHECK(l.erasePrefix("Z") == 0); CHECK(l.erasePrefix("") == 1); CHECK(l.count() == 0);
  }
#endif
#if 1
  {
    testing::CASE(1, 1) << "# Stream beolvasas, hibas es ismetelt sor" << std::endl;
        std::istringstream in("A;X;10\nhibas\n\nB;Y;20\nA;Z;50\nC;Z;0\n");
        ReadingList l("Ora"); CHECK(load(in, l) == 2); l.find("A").read(4);
        std::ostringstream out; l.write(out);
        CHECK(out.str() == "ReadingList: Ora\n0: Book[A; X; 4/10]\n1: Book[B; Y; 0/20]\n");
        ReadingList e("Ures"); std::ostringstream empty; e.write(empty); CHECK(empty.str() == "ReadingList: Ures\n");
  }
#endif

  return 0;
}
#endif
