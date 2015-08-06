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

// $Revision: 2276 $ $Date:: 2015-08-06 #$ $Author: serge $

#ifndef LIB_TEMPLTEXT_TEMPL_H
#define LIB_TEMPLTEXT_TEMPL_H

#include <string>                   // std::string
#include <vector>                   // std::vector
#include <map>                      // std::map
#include <set>                      // std::set
#include <functional>               // std::function

#include "namespace_lib.h"          // NAMESPACE_TEMPLTEXT_START

NAMESPACE_TEMPLTEXT_START

class Templ
{
    friend class Parser;
    friend class Renderer;

    class Elem;

    typedef std::vector<Elem*>  Elems;

    class Text;
    class Func;
    class Var;

public:
    typedef std::map<std::string, std::string>  MapKeyValue;
    typedef std::set<std::string>               SetStr;
    typedef std::function<bool( const std::string & name, const std::vector<std::string> & par )> FuncProc;

public:
    Templ( const std::string & templ, const std::string & name = std::string() );
    ~Templ();

    const std::string & get_name() const;
    const std::string & get_template() const;
    const SetStr & get_placeholders() const;
    bool validate_tokens( const MapKeyValue & tokens, std::string & missing_token ) const;
    void set_func_proc( FuncProc func_proc );

    std::string format( const MapKeyValue & tokens, bool throw_on_error = true ) const;

private:
    Templ( const Templ& )               = delete;
    Templ& operator= ( const Templ& )   = delete;

private:
    void parse();
    static void extract_placeholders( SetStr & res, const std::string & str );
    static std::string extract_name( const std::string & str );

private:
    std::string     name_;
    std::string     templ_;
    SetStr          placeholders_;
    Elems           elems_;
    FuncProc        func_proc_;
};


NAMESPACE_TEMPLTEXT_END

#endif // LIB_TEMPLTEXT_TEMPL_H
