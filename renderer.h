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

// $Revision: 2278 $ $Date:: 2015-08-06 #$ $Author: serge $

#ifndef LIB_TEMPLTEXT_RENDERER_H
#define LIB_TEMPLTEXT_RENDERER_H

#include <sstream>                      // std::ostringstream

#include "templ.h"                      // Templ

NAMESPACE_TEMPLTEXT_START

class Renderer
{
public:
    Renderer( const Templ::Elems & elems, const Templ::MapKeyValue & tokens, Templ::FuncProc func_proc, bool throw_on_error );

    const std::string & get() const;

private:
    void render();

    void render( std::ostringstream & os, const Templ::Elem * e );
    void render( std::ostringstream & os, const Templ::Text * e );
    void render( std::ostringstream & os, const Templ::Func * e );
    void render( std::ostringstream & os, const Templ::Var * e );

private:

    const Templ::Elems   & elems_;
    const Templ::MapKeyValue        & tokens_;
    Templ::FuncProc                 func_proc_;
    bool                            throw_on_error_;
    std::string                     res_;
};

NAMESPACE_TEMPLTEXT_END

#endif // LIB_TEMPLTEXT_RENDERER_H
