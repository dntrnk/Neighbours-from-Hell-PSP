#include "NFHSound.h"

#include <stdlib.h>

#include "../audio/pspaalib.h"
#include "../../objects/scene_manager.h"

#define MUSIC_CHANNEL 5
#define HOUSE_MUSIC_CHANNEL 6

typedef struct {
    const char* const path;
    const float volume;
    const bool priority;
} SfxBind;

static const char* const music_bindings[] = {
    [MUSIC_DNTRNK] = "assets_my/music/dntrnk.at3",
    [MUSIC_TITEL] = "assets_thq/music/titel.at3",
    [MUSIC_JINGLE_LEVELSTART] = "assets_thq/music/jingle_levelstart.at3",
    [MUSIC_INGAME1_NORMAL] = "assets_thq/music/ingame1_normal.at3",
    [MUSIC_INGAME2_NORMAL] = "assets_thq/music/ingame2_normal.at3",
    [MUSIC_JINGLE_SUCCESS_NORMAL] = "assets_thq/music/jingle_success_normal.at3", 
    [MUSIC_JINGLE_FAILED] = "assets_thq/music/jingle_failed.at3"
};

static const SfxBind sfx_bindings[] = {
    [SOUND_BUT1] = {"assets_thq/sfx/but1.wav", 1.0f, false},
    [SOUND_BUT_HOVER1] = {"assets_thq/sfx/but_hover1.wav", 1.0f, false},
    [SOUND_DOOR_CLOSE1] = {"assets_thq/sfx/door_close1.wav", 1.0f, false},
    [SOUND_DOOR_OPEN1] = {"assets_thq/sfx/door_open1.wav", 1.0f, false},
    [SOUND_ILLEGAL] = {"assets_thq/sfx/illegal.wav", 0.8f, false},
    [SOUND_LEVELSTART] = {"assets_thq/sfx/levelstart.wav", 1.0f, false},
    [SOUND_WOD_JUHU1] = {"assets_thq/sfx/wod_juhu1.wav", 1.0f, false},
    [SOUND_WOD_LAUGH1] = {"assets_thq/sfx/wod_laugh1.wav", 1.0f, false},
    [SOUND_WOD_STEP1A] = {"assets_thq/sfx/wod_step1a.wav", 1.0f, false},
    [SOUND_WOD_STEP2A] = {"assets_thq/sfx/wod_step2a.wav", 1.0f, false},
    [SOUND_OBJ_OPEN1] = {"assets_thq/sfx/obj_open1.wav", 1.0f, false},
    [SOUND_OBJ_CLOSE1] = {"assets_thq/sfx/obj_close1.wav", 1.0f, false},
    [SOUND_WOD_HA1] = {"assets_thq/sfx/wod_ha1.wav", 1.0f, false},
    [SOUND_GIVE_TAKE1] = {"assets_thq/sfx/give_take1.wav", 1.0f, false},
    [SOUND_GIVE_TAKE2] = {"assets_thq/sfx/give_take2.wav", 1.0f, false},
    [SOUND_WOD_EH1] = {"assets_thq/sfx/wod_eh1.wav", 1.0f, false},
    [SOUND_NA_USE1A] = {"assets_thq/sfx/na_use1a.wav", 1.0f, false},
    [SOUND_NA_STEP1] = {"assets_thq/sfx/na_step1.wav", 1.0f, false},
    [SOUND_NA_STEP2] = {"assets_thq/sfx/na_step2.wav", 1.0f, false},
    [SOUND_NA_SITSDOWN1] = {"assets_thq/sfx/na_sitsdown1.wav", 1.0f, false},
    [SOUND_NA_REMOTE1] = {"assets_thq/sfx/na_remote1.wav", 1.0f, false},
    [SOUND_NA_GETSUP1] = {"assets_thq/sfx/na_getsup1.wav", 1.0f, false},
    [SOUND_NA_PEEP1] = {"assets_thq/sfx/na_peep1.wav", 1.0f, false},
    [SOUND_NA_PEEP2] = {"assets_thq/sfx/na_peep2.wav", 1.0f, false},
    [SOUND_NA_PEEP3] = {"assets_thq/sfx/na_peep3.wav", 1.0f, false},
    [SOUND_NA_PEEP4] = {"assets_thq/sfx/na_peep4.wav", 1.0f, false},
    [SOUND_OBJ_SQUIEEK1] = {"assets_thq/sfx/obj_squieek1.wav", 0.7f, false},
    [SOUND_NA_WORKOUT1] = {"assets_thq/sfx/na_workout1.wav", 1.0f, false},
    [SOUND_NA_WORKOUT2] = {"assets_thq/sfx/na_workout2.wav", 1.0f, false},
    [SOUND_NA_AAA_LONG2] = {"assets_thq/sfx/na_aaa_long2.wav", 1.0f, false},
    [SOUND_OBJ_PLOP1] = {"assets_thq/sfx/obj_plop1.wav", 1.0f, false},
    [SOUND_WOD_NONO] = {"assets_thq/sfx/wod_nono.wav", 1.0f, false},
    [SOUND_WOD_LAUGH2] = {"assets_thq/sfx/wod_laugh2.wav", 1.0f, true},
    [SOUND_WOD_FEAR1] = {"assets_thq/sfx/wod_fear1.wav", 1.0f, false},
    [SOUND_INSTALL1] = {"assets_thq/sfx/install1.wav", 1.0f, true},
    [SOUND_INSTALL2] = {"assets_thq/sfx/install2.wav", 1.0f, true},
    [SOUND_USE1] = {"assets_thq/sfx/use1.wav", 1.0f, true},
    [SOUND_APPLAUSE] = {"assets_thq/sfx/applause.wav", 1.0f, false},
    [SOUND_NA_SHOUT1] = {"assets_thq/sfx/na_shout1.wav", 1.0f, true},
    [SOUND_NA_HUH1] = {"assets_thq/sfx/na_huh1.wav", 1.0f, false},
    [SOUND_NA_SUP_HUH1] = {"assets_thq/sfx/na_sup_huh1.wav", 1.0f, false},
    [SOUND_NA_RUN1] = {"assets_thq/sfx/na_run1.wav", 1.0f, false},
    [SOUND_NA_RUN2] = {"assets_thq/sfx/na_run2.wav", 1.0f, false},
    [SOUND_NA_USE1] = {"assets_thq/sfx/na_use1.wav", 1.0f, false},
    [SOUND_NA_PEEP5] = {"assets_thq/sfx/na_peep5.wav", 0.9f, false},
    [SOUND_JINGLE_JOKE] = {"assets_thq/sfx/jingle_joke.wav", 1.0f, true},
    [SOUND_NA_AAA_LONG4] = {"assets_thq/sfx/na_aaa_long4.wav", 1.0f, true},
    [SOUND_NA_BACKBREAK1] = {"assets_thq/sfx/na_backbreak1.wav", 1.0f, false},
    [SOUND_NA_AAA_WHINE1] = {"assets_thq/sfx/na_aaa_whine1.wav", 1.0f, false},
    [SOUND_NA_WHEEZE3] = {"assets_thq/sfx/na_wheeze3.wav", 1.0f, false},
    [SOUND_NA_WHEEZE4] = {"assets_thq/sfx/na_wheeze4.wav", 1.0f, false},
    [SOUND_OBJ_MICRO_BEEP1] = {"assets_thq/sfx/obj_micro_beep1.wav", 1.0f, false},
    [SOUND_BIG1] = {"assets_thq/sfx/big1.wav", 1.0f, true},
    [SOUND_BIG2] = {"assets_thq/sfx/big2.wav", 1.0f, true},
    [SOUND_BIG3] = {"assets_thq/sfx/big3.wav", 1.0f, true},
    [SOUND_NA_GRRR1] = {"assets_thq/sfx/na_grrr1.wav", 1.0f, false},
    [SOUND_NA_SUP_HUH2] = {"assets_thq/sfx/na_sup_huh2.wav", 1.0f, false},
    [SOUND_NA_FART2] = {"assets_thq/sfx/na_fart2.wav", 1.0f, false},
    [SOUND_NA_HANDS1] = {"assets_thq/sfx/na_hands1.wav", 1.0f, false},
    [SOUND_JINGLE_CAUGHT] = {"assets_thq/sfx/jingle_caught.wav", 1.0f, true},
    [SOUND_KILL_WOOSH1] = {"assets_thq/sfx/kill_woosh1.wav", 1.0f, false},
    [SOUND_NA_KILL_BLAH1] = {"assets_thq/sfx/na_kill_blah1.wav", 1.0f, false},
    [SOUND_NA_KILL_BLAH2] = {"assets_thq/sfx/na_kill_blah2.wav", 1.0f, false},
    [SOUND_NA_KILLSHOUT2] = {"assets_thq/sfx/na_killshout2.wav", 1.0f, false},
    [SOUND_OBJ_HIT1] = {"assets_thq/sfx/obj_hit1.wav", 1.0f, false},
    [SOUND_WOD_AU1] = {"assets_thq/sfx/wod_au1.wav", 1.0f, false},
    [SOUND_WOD_AU2] = {"assets_thq/sfx/wod_au2.wav", 1.0f, false},
    [SOUND_OBJ_HIT2] = {"assets_thq/sfx/obj_hit2.wav", 1.0f, false},
    [SOUND_NA_KILL_HITS1] = {"assets_thq/sfx/na_kill_hits1.wav", 1.0f, false},
    [SOUND_NA_BEATS3] = {"assets_thq/sfx/na_beats3.wav", 0.85f, false},
    [SOUND_NA_KILL_JUMPS1] = {"assets_thq/sfx/na_kill_jumps1.wav", 1.0f, false},
    [SOUND_NA_KILL_BLAH3] = {"assets_thq/sfx/na_kill_blah3.wav", 1.0f, false},
    [SOUND_NA_BEATS1] = {"assets_thq/sfx/na_beats1.wav", 1.0f, false}
};

