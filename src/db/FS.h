
#pragma once


#include <string>
#include <fstream>

namespace db {

	// ПУТЬ ОБЯЗАТЕЛЬНО ДОЛЖЕН В КОНЦЕ СОДЕРЖАТЬ СЛЕШ!!!
	const std::string dbPath = "/Users/noliktop/Desktop/aydan_db/db/"; // NOLINT(cert-err58-cpp)

	class FS {

	public:
		static char slash();

		static bool fileExists(const std::string& name);
		static bool createDir(const std::string& name);
		static void deleteDir(const std::string& name);
	};

}

