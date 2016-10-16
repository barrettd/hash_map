# hash_map

Simple substitute for std::unordered_map

I had compile errors relating to std::map and std::unordered_map.
Rather than hacking the std:: headers or messing with #defines, I decided to make a few templates that suited my needs.

HashMap

The table size of the HashMap can be configured when calling the constructor.

Note that I still use the std::hash function, as it works well for me.

This HashMap does not allow duplicates, so I suppose it really should be called HashSet.

IndexMap

IndexMap is a small extension to HashMap that I use to create a set of unique items.  
Each key item is assigned a unique sequential id during insertion.  Duplicates are not allowed.
