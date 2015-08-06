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

// $Revision: 2275 $ $Date:: 2015-08-06 #$ $Author: serge $

#ifndef LIB_TEMPLTEXT_ELEMS_H
#define LIB_TEMPLTEXT_ELEMS_H

#include "templ.h"                  // NAMESPACE_TEMPLTEXT_START

NAMESPACE_TEMPLTEXT_START

class Templ::Elem
{
public:
    virtual ~Elem() {}
};

class Templ::Text: virtual public Templ::Elem
{
public:
    Text( const std::string & text );
    std::string text_;
};

class Templ::Func: virtual public Templ::Elem
{
public:
    Func( const std::string & name, const Templ::Elems & elems );
    ~Func();
    std::string     name_;
    Templ::Elems    elems_;
};

class Templ::Var: virtual public Templ::Elem
{
public:
    Var( const std::string & name );
    std::string name_;
};

NAMESPACE_TEMPLTEXT_END

#endif // LIB_TEMPLTEXT_ELEMS_H
