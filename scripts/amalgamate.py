#!/usr/bin/env python3
import os
import re
import sys
import subprocess

# Fully comprehensive list of C++ standard library headers to deduplicate and move to the top
STD_HEADERS = {
    "algorithm", "any", "array", "atomic", "barrier", "bit", "bitset", "charconv", "chrono",
    "compare", "complex", "concepts", "condition_variable", "coroutine", "deque", "exception",
    "execution", "filesystem", "format", "forward_list", "fstream", "functional", "future",
    "initializer_list", "iomanip", "ios", "iosfwd", "iostream", "istream", "iterator", "latch",
    "limits", "list", "locale", "map", "memory", "memory_resource", "mutex", "new", "numbers",
    "numeric", "optional", "ostream", "queue", "random", "ranges", "ratio", "regex", "scoped_allocator",
    "semaphore", "set", "shared_mutex", "source_location", "span", "sstream", "stack", "stdexcept",
    "stop_token", "streambuf", "string", "string_view", "strstream", "syncstream", "system_error",
    "thread", "tuple", "typeindex", "typeinfo", "type_traits", "unordered_map", "unordered_set",
    "utility", "valarray", "variant", "vector", "version",
    # C compatibility headers
    "cassert", "cctype", "cerrno", "cfenv", "cfloat", "cinttypes", "climits", "clocale", "cmath",
    "csetjmp", "csignal", "cstdarg", "cstddef", "cstdint", "cstdio", "cstdlib", "cstring",
    "ctime", "cuchar", "cwchar", "cwctype"
}

# The unique license block to be printed at the top of the file
LICENSE_BLOCK = """/*
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
*/"""

def get_guard_info(filepath):
    normalized = filepath.replace("\\", "/")
    if "/adapters/eigen.h" in normalized or "/serialization/adapters/eigen.h" in normalized:
        # Eigen is automatically enabled if ORTHOTREE_EIGEN_SUPPORT is defined, 
        # OR if Eigen's geometry module has already been included.
        return "EIGEN", "#if defined(ORTHOTREE_EIGEN_SUPPORT) || defined(EIGEN_GEOMETRY_MODULE_H)"
    elif "/adapters/glm.h" in normalized or "/serialization/adapters/glm.h" in normalized:
        # GLM is automatically enabled if ORTHOTREE_GLM_SUPPORT is defined,
        # OR if GLM has already been included.
        return "GLM", "#if defined(ORTHOTREE_GLM_SUPPORT) || defined(GLM_VERSION)"
    elif "/adapters/boost.h" in normalized or "/serialization/adapters/boost.h" in normalized:
        # Boost is automatically enabled if ORTHOTREE_BOOST_SUPPORT is defined,
        # OR if Boost Geometry has already been included.
        return "BOOST", "#if defined(ORTHOTREE_BOOST_SUPPORT) || defined(BOOST_GEOMETRY_HPP) || defined(BOOST_VERSION)"
    elif "/adapters/cgal.h" in normalized or "/serialization/adapters/cgal.h" in normalized:
        # CGAL is automatically enabled if ORTHOTREE_CGAL_SUPPORT is defined,
        # OR if CGAL has already been included.
        return "CGAL", "#if defined(ORTHOTREE_CGAL_SUPPORT) || defined(CGAL_VERSION) || defined(CGAL_OCTREE_H)"
    elif "/adapters/unreal.h" in normalized:
        # Unreal remains explicit support macro since its headers are environment specific.
        return "UNREAL", "#if defined(ORTHOTREE_UNREAL_SUPPORT)"
    elif "/serialization/msgpack_archive.h" in normalized:
        # MsgPack is automatically enabled if ORTHOTREE_SERIALIZATION_MSGPACK_SUPPORT is defined,
        # OR if msgpack has already been included.
        return "MSGPACK", "#if defined(ORTHOTREE_SERIALIZATION_MSGPACK_SUPPORT) || defined(MSGPACK_HPP)"
    return None, None

def strip_licenses_and_pragmas(text):
    # Strips all block comments containing "mit license", "copyright", or "attila" to deduplicate licenses
    def comment_replacer(match):
        comment = match.group(0)
        comment_lower = comment.lower()
        if "mit license" in comment_lower or "copyright" in comment_lower or "attila" in comment_lower:
            return ""
        return comment
    text = re.sub(r'/\*.*?\*/', comment_replacer, text, flags=re.DOTALL)
    
    # Strip C++ style line comments that might be license-related
    text = re.sub(r'//\s*Copyright.*?\n', '\n', text, flags=re.IGNORECASE)
    text = re.sub(r'//\s*MIT License.*?\n', '\n', text, flags=re.IGNORECASE)
    text = re.sub(r'//\s*Attila Cs.*?\n', '\n', text, flags=re.IGNORECASE)
    
    return text

