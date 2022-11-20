##  Copyright 2021-2022 Kevin Ingles
##
##  Permission is hereby granted, free of charge, to any person obtaining
##  a copy of this software and associated documentation files (the
##  "Software"), to deal in the Software without restriction, including
##  without limitation the right to use, copy, modify, merge, publish,
##  distribute, sublicense, and/or sell copies of the Software, and to
##  permit persons to whom the Sofware is furnished to do so, subject to
##  the following conditions:
##
##  The above copyright notice and this permission notice shall be
##  included in all copies or substantial poritions of the Software.
##
##  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
##  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
##  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
##  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
##  CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
##  TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
##  SOFTWARE OR THE USE OF OTHER DEALINGS IN THE SOFTWARE
## 
##  ==================================
##  Author: Kevi Ingles
##  File: Makefile (for testing results data struct)
##  Description: Facilitate running tests for results project
##  ==================================

SRC = ./src/
TST = ./tests/
OBJ = ./build/

TST_FILES := $(shell find $(TST) -name '*.cpp')
TST_OBJ_FILES := $(patsubst $(TST)%.cpp,$(OBJ)%.o,$(TST_FILES))
TST_INC = -I$(SRC) -I$(TST)
TST_EXE = $(patsubst $(TST)%.cpp,$(OBJ)%.x,$(TST_FILES))

# $(info $(TST_FILES))
# $(info $(TST_OBJ_FILES))
# $(info $(TST_EXE))

CC = g++
OPT = -O3
WOPT = -Wall -Werror -Wextra -Wpedantic -Wshadow -Wconversion
FOPT = -fanalyzer
CXXFLAGS = $(OPT) $(WOPT) $(FOPT)

clean:
	rm $(OBJ)/*

.PHONY: test_all

test_all: test_OwningOk test_NonowningOk test_OwningErr test_NonowningErr
test_OwningOk: $(OBJ)NonOwningOk_test.x
test_NonowningOk: $(OBJ)OwningOk_test.x
test_OwningErr: $(OBJ)NonOwningErr_test.x
test_NonowningErr: $(OBJ)OwningErr_test.x

$(OBJ)%.x: $(OBJ)%.o
	# $(info $(CC) $(CXXFLAGS) -MMD -o $@ $^ $(TST_INC))
	$(CC) $(CXXFLAGS) -MMD -o $@ $< $(TST_INC)

$(TST_OBJ_FILES): $(TST_FILES)
	# $(info $(CC) $(CXXFLAGS) -MMD -c -o $@ $< $(TST_INC))
	$(CC) $(CXXFLAGS) -MMD -c -o $@ $< $(TST_INC)

run_test_all:
	$(OBJ)NonOwningOk_test.x
	$(OBJ)OwningOk_test.x
	$(OBJ)NonOwningErr_test.x
	$(OBJ)OwningErr_test.x
