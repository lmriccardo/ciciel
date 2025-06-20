#include <iostream>
#include <cassert>
#include <patterns/patterns.hpp>

using namespace ccl::dp::signals;

class SimpleLogger : public std::enable_shared_from_this<SimpleLogger>
{
public:
    void display(const std::string& content)
    {
        std::cout << "Logger::display Called succesfully " 
                  << "with content " << content
                  << std::endl;
    }

    std::shared_ptr<SimpleLogger> toShared()
    {
        return this->shared_from_this();
    }
};

class SignalEmitter
{
public:
    Signal<const std::string&> onSignalEmitted;

    void simulateSignal()
    {
        onSignalEmitted.emit( "Hello from Emitted signal!" );
    }
};

int main()
{
    SignalEmitter se;
    
    {
        auto sl_ptr = std::make_shared<SimpleLogger>();
        se.onSignalEmitted.connect(
            make_callback( &SimpleLogger::display, sl_ptr ),
            sl_ptr
        );

        auto lambda_conn = se.onSignalEmitted.connect(
            []( const std::string& text )
            {
                std::cout << "From Lambda: " << text << std::endl;
            }
        );

        assert( lambda_conn.isConnected() && "Lambda is not connected!!!" );
        assert( se.onSignalEmitted.getNofConnections() == 2 && "Nof connections != 2!!" );

        std::cout << "Emitting signal First time" << std::endl;
        se.simulateSignal();

        std::cout << "Manual Disconnection of lambda" << std::endl;
        lambda_conn.disconnect();
        assert( !lambda_conn.isConnected() && "Lambda is still connected!!!" );
        assert( se.onSignalEmitted.getNofConnections() == 1 && "Nof connections != 1!!" );

        std::cout << "Emitting signal Second time" << std::endl;
        se.simulateSignal();
    }

    std::cout << "Exiting the SimpleLogger scope" << std::endl;
    std::cout << "Emitting signal Third time" << std::endl;
    se.simulateSignal();
    assert( se.onSignalEmitted.getNofConnections() == 0 && "Nof connections != 0!!" );

    return 0;
}