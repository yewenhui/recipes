#ifndef _LOG_FILE_H_ 
#define _LOG_FILE_H_

#include "../thread/Mutex.h"

#include <string>
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>

using std::string;

class File 
	: private boost::noncopyable
{
public:
	explicit File(const string& rStrFileName)
		: m_pFP(::fopen(rStrFileName.data(), "ae"))
		, m_unWrittenBytes(0)
	{
		::setbuffer(m_pFP, m_Buffer, sizeof(m_Buffer));
	}

	~File() {
		::fclose(m_pFP);
	}

	void		Append(const char* pLogLine, const size_t unLength) {
		size_t n = write(pLogLine, unLength);
		size_t nRemain = unLength - n;
		while (nRemain > 0) {
			size_t x = write(pLogLine + n, nRemain);
			if (0 == x) {
				int nErr = ::ferror(m_pFP);
				if (nErr) {
					char buffer[128];
					strerror_r(nErr, buffer, sizeof(buffer));	// strerror_l
					fprintf(stderr, "LogFile::File::append() failed %s\n", buffer);
				}
				break;
			}
			n += x;
			nRemain = unLength - n;
		}

		m_unWrittenBytes += unLength;
	}

	void		Flush() {
		::fflush(m_pFP);
	}

	size_t		GetWrittenBytes() const {
		return m_unWrittenBytes;
	}

private:
	size_t		write(const char* pLogLine, const size_t unLength) {
		// return ::fwrite_unlocked(pLogLine, 1, unLength, m_pFP);
		return ::fwrite_unlocked(pLogLine, unLength, 1, m_pFP);
	}

private:
	FILE*		m_pFP;
	char		m_Buffer[64 * 1024];
	size_t		m_unWrittenBytes;

};

class LogFile
	: private boost::noncopyable
{
	enum {
		CHECK_TIME_ROLL		= 1024,
		ROLL_PER_SECONDS	= 60 * 60 * 24,
	};

public:
	LogFile(const string& strBaseName
		, size_t unRollSize
		, bool bThreadSafe = false
		, int nFlushInterval = 3);
	~LogFile();

	void			Append(const char* pLogLine, int nLen);
	void			Flush();

private:
	static string	getLogFileName(const string& rStrBaseNmae, time_t* pNow);
	
	void			appendUnlocked(const char* pStrLogLine, int nLen);
	void			rollFile();

private:
	const string	m_strBaseName;
	const unsigned int m_unRollSize;
	const int		m_nFlushInterval;

	int				m_nCnt;

	boost::scoped_ptr<MutexLock> m_pMutex;
	time_t			m_tStartOfPeriod;
	time_t			m_tLastRoll;
	time_t			m_tLaseFlush;
	boost::scoped_ptr<File> m_pFile;

};

#endif // _LOG_FILE_H_
