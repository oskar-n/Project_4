#include <SFML/Graphics.hpp>
#include <queue>
#include <set>
#include <unordered_map>

template <typename T>
concept Container = requires(T t) {
  { t.begin() } -> std::same_as<typename T::iterator>;
  { t.end() } -> std::same_as<typename T::iterator>;
};

template <typename T> void draw(T &Object, sf::RenderWindow &window) {
  Object.draw(window);
}

template <typename Key, typename Val>
void draw(std::unordered_map<Key, Val> &Object_cont, sf::RenderWindow &window) {
  for (auto &[key, value] : Object_cont) {
    draw(value, window);
  }
}

template <typename T> void draw(T *&Object, sf::RenderWindow &window) {
  if (Object != nullptr)
    Object->draw(window);
}

template <Container C> void draw(C &Object_cont, sf::RenderWindow &window) {
  for (auto &i : Object_cont) {
    draw(i, window);
  }
}

template <typename... Args> void draw(sf::RenderWindow &win, Args &...args) {
  (draw(args, win), ...);
}

template <typename Val> class UniqueQueue {
  std::deque<Val> cont;
  std::set<std::size_t> set;

public:
  constexpr bool add(Val const &val) {
    auto hash = val;
    if (set.insert(hash).second) {
      cont.push_back(val);
      return true;
    }
    return false;
  }
  constexpr void pop_front() {
    auto val = cont.front();
    cont.pop_front();
    set.erase(val);
  }
  constexpr auto &operator[](std::size_t i) const { return cont[i]; }
  constexpr auto size() const { return cont.size(); }
  constexpr auto begin() { return cont.begin(); }
  constexpr auto end() { return cont.end(); }
};
