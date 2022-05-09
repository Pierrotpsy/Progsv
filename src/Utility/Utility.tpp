/*
 * infosv
 * jan 2013
 * Marco Antognini
 */

#include <algorithm>
#include <list>
#include <vector>

using std::list;
using std::vector;


template <typename C, typename D>
void append(C const& src, D& dest)
{
    append(src.begin(), src.end(), dest);
}

template <typename C, typename InputIt>
void append(InputIt const first, InputIt const last, C& dest)
{
    std::copy(first, last, std::back_inserter(dest));
}

template <typename Map, typename F>
void map_erase_if(Map& m, F pred)
{
    for (typename Map::iterator i = m.begin();
         (i = std::find_if(i, m.end(), pred)) != m.end();
         m.erase(i++));
}


template <class T>
void clearPointerList(list<T*>& items) {
  while (!items.empty()) {
    delete items.front();
    items.pop_front();
  }
}

template <class T>
vector<T> filterImmutableList(const list<T>& input, const std::function<bool (T)>& predicate) {
  vector<T> output;

  std::copy_if(
      input.begin(),
      input.end(),
      std::back_inserter(output),
      predicate);

  return output;
}

template <class T>
void filterPointerList(list<T*>& items, const std::function<bool (const T*)>& predicate) {
  for (T*& item : items) {
    if (!predicate(item)) {
      delete item;
      item = nullptr;
    }
  }

  items.erase(std::remove(items.begin(), items.end(), nullptr), items.end());
}
