#include ".\i18n.h"
#include <string>

const char* ConvertResString(const char* str) {
	std::string key(str);

	if ('#' == *key.begin() && '#' == *key.rbegin()) {
		std::string temp = key.substr(1, key.size() - 2);
		return g_ResourceBundleManage.LoadResString(temp.c_str());
	}

	return str;
}
