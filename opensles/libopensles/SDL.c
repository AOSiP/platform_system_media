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

/* SDL platform implementation */

#include "sles_allinclusive.h"

#ifdef USE_SDL

static void SDLCALL SDL_callback(void *context, Uint8 *stream, int len)
{
    assert(len > 0);
    IEngine *thisEngine = (IEngine *) context;
    // A peek would be risky if output mixes are dynamic, so we pause during switch and use shared
    interface_lock_shared(thisEngine);
    COutputMix *outputMix = thisEngine->mOutputMix;
    interface_unlock_shared(thisEngine);
    if (NULL != outputMix) {
        SLOutputMixExtItf OutputMixExt = &outputMix->mOutputMixExt.mItf;
        (*OutputMixExt)->FillBuffer(OutputMixExt, stream, (SLuint32) len);
    } else {
        memset(stream, 0, (size_t) len);
    }
}

void SDL_start(IEngine *thisEngine)
{
    SDL_AudioSpec fmt;
    fmt.freq = 44100;
    fmt.format = AUDIO_S16;
    fmt.channels = STEREO_CHANNELS;
#ifdef _WIN32 // FIXME Either a bug or a serious misunderstanding
    fmt.samples = SndFile_BUFSIZE;
#else
    fmt.samples = SndFile_BUFSIZE / sizeof(short);
#endif
    fmt.callback = SDL_callback;
    fmt.userdata = (void *) thisEngine;

    if (SDL_OpenAudio(&fmt, NULL) < 0) {
        fprintf(stderr, "Unable to open audio: %s\n", SDL_GetError());
        exit(1);
    }
}

#endif // USE_SDL