//Sam Goodrick sdg31@zips.uakron.edu

#ifndef _LIST_HPP_
#define _LIST_HPP_

#include <iostream>
#include <initializer_list>
#include <cassert>

template<typename T>
class Node
{
public:
    //Node();
    Node( const T& x );
    T value;
    Node<T>* next;
    Node<T>* prev;
};

//iterator class that takes a node and
//can perform certain operations on a list
template<typename T>
class Iterator
{
private:
    Node<T>* current;

public:
    Iterator( Node<T>* n )
    {
        current = n;
    }
    Iterator& operator++()
    {
        //the list must have an element in order to be
        //incremented
        assert( current );
        //current is simply moved to the next element
        current = current->next;
        return *this;
    }
    Iterator& operator--()
    {
        assert( current );
        current = current->prev;
        return *this;
    }
    T& operator*()
    {
        assert( current );
        return current->value;
    }
    bool operator==( const Iterator& rhs )
    {
        //if both values exist, then there values
        //determine equality
        if( current && rhs.current )
            return( current->value == rhs.current->value );
        //if only one exists, then they are obviously not equal
        else if( ( !current && rhs.current ) || current && !rhs.current )
            return false;
        //if they're both equal to nullptr, then they're equal
        else
            return true;
    }
    bool operator!=( const Iterator& rhs )
    {
        return !( *this == rhs );
    }

    Node<T>* getNode()
    {
        return current;
    }
};

template <typename T>
class List
{
public:

    List(); //constructs empty list
    List( std::initializer_list<T> ); //constructor with a list of values
    List( const List& L );
    ~List();

    void push_back( T n ); //inserts a node containing "n" at the back
    void push_front( T n );//inserts a node containing "n" at the front
    void pop_back();//removes the last node
    void pop_front();//removes the first node
    void clear();//empties the list
    Node<T>* copy( Node<T>* p );

    bool empty();
    int size();

    T& front();
    T const& front() const;

    Iterator<T> begin();//iterator to the first node
    Iterator<T> end();//iterator to the last node
    Iterator<T> erase(Iterator<T> i);//erases chosen node
    Iterator<T> insert( Iterator<T> i, T n );//inserts a new node before the chosen node

    List<T>& operator=( List rhs );
//private:
    Node<T>* head;
    Node<T>* tail;
    int count;
};

//default constructor creates an empty list
//with head and tail equal to nullptr and count equal to 0
template<typename T>
List<T>::List()
    :head(), tail(), count()
{
}

//constructor with a list of values
template<typename T>
List<T>::List( std::initializer_list<T> list)
    :head(), tail(), count()
{
    //every element in the initializer list is pushed back
    for ( T n : list )
        push_back(n);
}

template<typename T>
List<T>::List( List const& L )
    :head(), tail( ), count()
{
    Node<T>* r = L.head;
    //every element in L is pushed back into
    //the new list
    for( int i = 0; i < L.count; i++ )
    {
        push_back( r->value );
        r = r->next;
    }
}

template<typename T>
List<T>::~List()
{
    //clear empties the list and deletes all
    //allocated memory
    //clear();
    for( int i = 0; i < size(); i++ )
      this->pop_back();
}

template<typename T>
T& List<T>::front()
{
    return head->value;
}

template<typename T>
T const& List<T>::front() const
{
    return head->value;
}

//terrible, too many cases
template<typename T>
void List<T>::push_back( T n )
{
    //if the list is empty
    //first a node is allocated, and then all relevant pointers are set
    if( !head )
    {
        head = new Node<T>(n);
        head->value = n;
        tail = head;
        tail->value = n;
        tail->prev = head;
        head->next = tail;
        head->prev = NULL;
        tail->next = NULL;
    }
    //if there is only a single element
    //both head and tail must be altered
    else if( head == tail )
    {
        tail = new Node<T>(n);
        tail->value = n;
        tail->prev = head;
        head->next = tail;
        head->prev = NULL;
        tail->next = NULL;
    }
    //if there are only two elements
    else if( head->next == tail )
    {
        Node<T>* p = new Node<T>(n);
        p->value = n;
        p->prev = tail;
        tail->next = p;
        tail = p;
        head->prev = NULL;
        tail->next = NULL;
    }
    //in all other cases, the process is the same
    //since head is not altered
    else
    {
        Node<T>* p = new Node<T>(n);
        p->value = n;
        tail->next = p;
        p->prev = tail;
        tail = p;
        head->prev = NULL;
        tail->next = NULL;
    }
    //increment count, since the size has increased by one
    ++count;
}

template<typename T>
void List<T>::push_front( T n )
{
    //if the list is empty, create a new head
    //tail == head
    if( !head )
    {
        head = new Node<T>(n);
        head->value = n;
        tail = head;
        tail->value = n;
        tail->prev = head;
        head->next = tail;
    }
    //if there is only one element, both head and tail
    //need to be modified
    else if( head == tail )
    {
        tail = new Node<T>(n);
        tail->value = head->value;
        head->value = n;
        tail->prev = head;
        head->next = tail;
    }
    //for any other amount of elements, only head needs to
    //be modified. a new node is created, head is moved forward
    //and the new object is made the head
    else
    {
        Node<T>* p = new Node<T>(n);
        p->value = n;
        head->prev = p;
        p->next = head;
        head = p;
    }
    ++count;
}

