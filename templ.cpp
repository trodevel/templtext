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

// $Revision: 2229 $ $Date:: 2015-08-03 #$ $Author: serge $

#include "templ.h"                      // self
#include "renderer.h"                   // Renderer
#include "parser.h"                     // Parser

#include <stdexcept>                    // std::invalid_argument


NAMESPACE_TEMPLTEXT_START

Templ::Text::Text( const std::string & text ):
    text_( text )
{
}

Templ::Templ( const std::string & name, const std::string & templ ):
    name_( name ),
    templ_( templ )
{
    parse();
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
    Renderer r( elems_, tokens, throw_on_error );

    std::string res = r.get();

    return res;
}

void Templ::parse()
{
    Parser p( templ_ );

    placeholders_   = p.get_placeholders();
    elems_          = p.get_elems();
}

NAMESPACE_TEMPLTEXT_END
