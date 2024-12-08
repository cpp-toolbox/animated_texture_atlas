#include "animated_texture_atlas.hpp"
#include <stb_image.h>
#include <fstream>
#include <iostream>
#include <glad/glad.h>

using json = nlohmann::json;

AnimatedTextureAtlas::AnimatedTextureAtlas(const std::string &json_path, const std::string &animated_texture_atlas_path,
                                           double ms_per_animation_frame, std::optional<TexturePacker> texture_packer)
    : texture_atlas{json_path, animated_texture_atlas_path, true, true},
      animated_texture_atlas_path(animated_texture_atlas_path), texture_packer(texture_packer),
      ms_per_animation_frame{ms_per_animation_frame}, ms_prev_time{0.0}, ms_accumulated_time{0.0},
      curr_animation_frame{0} {

    if (this->texture_packer.has_value()) {
        /*auto sprites = texture_packer.*/
        total_animation_frames = texture_packer->get_atlas_size_of_sub_texture(animated_texture_atlas_path);
    } else {
        std::ifstream file(json_path);
        json spritesheet_json;
        file >> spritesheet_json;
        auto sprites = spritesheet_json["sub_textures"];
        total_animation_frames = sprites.size();
    }
}

std::vector<glm::vec2> AnimatedTextureAtlas::get_texture_coordinates_of_current_animation_frame(double ms_curr_time) {
    if (ms_prev_time == 0.0) {
        // animation hasn't run yet
        ms_prev_time = ms_curr_time;
    }

    ms_accumulated_time += ms_curr_time - ms_prev_time;
    ms_prev_time = ms_curr_time;

    while (ms_accumulated_time >= ms_per_animation_frame) {
        curr_animation_frame = (curr_animation_frame + 1) % total_animation_frames;
        ms_accumulated_time -= ms_per_animation_frame;
    }

    std::string current_animation_frame = std::to_string(curr_animation_frame);

    std::vector<glm::vec2> texture_coords;
    if (this->texture_packer.has_value()) {
        auto val =
            texture_packer->get_packed_texture_sub_texture_atlas(animated_texture_atlas_path, current_animation_frame);
        texture_coords = val.texture_coordinates;
    } else {
        texture_coords = texture_atlas.get_texture_coordinates_of_sub_texture(current_animation_frame);
    }

    return texture_coords;
}
