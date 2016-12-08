/*
 * testing based on Ceph
 *
 * Author: Yankun Li <lioveni99@gmail.com>
 *
 */

#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <string>
#include <set>
#include <map>
#include <vector>

#include "observer.h"

struct config_option;
class config_obs_t;

typedef enum {OPT_INT, OPT_BOOL, OPT_STR
} opt_type_t;

struct config_t{
public:
typedef std::multimap<std::string, config_obs_t*> obs_map_t;
typedef std::set<std::string> changed_set_t;

config_t();
~config_t();

int set_val(const char *key, const char *val);
int get_val(const char *key, char **buf, int len) const;

int set_val_impl(const char *val, const config_option* opt);

int set_val_raw(const char *val, const config_option* opt);

private:
std::string cluster;
obs_map_t observers;
changed_set_t changed;

public:

#define OPTION_OPT_INT(name) const int name;
#define OPTION_OPT_BOOL(name) const bool name;
#define OPTION_OPT_STR(name) const std::string name;
#define OPTION(name, type, val) OPTION_##type(name)
#include "config_opts.h"
#undef OPTION_OPT_INT
#undef OPTION_OPT_BOOL
#undef OPTION_OPT_STR
#undef OPTION
void add_observer(config_obs_t *observer);
void remove_observer(config_obs_t *observer);

void apply_changes();
};

struct config_option{
const char *name;
opt_type_t type;
size_t conf_off;

void *conf_ptr(config_t *conf) const;

const void *conf_ptr(const config_t *conf) const;
};

#endif
