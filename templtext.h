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

// $Revision: 2209 $ $Date:: 2015-07-30 #$ $Author: serge $

#ifndef LIB_TEMPLTEXT_H
#define LIB_TEMPLTEXT_H

#include <string>                   // std::string
#include <vector>                   // std::vector
#include <map>                      // std::map
#include <set>                      // std::set

#include <boost/property_tree/ptree.hpp>    // boost::property_tree::ptree

#include "namespace_lib.h"          // NAMESPACE_TEMPLTEXT_START

NAMESPACE_TEMPLTEXT_START

std::string extract_function( const std::string & str );
bool find_function( const std::string & str );

class TemplText
{
public:
    typedef std::map<std::string, std::string>  MapKeyValue;
    typedef std::set<std::string>               SetStr;

    class Templ
    {
    public:
        Templ( const std::string & name, const std::string & templ, const SetStr & placeholders );

        const std::string & get_name() const;
        const std::string & get_template() const;
        const SetStr & get_placeholders() const;
        bool validate_tokens( const MapKeyValue & tokens, std::string & missing_token ) const;

        std::string format( const MapKeyValue & tokens, bool throw_on_error = true ) const;

    private:
        std::string     name_;
        std::string     templ_;
        SetStr          placeholders_;
    };

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
    static void extract_placeholders( SetStr & res, const std::string & str );
    static std::string extract_name( const std::string & str );

private:

    MapStrToTempl   templs_;
};

NAMESPACE_TEMPLTEXT_END

#endif // LIB_TEMPLTEXT_H
