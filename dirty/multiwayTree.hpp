//@file: N叉书

#ifndef  MULTIWAYTREE_INC
#define  MULTIWAYTREE_INC

#include <string>
#include <unordered_map>

class MultiwayTree
{
 public:
  typedef std::unordered_map<std::string, MultiwayTree*>::iterator       iterator;
  typedef std::unordered_map<std::string, MultiwayTree*>::const_iterator const_iterator;
  typedef std::unordered_map<std::string, MultiwayTree*>::value_type     value_type;
  typedef std::unordered_map<std::string, MultiwayTree*>::key_type       key_type;
  typedef std::unordered_map<std::string, MultiwayTree*>::mapped_type    T;

  ~MultiwayTree()
  {
    for (auto &pair : _t) {
      if (pair.second != nullptr) {
        delete pair.second;
        pair.second = nullptr;
      }
    }
    _t.clear();
  }

  std::pair<iterator, bool> insert(const key_type &key)
  {
    return _t.insert(std::make_pair(key, nullptr));
  }

  std::pair<iterator, bool> insert(const value_type &value)
  {
    return _t.insert(value);
  }

  iterator find(const key_type &key) {
    return _t.find(key);
  }

  const_iterator find(const key_type &key) const {
    return _t.find(key);
  }

  T& operator[](const key_type &key) {
    return _t[key];
  }

  T& operator[](key_type &&key) {
    return _t[key];
  }

  bool empty() const {
    return _t.empty();
  }

  size_t size() const {
    return _t.size();
  }

  iterator begin() {
    return _t.begin();
  }

  const_iterator begin() const {
    return _t.cbegin();
  }

  const_iterator cbegin() const {
    return _t.cbegin();
  }

  iterator end() {
    return _t.end();
  }

  const_iterator cend() const {
    return _t.cend();
  }

  const_iterator end() const {
    return _t.cend();
  }


  std::unordered_map<std::string, MultiwayTree*> _t;
};

////for debug TODO 答应全部 (只打印第1层）
//void nTree::showTable(int level) const;
//{
//  for (auto &pair : _t) {
//    printf("key:%s\n", pair.first.c_str());
//  }
//}


#endif   /* ----- #ifndef multiwaytree_INC  ----- */
