#include "LogFile.h"

#include <assert.h>
#include <stdio.h>
#include <time.h>


LogFile::LogFile(const string& strBaseName, size_t unRollSize, bool bThreadSafe /*= false */, int nFlushInterval /*= 3*/) 
	: m_strBaseName(strBaseName)
	, m_unRollSize(unRollSize)
	, m_nFlushInterval(nFlushInterval)
	, m_nCnt(0)
	, m_pMutex(bThreadSafe ? new MutexLock() : NULL)
	, m_tStartOfPeriod(0)
	, m_tLastRoll(0)
	, m_tLaseFlush(0)
{
	assert(strBaseName.find("/") == string::npos);
	rollFile();
}

LogFile::~LogFile() {

}

void LogFile::Append(const char* pLogLine, int nLen) {
	if (m_pMutex) {
		MutexLockGuard lock(*m_pMutex);
		appendUnlocked(pLogLine, nLen);
	} else {
		appendUnlocked(pLogLine, nLen);
	}
}

void LogFile::Flush() {
	if (m_pMutex) {
		MutexLockGuard lock(*m_pMutex);
		m_pFile->Flush();
	} else {
		m_pFile->Flush();
	}
}

void LogFile::appendUnlocked(const char* pStrLogLine, int nLen) {
	m_pFile->Append(pStrLogLine, nLen);
	if (m_pFile->GetWrittenBytes() > m_unRollSize) {
		rollFile();
	} else {
		if (m_nCnt > CHECK_TIME_ROLL) {
			m_nCnt = 0;
			time_t tNow = ::time(NULL);
			time_t tThisPeriod = tNow / ROLL_PER_SECONDS * ROLL_PER_SECONDS;
			if (tThisPeriod != m_tStartOfPeriod) {
				rollFile();
			} else if (tNow - m_tLaseFlush > m_nFlushInterval) {
				m_tLaseFlush = tNow;
				m_pFile->Flush();
			}
		} else {
			++ m_nCnt;
		}
	}
}

string LogFile::getLogFileName(const string& rStrBaseNmae, time_t* pNow) {
	if (NULL == pNow) {
		assert(false);
		return rStrBaseNmae;
	}
	string strFileName;
	strFileName.reserve(rStrBaseNmae.size() + 32);
	strFileName = rStrBaseNmae;

	char timeBuff[32];
	char pidBuff[32];
	struct tm curTm;
	*pNow = time(NULL);
	gmtime_r(pNow, &curTm); // localtime_r
	strftime(timeBuff, sizeof(timeBuff), ".%Y%m%d-%H%M%S", &curTm);
	strFileName += timeBuff;

	strFileName += timeBuff;
	snprintf(pidBuff, sizeof(pidBuff), ".%d", ::getpid());
	strFileName += pidbuf;
	strFileName += ".log";

	return strFileName;

}

void LogFile::rollFile() {
	time_t tNow = 0;
	string strFileName = getLogFileName(m_strBaseName, &tNow);
	time_t tStart = tNow / ROLL_PER_SECONDS * ROLL_PER_SECONDS;
	if (tNow > m_tLastRoll) {
		m_tLastRoll = tNow;
		m_tLaseFlush = tNow;
		m_tStartOfPeriod = tStart;
		m_pFile.reset(new File(strFileName));
	}
}
