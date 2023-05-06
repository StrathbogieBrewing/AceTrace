// MIT License

// Copyright (c) 2023 Strathbogie Brewing Company

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

// Based on concepts from protothreads and local coniunations
// originally described by Adam Dunkels <adam@sics.se>       
// Main Caveats!                                             
// Dont use case statements in host code                     
// Dont use local variables, put them in the ctx struct      

#ifndef PT_H
#define PT_H

#include <stdbool.h>
#include <stdint.h>

#define PT_THREAD bool

#define PT_BUSY true
#define PT_DONE false

#define PT_CONTROL uint16_t pt_line

#define PT_INIT(ctx) (ctx)->pt_line = 0;
#define PT_SET(ctx) (ctx)->pt_line = __LINE__; case __LINE__:

#define PT_BEGIN(ctx) { switch ((ctx)->pt_line) { case 0:
#define PT_YIELD(ctx) do { bool PT_YIELD = true; PT_SET(ctx); if (PT_YIELD) return PT_BUSY; } while (0)
#define PT_WAIT_WHILE(ctx, condition) do { PT_SET(ctx); if ((condition)) return PT_BUSY; } while (0)        
#define PT_WAIT_THREAD(ctx, thread) PT_WAIT_WHILE(thread)
#define PT_RESTART(ctx) do { (ctx)->pt_line = 0; return PT_BUSY; } while (0)
#define PT_EXIT(ctx) do { (ctx)->pt_line = 0; return PT_DONE; } while (0)
#define PT_END(ctx) } (ctx)->pt_line = 0; return PT_DONE; }

#endif /* PT_H */