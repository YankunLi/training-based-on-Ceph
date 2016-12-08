/*
 * Testing based on Ceph
 *
 * Author: Yankun Li <lioveni99@gmail.com>
 *
 */

#include <string>
#include <stdlib.h>
#include <sstream>
#include <set>
#include <list>
#include <map>
#include <string.h>
#include <stdio.h>

#include "config.h"

using std::set;
using std::list;
using std::multimap;
using std::pair;
using std::string;
using std::ostringstream;
using std::map;


void *config_option::conf_ptr(config_t *conf) const
{
    void *v = (void*) (((char*)conf) + conf_off);

    return v;
}

const void *config_option::conf_ptr(const config_t *conf) const
{
    const void *v = (const void*) (((const char*)conf) + conf_off);

    return v;
}


#define STRING(X) #X

struct config_option config_optionsp[] = {
#define OPTION(name, type, def_val) \
    {STRING(name), type, offsetof(struct config_t, name)},
#include "config_opts.h"
#undef OPTION
};

const int config_count = sizeof(config_optionsp)/sizeof(config_option);

config_t::config_t():
#define OPTION_OPT_INT(name, def_val) name(def_val),
#define OPTION_OPT_BOOL(name, def_val) name(def_val),
#define OPTION_OPT_STR(name, def_val) name(def_val),
#define OPTION(name, type, def_val) OPTION_##type(name, def_val)
#include "config_opts.h"
#undef OPTION_OPT_INT
#undef OPTION_OPT_BOOL
#undef OPTION_OPT_STR
#undef OPTION
    cluster("Ceph")
{
}

config_t::~config_t()
{
}

int config_t::set_val(const char *key, const char *val)
{
    std::string k(key);
    std::string v(val);

    for(int i = 0; i < config_count; i++)
    {
        config_option *opt = &config_optionsp[i];
        if(strcmp(k.c_str(), opt->name) == 0)
        {
            return set_val_impl(v.c_str(), opt);
        }
    }

    return -1;
}

int config_t::set_val_impl(const char *val, const config_option *opt)
{
    int ret = set_val_raw(val, opt);
    if(ret)
    {
        return ret;
    }
    changed.insert(opt->name);

    return 0;
}

int config_t::set_val_raw(const char* val, const config_option *opt)
{
    switch(opt->type){
        case OPT_INT:
            {
                char *res;
                int f = strtol(val, &res, 10);
                *(int*)opt->conf_ptr(this) = f;
                return 0;
            }
        case OPT_BOOL:
            {
                if(strcasecmp(val, "false") == 0)
                    *(bool*)opt->conf_ptr(this) = false;
                else if(strcasecmp(val, "true") == 0)
                    *(bool*)opt->conf_ptr(this) = true;
                else
                    return -1;
                return 0;
            }
        case OPT_STR:
            {
                *(std::string *)opt->conf_ptr(this) = val ? val : "";
                return 0;
            }
    }
    return -1;
}

int config_t::get_val(const char *key, char **buf, int len) const
{
    string k(key);
    for(int i = 0; i < config_count; i++)
    {
        config_option *opt = &config_optionsp[i];
        if(strcmp(opt->name, k.c_str()))
        {
            ostringstream oss;
            switch(opt->type)
            {
                case OPT_INT:
                    oss << *(int *)opt->conf_ptr(this);
                    break;
                case OPT_STR:
                    oss << *(string *)opt->conf_ptr(this);
                    break;
                case OPT_BOOL:
                    bool b = *(bool *)opt->conf_ptr(this);
                    oss << (b ? "true" : "false");
                    break;
            }
            string str(oss.str());
            int l = strlen(str.c_str()) + 1;
            if(len == -1)
            {
                *buf = (char *)malloc(l);
                strcpy(*buf, str.c_str());
            }
            snprintf(*buf, len, "%s", str.c_str());
            return (l > len) ? -1 : 0;
        }
    }

    return -1;
}

void config_t::add_observer(config_obs_t *observer)
{
    const char **keys = observer->get_tracked_conf_keys();
    for(const char **k = keys; *k; k++)
    {
        obs_map_t::value_type val(*k, observer);
        observers.insert(val);
    }
}

void config_t::remove_observer(config_obs_t *observer)
{
    for(obs_map_t::iterator o = observers.begin(); o != observers.end(); )
    {
        if(o->second == observer)
        {
            observers.erase(o++);
        }
        else
        {
            ++o;
        }
    }
}

void config_t::apply_changes()
{
    typedef std::map< config_obs_t*, std::set<string> > rev_obs_map_t;

    rev_obs_map_t robs;
    std::set<std::string> empty_set;
    for(changed_set_t::const_iterator c = changed.begin(); c != changed.end(); c++)
    {
        const std::string &key(*c);
        pair<obs_map_t::iterator, obs_map_t::iterator> range(observers.equal_range(key));
        for(obs_map_t::iterator r = range.first; r != range.second; ++r)
        {
            rev_obs_map_t::value_type robs_val(r->second, empty_set);
            pair<rev_obs_map_t::iterator, bool> robs_ret(robs.insert(robs_val));
            std::set<std::string> &keys(robs_ret.first->second);
            keys.insert(key);
        }
    }

    for(rev_obs_map_t::iterator s = robs.begin(); s != robs.end(); ++s)
    {
        config_obs_t *obs = s->first;
        obs->handle_conf_change(this, s->second);
    }

    changed.clear();
}
