#ifndef _WORK_H_
#define _WORK_H_

#include <string>
#include <set>
using namespace std;

#include "config.h"
#include "observer.h"

class Work: public config_obs_t
{
    public:
        Work();
        ~Work();
        virtual const char** get_tracked_conf_keys() const;
        virtual void handle_conf_change(const struct config_t *conf,
                const std::set <std::string> &changed);
        void set_work_log_level(int level);
        void set_work_log_file(string path_file);
        void set_enable_or_disable_log(bool b);

       // int get_work_log_level();
       // string get_work_log_file();
       // bool get_work_file();
       // testing for observer
        void process();

    private:
        bool loop;
        int log_level;
        string log_file;
        bool enable_log;
};

#endif
