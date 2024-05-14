#include "MPDataStream.h"

#include "MPStringUtil.h"
#include "log.h"

//=============================================================================
// MPDataStream
//=============================================================================
template <typename T>
MPDataStream& MPDataStream::operator>>(T& val) {
	read(static_cast<void*>(&val), sizeof(T));
}
//-----------------------------------------------------------------------------
String MPDataStream::getLine(bool trimAfter) {
	MPStringUtil::StrStreamType str;
	size_t c = MP_STREAM_TEMP_SIZE - 1;
	// 一直循环直到遇到分隔符
	while (c == MP_STREAM_TEMP_SIZE - 1) {
		c = readLine(m_TmpArea, MP_STREAM_TEMP_SIZE - 1);
		str << m_TmpArea;
	}

	String retString(str.str());
	if (trimAfter) {
		MPStringUtil::trim(retString);
	}

	return retString;
}
//-----------------------------------------------------------------------------
String MPDataStream::getAsString(void) {
	// 读取整个缓冲
	char* pBuf = new char[m_Size + 1];
	read(pBuf, m_Size);
	pBuf[m_Size] = '\0';
	String str;
	str.insert(0, pBuf, m_Size);
	delete[] pBuf;
	return str;
}
//=============================================================================
// MPMemoryDataStream
//=============================================================================
MPMemoryDataStream::MPMemoryDataStream(void* pMem, size_t size, bool freeOnClose)
	: MPDataStream() {
	m_Data = m_Pos = static_cast<uchar*>(pMem);
	m_Size = size;
	m_End = m_Data + m_Size;
	m_FreeOnClose = freeOnClose;
}
//-----------------------------------------------------------------------------
MPMemoryDataStream::MPMemoryDataStream(const String& name, void* pMem, size_t size,
									   bool freeOnClose)
	: MPDataStream(name) {
	m_Data = m_Pos = static_cast<uchar*>(pMem);
	m_Size = size;
	m_End = m_Data + m_Size;
	m_FreeOnClose = freeOnClose;
}
//-----------------------------------------------------------------------------
MPMemoryDataStream::MPMemoryDataStream(MPDataStream& sourceStream, bool freeOnClose)
	: MPDataStream() {
	// 从另一个流拷贝数据
	m_Size = sourceStream.size();
	m_Data = new uchar[m_Size];
	sourceStream.read(m_Data, m_Size);
	m_Pos = m_Data;
	m_End = m_Data + m_Size;
	m_FreeOnClose = freeOnClose;
}
//-----------------------------------------------------------------------------
MPMemoryDataStream::MPMemoryDataStream(MPDataStreamPtr& sourceStream, bool freeOnClose)
	: MPDataStream() {
	// 从另一个流拷贝数据
	m_Size = sourceStream->size();
	m_Data = new uchar[m_Size];
	sourceStream->read(m_Data, m_Size);
	m_Pos = m_Data;
	m_End = m_Data + m_Size;
	m_FreeOnClose = freeOnClose;
}
//-----------------------------------------------------------------------------
MPMemoryDataStream::MPMemoryDataStream(const String& name, MPDataStream& sourceStream, bool freeOnClose)
	: MPDataStream(name) {
	// 从另一个流拷贝数据
	m_Size = sourceStream.size();
	m_Data = new uchar[m_Size];
	sourceStream.read(m_Data, m_Size);
	m_Pos = m_Data;
	m_End = m_Data + m_Size;
	m_FreeOnClose = freeOnClose;
}
//-----------------------------------------------------------------------------
MPMemoryDataStream::MPMemoryDataStream(const String& name, const MPDataStreamPtr& sourceStream, bool freeOnClose)
	: MPDataStream(name) {
	// 从另一个流拷贝数据
	m_Size = sourceStream->size();
	m_Data = new uchar[m_Size];
	sourceStream->read(m_Data, m_Size);
	m_Pos = m_Data;
	m_End = m_Data + m_Size;
	m_FreeOnClose = freeOnClose;
}
//-----------------------------------------------------------------------------
MPMemoryDataStream::MPMemoryDataStream(size_t size, bool freeOnClose)
	: MPDataStream() {
	m_Size = size;
	m_Data = new uchar[m_Size];
	m_Pos = m_Data;
	m_End = m_Data + m_Size;
	m_FreeOnClose = freeOnClose;
}
//-----------------------------------------------------------------------------
MPMemoryDataStream::MPMemoryDataStream(const String& name, size_t size, bool freeOnClose)
	: MPDataStream(name) {
	m_Size = size;
	m_Data = new uchar[m_Size];
	m_Pos = m_Data;
	m_End = m_Data + m_Size;
	m_FreeOnClose = freeOnClose;
}
//-----------------------------------------------------------------------------
MPMemoryDataStream::~MPMemoryDataStream() {
	close();
}
//-----------------------------------------------------------------------------
size_t MPMemoryDataStream::read(void* buf, size_t count) {
	size_t cnt = count;

	if (m_Pos + cnt > m_End)
		cnt = m_End - m_Pos;
	if (cnt == 0)
		return 0;

	memcpy(buf, m_Pos, cnt);
	m_Pos += cnt;
	return cnt;
}
//-----------------------------------------------------------------------------
size_t MPMemoryDataStream::readLine(char* buf, size_t maxCount, const String& delim) {
	// 这里处理Unix和Windows的文件（基于 DOS/Windows 的文本文件在每一行末
	// 尾有一个 CR（回车）和 LF（换行），而 UNIX 文本只有一个换行）
	bool trimCR = false;
	if (delim.find_first_of('\n') != String::npos) {
		trimCR = true;
	}

	// 查找下一个分隔符("\n")的位置
	size_t pos = strcspn((const char*)m_Pos, delim.c_str());
	if (pos > maxCount)
		pos = maxCount;

	// 确保pos不会超出eof
	if (m_Pos + pos > m_End)
		pos = m_End - m_Pos;

	if (pos > 0) {
		memcpy(buf, (const void*)m_Pos, pos);
	}

	// 重新设置目前的指针位置
	m_Pos += pos + 1;

	// 剔除CR（回车）和 LF（换行）
	if (trimCR && buf[pos - 1] == '\r') {
		// 删除'\r'
		--pos;
	}
	// 中止符
	buf[pos] = '\0';

	return pos;
}
//-----------------------------------------------------------------------------
size_t MPMemoryDataStream::skipLine(const String& delim) {
	// 查找下一个分隔符("\n")的位置
	size_t pos = strcspn((const char*)m_Pos, delim.c_str());

	// 确保pos不会超出eof
	if (m_Pos + pos > m_End)
		pos = m_End - m_Pos;

	m_Pos += pos + 1;

	return pos;
}
//-----------------------------------------------------------------------------
void MPMemoryDataStream::skip(long count) {
	size_t newpos = (size_t)((m_Pos - m_Data) + count);
	assert(m_Data + newpos <= m_End);

	m_Pos = m_Data + newpos;
}
//-----------------------------------------------------------------------------
void MPMemoryDataStream::seek(size_t pos) {
	assert(m_Data + pos <= m_End);
	m_Pos = m_Data + pos;
}
//-----------------------------------------------------------------------------
size_t MPMemoryDataStream::tell(void) const {
	return m_Pos - m_Data;
}
//-----------------------------------------------------------------------------
bool MPMemoryDataStream::eof(void) const {
	return m_Pos >= m_End;
}
//-----------------------------------------------------------------------------
void MPMemoryDataStream::close(void) {
	if (m_FreeOnClose && m_Data) {
		delete[] m_Data;
		m_Data = 0;
	}
}
//=============================================================================
// MPFileStreamDataStream
//=============================================================================
MPFileStreamDataStream::MPFileStreamDataStream(std::ifstream* s, bool freeOnClose)
	: MPDataStream() {
	// 计算大小
	m_pStream->seekg(0, std::ios_base::end);
	m_Size = m_pStream->tellg();
	m_pStream->seekg(0, std::ios_base::beg);
}
//-----------------------------------------------------------------------------
MPFileStreamDataStream::MPFileStreamDataStream(const String& name, std::ifstream* s, bool freeOnClose)
	: MPDataStream(name) {
	// 计算大小
	m_pStream->seekg(0, std::ios_base::end);
	m_Size = m_pStream->tellg();
	m_pStream->seekg(0, std::ios_base::beg);
}
//-----------------------------------------------------------------------------
MPFileStreamDataStream::MPFileStreamDataStream(const String& name, std::ifstream* s, size_t size, bool freeOnClose)
	: MPDataStream(name) {
	m_Size = size;
}
//-----------------------------------------------------------------------------
MPFileStreamDataStream::~MPFileStreamDataStream() {
	close();
}
//-----------------------------------------------------------------------------
size_t MPFileStreamDataStream::read(void* buf, size_t count) {
	m_pStream->read(static_cast<char*>(buf), count);
	return m_pStream->gcount();
}
//-----------------------------------------------------------------------------
size_t MPFileStreamDataStream::readLine(char* buf, size_t maxCount, const String& delim) {
	if (delim.empty()) {
		LG("No delimiter provided", "FileStreamDataStream::readLine");
		assert(0);
	}
	if (delim.size() > 1) {
		LG("WARNING: FileStreamDataStream::readLine", "using only first delimeter");
	}
	// 这里处理Unix和Windows的文件
	bool trimCR = false;
	if (delim.at(0) == '\n') {
		trimCR = true;
	}
	// 因为不包括中止符，所有要maxCount + 1
	m_pStream->getline(buf, maxCount + 1, delim.at(0));
	size_t ret = m_pStream->gcount();
	// 3种情况
	// 1) 遇到了eof
	// 2) 超过缓存的空间
	// 3) 读取了一整行 - 这种情况下分隔符在流中，但并不写入到缓存，所以
	//    读取数据的长度为ret-1,结尾在ret-2处
	// 所有情况缓存都以null结尾

	if (m_pStream->eof()) {
		// 没问题
	} else if (m_pStream->fail()) {
		// Did we fail because of maxCount hit? No - no terminating character
		// in included in the count in this case
		if (ret == maxCount) {
			// clear failbit for next time
			m_pStream->clear();
		} else {
			LG("Streaming error occurred", "FileStreamDataStream::readLine");
			assert(0);
		}
	} else {
		--ret;
	}

	// 剔除CR（回车）和 LF（换行）
	if (trimCR && buf[ret - 1] == '\r') {
		--ret;
		buf[ret] = '\0';
	}
	return ret;
}
//-----------------------------------------------------------------------------
size_t MPFileStreamDataStream::skipLine(const String& delim) {
	size_t c = MP_STREAM_TEMP_SIZE - 1;
	size_t total = 0;
	while (c == MP_STREAM_TEMP_SIZE - 1) {
		c = readLine(m_TmpArea, MP_STREAM_TEMP_SIZE - 1);
		total += c;
	}
	return total;
}
//-----------------------------------------------------------------------------
void MPFileStreamDataStream::skip(long count) {
	m_pStream->clear(); //万一eof已经设置，则清除
	m_pStream->seekg(static_cast<std::ifstream::pos_type>(count), std::ios::cur);
}
//-----------------------------------------------------------------------------
void MPFileStreamDataStream::seek(size_t pos) {
	m_pStream->clear(); //万一eof已经设置，则清除
	m_pStream->seekg(static_cast<std::ifstream::pos_type>(pos), std::ios::beg);
}
//-----------------------------------------------------------------------------
size_t MPFileStreamDataStream::tell(void) const {
	m_pStream->clear(); //万一eof已经设置，则清除
	return m_pStream->tellg();
}
//-----------------------------------------------------------------------------
bool MPFileStreamDataStream::eof(void) const {
	return m_pStream->eof();
}
//-----------------------------------------------------------------------------
void MPFileStreamDataStream::close(void) {
	if (m_pStream) {
		m_pStream->close();
		if (m_FreeOnClose) {
			delete m_pStream;
			m_pStream = 0;
		}
	}
}
//=============================================================================
// MPFileHandleDataStream
//=============================================================================
MPFileHandleDataStream::MPFileHandleDataStream(FILE* handle)
	: MPDataStream(), m_FileHandle(handle) {
	// 计算大小
	fseek(m_FileHandle, 0, SEEK_END);
	m_Size = ftell(m_FileHandle);
	fseek(m_FileHandle, 0, SEEK_SET);
}
//-----------------------------------------------------------------------------
MPFileHandleDataStream::MPFileHandleDataStream(const String& name, FILE* handle)
	: MPDataStream(name), m_FileHandle(handle) {
	// 计算大小
	fseek(m_FileHandle, 0, SEEK_END);
	m_Size = ftell(m_FileHandle);
	fseek(m_FileHandle, 0, SEEK_SET);
}
//-----------------------------------------------------------------------------
MPFileHandleDataStream::~MPFileHandleDataStream() {
	close();
}
//-----------------------------------------------------------------------------
size_t MPFileHandleDataStream::read(void* buf, size_t count) {
	return fread(buf, count, 1, m_FileHandle);
}
//-----------------------------------------------------------------------------
size_t MPFileHandleDataStream::readLine(char* buf, size_t maxCount, const String& delim) {
	// 这里处理Unix和Windows的文件
	bool trimCR = false;
	if (delim.find_first_of('\n') != String::npos) {
		trimCR = true;
	}

	size_t chunkSize = min(maxCount, (size_t)MP_STREAM_TEMP_SIZE - 1);
	size_t totalCount = 0;
	size_t readCount;
	while (chunkSize && (readCount = fread(m_TmpArea, chunkSize, 1, m_FileHandle))) {
		// 中止符
		m_TmpArea[readCount] = '\0';
		// 查找第一个分隔符的位置
		size_t pos = strcspn(m_TmpArea, delim.c_str());

		if (pos < readCount) {
			// 找到分隔符，向后重新定位位置
			fseek(m_FileHandle, pos - readCount + 1, SEEK_CUR);
		}

		if (pos > 0) {
			// 如果找到CR，提前中止
			if (trimCR && m_TmpArea[pos - 1] == '\r') {
				--pos;
			}

			if (buf) {
				memcpy(buf, (const void*)m_TmpArea, pos);
				buf[pos] = '\0';
			}
			totalCount += pos;
		}

		if (pos < readCount) {
			break;
		}
		// 为下次查找重新调整块大小
		chunkSize = min(maxCount - totalCount, (size_t)MP_STREAM_TEMP_SIZE - 1);
	}
	return totalCount;
}
//-----------------------------------------------------------------------------
size_t MPFileHandleDataStream::skipLine(const String& delim) {
	// 重用readLine(), 但不拷贝数据
	char* nullBuf = 0;
	return readLine(nullBuf, 1024, delim);
}
//-----------------------------------------------------------------------------
void MPFileHandleDataStream::skip(long count) {
	fseek(m_FileHandle, count, SEEK_CUR);
}
//-----------------------------------------------------------------------------
void MPFileHandleDataStream::seek(size_t pos) {
	fseek(m_FileHandle, pos, SEEK_SET);
}
//-----------------------------------------------------------------------------
size_t MPFileHandleDataStream::tell(void) const {
	return ftell(m_FileHandle);
}
//-----------------------------------------------------------------------------
bool MPFileHandleDataStream::eof(void) const {
	return feof(m_FileHandle) != 0;
}
//-----------------------------------------------------------------------------
void MPFileHandleDataStream::close(void) {
	fclose(m_FileHandle);
	m_FileHandle = 0;
}
