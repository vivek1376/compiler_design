#ifndef __REPORTING_H__
#define __REPORTING_H__

#include <string>
#include <iostream>

#define LOGGER  Reporting::getInstance()

class Reporting {
    private:
        bool errorStatus;
        static Reporting* instance_;

    public:
        Reporting();
        void reportError(std::string message);
        void reportWarning(std::string message);
        bool getErrorStatus();
        static Reporting* getInstance();
};



#endif
