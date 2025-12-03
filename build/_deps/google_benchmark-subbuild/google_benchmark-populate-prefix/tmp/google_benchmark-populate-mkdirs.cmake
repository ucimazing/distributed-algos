# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file LICENSE.rst or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION ${CMAKE_VERSION}) # this file comes with cmake

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "/Users/umeshdas/Coding/projects/distributed-algos/build/_deps/google_benchmark-src")
  file(MAKE_DIRECTORY "/Users/umeshdas/Coding/projects/distributed-algos/build/_deps/google_benchmark-src")
endif()
file(MAKE_DIRECTORY
  "/Users/umeshdas/Coding/projects/distributed-algos/build/_deps/google_benchmark-build"
  "/Users/umeshdas/Coding/projects/distributed-algos/build/_deps/google_benchmark-subbuild/google_benchmark-populate-prefix"
  "/Users/umeshdas/Coding/projects/distributed-algos/build/_deps/google_benchmark-subbuild/google_benchmark-populate-prefix/tmp"
  "/Users/umeshdas/Coding/projects/distributed-algos/build/_deps/google_benchmark-subbuild/google_benchmark-populate-prefix/src/google_benchmark-populate-stamp"
  "/Users/umeshdas/Coding/projects/distributed-algos/build/_deps/google_benchmark-subbuild/google_benchmark-populate-prefix/src"
  "/Users/umeshdas/Coding/projects/distributed-algos/build/_deps/google_benchmark-subbuild/google_benchmark-populate-prefix/src/google_benchmark-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/Users/umeshdas/Coding/projects/distributed-algos/build/_deps/google_benchmark-subbuild/google_benchmark-populate-prefix/src/google_benchmark-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/Users/umeshdas/Coding/projects/distributed-algos/build/_deps/google_benchmark-subbuild/google_benchmark-populate-prefix/src/google_benchmark-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
