#pragma once

#include "fx/detail/data_stream.h"
#include "fx/frame.h"
#include "namespace.h"
#include "fx/video/frame_interpolator.h"

FASTLED_NAMESPACE_BEGIN

FASTLED_SMART_REF(Video);

class Video : public Referent {
public:
    // frameHistoryCount is the number of frames to keep in the buffer after draw. This
    // allows for time based effects like syncing video speed to audio triggers.
    Video(size_t pixelsPerFrame, float fpsVideo, size_t frameHistoryCount = 0);
    // Api
    void begin(FileHandleRef h);
    void beginStream(ByteStreamRef s);
    bool draw(uint32_t now, CRGB* leds, uint8_t* alpha = nullptr);
    void end();
    bool rewind();

    // internal use
    bool draw(uint32_t now, Frame* frame);
    bool full() const {
        return mInterpolator->getFrames()->full();
    }

    FrameRef popOldest() {
        FrameRef frame;
        mInterpolator->pop_back(&frame);
        return frame;
    }

    void pushNewest(FrameRef frame) {
        mInterpolator->push_front(frame, frame->getTimestamp());
    }

private:
    void updateBufferIfNecessary(uint32_t now);
    uint32_t mPixelsPerFrame = 0;
    DataStreamRef mStream;
    FrameInterpolatorRef mInterpolator;
};

FASTLED_NAMESPACE_END

