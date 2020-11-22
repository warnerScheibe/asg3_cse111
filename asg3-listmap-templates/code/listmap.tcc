// $Id: listmap.tcc,v 1.15 2019-10-30 12:44:53-07 - - $

#include "listmap.h"
#include "debug.h"

//
/////////////////////////////////////////////////////////////////
// Operations on listmap.
/////////////////////////////////////////////////////////////////
//

//
// listmap::~listmap()
//
template <typename key_t, typename mapped_t, class less_t>
listmap<key_t,mapped_t,less_t>::~listmap() {
   DEBUGF ('l', reinterpret_cast<const void*> (this));
   
}

//
// iterator listmap::insert (const value_type&)
//
template <typename key_t, typename mapped_t, class less_t>
typename listmap<key_t,mapped_t,less_t>::iterator
listmap<key_t,mapped_t,less_t>::insert (const value_type& pair) {
   DEBUGF ('l', &pair << "->" << pair);
   node* new_node;
   if(empty())
   {
      new_node = new node(anchor(), anchor(), pair);
      anchor() -> prev -> next = new_node;
      anchor() -> prev = new_node;
      iterator new_it = iterator(new_node);
      return new_it;
   }

   for (auto it = begin(); it != end(); ++it)
   {
      if(less(pair.first, it -> first))
      {
         new_node = new node(it.where, it.where -> prev, pair);
         it.where -> prev -> next = new_node;
         it.where -> prev = new_node;
         iterator new_it = iterator(new_node);
         return new_it;
      }
      else if(pair.first == it -> first)
      {
         it -> second = pair.second;
         return it;
      }
   }

   new_node = new node(anchor(), anchor() -> prev, pair);
   anchor() -> prev -> next = new_node;
   anchor() -> prev = new_node;
   iterator new_it = iterator(new_node);
   return new_it;
}

//
// listmap::find(const key_type&)
//
template <typename key_t, typename mapped_t, class less_t>
typename listmap<key_t,mapped_t,less_t>::iterator
listmap<key_t,mapped_t,less_t>::find (const key_type& that) {
   DEBUGF ('l', that);
   iterator it = begin();
   for( ; it != end() ; ++it )
   {
      if (it -> first == that)
      {
         break;
      }
   }
   return it;
}

//
// iterator listmap::erase (iterator position)
//
template <typename key_t, typename mapped_t, class less_t>
typename listmap<key_t,mapped_t,less_t>::iterator
listmap<key_t,mapped_t,less_t>::erase (iterator position) {
   DEBUGF ('l', &*position);
   position.where -> next -> prev = position.where -> prev;
   position.where -> prev -> next = position.where -> next;
   iterator new_it = position.where -> next;
   delete(position.where);
   return new_it;
}


template <typename key_t, typename mapped_t, class less_t>
typename listmap<key_t,mapped_t,less_t>::iterator
listmap<key_t,mapped_t,less_t>::print_list() {

  for (auto it = this->begin(); it != this->end(); ++it)
  {
     cout << *it << endl;
  }
  
  return iterator();
  
  
  
  
  
  
  
}
