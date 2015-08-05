/*

Text Template processing library.

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

// $Revision: 2256 $ $Date:: 2015-08-04 #$ $Author: serge $

#include "parser.h"                     // self

#include <stdexcept>                    // std::invalid_argument

#include <boost/regex.hpp>              // boost::regex - for GCC older than 4.9.0


NAMESPACE_TEMPLTEXT_START

Parser::Parser( const std::string & templ ):
    templ_( templ )
{
    parse();
}

const Parser::SetStr & Parser::get_placeholders() const
{
    return placeholders_;
}

const Parser::Elems & Parser::get_elems() const
{
    return elems_;
}


const std::string & get_regex_match()
{
    static const std::string res( "%\\{([A-Z0-9]+)\\}|%([A-Z0-9]+)([^A-Z0-9]|$)|\\$([A-Za-z][A-Za-z_0-9]+)\\s*\\(\\s*(%[A-Z0-9]+|[A-Za-z0-9]+)*(\\s*(,)\\s*([A-Za-z0-9]+)(\\s*(,)\\s*([A-Za-z0-9]+))*)*\\s*\\)" );
    //static const std::string res( "%\\{([A-Z0-9]+)\\}|%([A-Z0-9]+)([^A-Z0-9]|$)" );   // TEST: only variables

    return res;
}

void Parser::parse()
{
    static const boost::regex re( get_regex_match() );

    std::vector<PosLen> poslen;

    index_matches( re, templ_, poslen );

    process_poslen( poslen );

}

void Parser::process_poslen( const std::vector<PosLen> & poslen )
{
    PosLen::first_type i = 0;

    for( auto & e : poslen )
    {
        if( i < e.first )
        {
            Text * t = new Text( templ_.substr( i, e.first ) );
            elems_.push_back( t );
        }

        std::string match = templ_.substr( e.first, e.second );

        Elem * el = convert_match_to_elem( match );

        elems_.push_back( el );

        i = e.first + e.second;
    }
}

Parser::Elem* Parser::convert_match_to_elem( const std::string & str )
{
    Elem* res = nullptr;

    boost::regex re( get_regex_match() );

    boost::smatch matches;

    try
    {
        if( boost::regex_match( str, matches, re ) )
        {
            auto size = matches.size();
            for( unsigned i = 1; i < size; ++i )
            {
                std::string res( matches[i].first, matches[i].second );
                std::cout << "matches [" << i << "] = " << res << std::endl;  // DEBUG

//                if( res.empty() == false )
//                    return res;
            }

            return res;
        }
        else
        {
            std::cout << "'" << re << "' does not match '" << str << "'. matches size(" << matches.size() << ")" << std::endl; // DEBUG
            return res;
        }
    }
    catch( boost::regex_error& e )
    {
        //throw std::invalid_argument( e.what() );
        std::cout << "regex error " << e.what();
    }


    return res;
}

NAMESPACE_TEMPLTEXT_END
