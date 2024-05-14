#include "MPStringUtil.h"

//-----------------------------------------------------------------------------
void MPStringUtil::trim(String& str, bool left, bool right) {
	static const String delims = " \t\r";
	if (right)
		str.erase(str.find_last_not_of(delims) + 1); // 移去右侧的
	if (left)
		str.erase(0, str.find_first_not_of(delims)); // 移去左侧的
}
//-----------------------------------------------------------------------------
MPStringVector MPStringUtil::split(const std::string& str, const std::string& delims, unsigned int maxSplits) {
	std::vector<std::string> ret;
	unsigned int numSplits = 0;

	// 使用STL方法
	size_t start, pos;
	start = 0;
	do {
		pos = str.find_first_of(delims, start);
		if (pos == start) {
			// 不做任何事
			start = pos + 1;
		} else if (pos == std::string::npos || (maxSplits && numSplits == maxSplits)) {
			// 拷贝剩下的字符串
			ret.push_back(str.substr(start));
			break;
		} else {
			// 拷贝到分隔符
			ret.push_back(str.substr(start, pos - start));
			start = pos + 1;
		}
		// 查找下一个需要的字符串
		start = str.find_first_not_of(delims, start);
		++numSplits;

	} while (pos != std::string::npos);

	return ret;
}
//-----------------------------------------------------------------------------
void toLowerCase(String& str) {
	std::transform(str.begin(), str.end(), str.begin(), tolower);
}
//-----------------------------------------------------------------------------
void toUpperCase(String& str) {
	std::transform(str.begin(), str.end(), str.begin(), toupper);
}
