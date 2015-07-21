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

// $Revision: 2171 $ $Date:: 2015-07-20 #$ $Author: serge $

#ifndef LIB_TEMPLTEXT_H
#define LIB_TEMPLTEXT_H

#include <string>                   // std::string
#include <vector>                   // std::vector
#include <map>                      // std::map

#include <boost/property_tree/ptree.hpp>    // boost::property_tree::ptree

#include "namespace_lib.h"          // NAMESPACE_TEMPLTEXT_START

NAMESPACE_TEMPLTEXT_START

class TemplText
{
public:

    TemplText();

    bool init(
            const std::string & config_file );

    std::string get_template( const std::string & name );

    const std::vector<std::string> & get_placeholders( const std::string & name ) const;

private:
    typedef std::vector<std::string>        VectStr;
    typedef std::map<std::string, VectStr>  MapStrToVectStr;

private:

    void iterate_and_extract( const std::string & parent_name, const boost::property_tree::ptree & pt );

    void extract_all_placeholders();
    static void extract_placeholders( VectStr & res, const std::string & str );
    static void extract_name( const std::string & str );

private:
    boost::property_tree::ptree pt_;

    MapStrToVectStr             placeholders_;
};

NAMESPACE_TEMPLTEXT_END

#endif // LIB_TEMPLTEXT_H
