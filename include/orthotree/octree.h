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

#ifndef ORTHOTREE__OCTREE_H_INCLUDED
#define ORTHOTREE__OCTREE_H_INCLUDED
#endif

#if defined(ORTHOTREE__USE_PMR) || defined(_MSC_VER)
#ifndef ORTHOTREE__DISABLE_PMR
#define ORTHOTREE_IS_PMR_USED
#endif // !ORTHOTREE__DISABLE_PMR
#endif

#include "detail/bitset_arithmetic.h"
#include "detail/common.h"
#include "detail/inplace_vector.h"
#include "detail/internal_geometry_module.h"
#include "detail/memory_resource.h"
#include "detail/partitioning.h"
#include "detail/sequence_view.h"
#include "detail/si_mortongrid.h"
#include "detail/utils.h"
#include "detail/zip_view.h"

#include "core/build_config.h"
#include "core/configuration.h"
#include "core/entity_adapter.h"
#include "core/ot_dynamic_hash_core.h"
#include "core/ot_static_linear_core.h"
#include "core/types.h"


#include "core/ot_query.h"

#include "core/ot_managed.h"

#include "adapters/general.h"

#include "core/ot_aliases.h"
