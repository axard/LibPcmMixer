#include <PcmMixer.hpp>

PcmMixer::PcmMixer()
{}

PcmMixer::~PcmMixer()
{}

void PcmMixer::SetOnChunkReadyCallback(const std::function<OnChunkReadyCallback>& callback)
{
    callback_ = callback;
}

const std::function<PcmMixer::OnChunkReadyCallback>& PcmMixer::GetOnChunkReadyCallback() const
{
    return callback_;
}
