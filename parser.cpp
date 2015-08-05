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

// $Revision: 2274 $ $Date:: 2015-08-05 #$ $Author: serge $

#include "parser.h"                     // self

#include <stdexcept>                    // std::invalid_argument

#include <boost/regex.hpp>              // boost::regex - for GCC older than 4.9.0
#include <boost/algorithm/string/predicate.hpp>     // boost::algorithm::ends_with


NAMESPACE_TEMPLTEXT_START

const bool Parser::show_debug_output = true;

Parser::Parser( const std::string & templ ):
    templ_( templ )
{
    parse();
}

const Parser::SetStr & Parser::get_placeholders() const
{
    return placeholders_;
}

const Parser::Elems & Parser::get_elems() const
{
    return elems_;
}


const std::string & get_regex_match()
{
    static const std::string res(
            "%([A-Z0-9]+)$|" \
            "%([A-Z0-9]+)|" \
            "%\\{([A-Z0-9]+)\\}|" \
            "\\$([A-Za-z][A-Za-z_0-9]+)\\s*\\(" \
            "\\s*(%[A-Z0-9]+|[A-Za-z0-9]+|'[^']*')*(\\s*(,)\\s*(%[A-Z0-9]+|[A-Za-z0-9]+|'[^']*')(\\s*(,)\\s*(%[A-Z0-9]+|[A-Za-z0-9]+|'[^']*'))*)*\\s*" \
            "\\)" );
    //static const std::string res( "%\\{([A-Z0-9]+)\\}|%([A-Z0-9]+)([^A-Z0-9]|$)" );   // TEST: only variables

    return res;
}

void Parser::parse()
{
    static const boost::regex re( get_regex_match() );

    std::vector<PosLen> poslen;

    index_matches( re, templ_, poslen );

    process_poslen( poslen );

}

void Parser::process_poslen( const std::vector<PosLen> & poslen )
{
    PosLen::first_type i = 0;

    for( auto & e : poslen )
    {
        if( i < e.first )
        {
            Text * t = new Text( templ_.substr( i, e.first - i ) );
            elems_.push_back( t );
        }

        std::string match = templ_.substr( e.first, e.second );

        Elem * el = convert_match_to_elem( match );

        elems_.push_back( el );

        i = e.first + e.second;

        // std::cout << "i=" << i << std::endl; // DEBUG
    }
}

Parser::Elem* Parser::convert_match_to_elem( const std::string & str )
{
    static const boost::regex re( get_regex_match() );

    boost::smatch matches;

    static const size_t EXPECTED_SIZE   = 12;

    try
    {
        if( boost::regex_match( str, matches, re ) )
        {
            auto size = matches.size();

            if( size != EXPECTED_SIZE )
                throw std::logic_error( ( "unexpected number of groups " + std::to_string( size ) + ", expected " + std::to_string( EXPECTED_SIZE ) ).c_str() );

            std::string var_name1( matches[1].first, matches[1].second );
            std::string var_name2( matches[2].first, matches[2].second );
            std::string var_name3( matches[3].first, matches[3].second );
            std::string foo_name( matches[4].first, matches[4].second );
            std::string par1( matches[5].first, matches[5].second );
            std::string par2( matches[8].first, matches[8].second );
            std::string par3( matches[11].first, matches[11].second );

            if( show_debug_output )
            {
                for( unsigned i = 1; i < size; ++i )
                {
                    std::string res( matches[i].first, matches[i].second );
                    std::cout << "matches [" << i << "] = " << res << std::endl;  // DEBUG
                }
            }

            if( var_name1.size() || var_name2.size() || var_name3.size() )
            {
                const std::string & var_name = var_name1.size() ? var_name1 : ( var_name2.size() ? var_name2 : var_name3 );

                return create_var( var_name );
            }

            if( foo_name.size() )
            {
                return convert_match_to_func( foo_name, par1, par2, par3 );
            }

            throw std::logic_error( "unexpected match" );

            return nullptr;
        }
        else
        {
            if( show_debug_output )
                std::cout << "'" << re << "' does not match '" << str << "'. matches size(" << matches.size() << ")" << std::endl; // DEBUG
            return nullptr;
        }
    }
    catch( boost::regex_error& e )
    {
        if( show_debug_output )
            std::cout << "regex error " << e.what();

        throw std::logic_error( ( std::string( "wrong regex " ) + e.what() ).c_str() );
    }

    return nullptr;
}

Parser::Func* Parser::convert_match_to_func( const std::string & name, const std::string & par1, const std::string & par2, const std::string & par3 )
{
    Elems elems;

    elems.push_back( convert_func_par_to_elem( par1 ) );
    elems.push_back( convert_func_par_to_elem( par2 ) );
    elems.push_back( convert_func_par_to_elem( par3 ) );

    return new Func( name, elems );
}

Parser::Elem* Parser::convert_func_par_to_elem( const std::string & par )
{
    if( par.empty() )
        return new Text( "" );

    if( boost::algorithm::starts_with( par, "'" ) && boost::algorithm::ends_with( par, "'") && par.size() > 1 )
        return new Text( par.substr( 1 , par.size() - 2 ) );

    if( boost::algorithm::starts_with( par, "%" ) && par.size() > 1 )
        return create_var( par.substr( 1 , par.size() - 1 ) );

    return new Text( par );
}

Parser::Var* Parser::create_var( const std::string & name )
{
    placeholders_.insert( name );

    return new Var( name );
}

NAMESPACE_TEMPLTEXT_END
