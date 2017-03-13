/*
 * Training based on Ceph
 *
 * Author: Yankun Li <lioveni99@gmail.com>
 *
 */
#include <string>
#include <poll.h>
#include <set>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>
#include <fcntl.h>
#include <sstream>
#include <errno.h>
#include <stdio.h>

using std::ostringstream;

#include "AdminServer.h"

AdminServer::AdminServer()
{}

AdminServer::~AdminServer()
{}

bool AdminServer::init(std::string &path)
{
    std::string err;
    int sock_fd;
    err = bind_and_listen(path, &sock_fd);
    if(!err.empty())
    {
        return false;
    }

    s_sock_fd = sock_fd;
    s_path = path;

    return true;
}

std::string bind_and_listen(std::string &path, int *fd)
{
    //create socket address
    struct sockaddr_un address;
    if(path.size() > sizeof(address.sun_path) -1)
    {
        ostringstream oss;
        oss << "The length of path must less than address.sun_path";
        return oss.str();
    }
//create socket
    int sock_fd = socket(PF_UNIX, SOCK_STREAM, 0);
    if(sock_fd < 0)
    {
       ostringstream oss;
       oss << "create socket fail";
       return oss.str();
    }

    int r = fcntl(sock_fd, F_SETFD, FD_CLOEXEC);
    if(r < 0)
    {
        r = errno;
        close(sock_fd);
        ostringstream oss;
        oss << "fcntl fail";
        return oss.str();
    }
// init socket address
    memset(&address, 0, sizeof(struct sockaddr_un));
    address.sun_family = AF_UNIX;
    snprintf(address.sun_path, sizeof(address.sun_path), "%s", path.c_str());

    //bind socket with socket address
    if(bind(sock_fd, (struct sockaddr *)&address, sizeof(struct sockaddr_un)) != 0)
    {
        close(sock_fd);
        ostringstream oss;
        oss << "bind fail";
        return oss.str();
    }

    //listen socket
    if(listen(sock_fd, 5) != 0)
    {
        close(sock_fd);
        ostringstream oss;
        oss << "listen fail";
        return oss.str();
    }

    *fd = sock_fd;

    return "";
}

bool AdminServer::do_accept()
{}

void *AdminServer::entry()
{
    while(true)
    {
        struct pollfd pfd;
        pfd.fd = s_sock_fd;
        pfd.events = POLLIN | POLLRDBAND;

        int ret = poll(&pfd, 2, -1);
        if(ret < 0)
        {
            int err = errno;
            if(err == EINTR)
                continue;
            return false;
        }

        if(pfd.revents & POLLIN)
        {
            do_accept();
        }
    }
}

void AdminServer::shutdown()
{
}
