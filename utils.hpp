#include <iostream>
#include <set>
#include <vector>
#include <algorithm>
#include <queue>


template<typename Val, typename HashGen = std::hash<Val>>
class UniqueQueue{
  std::deque<Val> cont; 
  std::set<std::size_t> set;
public:
  constexpr bool add(Val const &val){
    auto hash = HashGen{}(val);
    if(set.insert(hash).second)
    {
      cont.push_back(val);
      return true;
    }
    return false;
  }
  constexpr void pop_front(){
    auto val = cont.front();
    cont.pop_front();
    set.erase(HashGen{}(val));
  }
  constexpr auto& operator[](std::size_t i) const { return cont[i]; }
  constexpr auto size() const { return cont.size(); }
  constexpr auto begin() { return cont.begin(); }
  constexpr auto end() { return cont.end(); }
};
