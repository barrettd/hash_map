// -----------------------------------------------------------------------------
//  HashMap.h
//
//  Created by Barrett Davis on 8/8/16.
//  Needed because std::map and std::unordered_map have compile time errors.
//  Copyright © 2016 Tree Frog Software. All rights reserved.
// -----------------------------------------------------------------------------
// The HashMap contains key,value pairs.
//
// This HashMap ignores duplicates.  I guess that fact really makes it a HashSet.
// If you want HashMap to allow duplicates, you can modify the add() method, or 
// create an additional additional method.
//
// This HashMap consists of an array that contains pointers to nodes.
// A hash function is used to generate an reproducible index into this array.
// The node found at an array index may form a singly linked list of nodes.
// Each list of nodes are independent.
//
// To find if a value is present in our HashMap, we generate an index into the array using
// the hash function.  Then we traverse through the singly linked list of nodes, looking 
// for a value that matches the target value.  We we reach the end of the node list, then
// the value is not present in our collection.
// -----------------------------------------------------------------------------
#ifndef HashMap_h
#define HashMap_h

#include <functional>       // Needed for std::hash
#include <cstring>          // Needed for memset()

namespace tfs {
    
    const size_t DEFAULT_HASH_MAP_TABLE_SIZE = 512;
    
    template< class Key, class T > class HashNode {
    public:
        Key key;
        T   value;
        HashNode<Key,T> *next;	// This points to the next node in the list.
    public:
        HashNode( Key key, T value ) : key( key ), value( value ),next(0) {}
        void deleteValue( void ) { delete value; value = 0; }   // Only needed if T is a pointer.
    };

    template< class Key, class T >
    using HashNodePtr = HashNode<Key,T>*;
    
    
    template< class Key, class T > class HashMap {
    protected:
        HashNode< Key, T > **m_table;        // Hash table
        size_t                m_table_size;
        size_t                m_size;        // Count of nodes in the HashMap
    public:
        HashMap( size_t tableSize = DEFAULT_HASH_MAP_TABLE_SIZE ) : // Best to keep table size a power of two.
        m_table_size( tableSize ),
        m_size( 0 ) {
            if( m_table_size < 1 ) {
                m_table_size = DEFAULT_HASH_MAP_TABLE_SIZE;
            }
            m_table = new HashNode<Key,T>*[m_table_size];
        }
        ~HashMap( void ) {
            clear();
            delete[] m_table;
            m_table = 0;
        }
        
        inline size_t size(void)  const noexcept { return m_size; }
        inline bool   isEmpty(void) const noexcept { return m_size == 0; }
        
        inline size_t hashIndex( Key key ) const noexcept { return std::hash<Key>()( key ) % m_table_size; }

        void clear( void ) noexcept {
            const size_t tableSize = m_table_size;
            for( size_t ii = 0; ii < tableSize; ii++ ) {
                HashNode<Key,T> *entry = m_table[ii];
                if( entry != 0 ) {
                    do {
                        HashNode<Key,T> *next = entry->next;
                        delete entry;
                        entry = next;
                    } while( entry != 0 );
                    m_table[ii] = 0;
                }
            }
            m_size = 0;
        }
        
        void deleteAll( void ) noexcept {
            // Same as clear, except that we assume that class T is a pointer to an object that needs delete
            const size_t tableSize = m_table_size;
            for( size_t ii = 0; ii < tableSize; ii++ ) {
                HashNode<Key,T> *entry = m_table[ii];
                if( entry != 0 ) {
                    do {
                        HashNode<Key,T> *next = entry->next;
                        entry->deleteValue();
                        delete entry;
                        entry = next;
                    } while( entry != 0 );
                    m_table[ii] = 0;
                }
            }
            m_size = 0;
        }
        
        HashNode<Key,T> *find( Key key ) const noexcept {
            const size_t index = hashIndex( key );
            HashNode<Key,T> *entry = m_table[index];
            while( entry != 0 ) {
                if( entry->key == key ) {
                    return entry;
                }
                entry = entry->next;
            }
            return 0;
        }
        
        void add( Key key, T value ) {
            const size_t index = hashIndex( key );
            HashNode<Key,T> *entry = m_table[index];
            while( entry != 0 ) {
                if( entry->key == key ) {
                    entry->value = value;
                    return;
                }
                entry = entry->next;
            }
            entry = new HashNode<Key,T>( key, value );
            entry->next = m_table[index];
            m_table[index] = entry;
            m_size++;
        }
        
        void remove( Key key ) {
            const size_t index = hashIndex( key );
            HashNode<Key,T> *previous = 0;
            HashNode<Key,T> *entry    = m_table[index];
            while( entry != 0 ) {
                if( entry->key == key ) {
                    if( previous == 0 ) {
                        m_table[index] = entry->next;
                    } else {
                        previous->next = entry->next;
                    }
                    delete entry;
                    m_size--;
                    return;
                } else {
                    previous = entry;
                    entry = entry->next;
                }
            }
        }
        
    };

    
}   // namespace tfs


#endif /* HashMap_h */
