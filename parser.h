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

// $Revision: 2266 $ $Date:: 2015-08-05 #$ $Author: serge $

#ifndef LIB_TEMPLTEXT_PARSER_H
#define LIB_TEMPLTEXT_PARSER_H

#include "templ.h"                  // Templ::Elems
#include "index_matches.h"          // PosLen

#include "namespace_lib.h"          // NAMESPACE_TEMPLTEXT_START

NAMESPACE_TEMPLTEXT_START

class Parser
{
    typedef Templ::SetStr   SetStr;
    typedef Templ::Elems    Elems;
    typedef Templ::Elem     Elem;
    typedef Templ::Text     Text;
    typedef Templ::Func     Func;
    typedef Templ::Var      Var;

public:
    Parser( const std::string & templ );

    const SetStr & get_placeholders() const;
    const Elems & get_elems() const;

private:
    void parse();
    void process_poslen( const std::vector<PosLen> & poslen );
    Elem* convert_match_to_elem( const std::string & str );
    Func* convert_match_to_func( const std::string & name, const std::string & par1, const std::string & par2, const std::string & par3 );
    Elem* convert_func_par_to_elem( const std::string & par );
    Var* create_var( const std::string & name );

private:
    std::string     templ_;
    SetStr          placeholders_;
    Elems           elems_;

    static const bool show_debug_output;
};


NAMESPACE_TEMPLTEXT_END

#endif // LIB_TEMPLTEXT_PARSER_H
