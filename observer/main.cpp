#include <iostream>
#include <string>
#include <stdlib.h>
#include <map>

using namespace std;

#include "observer.h"
#include "config.h"
#include "Work.h"

int main(int argc, char *argv[])
{
    config_obs_t *obs;
    Work wk;
    config_t *conf = new config_t;
    conf->add_observer(&wk);
    wk.process();
    conf->set_val("work_log_level", "1");
    conf->set_val("work_log_file", "/var/log/observer.log");
    conf->set_val("enable_log", "true");
    conf->apply_changes();
    wk.process();
    conf->remove_observer(&wk);
    conf->set_val("work_log_fil", "/opt/observer.log");
    conf->add_observer(&wk);
    conf->apply_changes();
    wk.process();
//int i = 0;
//#define OPTION(value)  i = value;
//#include "config_opts.h"
//cout << i << endl;
//string value = "1234r3";
//char* res;
//int ret = strtol(value.c_str(), &res, 10);
//cout<< "result: " << res << "\t" << "return: " << ret << endl;
//
//multimap<const string, int> config;
//config.insert(pair<const string, int>("ss", 1));
//config.insert(pair<const string, int>("ss", 2));
//config.insert(pair<const string, int>("ss", 2));
//config.insert(pair<const string, int>("ss", 2));
//config.insert(pair<const string, int>("ss", 2));
//config.insert(pair<const string, int>("ss", 2));
//config.insert(pair<const string, int>("ss", 2));
//config.insert(pair<const string, int>("ss", 2));
//int a = 0;
//for(multimap<const string, int>::iterator i = config.begin(); i != config.end(); )
//{
//    if(i->second == 2)
//    {
//        a++;
//        cout << a <<"\t";
//        cout << "remove :" << "\t key " << i->first << "\tvalue " << i->second << endl;
//        config.erase(i++);
//    }
//    else
//    {
//        ++i;
//    }
//}




return 0;
}
