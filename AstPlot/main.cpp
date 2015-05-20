/**************************************************************************
 * Copyright (c) 2015 Afa.L Cheng <afa@afa.moe>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 ***************************************************************************/


#include <iostream>
#include <cstdio>

#include <parser.h>
#include "astplottersyntaxvisitor.h"


int main()
{
    FILE *f = std::fopen("/home/afa/anna-lang/demo/sample.anna", "r");

    AnnaParser parser(f, "sample.anna");
    gcnCompilationUnit root = parser.parse();
    parser.printErrors();

    if (!root) {
        std::printf("parse failed\n");
        return 1;
    }

    ASTPlotterSyntaxVisitor visitor;
    visitor.Visit(*root);
    visitor.generateGraph("/home/afa/anna-lang/demo/sample.dot");

    return 0;
}
