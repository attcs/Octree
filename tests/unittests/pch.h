// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

// add headers that you want to pre-compile here

#include "orthotree/detail/bitset_arithmetic.h"
#include "orthotree/detail/common.h"
#include "orthotree/detail/inplace_vector.h"
#include "orthotree/detail/internal_geometry_module.h"
#include "orthotree/detail/memory_resource.h"
#include "orthotree/detail/partitioning.h"
#include "orthotree/detail/sequence_view.h"
#include "orthotree/detail/si_mortongrid.h"
#include "orthotree/detail/utils.h"
#include "orthotree/detail/zip_view.h"

#include <gtest/gtest.h>
#include "test_helpers.h"

using namespace OrthoTreeTestHelpers;

#endif // PCH_H
