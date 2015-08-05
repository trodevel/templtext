/*

Example.

Copyright (C) 2015 Sergey Kolevatov

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.

*/

// $Revision: 2257 $ $Date:: 2015-08-04 #$ $Author: serge $

#include <iostream>                         // std::cout
#include <sstream>                          // std::stringstream
#include <memory>                           // std::unique_ptr

#include "templ.h"                          // Templ

void test02( const templtext::Templ & t )
{
    std::cout << "TEST 02" << std::endl;

    templtext::Templ::MapKeyValue tokens  =
    {
            { "SALUTATION", "Mr." },
            { "NAME", "John Doe" },
    };

    std::string missing_token;

    if( t.validate_tokens( tokens, missing_token ) == false )
    {
        std::cout << "missing token '" << missing_token << "' in template " << t.get_name() << std::endl;
    }
}

void test03( const templtext::Templ & t )
{
    std::cout << "TEST 03" << std::endl;

    templtext::Templ::MapKeyValue tokens  =
    {
            { "SALUTATION", "Mr." },
            { "NAME", "John Doe" },
            { "TEXT", "Hello World" }
    };

    std::string missing_token;

    if( t.validate_tokens( tokens, missing_token ) )
    {
        std::cout << "all tokens are present in template " << t.get_name() << std::endl;
    }
}

void test04( const templtext::Templ & t )
{
    std::cout << "TEST 04" << std::endl;

    templtext::Templ::MapKeyValue tokens  =
    {
            { "SALUTATION", "Mr." },
            { "NAME", "John Doe" },
    };

    try
    {
        std::string res = t.format( tokens );
    }
    catch ( std::exception & e )
    {
        std::cout << "got exception '" << e.what() << "'" << std::endl;
    }
}

void test05( const templtext::Templ & t )
{
    std::cout << "TEST 05" << std::endl;

    templtext::Templ::MapKeyValue tokens  =
    {
            { "SALUTATION", "Mr." },
            { "NAME", "John Doe" },
    };

    try
    {
        std::string res = t.format( tokens, false );

        std::cout << "formatted string is '" << res << "'" << std::endl;
    }
    catch ( std::exception & e )
    {
        std::cout << "got exception '" << e.what() << "'" << std::endl;
    }
}


void test06( const templtext::Templ & t )
{
    std::cout << "TEST 06" << std::endl;

    templtext::Templ::MapKeyValue tokens  =
    {
            { "SALUTATION", "Mr." },
            { "NAME", "John Doe" },
            { "TEXT", "Hello World" }
    };

    try
    {
        std::string res = t.format( tokens );

        std::cout << "formatted string is '" << res << "'" << std::endl;
    }
    catch ( std::exception & e )
    {
        std::cout << "got exception '" << e.what() << "'" << std::endl;
    }
}

int main()
{
    using namespace templtext;

    {
        std::unique_ptr<Templ> t1( new Templ( "%TEST1" ) );
        std::unique_ptr<Templ> t2( new Templ( "%TEST2 " ) );
        std::unique_ptr<Templ> t3( new Templ( " %TEST3" ) );
        std::unique_ptr<Templ> t4( new Templ( " %TEST4 " ) );

        std::unique_ptr<Templ> t5( new Templ( "%{TEST5}" ) );
        std::unique_ptr<Templ> t6( new Templ( "%{TEST6} " ) );
        std::unique_ptr<Templ> t7( new Templ( " %{TEST7}" ) );
        std::unique_ptr<Templ> t8( new Templ( " %{TEST8} " ) );

        std::unique_ptr<Templ> t9( new Templ( " %{TEST9A} %TEST9B" ) );
        std::unique_ptr<Templ> t10( new Templ( " %{TEST9A} %TEST9B" ) );

        std::unique_ptr<Templ> t11( new Templ( "$foo11()") );
        std::unique_ptr<Templ> t12( new Templ( "$foo12(A)") );
        std::unique_ptr<Templ> t13( new Templ( "$foo13(AB)") );
        std::unique_ptr<Templ> t14( new Templ( "$foo14(AB,C)") );
        std::unique_ptr<Templ> t15( new Templ( "$foo15(AB,CD)") );
        std::unique_ptr<Templ> t16( new Templ( "$foo16(AB,CD,E)") );
        std::unique_ptr<Templ> t17( new Templ( "$foo17(AB,CD,EF)") );
        std::unique_ptr<Templ> t18( new Templ( "$foo( A )") );
        std::unique_ptr<Templ> t19( new Templ( "$foo( AB )") );
        std::unique_ptr<Templ> t20( new Templ( "$foo( AB,C )") );
        std::unique_ptr<Templ> t21( new Templ( "$foo( AB,CD )") );
        std::unique_ptr<Templ> t22( new Templ( "$foo( AB,CD,E )") );
        std::unique_ptr<Templ> t23( new Templ( "$foo( AB,CD,EF )") );
        std::unique_ptr<Templ> t24( new Templ( "$foo( A )") );
        std::unique_ptr<Templ> t25( new Templ( "$foo( AB )") );
        std::unique_ptr<Templ> t26( new Templ( "$foo( AB, C )") );
        std::unique_ptr<Templ> t27( new Templ( "$foo( AB, CD )") );
        std::unique_ptr<Templ> t28( new Templ( "$foo( AB, CD, E )") );
        std::unique_ptr<Templ> t29( new Templ( "$foo( AB, CD, EF )") );

        std::unique_ptr<Templ> t30( new Templ( "some text $foo( A ) what do think?") );
        std::unique_ptr<Templ> t31( new Templ( "some text $bar( A ) with several functions $bar( B )?") );

    }
    Templ * t = new Templ( "Hello. %SALUTATION %NAME is greeting you. %TEXT." );

    test02( *t );
    test03( *t );
    test04( *t );
    test05( *t );
    test06( *t );

    delete t;

    Templ * tf1 = new Templ( "current time is $now(). OK" );
    Templ * tf2 = new Templ( "first month is $month( 1, EN ) (English) $month( 1, DE ) (German)" );
    Templ * tf3 = new Templ( "first month is $month( 1, %LANG )" );
    Templ * tf4 = new Templ( "some complex function with 3 params $foo( par1, par2, par3 )" );

    delete tf1;
    delete tf2;
    delete tf3;
    delete tf4;

    return 0;
}
