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

// $Revision: 2176 $ $Date:: 2015-07-21 #$ $Author: serge $

#include "templtext.h"                  // self

#include <stdexcept>                    // std::invalid_argument
//#include <regex>                        // std::regex - not supported by GCC older than 4.9.0
#include "../utils/mutex_helper.h"      // MUTEX_SCOPE_LOCK

// for config reading
#include <boost/property_tree/ini_parser.hpp>
#include <boost/regex.hpp>              // boost::regex - for GCC older than 4.9.0


NAMESPACE_TEMPLTEXT_START

TemplText::TemplText()
{
}

bool TemplText::init(
        const std::string & config_file )
{
    if( config_file.empty() )
        return false;

    boost::property_tree::ini_parser::read_ini( config_file, pt_ );

    extract_all_placeholders();

    return true;
}

void TemplText::iterate_and_extract( const std::string & parent_name, const boost::property_tree::ptree & pt )
{
    auto it_end = pt.end();
    for( auto it = pt.begin(); it != it_end; ++it )
    {
        const std::string & name    = it->first;
        const std::string & str     = it->second.data();

        SetStr res_set;

        extract_placeholders( res_set, str );

        VectStr res( res_set.begin(), res_set.end() );

        placeholders_.insert( MapStrToVectStr::value_type( parent_name + "." + name, res ) );
    }
}

void TemplText::extract_all_placeholders()
{
    auto it_end = pt_.end();
    for( auto it = pt_.begin(); it != it_end; ++it )
    {
        const std::string & name    = it->first;

        iterate_and_extract( name, it->second );
    }
}

void TemplText::extract_placeholders( SetStr & res, const std::string & str )
{
    try
    {
        boost::regex re( "%\\{[A-Z0-9]+\\}|%[A-Z0-9]+" );
        boost::sregex_iterator it( str.begin(), str.end(), re );
        boost::sregex_iterator end;
        for( ; it != end; ++it )
        {
            boost::smatch match = *it;
            //std::cout << match.str() << "\n"; // DEBUG

            std::string name = extract_name( match.str() );

            if( res.count( name ) == 0 )
                res.insert( name );
        }
    } catch( boost::regex_error& e )
    {
        // Syntax error in the regular expression
        throw std::invalid_argument( e.what() );
    }
}

std::string TemplText::extract_name( const std::string & str )
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

std::string TemplText::get_template( const std::string & name )
{
    static const std::string empty;

    auto opt_value = pt_.get_optional<std::string>( name );

    if( opt_value.is_initialized() == false )
    {
        return empty;
    }

    std::string res = opt_value.get();

    return res;
}

const std::vector<std::string> & TemplText::get_placeholders( const std::string & name ) const
{
    static const std::vector<std::string> empty;

    auto it = placeholders_.find( name );

    if( it == placeholders_.end() )
        return empty;

    return it->second;
}

NAMESPACE_TEMPLTEXT_END
