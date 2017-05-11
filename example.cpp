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

// $Revision: 6925 $ $Date:: 2017-05-11 #$ $Author: serge $

#include <iostream>                         // std::cout
#include <sstream>                          // std::stringstream
#include <memory>                           // std::unique_ptr

#include "templ.h"                          // Templ

void test01()
{
    using namespace templtext;

    {
        std::unique_ptr<Templ> t1( new Templ( "$TEST1" ) );
        std::unique_ptr<Templ> t2( new Templ( "$TEST2 " ) );
        std::unique_ptr<Templ> t3( new Templ( " $TEST3" ) );
        std::unique_ptr<Templ> t4( new Templ( " $TEST4 " ) );

        std::unique_ptr<Templ> t5( new Templ( "${TEST5}" ) );
        std::unique_ptr<Templ> t6( new Templ( "${TEST6} " ) );
        std::unique_ptr<Templ> t7( new Templ( " ${TEST7}" ) );
        std::unique_ptr<Templ> t8( new Templ( " ${TEST8} " ) );

        std::unique_ptr<Templ> t9( new Templ( " ${TEST9A} $TEST9B" ) );
        std::unique_ptr<Templ> t10( new Templ( " ${TEST9A} $TEST9B" ) );

        std::unique_ptr<Templ> t11( new Templ( ". $TEXT." ) );
    }

    {

        std::unique_ptr<Templ> t11( new Templ( "%foo11()") );
        std::unique_ptr<Templ> t12( new Templ( "%foo12(P)") );
        std::unique_ptr<Templ> t13( new Templ( "%foo13(P1)") );
        std::unique_ptr<Templ> t14( new Templ( "%foo14(P1,P)") );
        std::unique_ptr<Templ> t15( new Templ( "%foo15(P1,P2)") );
        std::unique_ptr<Templ> t16( new Templ( "%foo16(P1,P2,P)") );
        std::unique_ptr<Templ> t17( new Templ( "%foo17(P1,P2,P3)") );
        std::unique_ptr<Templ> t18( new Templ( "%foo( P )") );
        std::unique_ptr<Templ> t19( new Templ( "%foo( P1 )") );
        std::unique_ptr<Templ> t20( new Templ( "%foo( P1,P )") );
        std::unique_ptr<Templ> t21( new Templ( "%foo( P1,P2 )") );
        std::unique_ptr<Templ> t22( new Templ( "%foo( P1,P2,P )") );
        std::unique_ptr<Templ> t23( new Templ( "%foo( P1,P2,P3 )") );
        std::unique_ptr<Templ> t24( new Templ( "%foo( P )") );
        std::unique_ptr<Templ> t25( new Templ( "%foo( P1 )") );
        std::unique_ptr<Templ> t26( new Templ( "%foo( P1, P )") );
        std::unique_ptr<Templ> t27( new Templ( "%foo( P1, P2 )") );
        std::unique_ptr<Templ> t28( new Templ( "%foo( P1, P2, P )") );
        std::unique_ptr<Templ> t29( new Templ( "%foo( P1, P2, P3 )") );

        std::unique_ptr<Templ> t30( new Templ( "%foo( $P )") );
        std::unique_ptr<Templ> t31( new Templ( "%foo( $P1 )") );
        std::unique_ptr<Templ> t32( new Templ( "%foo( $P1, $P )") );
        std::unique_ptr<Templ> t33( new Templ( "%foo( $P1, $P2 )") );
        std::unique_ptr<Templ> t34( new Templ( "%foo( $P1, $P2, $P )") );
        std::unique_ptr<Templ> t35( new Templ( "%foo( $P1, $P2, $P3 )") );

        std::unique_ptr<Templ> t36( new Templ( "%foo( 'free text1' )") );
        std::unique_ptr<Templ> t37( new Templ( "%foo( 'free text1', 'free text2' )") );
        std::unique_ptr<Templ> t38( new Templ( "%foo( 'free text1', 'free text2', 'free,text3' )") );

        std::unique_ptr<Templ> t39( new Templ( "some text %foo( A ) what do think?") );
        std::unique_ptr<Templ> t40( new Templ( "some text %bar( A ) with several functions %bar( B )?") );

    }
}

