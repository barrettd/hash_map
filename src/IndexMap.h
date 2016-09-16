//
//  IndexMap.h
//
//  Created by Barrett Davis on 8/10/16.
//  Copyright © 2016 Tree Frog Software. All rights reserved.
//

#ifndef IndexMap_h
#define IndexMap_h

#include "HashMap.h"

namespace tfs {
    
    template< class Key > class IndexMap : public HashMap<Key, unsigned long> {
        
    public:
        IndexMap( size_t tableSize = DEFAULT_HASH_MAP_TABLE_SIZE ) :
        HashMap<Key, unsigned long>( tableSize ) {
        }
        
        unsigned long add( Key key ) {
            // Returns the index of the mapped key.
            const size_t index = HashMap<Key, unsigned long>::hashIndex( key );
            HashNode<Key,unsigned long> *entry = HashMap<Key, unsigned long>::m_table[index];
            while( entry != 0 ) {
                if( entry->key == key ) {
                    return entry->value;
                }
                entry = entry->next;
            }
            const unsigned long value = HashMap<Key, unsigned long>::m_size;
            entry = new HashNode<Key,unsigned long>( key, value );
            entry->next = HashMap<Key, unsigned long>::m_table[index];
            HashMap<Key, unsigned long>::m_table[index] = entry;
            HashMap<Key, unsigned long>::m_size++;
            return value;
        }
        
    };

    
}   // namespace tfs

#endif /* IndexMap_h */

