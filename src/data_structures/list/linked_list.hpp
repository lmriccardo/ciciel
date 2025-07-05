#pragma once

#include <memory>

namespace ccl::ds::list
{
    // Forward declaration of the linked list class
    template <typename T>
    class LinkedList;

    template <typename T>
    class LinkedListNode
    {
        friend class LinkedList<T>;

    private:
        T m_value;
        std::shared_ptr<LinkedListNode> m_next;

        void setNext( const std::shared_ptr<LinkedListNode<T>>& next )
        {
            m_next = next;
        }

        std::shared_ptr<LinkedListNode> getNext()
        {
            return m_next;
        }
        
        LinkedListNode( const T& value ) : m_value( value ) {};

    public:
        LinkedListNode() = delete; // Delete the default constructor
        ~LinkedListNode() = default;

        const T& get() const { return m_value; }
        T& get() { return m_value; }
        void set( const T& value ) { m_value = value; }
    };

    template <typename T>
    class LinkedList
    {
    private:
        using node_type = typename LinkedListNode<T>;
        
        std::shared_ptr<node_type> m_head = nullptr;
        std::shared_ptr<node_type> m_tail = nullptr;

        size_t m_currSize = 0;

    public:
        ~LinkedList() = default;

        /**
         * Add a new element at the front of the linked list
         * @param value The input value
         */
        void pushFront( const T& value )
        {
            // Create the new head with the input value
            auto head = std::make_shared<node_type>( value );
            if ( m_head != nullptr ) head->setNext( m_head );
            m_head = head;
            m_currSize++;
        }

        /**
         * Add a new element at the end of the linked list
         * @param value The input value
         */
        void pushBack( const T& value )
        {
            auto tail = std::make_shared<node_type>( value );
            m_currSize++;
            
            // When pushing back it is important to check if the current
            // head of the linked list is nullptr or not.
            if ( m_head == nullptr )
            {
                m_head = tail;
                return;
            }

            if ( m_tail == nullptr )
            {
                m_tail = tail;
                m_head->setNext( m_tail );
                return;
            }

            m_tail->setNext( tail );
            m_tail = tail;
        }
    };
}