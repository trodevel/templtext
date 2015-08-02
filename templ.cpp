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

// $Revision: 2220 $ $Date:: 2015-07-31 #$ $Author: serge $

#include "templ.h"                      // self

#include <stdexcept>                    // std::invalid_argument
//#include <regex>                        // std::regex - not supported by GCC older than 4.9.0
#include "../utils/mutex_helper.h"      // MUTEX_SCOPE_LOCK

// for config reading
#include <boost/property_tree/ini_parser.hpp>
#include <boost/regex.hpp>              // boost::regex - for GCC older than 4.9.0
#include <boost/algorithm/string.hpp>   // boost::replace_all


NAMESPACE_TEMPLTEXT_START

Templ::Text::Text( const std::string & text ):
    text_( text )
{
}

Templ::Templ( const std::string & name, const std::string & templ ):
    name_( name ),
    templ_( templ )
{
    extract_placeholders( placeholders_, templ_ );
}

Templ::~Templ()
{
    for( auto e : elems_ )
        delete e;
}

const std::string & Templ::get_name() const
{
    return name_;
}

const std::string & Templ::get_template() const
{
    return templ_;
}

const Templ::SetStr & Templ::get_placeholders() const
{
    return placeholders_;
}

bool Templ::validate_tokens( const MapKeyValue & tokens, std::string & missing_token ) const
{
    for( auto & k : placeholders_ )
    {
        if( tokens.count( k ) == 0 )
        {
            missing_token = k;
            return false;
        }
    }

    return true;
}

std::string Templ::format( const MapKeyValue & tokens, bool throw_on_error ) const
{
    std::string res( templ_ );

    for( auto & key : placeholders_ )
    {
        auto it = tokens.find( key );

        if( it == tokens.end() )
        {
            if( throw_on_error )
                throw std::invalid_argument( ( "cannot find token '" + key + "' required for template '" + name_ + "'" ).c_str() );
            else
                continue;
        }

        auto & val = it->second;

        boost::replace_all( res, "%" + key, val );
        boost::replace_all( res, "%{" + key + "}", val );
    }

    return res;
}

void Templ::parse()
{
}


const std::string & get_regex_function_match()
{
    static const std::string res( "\\$([A-Za-z][A-Za-z_0-9]+)\\s*\\(\\s*([A-Z]+)*(\\s*(,)\\s*([A-Z]+)(\\s*(,)\\s*([A-Z]+))*)*\\s*\\)" );

    return res;
}

const std::string & get_regex_match()
{
    static const std::string res( "%\\{([A-Z0-9]+)\\}|%([A-Z0-9]+)^([A-Z0-9]+)|\\$([A-Za-z][A-Za-z_0-9]+)\\s*\\(\\s*([A-Z]+)*(\\s*(,)\\s*([A-Z]+)(\\s*(,)\\s*([A-Z]+))*)*\\s*\\)" );

    return res;
}

void Templ::extract_placeholders( SetStr & res, const std::string & str )
{
    try
    {
        boost::regex re( get_regex_match() );
        boost::sregex_iterator it( str.begin(), str.end(), re );
        boost::sregex_iterator end;
        for( ; it != end; ++it )
        {
            boost::smatch match = *it;
            std::cout << match.str() << "\n"; // DEBUG

//            std::string name = extract_name( match.str() );

//            if( res.count( name ) == 0 )
//                res.insert( name );
        }
    } catch( boost::regex_error& e )
    {
        // Syntax error in the regular expression
        throw std::invalid_argument( e.what() );
    }
}

std::string Templ::extract_name( const std::string & str )
{
    boost::regex re( "%\\{([A-Z0-9]+)\\}$|%([A-Z0-9]+)$" );

    boost::smatch matches;

    try
    {
        if( boost::regex_match( str, matches, re ) )
        {
            auto size = matches.size();
            for( unsigned i = 1; i < size; ++i )
            {
                std::string res( matches[i].first, matches[i].second );
                //std::cout << "matches [" << i << "] = " << res << std::endl;  // DEBUG

                if( res.empty() == false )
                    return res;
            }

            return std::string();
        }
        else
        {
            //std::cout << "'" << re << "' does not match '" << str << "'. matches size(" << matches.size() << ")" << std::endl; // DEBUG
            return std::string();
        }
    }
    catch( boost::regex_error& e )
    {
        throw std::invalid_argument( e.what() );
    }
}

// stackoverflow
void index( boost::regex& re, const std::string& input )
{
    std::string::difference_type abs = 0;

    boost::match_results<std::string::const_iterator> what;
    boost::match_flag_type flags = boost::match_default;
    std::string::const_iterator s = input.begin();
    std::string::const_iterator e = input.end();
    while( boost::regex_search( s, e, what, re, flags ) )
    {
        std::string::difference_type l = what.length();
        std::string::difference_type p = what.position();

        abs += p;

        std::cout << "found at pos " << p << " len " << l << " '" << input.substr( abs, l ) << "'" << std::endl;

        s += p + l;

        abs += l;
    }
}

bool find_function( const std::string & str )
{
    boost::regex re( get_regex_function_match() );

    index( re, str );
    return true;
}

std::string extract_function( const std::string & str )
{
    std::cout << "input = " << str << std::endl;  // DEBUG

    std::ostringstream s;

    boost::regex re( get_regex_function_match() );

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

            return std::string();
        }
        else
        {
            std::cout << "'" << re << "' does not match '" << str << "'. matches size(" << matches.size() << ")" << std::endl; // DEBUG
            return std::string();
        }
    }
    catch( boost::regex_error& e )
    {
        //throw std::invalid_argument( e.what() );
        s << "regex error " << e.what();
    }

    return s.str();
}

NAMESPACE_TEMPLTEXT_END