static unsigned short sound_channels_bindings[sizeof(sfx_bindings) / sizeof(sfx_bindings[0])] = {0};

typedef struct {
    int sound;
    int channel;
} SoundChannel;

static SoundChannel sound_channels[31];
static short sound_channels_order[31] = {
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
    -1,-1,-1,-1,-1,-1,-1,-1,-1
};

static unsigned short next_free_sound_channel = 17;
static short music_loaded = -1;
static short house_music_loaded = -1;

void NFHMusicPlay(int new_music, bool loop) {
    if (music_loaded != new_music) {
        int error_code = AalibLoad(music_bindings[new_music], MUSIC_CHANNEL, true);
        if (error_code != PSPAALIB_SUCCESS) {
            scene_error("Ошибка загрузки %s: %d", music_bindings[new_music], error_code);
        }

        music_loaded = new_music;
    }

    if (AalibGetStatus(MUSIC_CHANNEL) == PSPAALIB_STATUS_STOPPED) {   
        AalibSetAutoloop(MUSIC_CHANNEL, loop);
        AalibPlay(MUSIC_CHANNEL);
    }
}

void NFHMusicStop(void) {
    AalibStop(MUSIC_CHANNEL);
    AalibDisable(MUSIC_CHANNEL, PSPAALIB_EFFECT_VOLUME_MANUAL);
}

