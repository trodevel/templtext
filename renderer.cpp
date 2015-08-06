/*

Text Template processing library. Renderer.

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

// $Revision: 2279 $ $Date:: 2015-08-06 #$ $Author: serge $

#include "renderer.h"                   // self

#include <stdexcept>                    // std::invalid_argument
#include <typeinfo>                     // typeid

#include "elems.h"                      // Elem, Text, Func, Var

NAMESPACE_TEMPLTEXT_START

Renderer::Renderer( const Templ::Elems & elems, const Templ::MapKeyValue & tokens, Templ::FuncProc func_proc, bool throw_on_error ):
        elems_( elems ),
        tokens_( tokens ),
        func_proc_( func_proc ),
        throw_on_error_( throw_on_error )
{
    render();
}

const std::string & Renderer::get() const
{
    return res_;
}


void Renderer::render()
{
    std::ostringstream os;

    for( auto e : elems_ )
    {
        render( os, e );
    }

    res_    = os.str();
}

void Renderer::render( std::ostringstream & os, const Templ::Elem * e )
{
    if( typeid( *e ) == typeid( Templ::Text ) )
    {
        render( os, dynamic_cast< const Templ::Text *>( e ) );
    }
    else if( typeid( *e ) == typeid( Templ::Func ) )
    {
        render( os, dynamic_cast< const Templ::Func *>( e ) );
    }
    else if( typeid( *e ) == typeid( Templ::Var ) )
    {
        render( os, dynamic_cast< const Templ::Var *>( e ) );
    }
}

void Renderer::render( std::ostringstream & os, const Templ::Text * e )
{
    os << e->text_;
}

void Renderer::render( std::ostringstream & os, const Templ::Func * e )
{
    if( func_proc_ == false )
    {
        if( throw_on_error_ )
            throw std::runtime_error( "Renderer: cannot call functions - FuncProc is not defined" );

        return;
    }
}

void Renderer::render( std::ostringstream & os, const Templ::Var * e )
{
    auto it = tokens_.find( e->name_ );

    if( it == tokens_.end() )
    {
        if( throw_on_error_ )
            throw std::invalid_argument( ( "cannot find token '" + e->name_ + "'" ).c_str() );
        else
            return;
    }

    os << it->second;
}

NAMESPACE_TEMPLTEXT_END
