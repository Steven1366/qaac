#ifndef MIXER_H
#define MIXER_H

#include <complex>
#include <deque>
#include "iointer.h"
#include "soxdsp.h"

typedef std::complex<double> complex_t;

class MatrixMixer: public DelegatingSource {
    SampleFormat m_format;
    SoxModule m_module;
    std::vector<std::vector<complex_t> > m_matrix;
    x::shared_ptr<lsx_fir_t> m_filter;
    std::vector<uint8_t> m_ibuffer;
    std::vector<float> m_fbuffer[2];
    std::vector<uint32_t> m_shift_channels, m_pass_channels;
    std::deque<float> m_syncque;
    uint32_t m_shiftMask;
    size_t m_input_frames;
    bool m_end_of_input;
    double m_filter_gain;
public:
    MatrixMixer(const x::shared_ptr<ISource> &source, const SoxModule &module,
		const std::vector<std::vector<complex_t> > &spec, bool mt);
    uint64_t length() const { return -1; }
    const SampleFormat &getSampleFormat() const { return m_format; }
    const std::vector<uint32_t> *getChannelMap() const { return 0; }
    size_t readSamples(void *buffer, size_t nsamples);
private:
    size_t phaseShift(void *buffer, size_t nsamples);
};

#endif