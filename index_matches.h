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

// $Revision: 2232 $ $Date:: 2015-08-03 #$ $Author: serge $


#ifndef LIB_TEMPLTEXT_INDEX_MATCHES_H
#define LIB_TEMPLTEXT_INDEX_MATCHES_H

#include <string>
#include <vector>
#include <utility>         // std::pair
//#include <regex>                  // std::regex - not supported by GCC older than 4.9.0

#include <boost/regex.hpp>          // boost::regex - for GCC older than 4.9.0

#include "namespace_lib.h"          // NAMESPACE_TEMPLTEXT_START

NAMESPACE_TEMPLTEXT_START

typedef std::pair<std::string::difference_type, std::string::difference_type>   PosLen;

void index_matches( boost::regex& re, const std::string& input, std::vector<PosLen> & res );

NAMESPACE_TEMPLTEXT_END

#endif // LIB_TEMPLTEXT_INDEX_MATCHES_H
