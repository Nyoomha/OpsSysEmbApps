// Neha Khan 11/5/2023
// g++ thinkcell.cpp -o thinkcell
// ./thinkcell

#include <iostream>
#include <map>

template <typename K, typename V> 
class interval_map {
    friend void IntervalMapTest();
    V m_valBegin;
    std::map<K, V> m_map;

public:
    // constructor associates the whole range of K with val
    interval_map(V const &val) : m_valBegin(val) {}

    void assign(K const &keyBegin, K const &keyEnd, V const &val) 
    {
        if (!(keyBegin < keyEnd)) 
        {
            return;
        }
    
        typename std::map<K, V>::iterator starting = m_map.lower_bound(keyBegin);
        typename std::map<K, V>::iterator ending = m_map.upper_bound(keyEnd);
        
        if (starting != m_map.begin() && (--starting)->second > val) {
            return;
        }

        // Erase intervals within the specified range
        m_map.erase(starting, ending);

        // Insert the new interval
        m_map[keyBegin] = val;
        m_map[keyEnd] = m_valBegin;

    // Look-up of the value associated with key
    V const& operator[](K const& key) const {
        auto it = m_map.upper_bound(key);
        if (it == m_map.begin()) {
            return m_valBegin;
        } else {
            return (--it)->second;
        }
    }
};

void IntervalMapTest() 
{
    interval_map<int, char> M('A');

    
    // Assign intervals
    M.assign(1, 3, 'B');
    M.assign(5, 8, 'C');
    M.assign(10, 15, 'D');

    // Look up values for specific keys
    for (int key = -2; key <= 20; ++key) {
        std::cout << key << " -> " << M[key] << std::endl;
    }
}
    // Implement the test function if needed


int main() {
    // Implement the main function if needed
    IntervalMapTest();
    return 0;
}
