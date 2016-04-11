//
// Created by Harry Scells on 6/04/2016.
//

#ifndef IFN660_ECMASCRIPT_ECMA_DRIVER_H
#define IFN660_ECMASCRIPT_ECMA_DRIVER_H 1

#include <string>
#include "scanner.hpp"
#include "parser.tab.hh"

namespace ECMA{

    class Driver {

    public:
        Driver() = default;
        virtual ~Driver();

        void parse( const char *filename );

        std::ostream& print(std::ostream &stream);
    private:
        ECMA::Parser *parser = nullptr;
        ECMA::Scanner *scanner = nullptr;
    };

} /* end namespace ECMA */

#endif //IFN660_ECMASCRIPT_ECMA_DRIVER_H
