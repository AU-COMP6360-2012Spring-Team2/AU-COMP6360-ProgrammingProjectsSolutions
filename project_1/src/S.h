#include <string>
#include "configuration.h"

class S {
    private:
        static S * instance;
        S();
        
        //here put objects that should be contained in S. objects should not be 'static'.
        configuration * config;

    public:
        static void initialize(std::string * config_file);
        static S * get();
};