template<typename T>
void List<T>::pop_back()
{
    //the list may not be empty
    assert( head );

    //if there is only one element
    //tail->prev is inaccessible, so simply delete the head
    if( head == tail )
    {
        delete head;
        head = NULL;
        tail = NULL;
    }
    else
    {
        //the tail is now the second-to-last node
        //the second to last node's next (the old tail) is deleted
        //tail->next is set to null because no such thing exists
        tail = tail->prev;
        delete tail->next;
        tail->next = NULL;
    }
    //size is decremented by one node
    --count;
}

template<typename T>
void List<T>::pop_front()
{
    assert( head );
    //in the case that there is one element,
    //simply delete the head
    if( head == tail )
    {
        delete head;
        head = nullptr;
        tail = nullptr;
    }
    //otherwise move the head up one element
    //then delete the previous element
    else
    {
        head = head->next;
        delete head->prev;
        head->prev = NULL;
    }
    --count;
}

template<typename T>
void List<T>::clear()
{
    Node<T>* p = head;
    if( !p ) //if the list is empty, do nothing
        return;
    while( p ) //while the list is not empty
    {
        Node<T>* q = p->next;
        delete p;
        p = q;
        --count;
    }
    //setting head to nullptr here assures that the clear function
    //recognizes the list is empty if called multiple times
    head = nullptr;
}

template<typename T>
Node<T>* List<T>::copy( Node<T>* p )
{

}

template<typename T>
int List<T>::size()
{
    return count;
}

template<typename T>
bool List<T>::empty()
{
    return( size() == 0 );
}

template<typename T>
Iterator<T> List<T>::begin()
{
    return Iterator<T>( head );
}

template<typename T>
Iterator<T> List<T>::end()
{
    if( tail )
    {
        tail->next = nullptr;
        return Iterator<T>( tail->next );
    }
    else
    {
        return Iterator<T>( head );
    }
}

template<typename T>
Iterator<T> List<T>::erase( Iterator<T> i )
{
    Node<T>* n = i.getNode();
    assert( head );

    if( head == tail )
    {
        delete head;
        head = NULL;
        tail = NULL;
        --count;
    }
    else if( n == head && head != tail )
    {
        pop_front();
    }
    else if( n == tail && head != tail )
    {
        pop_back();
    }
    else if( n->next == tail && n->prev == head )
    {
        head->next = tail;
        tail->prev = head;
        delete n;
        --count;
    }
    else
    {
        n->next = n->next->next;
        std::cout << n->next->value << std::endl;
        n->prev->next = n->next;
        delete n;
        --count;
    }

}

template<typename T>
Iterator<T> List<T>::insert( Iterator<T> i, T n )
{

    Node<T>*x = i.getNode();
    //if there is zero or one element(s), then push back and push front
    //serve the needed functionality
    if( !head )
    {
        push_back( n );
        return head;
    }
    if( x == head )
    {
        push_front( n );
        return head;
    }
    //if there are only two nodes in the list, both must be modified
    else if( x == head->next )
    {
        x = new Node<T>;
        x->value = n;
        x->prev = head;
        x->next = head->next;
        head->next->prev = x;
        head->next = x;
        count++;
        return x;
    }
    //if x is the second to last element, tail needs to be modified
    else if( x == tail->prev )
    {
        x = new Node<T>;
        x->value = n;
        x->next = tail;
        x->prev = tail->prev;
        tail->prev->next = x;
        tail->prev = x;
        count++;
        return x;
    }
    //if x is the end of the list, the element
    //is pushed back
    else if( x == tail->next )
    {
        push_back( n );
        return tail;
    }
    //otherwise, a node is simply created and sewn in
    else
    {
        x = new Node<T>;
        Node<T>* p = i.getNode();
        x->value = n;
        x->next = p;
        x->prev = p->prev;
        x->next->prev = x;
        x->prev->next = x;
        count++;
        return x;
    }
}

//copy constructor
template<typename T>
List<T>& List<T>::operator=( List rhs )
{
    if( rhs.size() == size() ) //if the sizes are equal, simply copy the values over
    {
        Node<T>* r = rhs.head;
        Node<T>* l = head;
        for( int i = 0; i < size(); i++ )
        {
            l->value = r->value;
            r = r->next;
            l = l->next;
        }
    }
    else if( size() < rhs.size() )
    {
        //it may be more efficient to first copy what already exists and then move on,
        //but this way is easier to implement
        clear();

        Node<T>* r = rhs.head;
        for( int i = 0; i < rhs.count; i++ )
        {
            push_back( r->value );
            r = r->next;
        }
    }
    else
    {
        //case 3: copying a smaller list
        //first, make the sizes equal so that the values can be easily copied
        while( size() != rhs.size() )
            pop_back();
        Node<T>* r = rhs.head;
        Node<T>* l = head;
        for( int i = 0; i < size(); i++ )
        {
            l->value = r->value;
            r = r->next;
            l = l->next;
        }
    }
}

//node default constructor
//next and prev are null, since they will be set manually
//in the list class
//template<typename T>
//Node<T>::Node()
//    :next( nullptr ), prev( nullptr ), value()
//{
//}

template<typename T>
Node<T>::Node( const T& x )
    :next( nullptr ), prev( nullptr ), value( x )
{
}

#endif
