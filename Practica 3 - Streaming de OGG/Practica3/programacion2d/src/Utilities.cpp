#include <string>
#include <Utilities.h>

std::string extractExt(const std::string& filename) {
	return filename.substr(
		filename.rfind('.') + 1,
		filename.length() - filename.rfind('.') - 1);
}