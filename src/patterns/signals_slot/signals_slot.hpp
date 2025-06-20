#pragma once

#include <map>
#include <vector>
#include <memory>
#include <functional>
#include <algorithm>
#include <mutex>
#include <cstddef>
#include <type_traits>

namespace ccl::dp::signals
{
    /**
     * Generate a monotonic increasing connection unique identifiers
     * for connections handling.
     */
    inline size_t nextConnectionId()
    {
        static size_t connection_id = 0;
        return ++connection_id;
    }

    // Trait to extract the class type from a member function pointer
    template <typename T>
    struct member_function_class;

    template <typename ClassType, typename ReturnType, typename... Args>
    struct member_function_class<ReturnType (ClassType::*)(Args...)> {
        using type = ClassType;
    };

    template <typename ClassType, typename ReturnType, typename... Args>
    struct member_function_class<ReturnType (ClassType::*)(Args...) const> {
        using type = ClassType;
    };

    /**
     * Creates a callback wrapper that holds a weak reference to an object and
     * safely invokes a member function on that object if it is still alive.
     * 
     * This function accepts a member function pointer and a shared pointer to an object
     * and returns a callable lambda that, when invoked, attempts to lock the weak pointer
     * to the object. If successful, it calls the member function otherwise do nothing.
     * 
     * @tparam T The class type of the object owning the member function
     * @tparam _Callable Type of the member function pointer
     * 
     * @param func The member function pointer to invoke on the object
     * @param ptr  Shared pointer to the object instance
     * 
     * @return The callable lambda that can safely be invoked
     */
    template <typename T, typename _Callable>
    auto make_callback( _Callable&& func, std::shared_ptr<T> ptr )
    {
        using func_type = std::remove_reference_t<_Callable>;

        // Check if the function is actually a member function of T
        static_assert( std::is_member_function_pointer_v<func_type>,
            "Input function must be a member function pointer" );

        using class_type = typename member_function_class<func_type>::type;
        
        static_assert( std::is_base_of_v<class_type, T>,
            "Input function must be a member function of T (or its base)" );

        std::weak_ptr<T> wptr = ptr;
        return [ wptr, func = std::forward<_Callable>(func)](auto&&... args)
        {
            if ( std::shared_ptr<T> sptr = wptr.lock() )
            {
                ( sptr.get()->*func )( std::forward<decltype(args)>(args)... );
            }
        };
    }

    // Forward declaration of the Connection class
    template <typename... Args>
    class Connection;

    // --------------------------------------------------
    // ---------- SIGNAL CLASS DECLARATION --------------
    // --------------------------------------------------
    /**
     * 
     */
    template <typename... Args>
    class Signal
    {
        friend class Connection<Args...>; // Used for auto-disconnection

    private:

        using conn_type = Connection<Args...>;
        using slot_type = std::function<void(Args...)>;
        using lifetime_ptr = std::shared_ptr<void>;
        using lifetime_token = std::weak_ptr<void>;

        struct slot_info
        {
            slot_type callback;   // The actual callback
            lifetime_token token; // Manages the lifetime token
        };

        using conn_container = std::map<size_t, slot_info>;

        std::shared_ptr<conn_container> m_connections; // Contains connections with the callback
        static std::mutex m_mutex;

    public:
        Signal() : m_connections( std::make_shared<conn_container>() ) {};
        
        /**
         * Connect a slot to the current signal. The connection is managed
         * either using a lifetime token or manually.
         * 
         * @param callback The callback to run when emitting the signal
         * @param lf_token The lifetime token to handle disconnection
         * 
         * @return The created connection
         */
        conn_type connect( slot_type callback, lifetime_ptr lf_token = nullptr );

        /**
         * Emits the signal with the input arguments.
         */
        void emit( Args... args );

        /**
         * Returns the total number of active connections
         * @return The size of the connection map
         */
        size_t getNofConnections() const;
    };

    template <typename... Args>
    std::mutex Signal<Args...>::m_mutex;

