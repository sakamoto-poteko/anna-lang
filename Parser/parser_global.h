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


#ifndef PARSER_GLOBAL_H
#define PARSER_GLOBAL_H


#ifdef _WIN32
#if defined(PARSER_LIBRARY)
#define PARSERSHARED_EXPORT __declspec(dllexport)
#else
#define PARSERSHARED_EXPORT __declspec(dllimport)
#endif

#define ANSI_COLOR_RED
#define ANSI_COLOR_GREEN
#define ANSI_COLOR_YELLOW
#define ANSI_COLOR_BLUE
#define ANSI_COLOR_MAGENTA
#define ANSI_COLOR_CYAN
#define ANSI_COLOR_RESET
#define ANSI_COLOR_BOLDBLACK
#define ANSI_COLOR_BOLDRED
#define ANSI_COLOR_BOLDGREEN
#define ANSI_COLOR_BOLDYELLOW
#define ANSI_COLOR_BOLDBLUE
#define ANSI_COLOR_BOLDMAGENTA
#define ANSI_COLOR_BOLDCYAN
#define ANSI_COLOR_BOLDWHITE


#else
#define PARSERSHARED_EXPORT

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"
#define ANSI_COLOR_BOLDBLACK   "\033[1m\033[30m"
#define ANSI_COLOR_BOLDRED     "\033[1m\033[31m"
#define ANSI_COLOR_BOLDGREEN   "\033[1m\033[32m"
#define ANSI_COLOR_BOLDYELLOW  "\033[1m\033[33m"
#define ANSI_COLOR_BOLDBLUE    "\033[1m\033[34m"
#define ANSI_COLOR_BOLDMAGENTA "\033[1m\033[35m"
#define ANSI_COLOR_BOLDCYAN    "\033[1m\033[36m"
#define ANSI_COLOR_BOLDWHITE   "\033[1m\033[37m"

#endif

#include <vector>
#include <string>
#include <memory>
#include <cstdio>


typedef std::shared_ptr<std::string> gcString;

void set_log_output(FILE *out);
extern FILE *__log_out;
void log_print_indicators(int start, int width);
void log_print_pos(int row, int col, const std::string &filename);


#endif // PARSER_GLOBAL_H
