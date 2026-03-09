# User Rules for Octree Project

## Safety and Destructive Operations
- **Strictly Prohibited**: `git reset --hard`, `git clean -fd` or any destructive git commands
- **File Deletion**: Do not delete any files unless explicitly instructed to remove.
- **External libraries**: Do not modify external libraries (subfolders: `packages`, `external`, `libs`, `vendor`, `vcpkg_installed`). Scan only allowed if the USER explicitly asks to. If you write a script beaware of these folders.

## Build and Compilation
- **No Automatic Builds**: Do not automatically trigger build scripts (e.g., `test_msvc.ps1`, `benchmark.ps1`, `cmake --build`) after editing files. 
- **Explicit Request Only**: Only run compilation and tests when the USER explicitly asks to:
  - "Run tests"
  - "Verify build"
  - "Iterate on compile errors"
- When a build is requested, report the status clearly but do not proceed to fix errors automatically unless the USER approves the next step.

## Refactoring and Bulk Operations
- **Encoding Awareness**: Be cautious with file encodings. The project may contain UTF-8 and UTF-16 files. Always verify the encoding before writing.
- **Validation**: When performing bulk operations, prefer precise manual edits or highly targeted regex over broad search-and-replace.
- **Task scope**: Focus on the task, do not change proactively other lines (e.g. do not solve build errors proactively).
