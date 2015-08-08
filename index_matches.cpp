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

// $Revision: 2280 $ $Date:: 2015-08-07 #$ $Author: serge $

#include "index_matches.h"          // self

NAMESPACE_TEMPLTEXT_START

// stackoverflow
void index_matches( const boost::regex& re, const std::string& input, std::vector<PosLen> & res )
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

        // std::cout << "found at pos " << abs << " len " << l << " '" << input.substr( abs, l ) << "'" << std::endl; // DEBUG

        res.push_back( PosLen( abs, l ) );

        s += p + l;

        abs += l;
    }
}

NAMESPACE_TEMPLTEXT_END
