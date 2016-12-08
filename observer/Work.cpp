#include <set>
#include <string>
#include <iostream>

using namespace std;

#include "Work.h"
#include "config.h"

Work::Work():log_level(2), log_file("/var/log/observer/observer.log"), enable_log(false), loop(false)
{

}

Work::~Work()
{
}

const char** Work::get_tracked_conf_keys() const
{
    static const char *KEYS[] = {
        "work_log_level",
        "work_log_file",
        "enable_log",
        NULL
    };

    return KEYS;
}

void Work::handle_conf_change(const struct config_t *conf, const std::set<string> &changed)
{
    if(changed.count("work_log_level"))
        set_work_log_level(conf->work_log_level);

    if(changed.count("work_log_file"))
    {
        set_work_log_file(conf->work_log_file);
    }

    if(changed.count("enable_log"))
    {
        set_enable_or_disable_log(conf->enable_log);
    }
}

void Work::set_work_log_level(int level)
{
    log_level = level;
}

void Work::set_work_log_file(string path_file)
{
    log_file = path_file;
}

void Work::set_enable_or_disable_log(bool b)
{
    enable_log = b;
}

void Work::process()
{
    if(!loop)
    {
        loop = true;
        cout << "work_log_level\t" << "work_log_file\t\t\t" << "enable_log" <<endl;
    }
    cout << log_level << "\t\t" << log_file << "\t\t" << enable_log << endl;
}
