#pragma once

#ifdef BUILDING_SHARED_LIB
#ifdef _WIN32
#define LIBEXPORT __declspec(dllexport)
#else
#define LIBEXPORT __attribute__((visibility("default")))
#endif
#else
#ifdef _WIN32
#define LIBEXPORT __declspec(dllimport)
#else
#define LIBEXPORT
#endif
#endif

int LIBEXPORT function_from_shared_library(int v);
