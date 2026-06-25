#ifndef LIST_HPP
#define LIST_HPP

template < class T > struct List
{
  T val;
  List< T > *next;
  List< T > *prev;
};

template < class T > List< T > *createList(T val)
{
  return new List< T >{val, nullptr, nullptr};
}

template < class T > List< T > *pushback(List< T > *tail, T val)
{
  List< T > *next = new List< T >{val, nullptr, tail};
  tail->next = next;
  return next;
}

template < class T > void clearList(List< T > *head)
{
  while (head) {
    List< T > *t = head;
    head = head->next;
    delete t;
  }
}

#endif // !LIST_HPP
