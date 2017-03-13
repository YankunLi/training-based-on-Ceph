#ifndef _ADMINSERVER_H_
#define _ADMINSERVER_H_

#include <string>

#include "Thread.h"

class AdminServer: public Thread
{
    public:
        void usage();

        AdminServer();
        virtual ~AdminServer();

        bool init(std::string &path);
        std::string bind_and_listen(std::string &path, int *fd);
        bool do_accept();
        void *entry();
        void shutdown();

    private:
        std::string s_path;
        int s_sock_fd;

};

#endif
