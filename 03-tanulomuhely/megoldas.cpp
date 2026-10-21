#include <cstdlib>
#include <cstddef>
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

class Task {
    std::string id_;
    unsigned maximum_, points_;

  public:
    Task(const std::string &id, unsigned maximum, unsigned points = 0)
        : id_(id), maximum_(maximum), points_(points) {
        if (id.empty() || maximum == 0 || maximum > 100 || points > maximum)
            throw std::invalid_argument("Invalid task");
    }
    const std::string &getId() const { return id_; }
    unsigned getMax() const { return maximum_; }
    unsigned getPoints() const { return points_; }
    unsigned percentage() const { return points_ * 100 / maximum_; }
    Task &operator++() {
        if (points_ < maximum_)
            ++points_;
        return *this;
    }
    Task operator++(int) {
        Task copy(*this);
        ++*this;
        return copy;
    }
    Task &operator--() {
        if (points_ > 0)
            --points_;
        return *this;
    }
    Task operator--(int) {
        Task copy(*this);
        --*this;
        return copy;
    }
    Task &operator+=(unsigned amount) {
        if (amount > maximum_ - points_)
            points_ = maximum_;
        else
            points_ += amount;
        return *this;
    }
    Task &operator-=(unsigned amount) {
        if (amount > points_)
            points_ = 0;
        else
            points_ -= amount;
        return *this;
    }
    Task operator+(unsigned amount) const {
        Task copy(*this);
        copy += amount;
        return copy;
    }
    bool operator==(const Task &other) const { return id_ == other.id_; }
    bool operator!=(const Task &other) const { return !(*this == other); }
    bool operator<(const Task &other) const {
        if (percentage() != other.percentage())
            return percentage() > other.percentage();
        return id_ < other.id_;
    }
    explicit operator std::string() const {
        return "Task[" + id_ + "; " + std::to_string(points_) + "/" + std::to_string(maximum_) +
               "]";
    }
};
std::ostream &operator<<(std::ostream &out, const Task &task) {
    return out << static_cast<std::string>(task);
}

class TaskBuilder {
    std::string prefix_;
    unsigned count_ = 0;

  public:
    explicit TaskBuilder(const std::string &prefix) : prefix_(prefix) {}
    Task newTask(unsigned maximum = 10) {
        Task result(prefix_ + "-" + std::to_string(count_ + 1), maximum);
        ++count_;
        return result;
    }
    unsigned getCount() const { return count_; }
};

class Workshop {
    std::string name_;
    std::map<std::string, Task> tasks_;

