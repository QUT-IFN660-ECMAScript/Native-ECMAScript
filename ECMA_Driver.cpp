#include <cctype>
#include <fstream>
#include <cassert>

#include "ECMA_Driver.hpp"

using namespace std;

ECMA::ECMA_Driver::~ECMA_Driver(){
    delete(scanner);
    scanner = nullptr;
    delete(parser);
    parser = nullptr;
}

void ECMA::ECMA_Driver::parse( const char *filename ) {
    assert( filename != nullptr );
    std::ifstream in_file( filename );
    if( ! in_file.good() )  {
        cerr << "Input file is bad" << endl;
        exit( EXIT_FAILURE );
    }

    delete(scanner);
    try
    {
        scanner = new ECMA::ECMA_Scanner( &in_file );
    } catch( std::bad_alloc &ba ) {
        std::cerr << "Failed to allocate scanner: (" <<
        ba.what() << "), exiting!!\n";
        exit( EXIT_FAILURE );
    }



    delete(parser);
    try {
        parser = new ECMA::ECMA_Parser( (*scanner) /* scanner */,
                                    (*this) /* driver */ );
    } catch( std::bad_alloc &ba ) {
        std::cerr << "Failed to allocate parser: (" <<
        ba.what() << "), exiting!!\n";
        exit( EXIT_FAILURE );
    }
    const int accept( 0 );
    if( parser->parse() != accept ) {
        std::cerr << "Parse failed!!\n";
    }
}
