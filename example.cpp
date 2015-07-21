/*

Example.

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

// $Revision: 2170 $ $Date:: 2015-07-20 #$ $Author: serge $

#include <cstdio>
#include <sstream>                          // std::stringstream

#include "templtext.h"                      // TemplText

std::string create_key( const std::string & name, int num, const std::string & lang )
{
    std::stringstream os;

    os << "Templates." << name << "0" << num << "[" << lang << "]";

    return os.str();
}

std::string show_placeholders( const std::vector<std::string> & ph )
{
    std::stringstream os;

    os << ph.size() << ": ";

    for( auto & s : ph )
        os << s << " ";

    return os.str();
}

int main()
{
    templtext::TemplText t;

    t.init( "templates.ini" );

    for( int i = 1; i <= 5; ++i )
    {
        std::string key_1 = create_key( "Text", i, "en" );
        std::string key_2 = create_key( "Text", i, "de" );

        std::cout << "templ 0" << i << " - " << t.get_template( key_1 ) << " " << show_placeholders( t.get_placeholders( key_1 )) << std::endl;
        std::cout << "templ 0" << i << " - " << t.get_template( key_2 ) << " " << show_placeholders( t.get_placeholders( key_2 )) << std::endl;
    }

    return 0;
}
