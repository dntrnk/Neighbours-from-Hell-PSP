#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

typedef struct {
    void (*init)(void);
    void (*update)(void);
    void (*draw)(void);
    void (*unload)(void);
} Scene;

// костыль который надо будет убрать (мне прост как-то надо в меню паузы узнать, какой уровень запущен)
int is_this_scene(const Scene* this_scene);

void scene_push(Scene* new_scene);
void scene_pop(void);
void scene_change(Scene* new_scene);
void scene_restart(void);
void scene_handle_requests(void);
void scene_update(void);
void scene_draw(void);
void scene_error(const char* format, ...);

#endif // SCENE_MANAGER_H