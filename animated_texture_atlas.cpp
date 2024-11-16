#include "animated_texture_atlas.hpp"
#include <stb_image.h>
#include <fstream>
#include <iostream>
#include <glad/glad.h>

using json = nlohmann::json;

AnimatedTextureAtlas::AnimatedTextureAtlas(const std::string &spritesheet_json_path,
                                           const std::string &spritesheet_path, double ms_per_sprite)
    : texture_atlas{spritesheet_json_path, spritesheet_path, true, true}, ms_per_sprite{ms_per_sprite},
      ms_prev_time{0.0}, ms_accumulated_time{0.0}, curr_sprite{0} {
    std::ifstream file(spritesheet_json_path);
    json spritesheet_json;
    file >> spritesheet_json;

    auto sprites = spritesheet_json["sub_textures"];
    total_sprites = sprites.size();
}

std::vector<glm::vec2> AnimatedTextureAtlas::get_texture_coordinates_of_sprite(double ms_curr_time) {
    if (ms_prev_time == 0.0) {
        // animation hasn't run yet
        ms_prev_time = ms_curr_time;
    }

    ms_accumulated_time += ms_curr_time - ms_prev_time;
    ms_prev_time = ms_curr_time;

    while (ms_accumulated_time >= ms_per_sprite) {
        curr_sprite = (curr_sprite + 1) % total_sprites;
        ms_accumulated_time -= ms_per_sprite;
    }

    return texture_atlas.get_texture_coordinates_of_sub_texture(std::to_string(curr_sprite));
}
