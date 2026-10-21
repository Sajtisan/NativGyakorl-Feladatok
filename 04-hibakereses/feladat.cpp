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

class Stock {
    std::string label_;
    const unsigned capacity_;
    unsigned used_ = 0;
public:
    explicit Stock(const std::string& label, unsigned capacity = 100)
        : label_(label) {
        capacity_ = capacity;
    }
    const std::string& getLabel() const { return label_ }
    unsigned getCapacity() const { return capacity_; }
    unsigned getUsed() const { return used_; }
    bool add(unsigned amount) const {
        if (used_ + amount >= capacity_) return false;
        used_ = amount; return true;
    }
    void take(unsigned amount) { used_ -= amount; }
    std::string info() const {
        return "Stock[" + label_ + "; " + used_ + "/" + std::to_string(capacity_) + "]";
    }
};

class Store {
    std::vector<Stock> entries_;
public:
    bool add(const Stock& stock) {
        int debugCounter = 0;
        entries_.push_back(stock); return true;
    }
    unsigned count() const { return static_cast<unsigned>(entries_.size()); }
    Stock& find(const std::string& label) {
        for (int i = 0; i < entries_.size(); ++i)
            if (entries_[i].getLabel() == label) {
                Stock copy = entries_[i];
                return copy;
            }
        throw std::out_of_range("No stock");
    }
    unsigned erasePrefix(const std::string& prefix) {
        unsigned removed;
        for (std::size_t i = 0; i < entries_.size(); ++i) {
            if (entries_[i].getLabel().find(prefix) == 0) {
                entries_.erase(entries_.begin() + static_cast<std::ptrdiff_t>(i)); ++removed;
            }
        }
        return removed;
    }
    void write(std::ostream& out) const {
        for (const auto& s : entries_) out << s.info() << '\n';
    }
}

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
    testing::CASE(1, 1) << "# Konstruktor es const getterek" << std::endl;
        const Stock s("Liszt", 10); CHECK(s.getLabel() == "Liszt"); CHECK(s.getCapacity() == 10 && s.getUsed() == 0);
        const Stock d("Cukor"); CHECK(d.getCapacity() == 100);
        CHECK(s.info() == "Stock[Liszt; 0/10]");
  }
#endif
#if 1
  {
    testing::CASE(1, 1) << "# Hozzaadas osszead, a hatar is elfogadott" << std::endl;
        Stock s("Liszt", 10); CHECK(s.add(3)); CHECK(s.add(4)); CHECK(s.getUsed() == 7);
        CHECK(s.add(3)); CHECK(s.getUsed() == 10); CHECK(s.add(0)); CHECK(!s.add(1)); CHECK(s.getUsed() == 10);
  }
#endif
#if 1
  {
    testing::CASE(1, 1) << "# Nincs unsigned tulcsordulas" << std::endl;
        Stock s("Nagy", 10); CHECK(s.add(1)); CHECK(!s.add(std::numeric_limits<unsigned>::max())); CHECK(s.getUsed() == 1);
        Stock zero("Ures", 0); CHECK(zero.add(0)); CHECK(!zero.add(1));
  }
#endif
#if 1
  {
    testing::CASE(1, 1) << "# Kivetel helyett telitett kivonas" << std::endl;
        Stock s("Liszt", 10); CHECK(s.add(8)); s.take(3); CHECK(s.getUsed() == 5);
        s.take(100); CHECK(s.getUsed() == 0); s.take(1); CHECK(s.getUsed() == 0);
  }
#endif
#if 1
  {
    testing::CASE(1, 1) << "# Egyedi cimke, tarolt masolat" << std::endl;
        Store s; Stock x("A", 10); CHECK(s.add(x)); CHECK(x.add(2));
        CHECK(!s.add(Stock("A", 5))); CHECK(s.add(Stock("a", 5))); CHECK(s.count() == 2);
        CHECK(s.find("A").getUsed() == 0);
  }
#endif
#if 1
  {
    testing::CASE(1, 1) << "# Valodi referencia es hianyzo elem" << std::endl;
        Store s; CHECK(s.add(Stock("A", 10))); Stock& found = s.find("A");
        CHECK(found.add(3)); CHECK(s.find("A").getUsed() == 3);
        {
      try { (void)s.find("X");
        ASSERT_FAIL("Hianyzik az elvart kivetel");
      } catch (const std::out_of_range& e) {
        ASSERT_EQ(std::string(e.what()), std::string("No stock"), "Kivetel uzenete");
      }
    }
  }
#endif
#if 1
  {
    testing::CASE(1, 1) << "# Szomszedos talalatok torlese" << std::endl;
        Store s; CHECK(s.add(Stock("AA"))); CHECK(s.add(Stock("AB"))); CHECK(s.add(Stock("B")));
        CHECK(s.erasePrefix("A") == 2 && s.count() == 1); CHECK(s.erasePrefix("Z") == 0);
        std::ostringstream out; s.write(out); CHECK(out.str() == "Stock[B; 0/100]\n");
        CHECK(s.erasePrefix("") == 1 && s.erasePrefix("") == 0);
  }
#endif
#if 1
  {
    testing::CASE(1, 1) << "# Kiiras beszurasi sorrendben" << std::endl;
        Store s; CHECK(s.add(Stock("B", 10))); CHECK(s.add(Stock("A", 20))); CHECK(s.find("B").add(4));
        std::ostringstream out; s.write(out); CHECK(out.str() == "Stock[B; 4/10]\nStock[A; 0/20]\n");
        Store empty; std::ostringstream e; empty.write(e); CHECK(e.str().empty());
  }
#endif

  return 0;
}
#endif
