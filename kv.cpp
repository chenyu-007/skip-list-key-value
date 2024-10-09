#include<iostream>
#include<string>
#include<set>
#include<time.h>
#include<vector>
#include<random>
namespace key_value {
    template<typename K>
    struct Default_compare {
        bool  operator() (const K& key1, const K& key2) {
            return key1 < key2;
        }
    };
    template<typename K, typename V>
    struct node {
        K key;
        V value;
        node<K, V>* right, * down;
        node() :key(K()), value(V()) {
            right = nullptr;
            down = nullptr;
        }
        node(node<K, V>* _right, node<K, V>* _down, const K&_key, const V& _value) :key(_key), value(_value) {
            right = _right;
            down = _down;
        }
    };
    template<typename K, typename V, typename Compare = Default_compare<K>>
    class skip_list {
    private:
        node<K, V>* head;
        Compare compare;
        std::vector<node<K, V>*>visited_list;
    public:
        struct Iter {
            node<K, V>* p;
            Iter(node<K, V>* _p):p(_p) {}
            Iter():p(nullptr) {}
            node<K, V>* operator ->()const {
                return p;
            }
            node<K, V>& operator *()const {
                return *p;
            }
            node<K, V>*& operator ++(int) {
                node<K, V>* temp = p;
                p=p->right;
                return temp;
            }
            node<K, V>*& operator ++() {
                p = p->right;
                return p;
            }
            bool operator == (const Iter& rhs) { return rhs.p == p; }
            bool operator != (const Iter& rhs) { return !(rhs.p == p); }
        };
        skip_list(Compare cmp) :compare(cmp) {
             head = new node<K, V>();
            srand((unsigned int)time(nullptr));
        }
        skip_list() {
             head = new node<K, V>();
            srand((unsigned int)time(nullptr));
        }
        void insert(const K&key,  V value) {
            node<K, V>* p = head;
            while (p != nullptr)
            {
                while (p->right &&compare(p->right->key, key))
                {
                    p = p->right;
                }
                visited_list.push_back(p);
                p = p->down;
            }
            bool insert = true;
            node<K, V>* down_node = nullptr;
            while (insert&&visited_list.size()>0)
            {
                node<K,V>*p= visited_list.back();
                visited_list.pop_back();
                p->right = new node<K, V>(p->right,down_node,key,value);
                down_node = p->right;
                double random_value = rand() / (double)RAND_MAX;
                if (random_value > 0.5)
                    insert = false;
            }
            if (insert == true) {
                head= new node<K, V>(nullptr, head, K(), V());
            }
        }
        bool erase(const K& key) {
            node<K, V>* p = head;
            while (p) {
                while (p->right && compare(p->right->key, key)) {
                    p = p->right;
                }
                // 无效点，或者 太大， 则到下一层去
                if (!p->right || !compare(p->right->key , key))
                {
                    if (p->right&&p->right->key == key) {
                        auto temp = p->right->right;
                        free(p->right);
                        p->right = temp;
                    }
                    visited_list.push_back(p);
                    p = p->down;
                }
            }
            return true;
        };
        Iter begin() {
            node<K, V>* p = head;
            while (p != nullptr) {
                if (p->down == nullptr)
                {
                    Iter A(p->right);
                    return A;
                }
                else {
                    p = p->down;
                }
            };
        }
        Iter end() {
            node<K, V>* p = nullptr;
            Iter A(p);
            return A;
        };
        Iter find(const V& key) {
            node<K, V>* p = head;
            while (p) {
                while (p->right && compare(p->right->key, key)) {
                    p = p->right;
                }
                // 无效点，或者 太大， 则到下一层去
                if (!p->right || !compare(p->right->key , key))
                {
                    if (p->right&&p->right->key == key) break;
                    p = p->down;
                }
            }
            if(p==nullptr)  return Iter();
            p = p->right;
            while (p->down != nullptr)
            {
                p = p->down;
            }
             Iter A(p);
             return A;
        }
    };
}

    ; int main()
{
    {
        //使用lambda
        auto cmp = [](const std::string& a, const std::string& b) {return a.length() < b.length(); };
        key_value::skip_list < std::string, int, std::decay_t<decltype(cmp)>> list(cmp);
        list.insert("aab", 1321);
        list.insert("hello", 54342);
        list.insert("world", 544);
        list["aab"] = 2;
        //list.erase("aab");
        for (auto it = list.begin(); it != list.end(); it++) {
            std::cout << it->key << " " << it->value << std::endl;
        }
    }

    std::cout << "==================================" << std::endl;

    {
        //使用仿函数
        struct cmp {
            bool operator()(int a, int b) {
                return a > b;
            }
        };
        key_value::skip_list < int, int, cmp> list{};
        for (int i = 1; i <= 10; i++)
            list.insert(i+9, i+9);
        for (auto it = list.find(19); it != list.end(); it++) {
            std::cout << it->key << " " << it->value << std::endl;
        }
    }

    std::cout << "==================================" << std::endl;

    //{
    //    //可以添加 T && 实现move语义
    //    //重载 []涉及到修改索引的问题
    //}
}