def amalgamate(root_dir, entry_points, output_file):
    processed = set()
    system_includes = set()
    output_lines = []

    def process_file(filepath):
        canonical_path = os.path.abspath(filepath)
        if canonical_path in processed:
            return []
        processed.add(canonical_path)

        if not os.path.exists(filepath):
            print(f"Warning: File not found: {filepath}", file=sys.stderr)
            return []

        print(f"Processing: {os.path.relpath(filepath, root_dir)}")

        with open(filepath, 'r', encoding='utf-8-sig') as f:
            content = f.read()

        # Clean license headers from the file content
        content = strip_licenses_and_pragmas(content)

        # Process the clean content line-by-line
        lines = []
        for line in content.splitlines(keepends=True):
            # Skip #pragma once
            if line.strip().startswith('#pragma once'):
                continue

            # Match local includes like #include "core/types.h" or #include "../adapters/xyz.h"
            m = re.match(r'^\s*#include\s+"([^"]+)"', line)
            if m:
                include_rel = m.group(1)
                current_dir = os.path.dirname(filepath)
                include_abs = os.path.abspath(os.path.join(current_dir, include_rel))

                guard_name, guard_expr = get_guard_info(include_abs)
                inlined_lines = process_file(include_abs)

                if inlined_lines:
                    if guard_name:
                        lines.append(f"\n// --- Start of Guarded Adapter: {guard_name} ---\n")
                        lines.append(f"{guard_expr}\n")
                        lines.extend(inlined_lines)
                        lines.append("\n#endif\n")
                        lines.append(f"// --- End of Guarded Adapter: {guard_name} ---\n\n")
                    else:
                        lines.extend(inlined_lines)
                continue

            # Match system includes like #include <vector>
            m_sys = re.match(r'^\s*#include\s+<([^>]+)>', line)
            if m_sys:
                header_name = m_sys.group(1)
                if header_name in STD_HEADERS:
                    system_includes.add(header_name)
                    continue

            lines.append(line)

        return lines

    # Process all entry points
    for entry in entry_points:
        entry_abs = os.path.abspath(os.path.join(root_dir, entry))
        guard_name, guard_expr = get_guard_info(entry_abs)
        inlined_lines = process_file(entry_abs)
        if inlined_lines:
            if guard_name:
                output_lines.append(f"\n// --- Start of Guarded Adapter: {guard_name} ---\n")
                output_lines.append(f"{guard_expr}\n")
                output_lines.extend(inlined_lines)
                output_lines.append("\n#endif\n")
                output_lines.append(f"// --- End of Guarded Adapter: {guard_name} ---\n\n")
            else:
                output_lines.extend(inlined_lines)

    # Build the final single-header content
    final_content = []
    final_content.append(LICENSE_BLOCK + "\n\n")
    final_content.append("/*\n")
    final_content.append(" * OrthoTree - Single Header Edition\n")
    final_content.append(" * Amalgamated version containing Octree, BVH, and Serialization modules.\n")
    final_content.append(" * Generated automatically.\n")
    final_content.append(" */\n\n")
    final_content.append("#pragma once\n\n")

    # Add system headers sorted alphabetically
    final_content.append("// --- Standard Library Dependencies ---\n")
    for header in sorted(system_includes):
        final_content.append(f"#include <{header}>\n")
    final_content.append("\n")

    # Add the code lines
    final_content.extend(output_lines)

    # Write to output file
    os.makedirs(os.path.dirname(output_file), exist_ok=True)
    with open(output_file, 'w', encoding='utf-8') as f:
        f.writelines(final_content)

    print(f"\nSuccessfully amalgamated {len(processed)} files into {output_file}!")

    # Format the generated file using clang-format
    try:
        print("Running clang-format on the generated file...")
        subprocess.run(["clang-format", "-i", output_file], check=True)
        print("clang-format completed successfully!")
    except Exception as e:
        print(f"Warning: Could not run clang-format: {e}. The file is still fully functional.", file=sys.stderr)

if __name__ == "__main__":
    base_dir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
    root_include = os.path.join(base_dir, "include")
    
    entries = [
        "orthotree/octree.h",
        "orthotree/bvh.h",
        "orthotree/serialization.h",
        "orthotree/serialization/stl.h",
        "orthotree/serialization/binary_archive.h",
        "orthotree/serialization/msgpack_archive.h",
        "orthotree/serialization/adapters/xyz.h",
        "orthotree/serialization/adapters/eigen.h",
        "orthotree/serialization/adapters/glm.h",
        "orthotree/serialization/adapters/boost.h",
        "orthotree/serialization/adapters/cgal.h",
        "orthotree/adapters/boost.h",
        "orthotree/adapters/cgal.h",
        "orthotree/adapters/eigen.h",
        "orthotree/adapters/glm.h",
        "orthotree/adapters/unreal.h",
        "orthotree/adapters/xyz.h"
    ]
    
    out_file = os.path.join(base_dir, "single_include", "orthotree.h")
    amalgamate(root_include, entries, out_file)
