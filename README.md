# Templtext - Text Template processing library for C++

Create a template using **$VAR** or **${VAR}** tags for variables and **%foo()**
tag for functions. Pass values of variables to format function and get a formatted
result in std::string.

The template is parsed once during the creation and the values of the variables are
quickly put into placeholders during the rendering.

## Features

- Template parsing
- Variable replacement
- User defined functions in template

## Requirements

- C++11
- boost regex

## Installation

You can compile library or simply include the source files into your project. BOOST regex library is required.
In the next version BOOST regex will be replaced with std::regex.

## Building an example


``` bash
git clone https://github.com/trodevel/templtext.git
cd templtext
export BOOST_PATH=...your_boost_directory...
gcc -Wall -o example -std=c++0x -Wl,--no-as-needed -lstdc++ -lrt -ldl -lm -I$BOOST_PATH *.cpp $BOOST_PATH/stage/lib/libboost_regex.a
./example
```

## Example - Variables

``` c++
#include <iostream>                         // std::cout
 
#include "templ.h"                          // Templ
    
int main()
{
    using namespace templtext;

    Templ * t = new Templ( "Dear $SALUTATION $NAME. I would like to invite you for $TEXT. Sincerely yours, $MYNAME." );
    
    std::map<std::string, std::string> tokens01 =
    {
            { "SALUTATION", "Mr." },
            { "NAME", "John Doe" },
            { "TEXT", "an interview" },
            { "MYNAME", "Ty Coon" }
    };

    std::map<std::string, std::string> tokens02 =
    {
            { "SALUTATION", "Sweetheart" },
            { "NAME", "Mary" },
            { "TEXT", "a cup of coffee" },
            { "MYNAME", "Bob" }
    };

    std::cout << t->format( tokens01 ) << std::endl;

    std::cout << t->format( tokens02 ) << std::endl;

    delete t;
    
    return 0;
}

```

## Example - Output

```
Dear Mr. John Doe. I would like to invite you for an interview. Sincerely yours, Ty Coon.
Dear Sweetheart Mary. I would like to invite you for a cup of coffee. Sincerely yours, Bob.
```

### Example - Functions

``` c++
#include <iostream>                         // std::cout
#include <memory>                           // std::unique_ptr
 
#include "templ.h"                          // Templ
    
int main()
{
    using namespace templtext;

    std::unique_ptr<Templ> tf1( new Templ( "You have got an %decode( 1 )." ) );
    std::unique_ptr<Templ> tf2( new Templ( "You have got an %decode( 2 )." ) );
    std::unique_ptr<Templ> tf3( new Templ( "English version - %decode_loc( 1, EN )." ) );
    std::unique_ptr<Templ> tf4( new Templ( "German version  - %decode_loc( 1, DE )." ) );
    std::unique_ptr<Templ> tf5( new Templ( "Flexible version - %decode_loc( 1, $LANG )." ) );

    tf1->set_func_proc( func );
    tf2->set_func_proc( func );
    tf3->set_func_proc( func );
    tf4->set_func_proc( func );
    tf5->set_func_proc( func );

    Templ::MapKeyValue map1 =
    {
            { "LANG", "EN" }
    };

    Templ::MapKeyValue map2 =
    {
            { "LANG", "DE" }
    };

    std::cout << tf1->format() << std::endl;
    std::cout << tf2->format() << std::endl;
    std::cout << tf3->format() << std::endl;
    std::cout << tf4->format() << std::endl;
    std::cout << tf5->format( map1 ) << std::endl;
    std::cout << tf5->format( map2 ) << std::endl;
    
    return 0;
}
```

## Example - Output

```
You have got an apple.
You have got an orange.
English version - apple.
German version  - Apfel.
Flexible version - apple.
Flexible version - Apfel.
```

## Contact info

I'll appreciate your feedback. Please feel free to contact me at trodevel@gmail.com. Thanks!

## See also

Additionally you may use [TemplTextKeeper](https://github.com/trodevel/templtextkeeper) - a lightweight keeper of Templtext templates in INI files.
