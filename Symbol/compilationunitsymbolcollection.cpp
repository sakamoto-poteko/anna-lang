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


#include "compilationunitsymbolcollection.h"
#include <sstream>
#include <fstream>
#include <iostream>

CompilationUnitSymbolCollection::CompilationUnitSymbolCollection()
{

}

CompilationUnitSymbolCollection::~CompilationUnitSymbolCollection()
{

}

bool CompilationUnitSymbolCollection::exportSymbols(const std::string &filename)
{
    std::ofstream sym(filename);
    sym << exportSymbols();
    return sym.is_open();
}

std::string CompilationUnitSymbolCollection::exportSymbols()
{
    std::stringstream exported;
    exported << "Anna Metadata / 1.0" << "\n";
    exported << *compilationUnitName << "\n";
    for (auto global : globals) {
        exported << "G" << (char)0 << *global->name << "\n";
    }
    for (auto func : functions) {
        exported << "F" << (char)0 << *func->name << (char)0 << func->paramsCount << "\n";
    }
    exported << "EXTENDED\n";

    return exported.str();
}

CompilationUnitSymbolCollection CompilationUnitSymbolCollection::importSymbols(const std::string &metadata)
{
    CompilationUnitSymbolCollection result;
    std::stringstream metastream;
    metastream.str(metadata);

    std::string line;
    std::getline(metastream, line);
    if (line.compare("Anna Metadata / 1.0")) {
        std::cerr << "Not a Metadata file" << std::endl;
        return result;
    }

    if (metastream) {
        std::getline(metastream, line);
        result.compilationUnitName = std::make_shared<std::string>(line);
    } else {
        std::cerr << "Broken Metadata file" << std::endl;
        return result;
    }

    while (metastream) {
        std::getline(metastream, line);

        if (line.size() < 1)
            break;

        switch (line.at(0)) {
            case 'G':
            {
                gcVariableDeclarationSymbol sym = std::make_shared<VariableDeclarationSymbol>();
                sym->name = std::make_shared<std::string>(line.substr(2, line.size() - 2));
                result.globals.push_back(sym);
            }
                break;
            case 'F':
            {
                gcFunctionDefinitionSymbol sym = std::make_shared<FunctionDefinitionSymbol>();
                int idx = line.find_first_of((char)0, 2);
                if (idx <= 0)
                    break;
                sym->name = std::make_shared<std::string>(line.substr(2, idx - 2));
                sym->paramsCount = std::atoi(line.substr(idx + 1, line.size() - idx ).c_str());
                result.functions.push_back(sym);
            }
                break;
            default:
                break;
        }

    }

    if (metastream) {
        std::getline(metastream, line);
        if (!line.compare("EXTENDED")) {
            // Process extended meta
        }
    }

    return result;
}

CompilationUnitSymbolCollection CompilationUnitSymbolCollection::importSymbolsFromFile(const std::string &filename)
{
    std::ifstream metafile(filename, std::ios::binary);
    metafile.seekg(0, std::ios::end);
    std::streamsize size = metafile.tellg();
    metafile.seekg(0, std::ios::beg);

    if (size) {
        std::shared_ptr<char> file(new char[size]);
        if (metafile.read(file.get(), size))
            return importSymbols(std::string(file.get(), size));
        else
            return CompilationUnitSymbolCollection();
    }
    return CompilationUnitSymbolCollection();
}

void CompilationUnitSymbolCollection::print()
{
    std::cout << "Symbols for " << *compilationUnitName << std::endl;
    for (auto global : globals)
        std::cout << "    Global: " << *global->name << std::endl;

    for (auto function : functions)
        std::cout << "    Function: " << *function->name << " with "
                  << function->paramsCount << " parameters" << std::endl;
}