void NFHHouseMusicLoad(void) {
    int new_music = (rand() % 2) ? MUSIC_INGAME1_NORMAL : MUSIC_INGAME2_NORMAL;

    if (house_music_loaded != new_music) {
        int error_code = AalibLoad(music_bindings[new_music], HOUSE_MUSIC_CHANNEL, true);
        if (error_code != PSPAALIB_SUCCESS) {
            scene_error("Ошибка загрузки %s: %d", music_bindings[new_music], error_code);
        }

        house_music_loaded = new_music;
    }
}

void NFHHouseMusicPlay(void) {
    if (AalibGetStatus(HOUSE_MUSIC_CHANNEL) != PSPAALIB_STATUS_STOPPED) {
        AalibStop(HOUSE_MUSIC_CHANNEL);
    }
    
    AalibSetAutoloop(HOUSE_MUSIC_CHANNEL, true);
    AalibPlay(HOUSE_MUSIC_CHANNEL);
}

void NFHHouseMusicPause(void) {
    if (AalibGetStatus(HOUSE_MUSIC_CHANNEL) == PSPAALIB_STATUS_PLAYING) {
        AalibPause(HOUSE_MUSIC_CHANNEL);
    }
}

void NFHHouseMusicResume(void) {
    if (AalibGetStatus(HOUSE_MUSIC_CHANNEL) == PSPAALIB_STATUS_PAUSED) {
        AalibPause(HOUSE_MUSIC_CHANNEL);
    }
}

void NFHHouseMusicStop(void) {
    AalibStop(HOUSE_MUSIC_CHANNEL);
    AalibDisable(HOUSE_MUSIC_CHANNEL, PSPAALIB_EFFECT_VOLUME_MANUAL);
}

void NFHSoundLoad(int sound, int channel) {
    SoundChannel* current_sound_channel = &sound_channels[channel - 17];
    current_sound_channel->sound = sound;
    current_sound_channel->channel = channel;
    sound_channels_bindings[sound] = channel;

    const SfxBind current_sfx = sfx_bindings[sound];
    const AalibVolume volume = {current_sfx.volume, current_sfx.volume};

    int error_code = AalibLoad(current_sfx.path, channel, true);
    if (error_code != PSPAALIB_SUCCESS) {
        scene_error("Ошибка загрузки %s: %d", current_sfx.path, error_code);
    } else {
        AalibEnable(channel, PSPAALIB_EFFECT_VOLUME_MANUAL);
        AalibSetVolume(channel, volume);
    }
}

