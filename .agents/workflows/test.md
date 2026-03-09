---
description: Run unit tests using MSVC or GCC scripts
---

To run the project's unit tests, follow these steps:

1. Determine which compiler to use (MSVC is default on Windows).
2. Run the corresponding PowerShell script:

// turbo
- For MSVC: `powershell -ExecutionPolicy Bypass -File ./tests/unittests/test_msvc.ps1`
- For GCC: `powershell -ExecutionPolicy Bypass -File ./tests/gcc/test_gcc.ps1`

Ensure you are in the project root directory before running these commands.
DO NOT RUN TESTS PROACTIVELY.