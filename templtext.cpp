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

// $Revision: 2185 $ $Date:: 2015-07-22 #$ $Author: serge $

#include "templtext.h"                  // self

#include <stdexcept>                    // std::invalid_argument
//#include <regex>                        // std::regex - not supported by GCC older than 4.9.0
#include "../utils/mutex_helper.h"      // MUTEX_SCOPE_LOCK

// for config reading
#include <boost/property_tree/ini_parser.hpp>
#include <boost/regex.hpp>              // boost::regex - for GCC older than 4.9.0
#include <boost/algorithm/string.hpp>   // boost::replace_all


NAMESPACE_TEMPLTEXT_START

TemplText::Templ::Templ( const std::string & name, const std::string & templ, const SetStr & placeholders ):
    name_( name ),
    templ_( templ ),
    placeholders_( placeholders )
{
}

const std::string & TemplText::Templ::get_name() const
{
    return name_;
}

const std::string & TemplText::Templ::get_template() const
{
    return templ_;
}

const TemplText::SetStr & TemplText::Templ::get_placeholders() const
{
    return placeholders_;
}

bool TemplText::Templ::validate_tokens( const MapKeyValue & tokens, std::string & missing_token ) const
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

std::string TemplText::Templ::format( const MapKeyValue & tokens, bool throw_on_error ) const
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


TemplText::TemplText()
{
}

bool TemplText::init(
        const std::string & config_file )
{
    if( config_file.empty() )
        return false;

    boost::property_tree::ptree pt;

    boost::property_tree::ini_parser::read_ini( config_file, pt );

    extract_templates( pt );

    return true;
}

void TemplText::iterate_and_extract( const std::string & parent_name, const boost::property_tree::ptree & pt )
{
    auto it_end = pt.end();
    for( auto it = pt.begin(); it != it_end; ++it )
    {
        const std::string & name    = it->first;
        const std::string & str     = it->second.data();

        SetStr ph;

        extract_placeholders( ph, str );

        std::string templ_name = parent_name + "." + name;

        Templ t( templ_name, str, ph );

        templs_.insert( MapStrToTempl::value_type( templ_name, t ) );
    }
}

void TemplText::extract_templates( const boost::property_tree::ptree & pt )
{
    auto it_end = pt.end();
    for( auto it = pt.begin(); it != it_end; ++it )
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

bool TemplText::has_template( const std::string & name ) const
{
    return ( templs_.count( name ) > 0 );
}

const TemplText::Templ & TemplText::get_template( const std::string & name ) const
{
    auto it = templs_.find( name );

    if( it == templs_.end() )
    {
        throw std::invalid_argument( ( "cannot find template '" + name + "'" ).c_str() );
    }

    return it->second;
}

NAMESPACE_TEMPLTEXT_END
