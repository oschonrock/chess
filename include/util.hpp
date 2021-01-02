/*
    File: board.h

    Content on and manipulation of a chess board implemented as 10x12 squares,
    with default out_of_border values.

    "magic" numbers will be used, they aren't magic if you know this structure.
    Source way too messy with a lot of variable names and variable calculations instead of plain
   numbers.

    Author: Ken Rouwas
    Date 2017-09-14
*/

#pragma once

#include "board.hpp"
#include "move.hpp"

namespace chess {

// for testing purposes
unsigned long performance_test(board& b, int depth, color turn);
int run_perf_test(board& b);
int run_ai_test(board& b, color turn);

} // namespace chess
