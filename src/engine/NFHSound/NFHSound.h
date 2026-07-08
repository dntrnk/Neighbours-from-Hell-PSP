#ifndef NFHSOUND_H
#define NFHSOUND_H

#include <stdbool.h>

#include "../../types/music_types.h"
#include "../../types/sfx_types.h"

void NFHMusicPlay(int new_music, bool loop);
void NFHMusicStop(void);
void NFHHouseMusicLoad(void);
void NFHHouseMusicPlay(void);
void NFHHouseMusicPause(void);
void NFHHouseMusicResume(void);
void NFHHouseMusicStop(void);
void NFHSoundLoad(int sound, int channel);
void NFHSoundPlay(int sound);
void NFHSoundPreload(int sound);
void NFHSoundUnloadAll(void);

#endif // NFHSOUND_H