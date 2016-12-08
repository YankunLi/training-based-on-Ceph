/*
 *
 * Testing based on Ceph
 * Auther: Yankun Li <lioveni99@gmail.com>
 *
 */

#ifndef _OBSERVER_H_
#define _OBSERVER_H_

#include <set>
#include <string>

#include "config.h"

//struct config_t;

class config_obs_t {
public:
    virtual ~config_obs_t(){}
    virtual const char** get_tracked_conf_keys() const = 0;
    virtual void handle_conf_change(const struct config_t * conf,
            const std::set <std::string> &changed) = 0;
    virtual void handle_subsys_change(const struct config_t * conf,
                const std::set<int>& changed) {}
};

#endif
