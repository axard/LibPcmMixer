#include <PcmMixer.hpp>

namespace Pcm {

Mixer::Mixer()
{}

Mixer::~Mixer()
{}

void Mixer::SetOnChunkReadyCallback(const std::function<OnChunkReadyCallback>& callback)
{
    callback_ = callback;
}

const std::function<Mixer::OnChunkReadyCallback>& Mixer::GetOnChunkReadyCallback() const
{
    return callback_;
}

} // namespace Pcm
