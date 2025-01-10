#include "process_base.h"

namespace process
{

    class ControllerBase : public BaseHandler 
    {
        public:
            static bool &running();

        protected:
            static int         numProcesses_;
            static std::string processType_;
            static bool        running_;
    };
}