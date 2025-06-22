#include <sys/epoll.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <cli/ui/screen/terminal.hpp>
#include <cli/ui/events/keys.hpp>

using namespace ccl::cli::ui;

int main()
{
    Terminal::getInstance().enableMouseEvents();

    int epfd = epoll_create1(0);
    struct epoll_event event;
    event.events = EPOLLIN; // ready to read
    event.data.fd = STDIN_FILENO;

    epoll_ctl(epfd, EPOLL_CTL_ADD, STDIN_FILENO, &event);

    struct epoll_event events[10];
    printf("Waiting for input (Ctrl+D to quit)...\n");

    bool exit = false;
    while (!exit) {
        int n = epoll_wait(epfd, events, 1, -1);
        for (int i = 0; i < n; i++) {
            if (events[i].data.fd == STDIN_FILENO) {
                char buf[100];
                int r = read(STDIN_FILENO, buf, sizeof(buf));
                if (r <= 0) return 0;
                buf[r] = 0;

                if ( buf[0] == 3 )
                {
                    exit = true;
                    break;
                }

                for (char* p = buf; *p; ++p) {
                    printf("\\x%02X", (unsigned char)*p);
                }
                
                printf( "\n" );
                fflush( stdout );
            }
        }
    }
}