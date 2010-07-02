/*
 * Copyright (C) 2010 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* OutputMixExt interface */

#ifdef USE_OUTPUTMIXEXT

#ifdef __cplusplus
extern "C" {
#endif
extern const SLInterfaceID SL_IID_OUTPUTMIXEXT;
#ifdef __cplusplus
}
#endif

typedef const struct SLOutputMixExtItf_ * const * SLOutputMixExtItf;

struct SLOutputMixExtItf_ {
    void (*FillBuffer)(SLOutputMixExtItf self, void *pBuffer, SLuint32 size);
};

// Track describes each input to OutputMix

struct Track {
    struct BufferQueue_interface *mBufferQueue;
    CAudioPlayer *mAudioPlayer; // mixer examines this track if non-NULL
    const void *mReader;    // pointer to next frame in BufferHeader.mBuffer
    SLuint32 mAvail;        // number of available bytes
    float mGains[STEREO_CHANNELS]; // computed gain based on volume, mute, solo, and stereo position
    SLuint32 mFrameCounter; // number of sample frames consumed from this track, will roll over
};

#ifndef this
#define this this_
#endif
extern SLresult IOutputMixExt_checkAudioPlayerSourceSink(CAudioPlayer *this);
extern void audioPlayerGainUpdate(CAudioPlayer *this);

#endif