  public:
    explicit Workshop(const std::string &name = "Gyakorlas") : name_(name) {}
    bool add(const std::string &topic, const Task &task) {
        if (tasks_.find(topic) != tasks_.end())
            return false;
        for (const auto &entry : tasks_)
            if (entry.second.getId() == task.getId())
                return false;
        tasks_.insert(std::make_pair(topic, task));
        return true;
    }
    unsigned count() const { return static_cast<unsigned>(tasks_.size()); }
    explicit operator unsigned() const { return count(); }
    Task &operator[](const std::string &topic) {
        auto it = tasks_.find(topic);
        if (it == tasks_.end())
            throw std::out_of_range("No topic");
        return it->second;
    }
    const Task &operator[](const std::string &topic) const {
        auto it = tasks_.find(topic);
        if (it == tasks_.end())
            throw std::out_of_range("No topic");
        return it->second;
    }
    std::string locateById(const std::string &id) const {
        for (const auto &entry : tasks_)
            if (entry.second.getId() == id)
                return entry.first;
        return "[UNKNOWN]";
    }
    bool operator-(const std::string &topic) { return tasks_.erase(topic) != 0; }
    unsigned erasePrefix(const std::string &prefix) {
        unsigned removed = 0;
        for (auto it = tasks_.begin(); it != tasks_.end();) {
            if (it->second.getId().find(prefix) == 0) {
                it = tasks_.erase(it);
                ++removed;
            } else
                ++it;
        }
        return removed;
    }
    const Task &operator~() const {
        if (tasks_.empty())
            throw std::out_of_range("No task");
        auto best = tasks_.begin();
        for (auto it = tasks_.begin(); it != tasks_.end(); ++it)
            if (it->second < best->second)
                best = it;
        return best->second;
    }
    unsigned operator!() const {
        if (tasks_.empty())
            throw std::out_of_range("No task");
        unsigned sum = 0;
        for (const auto &entry : tasks_)
            sum += entry.second.percentage();
        return sum / count();
    }
    std::vector<Task> operator>=(unsigned minimum) const {
        std::vector<Task> result;
        for (const auto &entry : tasks_)
            if (entry.second.percentage() >= minimum)
                result.push_back(entry.second);
        for (std::size_t i = 0; i < result.size(); ++i) {
            for (std::size_t j = i + 1; j < result.size(); ++j) {
                if (result[j] < result[i]) {
                    Task temp(result[i]);
                    result[i] = result[j];
                    result[j] = temp;
                }
            }
        }
        return result;
    }
    Workshop operator()(const std::string &prefix, unsigned minimum) const {
        Workshop result(name_);
        for (const auto &entry : tasks_)
            if (entry.second.getId().find(prefix) == 0 && entry.second.percentage() >= minimum)
                result.add(entry.first, entry.second);
        return result;
    }
    void write(std::ostream &out) const {
        out << "Workshop: " << name_ << '\n';
        for (const auto &entry : tasks_)
            out << entry.first << ": " << entry.second << '\n';
    }
};
std::ostream &operator<<(std::ostream &out, const Workshop &workshop) {
    workshop.write(out);
    return out;
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
    testing::CASE(1, 1) << "# Task konstruktor, getterek, percentage" << std::endl;
    const Task t("NTV-1", 30, 20);
    ASSERT_EQ(t.getId(), std::string("NTV-1"), "getId");
    ASSERT_EQ(t.getMax(), 30u, "getMax");
    ASSERT_EQ(t.getPoints(), 20u, "getPoints");
    ASSERT_EQ(t.percentage(), 66u, "percentage");
    ASSERT_EQ(Task("A", 10).getPoints(), 0u, "Kezdeti pontszam");
    {
      try { Task t("", 10);
        ASSERT_FAIL("Hianyzik az elvart kivetel");
      } catch (const std::invalid_argument& e) {
        ASSERT_EQ(std::string(e.what()), std::string("Invalid task"), "Kivetel uzenete");
      }
    }
    {
      try { Task t("A", 0);
        ASSERT_FAIL("Hianyzik az elvart kivetel");
      } catch (const std::invalid_argument& e) {
        ASSERT_EQ(std::string(e.what()), std::string("Invalid task"), "Kivetel uzenete");
      }
    }
    {
      try { Task t("A", 101);
        ASSERT_FAIL("Hianyzik az elvart kivetel");
      } catch (const std::invalid_argument& e) {
        ASSERT_EQ(std::string(e.what()), std::string("Invalid task"), "Kivetel uzenete");
      }
    }
    {
      try { Task t("A", 10, 11);
        ASSERT_FAIL("Hianyzik az elvart kivetel");
      } catch (const std::invalid_argument& e) {
        ASSERT_EQ(std::string(e.what()), std::string("Invalid task"), "Kivetel uzenete");
      }
    }
  }
#endif
#if 1
  {
    testing::CASE(1, 1) << "# Task prefix es postfix operatorok" << std::endl;
    Task t("A", 2);
    const Task first = t++;
    ASSERT_EQ(first.getPoints(), 0u, "Postfix eredmeny");
    ASSERT_EQ(t.getPoints(), 1u, "Postfix allapot");
    CHECK(&(++t) == &t);
    ASSERT_EQ((++t).getPoints(), 2u, "Felso hatar");
    const Task before = t--;
    ASSERT_EQ(before.getPoints(), 2u, "Postfix csokkentes eredmenye");
    ASSERT_EQ(t.getPoints(), 1u, "Postfix csokkentes allapota");
    CHECK(&(--t) == &t);
    ASSERT_EQ((--t).getPoints(), 0u, "Also hatar");
  }
