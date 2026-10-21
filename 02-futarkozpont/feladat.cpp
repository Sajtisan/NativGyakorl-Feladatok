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
    testing::CASE(1, 1) << "# Pozicio es csomag" << std::endl;
        const Position a(-2, 3), b(1, -1); CHECK(a.getX() == -2 && a.getY() == 3 && a - b == 7 && b - a == 7);
        CHECK(Position().getX() == 0);
        const Parcel p("P1", "Szeged", a, 120, 2);
        CHECK(p.getId() == "P1" && p.getCity() == "Szeged" && p.getFee() == 120 && p.getPriority() == 2);
        CHECK(p.getDestination().getX() == -2);
        {
      try { Parcel p("X", "Y", {}, -1);
        ASSERT_FAIL("Hianyzik az elvart kivetel");
      } catch (const std::invalid_argument& e) {
        ASSERT_EQ(std::string(e.what()), std::string("Invalid parcel"), "Kivetel uzenete");
      }
    }
        {
      try { Parcel p("X", "Y", {}, 1, 6);
        ASSERT_FAIL("Hianyzik az elvart kivetel");
      } catch (const std::invalid_argument& e) {
        ASSERT_EQ(std::string(e.what()), std::string("Invalid parcel"), "Kivetel uzenete");
      }
    }
  }
#endif
#if 1
  {
    testing::CASE(1, 1) << "# Prefix, postfix, telites es lancolas" << std::endl;
        Parcel p("P1", "Szeged", {}, 100, 4);
        const Parcel old = p++; CHECK(old.getPriority() == 4 && p.getPriority() == 5);
        CHECK(&++p == &p && p.getPriority() == 5);
        CHECK((p--).getPriority() == 5 && p.getPriority() == 4);
        for (int i = 0; i < 8; ++i) { --p; }
        CHECK(p.getPriority() == 0 && &--p == &p);
        CHECK(&(p += 30) == &p); (p += 20) -= 60; CHECK(p.getFee() == 90);
        const Parcel& c = p; Parcel extra = c + 5; CHECK(extra.getFee() == 95 && p.getFee() == 90);
        p -= 100; CHECK(p.getFee() == 0);
        {
      try { p += -1;
        ASSERT_FAIL("Hianyzik az elvart kivetel");
      } catch (const std::invalid_argument& e) {
        ASSERT_EQ(std::string(e.what()), std::string("Negative amount"), "Kivetel uzenete");
      }
    }
        {
      try { p -= -1;
        ASSERT_FAIL("Hianyzik az elvart kivetel");
      } catch (const std::invalid_argument& e) {
        ASSERT_EQ(std::string(e.what()), std::string("Negative amount"), "Kivetel uzenete");
      }
    } CHECK(p.getFee() == 0);
  }
#endif
#if 1
  {
    testing::CASE(1, 1) << "# Osszehasonlitas, explicit konverzio es stream" << std::endl;
        const Parcel a("A", "Szeged", {1, -2}, 100, 3), b("B", "Pecs", {}, 40, 3), c("C", "Pecs", {}, 30, 4);
        CHECK(a == Parcel("A", "Mas", {}, 0)); CHECK(a != b && a < b && c < a && !(a < a));
        CHECK(static_cast<std::string>(a) == "Parcel[A; Szeged; (1,-2); fee=100; priority=3]");

        std::ostringstream out; CHECK(&(out << a) == &out); CHECK(out.str() == static_cast<std::string>(a));
  }
#endif
#if 1
  {
    testing::CASE(1, 1) << "# Map, masolat, kulcsok es const referencia" << std::endl;
        Depot d("Del"); Parcel a("A", "Szeged", {}, 100, 2);
        CHECK(d.put("R2", a)); a += 50; CHECK(d["R2"].getFee() == 100);
        CHECK(!d.put("R2", Parcel("B", "Pecs", {}, 1))); CHECK(!d.put("R1", a));
        CHECK(d.put("R1", Parcel("B", "Pecs", {}, 80, 3)));
        d["R2"] += 20; const Depot& c = d;


        CHECK(&c["R2"] == &d["R2"] && c["R2"].getFee() == 120);
        CHECK(c.count() == 2 && static_cast<unsigned>(c) == 2);
        {
      try { (void)d["X"];
        ASSERT_FAIL("Hianyzik az elvart kivetel");
      } catch (const std::out_of_range& e) {
        ASSERT_EQ(std::string(e.what()), std::string("No locker"), "Kivetel uzenete");
      }
    } CHECK(d.count() == 2);
        CHECK(c.cities() == std::set<std::string>({"Pecs", "Szeged"}));
        CHECK(d.erasePrefix("A") == 1 && d.erasePrefix("X") == 0 && d.erasePrefix("") == 1);
  }
#endif
#if 1
  {
    testing::CASE(1, 1) << "# Legjobb, dij szerinti szures, tavolsag szerinti reszhalmaz" << std::endl;
        Depot d("Del"); d.put("R3", Parcel("B", "Szeged", {1, 2}, 100, 3));
        d.put("R1", Parcel("A", "Szeged", {0, 0}, 100, 3)); d.put("R2", Parcel("C", "Pecs", {5, 0}, 80, 4));
        const Depot& c = d; CHECK(&(~c) == &c["R2"]);
        const auto v = c <= 100; CHECK(v.size() == 3 && v[0].getId() == "C" && v[1].getId() == "A" && v[2].getId() == "B");
        CHECK((c <= -1).empty()); CHECK((c <= 80).size() == 1);
        Depot near = c(Position(), 3); CHECK(near.count() == 2); near["R1"] += 1; CHECK(c["R1"].getFee() == 100);
        CHECK(c(Position(), 0).count() == 1);
        {
      try { (void)c(Position(), -1);
        ASSERT_FAIL("Hianyzik az elvart kivetel");
      } catch (const std::invalid_argument& e) {
        ASSERT_EQ(std::string(e.what()), std::string("Negative radius"), "Kivetel uzenete");
      }
    }
        Depot empty; {
      try { (void)~empty;
        ASSERT_FAIL("Hianyzik az elvart kivetel");
      } catch (const std::out_of_range& e) {
        ASSERT_EQ(std::string(e.what()), std::string("No parcel"), "Kivetel uzenete");
      }
    }
  }
#endif
#if 1
  {
    testing::CASE(1, 1) << "# Rekesz szerinti kiiras es ures tarolo" << std::endl;
        Depot d("Del"); d.put("R2", Parcel("B", "Pecs", {}, 80, 3)); d.put("R1", Parcel("A", "Szeged", {1, -2}, 100, 2));
        std::ostringstream out; CHECK(&(out << d) == &out);
        CHECK(out.str() == "Depot: Del\nR1: Parcel[A; Szeged; (1,-2); fee=100; priority=2]\nR2: Parcel[B; Pecs; (0,0); fee=80; priority=3]\n");
        Depot e; std::ostringstream empty; empty << e; CHECK(empty.str() == "Depot: Kozpont\n");
  }
#endif

  return 0;
}
#endif
