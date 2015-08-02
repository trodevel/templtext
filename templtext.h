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

#ifndef LIB_TEMPLTEXT_H
#define LIB_TEMPLTEXT_H

#include <string>                   // std::string
#include <vector>                   // std::vector
#include <map>                      // std::map
#include <set>                      // std::set

#include <boost/property_tree/ptree.hpp>    // boost::property_tree::ptree

#include "templ.h"                  // Templ

NAMESPACE_TEMPLTEXT_START

std::string extract_function( const std::string & str );
bool find_function( const std::string & str );

class TemplText
{
public:

    TemplText();

    bool init(
            const std::string & config_file );

    bool has_template( const std::string & name ) const;

    const Templ & get_template( const std::string & name ) const;

private:
    typedef std::map<std::string, Templ>  MapStrToTempl;

private:

    void iterate_and_extract( const std::string & parent_name, const boost::property_tree::ptree & pt );

    void extract_templates( const boost::property_tree::ptree & pt );

private:

    MapStrToTempl   templs_;
};

NAMESPACE_TEMPLTEXT_END

#endif // LIB_TEMPLTEXT_H