#endif
#if 1
  {
    testing::CASE(1, 1) << "# Task osszetett operatorok" << std::endl;
    Task t("A", 10, 3);
    CHECK(&(t += 2u) == &t);
    (t += 1u) -= 4u;
    ASSERT_EQ(t.getPoints(), 2u, "Lancolt modositas");
    const Task& c = t;
    Task copy = c + 4u;
    ASSERT_EQ(copy.getPoints(), 6u, "Uj eredmeny");
    ASSERT_EQ(c.getPoints(), 2u, "Eredeti pontszam");
    t += std::numeric_limits<unsigned>::max();
    ASSERT_EQ(t.getPoints(), 10u, "Felso hatar");
    CHECK(&(t -= 100u) == &t);
    ASSERT_EQ(t.getPoints(), 0u, "Also hatar");
  }
#endif
#if 1
  {
    testing::CASE(1, 1) << "# Task osszehasonlitas es kiiras" << std::endl;
    const Task a("A", 3, 2), b("B", 100, 66), c("C", 10, 9);
    CHECK(a == Task("A", 10, 0)); CHECK(a != b);
    CHECK(a < b); CHECK(c < a); CHECK(!(a < a));
    ASSERT_EQ(static_cast<std::string>(a), std::string("Task[A; 2/3]"), "String konverzio");
    std::ostringstream out;
    CHECK(&(out << a) == &out);
    ASSERT_EQ(out.str(), std::string("Task[A; 2/3]"), "Stream operator");
  }
#endif
#if 1
  {
    testing::CASE(1, 1) << "# TaskBuilder" << std::endl;
    TaskBuilder builder("NTV");
    const TaskBuilder& view = builder;
    ASSERT_EQ(view.getCount(), 0u, "Kezdeti darabszam");
    const Task a = builder.newTask();
    const Task b = builder.newTask(25);
    ASSERT_EQ(a.getId(), std::string("NTV-1"), "Elso azonosito");
    ASSERT_EQ(a.getMax(), 10u, "Alapertelmezett maximum");
    ASSERT_EQ(b.getId(), std::string("NTV-2"), "Masodik azonosito");
    ASSERT_EQ(b.getMax(), 25u, "Megadott maximum");
    ASSERT_EQ(b.getPoints(), 0u, "Elert pontszam");
    {
      try { (void)builder.newTask(0);
        ASSERT_FAIL("Hianyzik az elvart kivetel");
      } catch (const std::invalid_argument& e) {
        ASSERT_EQ(std::string(e.what()), std::string("Invalid task"), "Kivetel uzenete");
      }
    }
    ASSERT_EQ(view.getCount(), 2u, "Darabszam");
    const Task next = builder.newTask();
    ASSERT_EQ(next.getId(), std::string("NTV-3"), "Kovetkezo azonosito");
  }
#endif
#if 1
  {
    testing::CASE(1, 1) << "# Workshop tarolas, referencia, kereses" << std::endl;
    Workshop w("Ora"); Task source("A", 10, 3);
    ASSERT_EQ(w.count(), 0u, "Ures muhely");
    CHECK(w.add("string", source)); ++source;
    CHECK(!w.add("string", Task("B", 10)));
    CHECK(!w.add("vector", source));
    CHECK(w.add("vector", Task("B", 10, 8)));
    w["string"] += 2u;
    const Workshop& c = w;
    const Task& found = c["string"];
    CHECK(&found == &w["string"]);
    ASSERT_EQ(found.getPoints(), 5u, "Tarolt pontszam");
    ASSERT_EQ(source.getPoints(), 4u, "Eredeti peldany");
    ASSERT_EQ(static_cast<unsigned>(c), 2u, "Unsigned konverzio");
    ASSERT_EQ(c.locateById("B"), std::string("vector"), "Kereso");
    ASSERT_EQ(c.locateById("X"), std::string("[UNKNOWN]"), "Hianyzo azonosito");
    {
      try { (void)c["X"];
        ASSERT_FAIL("Hianyzik az elvart kivetel");
      } catch (const std::out_of_range& e) {
        ASSERT_EQ(std::string(e.what()), std::string("No topic"), "Kivetel uzenete");
      }
    }
    {
      try { (void)w["X"];
        ASSERT_FAIL("Hianyzik az elvart kivetel");
      } catch (const std::out_of_range& e) {
        ASSERT_EQ(std::string(e.what()), std::string("No topic"), "Kivetel uzenete");
      }
    }
    ASSERT_EQ(w.count(), 2u, "Kereses utani darabszam");
  }
