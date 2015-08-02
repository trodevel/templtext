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

// $Revision: 2217 $ $Date:: 2015-07-31 #$ $Author: serge $

#include "templtext.h"                  // self

#include <stdexcept>                    // std::invalid_argument

// for config reading
#include <boost/property_tree/ini_parser.hpp>

NAMESPACE_TEMPLTEXT_START

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

        std::string templ_name = parent_name + "." + name;

        Templ t( templ_name, str );

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

bool TemplText::has_template( const std::string & name ) const
{
    return ( templs_.count( name ) > 0 );
}

const Templ & TemplText::get_template( const std::string & name ) const
{
    auto it = templs_.find( name );

    if( it == templs_.end() )
    {
        throw std::invalid_argument( ( "cannot find template '" + name + "'" ).c_str() );
    }

    return it->second;
}

NAMESPACE_TEMPLTEXT_END