    // --------------------------------------------------
    // ---------- CONNECTION CLASS DECLARATION ----------
    // --------------------------------------------------
    /**
     * 
     */
    template <typename... Args>
    class Connection
    {
        friend class Signal<Args...>; // For accessing the signal mutex

    private:
        using signal_type = Signal<Args...>;
        using signal_slots_type = std::weak_ptr<typename signal_type::conn_container>;
        using lifetime_token = typename signal_type::lifetime_token;

        size_t m_conn_id;
        signal_slots_type m_weak_slots;
        bool m_has_token;

        // Private constructor only accessible by the Signal class itself
        Connection( size_t id, signal_slots_type slots_wptr, bool token ) 
            : m_conn_id( id ), m_weak_slots( slots_wptr ), m_has_token( token ) {};

    public:
        Connection() : m_conn_id( 0 ) {}; // Invalid connection constructor

        /**
         * Manual disconnection from the Signal.
         */
        void disconnect();

        /**
         * Returns true if the current connection is still alive. In this context
         * a connection is alive if the identifier != 0, the weak pointer to the
         * connection map is still valid and the lifetime token has not expired.
         */
        bool isConnected() const;

        /**
         * Returns the connection id
         */
        size_t getConnectionId() const;
    };

    // --------------------------------------------------
    // ---------- CONNECTION CLASS DEFINITION -----------
    // --------------------------------------------------
    template <typename... Args>
    inline void Connection<Args...>::disconnect()
    {
        if ( auto signal_slots = m_weak_slots.lock() )
        {
            std::lock_guard<std::mutex> _lock( signal_type::m_mutex );
            signal_slots->erase( m_conn_id );
            m_conn_id = 0;
        }
    }

    template <typename... Args>
    inline bool Connection<Args...>::isConnected() const
    {
        // First, return False if the connection id is 0 or if the weak pointer
        // to the signal slot is nullptr, meaning the Signal went out of scope
        // or freed
        if ( m_conn_id == 0 || m_weak_slots.lock() == nullptr ) return false;

        // Finally, check for the presence of the current connection id
        // in the signal map and in case if the token has expired
        auto it = m_weak_slots.lock()->find( m_conn_id );
        if ( it == m_weak_slots.lock()->end() ) return false;
        return !m_has_token || !it->second.token.expired();
    }

    template <typename... Args>
    inline size_t Connection<Args...>::getConnectionId() const
    {
        return m_conn_id;
    }

    // --------------------------------------------------
    // ---------- SIGNAL CLASS DEFINITION ---------------
    // --------------------------------------------------
    template <typename... Args>
    inline Signal<Args...>::conn_type Signal<Args...>::connect(
        slot_type callback, lifetime_ptr lf_token)
    {
        std::lock_guard<std::mutex> _lock( m_mutex );
        size_t conn_id = nextConnectionId();
        (*m_connections).insert_or_assign( conn_id, 
                slot_info{ std::move( callback ), lf_token } );

        return conn_type( conn_id, m_connections, lf_token != nullptr );
    }

    template <typename... Args>
    inline void Signal<Args...>::emit( Args... args )
    {
        std::vector< slot_type > slots_to_call;

        // Before calling all the callbacks, we need to handle invalid
        // connections which are still in the connection map
        {
            std::lock_guard<std::mutex> _lock( m_mutex );
            auto conn_it = m_connections->begin();
            for ( ; conn_it != m_connections->end() ; )
            {
                const lifetime_token lf_token = conn_it->second.token;
                if ( lf_token.expired() )
                {
                    // If the token has expired this means that the object
                    // pointed by the weak_pointer is no longer valid.
                    conn_it = m_connections->erase( conn_it );
                    continue;
                }

                slots_to_call.push_back( conn_it->second.callback );
                conn_it++;
            }
        }

        // At this point, we can call the callbacks
        for ( auto& slot: slots_to_call )
        {
            slot( args... );
        }
    }

    template <typename... Args>
    inline size_t Signal<Args...>::getNofConnections() const
    {
        return m_connections->size();
    }
}