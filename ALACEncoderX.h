#ifndef ALACENC_H
#define ALACENC_H

#include "IEncoder.h"
#include <stdint.h>
#include <ALACEncoder.h>

class ALACEncoderX: public IEncoder, public IEncoderStat {
    union ASBD {
        AudioStreamBasicDescription asbd;
        AudioFormatDescription afd;
    };
    std::shared_ptr<ISource> m_src;
    std::shared_ptr<ISink> m_sink;
    std::shared_ptr<ALACEncoder> m_encoder;
    std::vector<uint8_t> m_input_buffer;
    std::vector<uint8_t> m_output_buffer;
    AudioStreamBasicDescription m_iasbd;
    AudioFormatDescription m_iafd;
    ASBD m_odesc;
    EncoderStat m_stat;
public:
    ALACEncoderX(const AudioStreamBasicDescription &desc);
    void setFastMode(bool fast) { m_encoder->SetFastMode(fast); }
    uint32_t encodeChunk(UInt32 npackets);
    std::vector<uint8_t> getMagicCookie();
    void setSource(const std::shared_ptr<ISource> &source) { m_src = source; }
    void setSink(const std::shared_ptr<ISink> &sink) { m_sink = sink; }
    ISource *src() { return m_src.get(); }
    const AudioStreamBasicDescription &getInputDescription() const
    {
        return m_iasbd;
    }
    const AudioStreamBasicDescription &getOutputDescription() const
    {
        return m_odesc.asbd;
    }
    uint64_t samplesRead() const { return m_src->getPosition(); }
    uint64_t samplesWritten() const { return m_stat.samplesWritten(); }
    uint64_t framesWritten() const { return m_stat.framesWritten(); }
    double currentBitrate() const { return m_stat.currentBitrate(); }
    double overallBitrate() const { return m_stat.overallBitrate(); }

    static bool isAvailableOutputChannelLayout(uint32_t channel_layout_tag)
    {
        switch (channel_layout_tag) {
        case kAudioChannelLayoutTag_Mono:
        case kAudioChannelLayoutTag_Stereo:
        case kAudioChannelLayoutTag_AAC_3_0:
        case kAudioChannelLayoutTag_AAC_4_0:
        case kAudioChannelLayoutTag_AAC_5_0:
        case kAudioChannelLayoutTag_AAC_5_1:
        case kAudioChannelLayoutTag_AAC_6_1:
        case kAudioChannelLayoutTag_AAC_7_1:
            return true;
        }
        return false;
    }
};

#endif