void NFHSoundPlay(int sound) {
    int current_sound_channel = sound_channels_bindings[sound];

    if (current_sound_channel != 0) {
        if (AalibGetStatus(current_sound_channel) != PSPAALIB_STATUS_STOPPED) {
            AalibStop(current_sound_channel);
        }
    } else {
        if (next_free_sound_channel < 48) {
            NFHSoundLoad(sound, next_free_sound_channel);
            current_sound_channel = next_free_sound_channel;
            next_free_sound_channel++;
        } else {
            short new_sound_channel = sound_channels_order[30];

            for (int i = 0; i < sizeof(sound_channels_bindings) / sizeof(sound_channels_bindings[0]); i++) {
                if (sound_channels_bindings[i] == new_sound_channel) {
                    sound_channels_bindings[i] = 0;
                    sound_channels_order[30] = -1;
                    break;
                }
            }

            AalibStop(new_sound_channel);
            AalibUnload(new_sound_channel);
            NFHSoundLoad(sound, new_sound_channel);

            current_sound_channel = new_sound_channel;
        }
    }

    if (current_sound_channel != 0) {
        AalibSetAutoloop(current_sound_channel, false);
        AalibPlay(current_sound_channel);

        int order_index = 11; // Да лучше просто тут захардкодить я всё равно систему звуков потом перепишу

        for (int i = order_index; i < 31; i++) {
            if (sound_channels_order[i] == -1) {
                for (int j = 30; j > order_index; j--) {
                    sound_channels_order[j] = sound_channels_order[j-1];
                }
                sound_channels_order[order_index] = current_sound_channel;

                break;
            } else if (sound_channels_order[i] == current_sound_channel) {
                for (int j = i; j > order_index; j--) {
                    sound_channels_order[j] = sound_channels_order[j-1];
                }
                sound_channels_order[order_index] = current_sound_channel;

                break;
            }
        }
    }
}

void NFHSoundPreload(int sound) {
    int current_sound_channel = sound_channels_bindings[sound];

    if (current_sound_channel == 0) {
        if (next_free_sound_channel < 48) {
            NFHSoundLoad(sound, next_free_sound_channel);
            current_sound_channel = next_free_sound_channel;
            next_free_sound_channel++;
        } else {
            short new_sound_channel = sound_channels_order[30];

            for (int i = 0; i < sizeof(sound_channels_bindings) / sizeof(sound_channels_bindings[0]); i++) {
                if (sound_channels_bindings[i] == new_sound_channel) {
                    sound_channels_bindings[i] = 0;
                    sound_channels_order[30] = -1;
                    break;
                }
            }

            AalibStop(new_sound_channel);
            AalibUnload(new_sound_channel);
            NFHSoundLoad(sound, new_sound_channel);

            current_sound_channel = new_sound_channel;
        }
    }

    for (int i = 0; i < 31; i++) {
        if (sound_channels_order[i] == -1) {
            for (int j = 30; j > 0; j--) {
                sound_channels_order[j] = sound_channels_order[j-1];
            }
            sound_channels_order[0] = current_sound_channel;

            break;
        } else if (sound_channels_order[i] == current_sound_channel) {
            for (int j = i; j > 0; j--) {
                sound_channels_order[j] = sound_channels_order[j-1];
            }
            sound_channels_order[0] = current_sound_channel;

            break;
        }
    }
}

void NFHSoundUnloadAll(void) {
    for (int channel = 17; channel < 48; channel++) {
        int sound_status = AalibGetStatus(channel);

        if (sound_status == PSPAALIB_STATUS_PLAYING || sound_status == PSPAALIB_STATUS_PAUSED || sound_status == PSPAALIB_STATUS_STOPPED) {
            if (sound_status == PSPAALIB_STATUS_STOPPED) {
                AalibStop(channel);
                AalibUnload(channel);
            }
        }
    }

    for (unsigned int i = 0; i < sizeof(sound_channels_bindings) / sizeof(sound_channels_bindings[0]); i++) {
        sound_channels_bindings[i] = 0;
    }

    for (int i = 0; i < 31; i++) {
        sound_channels[i].sound = -1;
        sound_channels[i].channel = 0;
    }

    next_free_sound_channel = 17;

    for (int i = 0; i < 31; i++) {
        sound_channels_order[i] = -1;
    }
}
