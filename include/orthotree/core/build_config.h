/*
MIT License

Copyright (c) 2021 Attila Csikós

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#pragma once

/* Settings
* Use the following define-s before the header include

// PMR is used with MSVC only by default. To use PMR anyway
#define ORTHOTREE__USE_PMR

// To disable PMR on all platforms use:
#define ORTHOTREE__DISABLE_PMR

// If the depth is less than 10, 32bit location code is enough (otherwise 64bit will be used)
#define ORTHOTREE__LOCATIONCODE_32

// Contiguous container of geometry data does not have specified index type. Octree lib uses index_t for it, it can specified to int or std::size_t.
ORTHOTREE_INDEX_T__INT / ORTHOTREE_INDEX_T__SIZE_T / ORTHOTREE_INDEX_T__UINT_FAST32_T

// By default, the node interface is not available. Use this macro to make node-related functions public.
#define ORTHOTREE_PUBLIC_NODE_INTERFACE

// Enable debug checks
#define ORTHOTREE_DEBUG_CHECKS
*/
