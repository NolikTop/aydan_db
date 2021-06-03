#include "FS.h"
using namespace db;

bool FS::fileExists(const std::string &name) {
	std::ifstream infile(dbPath + name);

	return infile.good();
}
