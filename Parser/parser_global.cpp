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

#include "parser_global.h"

#include <cstdio>

void set_log_output(FILE *out)
{
    __log_out = out;
}

void log_print_indicators(int start, int width)
{
    std::fprintf(__log_out, ANSI_COLOR_BOLDGREEN);
    for (int i = 0; i < start; ++i)
        std::fprintf(__log_out, " ");
    for (int i = 0; i < width; ++i)
        std::fprintf(__log_out, "^");
    std::fprintf(__log_out, ANSI_COLOR_RESET "\n");
}



void log_print_pos(int row, int col, const std::string &filename)
{
    std::fprintf(__log_out, ANSI_COLOR_CYAN "%s:%d:%d: " ANSI_COLOR_BOLDRED "error: " ANSI_COLOR_RESET,
                 filename.c_str(), row + 1, col + 1);
}


FILE *__log_out = stdout;
