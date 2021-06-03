
#pragma once


#include <string>
#include <fstream>

namespace db {

	const std::string dbPath = "/Users/noliktop/Desktop/aydan_db/db/"; // NOLINT(cert-err58-cpp)

	class FS {

	public:
		static bool fileExists(const std::string& name);
	};

}

