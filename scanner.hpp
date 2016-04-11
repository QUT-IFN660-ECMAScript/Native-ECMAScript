//
// Created by Harry Scells on 6/04/2016.
//

#ifndef IFN660_ECMASCRIPT_ECMA_SCANNER_H
#define IFN660_ECMASCRIPT_ECMA_SCANNER_H 1

#if ! defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

#include "parser.tab.hh"
#include "location.hh"

namespace ECMA{

    class Scanner : public yyFlexLexer{
    public:

        Scanner(std::istream *in) : yyFlexLexer(in), yylval( nullptr ){
            loc = new ECMA::Parser::location_type();
        };

        using yyFlexLexer::yylex;
        int yylex(ECMA::Parser::semantic_type *lval, ECMA::Parser::location_type *location);

        void error(const ECMA::Parser::location_type &l, const std::string &err_message);

    private:
        /* yyval ptr */
        ECMA::Parser::semantic_type *yylval;
        /* location ptr */
        ECMA::Parser::location_type *loc;
    };

}

#endif //IFN660_ECMASCRIPT_ECMA_SCANNER_H
