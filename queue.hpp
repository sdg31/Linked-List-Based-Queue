//Sam Goodrick sdg31@zips.uakron.edu

#ifndef QUEUE_HPP
#define QUEUE_HPP

#include "list.hpp"


template<typename T>
struct Queue : List<T>
{
  T const& front() const
  {
     return this->head->value;
  }

  T& front()
  {
    return this->head->value;
  }

  void enqueue(T const& x)
  {
    this->push_front( x );
  }

  void dequeue()
  {
    this->pop_front();
  }

  bool is_empty() const
  {
    return ( this->count == 0 );
  }

  Node<T>* parent;
  Queue();
};

template<typename T>
Queue<T>::Queue()
  :parent()
  {}

#endif