void test02( const templtext::Templ & t )
{
    std::cout << "TEST 02" << std::endl;

    templtext::Templ::MapKeyValue tokens  =
    {
            { "SALUTATION", "Mr." },
            { "NAME", "John Doe" },
            { "MYNAME", "Ty Coon" }
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
            { "TEXT", "an interview" },
            { "MYNAME", "Ty Coon" }
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
            { "MYNAME", "Ty Coon" }
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
            { "MYNAME", "Ty Coon" }
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
            { "TEXT", "an interview" },
            { "MYNAME", "Ty Coon" }
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

void test07( const templtext::Templ & t )
{
    std::cout << "TEST 06" << std::endl;

    templtext::Templ::MapKeyValue tokens  =
    {
            { "SALUTATION", "Mr." },
            { "NAME", "John Doe" },
            { "TEXT", "a cup of coffee" },
            { "MYNAME", "Ty Coon" }
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

const std::string & decode( int i )
{
    static const std::string fruits[]=
    {
        "unknown",
        "apple",
        "orange"
    };

    if( i < 0 || i > 2 )
        return fruits[0];

    return fruits[i];
}

const std::string & decode_loc( int i, const std::string & loc )
{
    static const std::string fruits[2][3]=
    {
        {
            "unknown",
            "apple",
            "orange"
        },
        {
            "Unbekannt",
            "Apfel",
            "Orange"
        },
    };

    if( i < 0 || i > 2 )
        i = 0;

    if( loc == "EN" )
        return fruits[0][i];
    else if( loc == "DE" )
        return fruits[1][i];

    return fruits[0][i];
}

bool func( std::string & res, const std::string & name, const std::vector<std::string> & par )
{
    if( name == "decode" )
    {
        if( par.empty() )
            return false;

        res = decode( std::stoi( par[0] ));
        return true;
    }
    else if( name == "decode_loc" )
    {
        if( par.size() != 2 )
            return false;

        res = decode_loc( std::stoi( par[0] ), par[1] );
        return true;
    }

    return false;
}

void test08()
{
    using namespace templtext;

    std::unique_ptr<Templ> tf1( new Templ( "You have got an %decode( 1 )." ) );
    std::unique_ptr<Templ> tf2( new Templ( "You have got an %decode( 2 )." ) );
    std::unique_ptr<Templ> tf3( new Templ( "English version - %decode_loc( 1, EN )." ) );
    std::unique_ptr<Templ> tf4( new Templ( "German version  - %decode_loc( 1, DE )." ) );
    std::unique_ptr<Templ> tf5( new Templ( "Flexible version - %decode_loc( 1, $LANG )." ) );

    tf1->set_func_proc( func );
    tf2->set_func_proc( func );
    tf3->set_func_proc( func );
    tf4->set_func_proc( func );
    tf5->set_func_proc( func );

    Templ::MapKeyValue map1 =
    {
            { "LANG", "EN" }
    };

    Templ::MapKeyValue map2 =
    {
            { "LANG", "DE" }
    };

    std::cout << tf1->format() << std::endl;
    std::cout << tf2->format() << std::endl;
    std::cout << tf3->format() << std::endl;
    std::cout << tf4->format() << std::endl;
    std::cout << tf5->format( map1 ) << std::endl;
    std::cout << tf5->format( map2 ) << std::endl;
}

void simple_example01()
{
    using namespace templtext;

    Templ * t = new Templ( "Dear $SALUTATION $NAME. I would like to invite you for $TEXT. Sincerely yours, $MYNAME." );

    std::map<std::string, std::string> tokens01 =
    {
            { "SALUTATION", "Mr." },
            { "NAME", "John Doe" },
            { "TEXT", "an interview" },
            { "MYNAME", "Ty Coon" }
    };

    std::map<std::string, std::string> tokens02 =
    {
            { "SALUTATION", "Sweetheart" },
            { "NAME", "Mary" },
            { "TEXT", "a cup of coffee" },
            { "MYNAME", "Bob" }
    };

    std::cout << t->format( tokens01 ) << std::endl;

    std::cout << t->format( tokens02 ) << std::endl;

    delete t;
}

int main()
{
    using namespace templtext;

    test01();

    Templ * t = new Templ( "Dear $SALUTATION $NAME. I would like to invite you for $TEXT. Sincerely yours, $MYNAME." );

    test02( *t );
    test03( *t );
    test04( *t );
    test05( *t );
    test06( *t );
    test07( *t );

    delete t;

    test08();

    simple_example01();

    return 0;
}
