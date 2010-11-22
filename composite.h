#ifndef _COMPOSITE_H
#define _COMPOSITE_H

#if _MSC_VER >= 1500
# include <memory>
#else
# include <boost/tr1/memory.hpp>
#endif
#include "iointer.h"

class CompositeSource: public ISource {
    typedef std::tr1::shared_ptr<ISource> source_t;
    std::vector<source_t> m_sources;
    SampleFormat m_format;
    size_t m_curpos;
public:
    CompositeSource() : m_curpos(0) {}
    const std::vector<uint32_t> *getChannelMap() const { return 0; }
    const SampleFormat &getSampleFormat() const { return m_format; }

    // takes ownership.
    void addSource(ISource *src)
    {
	if (!m_sources.size())
	    m_format = src->getSampleFormat();
	else if (m_format != src->getSampleFormat())
	    throw std::runtime_error(
		    "CompositeSource: can't compose different sample format");
	m_sources.push_back(source_t(src));
    }
    uint64_t length() const
    {
	uint64_t len = 0;
	for (size_t i = 0; i < m_sources.size(); ++i)
	    len += m_sources[i]->length();
	return len;
    }
    size_t readSamples(void *buffer, size_t nsamples)
    {
	if (m_curpos == m_sources.size())
	    return 0;
	size_t rc = m_sources[m_curpos]->readSamples(buffer, nsamples);
	if (rc == nsamples)
	    return rc;
	if (rc == 0) {
	    ++m_curpos;
	    return readSamples(buffer, nsamples);
	}
	return rc + readSamples(
	    reinterpret_cast<char*>(buffer) + rc * m_format.bytesPerFrame(),
	    nsamples - rc);
    }
    void setRange(int64_t start=0, int64_t length=-1)
    {
	throw std::runtime_error("CompositeSource::setRange: not implemented");
    }
};

#endif