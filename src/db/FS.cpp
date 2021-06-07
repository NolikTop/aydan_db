#include "FS.h"
#include "Exception.h"

#ifdef __APPLE__
#include <Availability.h> // for deployment target to support pre-catalina targets without std::fs
#endif
#if ((defined(_MSVC_LANG) && _MSVC_LANG >= 201703L) || (defined(__cplusplus) && __cplusplus >= 201703L)) && defined(__has_include)
#if __has_include(<filesystem>) && (!defined(__MAC_OS_X_VERSION_MIN_REQUIRED) || __MAC_OS_X_VERSION_MIN_REQUIRED >= 101500)
#define GHC_USE_STD_FS
#include <filesystem>
namespace fs = std::filesystem;
#endif
#endif
#ifndef GHC_USE_STD_FS
#include <ghc/filesystem.hpp>
namespace fs = ghc::filesystem;
#endif

using namespace db;

bool FS::fileExists(const std::string &name) {
	return fs::exists(dbPath + name);
}

bool FS::createDir(const std::string &name) {
	return fs::create_directory(dbPath + name);
}

void FS::deleteDir(const std::string &name) {
	fs::remove_all(dbPath + name);
}

char FS::slash() {
#if WINDOWS
	return "\\";
#else
	return '/';
#endif
}