#endif
#if 1
  {
    testing::CASE(1, 1) << "# Workshop torles" << std::endl;
    Workshop w;
    w.add("a", Task("AA", 10)); w.add("b", Task("AB", 10)); w.add("c", Task("B", 10));
    CHECK(w - "c"); CHECK(!(w - "c"));
    ASSERT_EQ(w.erasePrefix("Z"), 0u, "Nincs talalat");
    ASSERT_EQ(w.erasePrefix("A"), 2u, "Torolt bejegyzesek");
    ASSERT_EQ(w.count(), 0u, "Maradek darabszam");
    w.add("a", Task("AA", 10)); w.add("b", Task("B", 10));
    ASSERT_EQ(w.erasePrefix(""), 2u, "Ures prefix");
  }
#endif
#if 1
  {
    testing::CASE(1, 1) << "# Workshop eredmenyek" << std::endl;
    Workshop w("Ora");
    w.add("x", Task("B", 100, 66)); w.add("y", Task("A", 3, 2)); w.add("z", Task("C", 10, 9));
    const Workshop& c = w;
    CHECK(&(~c) == &c["z"]);
    ASSERT_EQ(!c, 74u, "Atlagos szazalek");
    const std::vector<Task> selected = c >= 66u;
    ASSERT_EQ(selected.size(), 3u, "Kivalasztott darabszam");
    ASSERT_EQ(selected[0].getId(), std::string("C"), "Elso");
    ASSERT_EQ(selected[1].getId(), std::string("A"), "Masodik");
    ASSERT_EQ(selected[2].getId(), std::string("B"), "Harmadik");
    CHECK((c >= 101u).empty()); CHECK((c >= 90u).size() == 1);
    Workshop empty;
    {
      try { (void)~empty;
        ASSERT_FAIL("Hianyzik az elvart kivetel");
      } catch (const std::out_of_range& e) {
        ASSERT_EQ(std::string(e.what()), std::string("No task"), "Kivetel uzenete");
      }
    }
    {
      try { (void)!empty;
        ASSERT_FAIL("Hianyzik az elvart kivetel");
      } catch (const std::out_of_range& e) {
        ASSERT_EQ(std::string(e.what()), std::string("No task"), "Kivetel uzenete");
      }
    }
    CHECK((empty >= 0u).empty());
  }
#endif
#if 1
  {
    testing::CASE(1, 1) << "# Workshop reszhalmaz es masolat" << std::endl;
    Workshop w("Ora");
    w.add("map", Task("A-2", 10, 8)); w.add("string", Task("A-1", 10, 3)); w.add("vector", Task("B-1", 10, 9));
    const Workshop& c = w;
    Workshop part = c("A", 50u);
    ASSERT_EQ(part.count(), 1u, "Reszhalmaz merete");
    part["map"] += 1u;
    ASSERT_EQ(c["map"].getPoints(), 8u, "Eredeti valtozatlan");
    std::ostringstream out; out << part;
    ASSERT_EQ(out.str(), std::string("Workshop: Ora\nmap: Task[A-2; 9/10]\n"), "Reszhalmaz kiirasa");
    Workshop copy = w; --copy["vector"];
    ASSERT_EQ(w["vector"].getPoints(), 9u, "Masolt muhely fuggetlen");
    CHECK(c("", 0u).count() == 3); CHECK(c("X", 0u).count() == 0);
  }
#endif
#if 1
  {
    testing::CASE(1, 1) << "# Workshop stream" << std::endl;
    Workshop w("Ora"); w.add("vector", Task("B", 10, 8)); w.add("map", Task("A", 20, 12));
    std::ostringstream out;
    CHECK(&(out << w) == &out);
    ASSERT_EQ(out.str(), std::string("Workshop: Ora\nmap: Task[A; 12/20]\nvector: Task[B; 8/10]\n"), "Sorrend es formatum");
    Workshop empty; std::ostringstream e; empty.write(e);
    ASSERT_EQ(e.str(), std::string("Workshop: Gyakorlas\n"), "Ures muhely");
  }
#endif

  return 0;
}
#endif
