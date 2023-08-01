#include "reporting.hpp"

Reporting* Reporting::instance_ = nullptr;


Reporting::Reporting() {
    this->errorStatus = false;
}

Reporting* Reporting::getInstance() {

    if (!instance_) {
        instance_ = new Reporting();
    }

    return instance_;
}


void Reporting::reportWarning(std::string msg) {
    std::cout << "Warning: " << msg << std::endl;
}
void Reporting::reportError(std::string msg, int num) {
    std::cout << "Error [line " << (num + 1)
        << "] : "
        << msg 
        << std::endl;
}
