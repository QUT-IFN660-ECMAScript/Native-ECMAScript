//
// Created by Harry Scells on 6/04/2016.
//

#ifndef IFN660_ECMASCRIPT_ECMA_DRIVER_H
#define IFN660_ECMASCRIPT_ECMA_DRIVER_H 1

#include <string>
#include "ECMA_Scanner.hpp"
#include "parser.tab.hh"

namespace ECMA{

    class ECMA_Driver {

    public:
        ECMA_Driver() = default;
        virtual ~ECMA_Driver();

        void parse( const char *filename );

        std::ostream& print(std::ostream &stream);
    private:
        ECMA::ECMA_Parser *parser = nullptr;
        ECMA::ECMA_Scanner *scanner = nullptr;
    };

} /* end namespace ECMA */

#endif //IFN660_ECMASCRIPT_ECMA_DRIVER_H
