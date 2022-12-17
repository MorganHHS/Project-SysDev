#ifndef GET_ACTION_H
#define GET_ACTION_H

#include <map>

template<class K, class V>
bool get_action(std::map<K,V> *src_map, K key, V *handler)//Check if key is in the map 
{
    try
    {
        *handler = src_map->at(key);
        return true;
    } catch (const std::out_of_range& e)
    {
       return false; 
    }
}

#endif // !GET_ACTION_H

