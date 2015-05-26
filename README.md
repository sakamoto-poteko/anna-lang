# Anna Language

__Anna language (annalang)__ is an experimental programming language designed for ~~_flirting with high school girl_~~.

## Prequesities
- Build System
  - QMake
- Parser
  - lex
  - C++11 compiler
- SyntaxPlot
  - Boost Graph Library
  - C++14 compiler with generic lambda argument support (Minimum GCC 4.9 and VS 2015)
  - Graphviz for converting dot file to image
- Symbol
  - C++11 compiler

## Modules

- Parser: Tokenizer (lex) and parser (hand-written recursive descent parser) of annalang
- SyntaxPlot: Graph generator for Syntax Tree
- ParserTest: Parser driver used in development. Can be treated as a minimal example
- Symbol: Import and export symbol, generate symbol tree for Syntax Tree

## Language Demo
```
import stdlib;   -_- This is optional. stdlib is always imported >_< -_- Another comment >_<

var a`6 = false;

def @main()
{
    @nna1;
}

def @nna1(a`8, a`9, a`10)
{
    var 50USD   = 6.33          -_- This is a special case for ``var ana'' >_<
    var 500USD  = 4             -_- This is a special case for ``var anna'' >_<
    var annna   = 59.663        -_- Semicolon is not necessary >_<
    var a`4     = true;
    var annnnna = "ann aaannnnaa nannaanaa!";
    -_-var bnnnnnnb = "dummy";>_<
    -_- Syntax Error: expected variable, but got `bnnnnnnb' >_<

    -_-
    This is a multiline comment.
    Second line
    Thrid line
    >_<

    if (anna == a`2)    -_- a`n is just a syntactic sugar >_<
        printf "Num int %d\n", a`2;
    else
        printf("Won't print\n");

    return anna;
}

def @bnnb(a`1, a`2, a`3, a`4)
{
    return

    while (a`2) {
        @anna1(a`1, a`2, a`3);
    }

    while (ana + 5 == 6) {    }

    while ((annna % 5) < a`1)
        ;

    while (a`4)
}
```

## Syntax Tree Demo
See https://github.com/sakamoto-poteko/anna-lang/blob/master/demo/sample.svg for directed graph of Syntax Tree.

## License
```
   Copyright (c) 2015 Afa.L Cheng <afa@afa.moe>

   Permission is hereby granted, free of charge, to any person obtaining
   a copy of this software and associated documentation files (the
   "Software"), to deal in the Software without restriction, including
   without limitation the rights to use, copy, modify, merge, publish,
   distribute, sublicense, and/or sell copies of the Software, and to
   permit persons to whom the Software is furnished to do so, subject to
   the following conditions:

   The above copyright notice and this permission notice shall be
   included in all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
   NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
   LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
   OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
   WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
```

## Additional Licenses
See https://github.com/sakamoto-poteko/anna-lang/blob/master/additional_licenses.
