//
// Created by Harry Scells on 6/04/2016.
//

#ifndef IFN660_ECMASCRIPT_ECMA_DRIVER_H
#define IFN660_ECMASCRIPT_ECMA_DRIVER_H

#include <string>
#include "ECMA_Scanner.hpp"
#include "grammar.tab.hh"

namespace ECMA{

    class ECMA_Driver{
    public:
        ECMA_Driver() : chars(0),
                      words(0),
                      lines(0),
                      uppercase(0),
                      lowercase(0),
                      parser( nullptr ),
                      scanner( nullptr ){};

        virtual ~ECMA_Driver();

        void parse( const char *filename );


        void add_upper();
        void add_lower();
        void add_word( const std::string &word );
        void add_newline();
        void add_char();

        std::ostream& print(std::ostream &stream);
    private:
        int chars;
        int words;
        int lines;
        int uppercase;
        int lowercase;
        ECMA::ECMA_Parser *parser;
        ECMA::ECMA_Scanner *scanner;
    };

} /* end namespace ECMA */

#endif //IFN660_ECMASCRIPT_ECMA_DRIVER_H
