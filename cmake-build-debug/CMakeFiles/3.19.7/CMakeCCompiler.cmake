set(CMAKE_C_COMPILER "/run/current-system/sw/bin/cc")
set(CMAKE_C_COMPILER_ARG1 "")
set(CMAKE_C_COMPILER_ID "Clang")
set(CMAKE_C_COMPILER_VERSION "12.0.1")
set(CMAKE_C_COMPILER_VERSION_INTERNAL "")
set(CMAKE_C_COMPILER_WRAPPER "")
set(CMAKE_C_STANDARD_COMPUTED_DEFAULT "11")
set(CMAKE_C_COMPILE_FEATURES "c_std_90;c_function_prototypes;c_std_99;c_restrict;c_variadic_macros;c_std_11;c_static_assert")
set(CMAKE_C90_COMPILE_FEATURES "c_std_90;c_function_prototypes")
set(CMAKE_C99_COMPILE_FEATURES "c_std_99;c_restrict;c_variadic_macros")
set(CMAKE_C11_COMPILE_FEATURES "c_std_11;c_static_assert")

set(CMAKE_C_PLATFORM_ID "Linux")
set(CMAKE_C_SIMULATE_ID "")
set(CMAKE_C_COMPILER_FRONTEND_VARIANT "GNU")
set(CMAKE_C_SIMULATE_VERSION "")




set(CMAKE_AR "/run/current-system/sw/bin/ar")
set(CMAKE_C_COMPILER_AR "/run/current-system/sw/bin/llvm-ar")
set(CMAKE_RANLIB "/run/current-system/sw/bin/ranlib")
set(CMAKE_C_COMPILER_RANLIB "/run/current-system/sw/bin/llvm-ranlib")
set(CMAKE_LINKER "/run/current-system/sw/bin/ld")
set(CMAKE_MT "")
set(CMAKE_COMPILER_IS_GNUCC )
set(CMAKE_C_COMPILER_LOADED 1)
set(CMAKE_C_COMPILER_WORKS TRUE)
set(CMAKE_C_ABI_COMPILED TRUE)
set(CMAKE_COMPILER_IS_MINGW )
set(CMAKE_COMPILER_IS_CYGWIN )
if(CMAKE_COMPILER_IS_CYGWIN)
  set(CYGWIN 1)
  set(UNIX 1)
endif()

set(CMAKE_C_COMPILER_ENV_VAR "CC")

if(CMAKE_COMPILER_IS_MINGW)
  set(MINGW 1)
endif()
set(CMAKE_C_COMPILER_ID_RUN 1)
set(CMAKE_C_SOURCE_FILE_EXTENSIONS c;m)
set(CMAKE_C_IGNORE_EXTENSIONS h;H;o;O;obj;OBJ;def;DEF;rc;RC)
set(CMAKE_C_LINKER_PREFERENCE 10)

# Save compiler ABI information.
set(CMAKE_C_SIZEOF_DATA_PTR "8")
set(CMAKE_C_COMPILER_ABI "ELF")
set(CMAKE_C_LIBRARY_ARCHITECTURE "")

if(CMAKE_C_SIZEOF_DATA_PTR)
  set(CMAKE_SIZEOF_VOID_P "${CMAKE_C_SIZEOF_DATA_PTR}")
endif()

if(CMAKE_C_COMPILER_ABI)
  set(CMAKE_INTERNAL_PLATFORM_ABI "${CMAKE_C_COMPILER_ABI}")
endif()

if(CMAKE_C_LIBRARY_ARCHITECTURE)
  set(CMAKE_LIBRARY_ARCHITECTURE "")
endif()

set(CMAKE_C_CL_SHOWINCLUDES_PREFIX "")
if(CMAKE_C_CL_SHOWINCLUDES_PREFIX)
  set(CMAKE_CL_SHOWINCLUDES_PREFIX "${CMAKE_C_CL_SHOWINCLUDES_PREFIX}")
endif()





set(CMAKE_C_IMPLICIT_INCLUDE_DIRECTORIES "/run/current-system/sw/include;/nix/store/bwa5qgvyk898w7n47j2chmgc3gsjg207-clang-wrapper-12.0.1/resource-root/include;/nix/store/q141hd8jl7in5223jmf7kmx9h517km4p-glibc-2.32-54-dev/include")
set(CMAKE_C_IMPLICIT_LINK_LIBRARIES "gcc;gcc_s;c;gcc;gcc_s")
set(CMAKE_C_IMPLICIT_LINK_DIRECTORIES "/run/current-system/sw/lib;/nix/store/jsp3h3wpzc842j0rz61m5ly71ak6qgdn-glibc-2.32-54/lib;/nix/store/sjhz1j2d1ssn59f66kqp92xj9mpsww2d-gcc-10.3.0/lib/gcc/x86_64-unknown-linux-gnu/10.3.0;/nix/store/50msfhkz5wbyk8i78pjv3y9lxdrp7dlm-gcc-10.3.0-lib/x86_64-unknown-linux-gnu/lib;/nix/store/ydgmg92kvyhjs0swm7x3r21dqfdhcxf2-clang-12.0.1-lib/lib;/nix/store/sjhz1j2d1ssn59f66kqp92xj9mpsww2d-gcc-10.3.0/lib64/gcc/x86_64-unknown-linux-gnu/10.3.0;/nix/store/sjhz1j2d1ssn59f66kqp92xj9mpsww2d-gcc-10.3.0/lib64")
set(CMAKE_C_IMPLICIT_LINK_FRAMEWORK_DIRECTORIES "")
