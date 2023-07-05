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
void Reporting::reportError(std::string msg) {
    std::cout << "Error: " << msg << std::endl;
}
