//
// Created by Harry Scells on 6/04/2016.
//

#ifndef IFN660_ECMASCRIPT_ECMA_SCANNER_H
#define IFN660_ECMASCRIPT_ECMA_SCANNER_H

#if ! defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

#undef  YY_DECL
#define YY_DECL int ECMA::ECMA_Scanner::yylex()

#include "grammar.tab.hh"

namespace ECMA{

    class ECMA_Scanner : public yyFlexLexer{
    public:

        ECMA_Scanner(std::istream *in) : yyFlexLexer(in), yylval( nullptr ){};

        int yylex(ECMA::ECMA_Parser::semantic_type *lval)
        {
            yylval = lval;
            return( yylex() );
        }


    private:
        /* hide this one from public view */
        int yylex();
        /* yyval ptr */
        ECMA::ECMA_Parser::semantic_type *yylval;
    };

}

#endif //IFN660_ECMASCRIPT_ECMA_SCANNER_H
