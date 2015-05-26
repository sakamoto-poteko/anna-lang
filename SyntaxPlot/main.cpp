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
#include "syntaxplottersyntaxvisitor.h"

void printVersion();
void printHelp();

int main(int argc, char **argv)
{
    if (argc != 3) {
        if (argc == 2) {
            if (!std::strcmp(argv[1], "-v")) {
                printVersion();
                return 0;
            }
            if (!std::strcmp(argv[1], "-h")) {
                printHelp();
                return 0;
            }
        }
        printHelp();
        return 255;
    }


    FILE *f = std::fopen(argv[1], "r");
    if (!f) {
        std::cerr << "Cannot open file " << argv[1] << " for read." << std::endl;
        return 2;
    }

    std::string path(argv[2]);
    std::string compilationUnitName(path.substr(path.find_last_of("/\\") + 1));


    AnnaParser parser(f, "sample.anna", compilationUnitName);
    gcnCompilationUnit root = parser.parse();
    parser.printErrors();

    if (!root) {
        std::cerr << "Parse failed" << std::endl;
        return 1;
    }

    SyntaxPlotterSyntaxVisitor visitor;
    visitor.Visit(*root);
    std::string dot = visitor.generateGraph();

    std::ofstream dotfile(argv[2]);
    if (!dotfile.is_open()) {
        std::cerr << "Cannot open file " << argv[2] << " for write." << std::endl;
    }
    dotfile << dot;

    return 0;
}

void printVersion()
{
    std::cout << "annaplot" << std::endl;
}

void printHelp()
{
    std::cout << "annaplot compiled on " __DATE__ " " __TIME__"\n"
                 "Usage: annaplot sourcefile graphfile\n"
                 "       annaplot -h\n"
                 "       annaplot -v\n"
                 "\n"
                 "annaplot is a simple tool for plotting Syntax Tree of anna source"
                 "file. It generates an graphviz dot output."
              << std::endl;
}
