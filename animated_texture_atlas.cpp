#include "animated_texture_atlas.hpp"
#include <stb_image.h>
#include <fstream>
#include <iostream>
#include <glad/glad.h>

using json = nlohmann::json;

AnimatedTextureAtlas::AnimatedTextureAtlas(const std::string &json_path, const std::string &animated_texture_atlas_path,
                                           double frame_rate_hz, bool looping,
                                           std::optional<TexturePacker> texture_packer)
    : texture_atlas{json_path, animated_texture_atlas_path}, looping(looping),
      animated_texture_atlas_path(animated_texture_atlas_path), texture_packer(texture_packer),
      ms_per_animation_frame{1000.0 / frame_rate_hz}, ms_prev_time{0.0}, ms_accumulated_time{0.0},
      curr_animation_frame{0} {

    if (this->texture_packer.has_value()) {
        /*auto sprites = texture_packer.*/
        total_animation_frames =
            texture_packer->get_packed_texture_sub_texture(animated_texture_atlas_path).sub_atlas.size();
    } else {
        std::ifstream file(json_path);
        json spritesheet_json;
        file >> spritesheet_json;
        auto sprites = spritesheet_json["sub_textures"];
        total_animation_frames = sprites.size();
    }
}

void AnimatedTextureAtlas::reset_processed_state() {
    ms_prev_time = 0;
    ms_accumulated_time = 0;
    curr_animation_frame = 0;
}

std::vector<glm::vec2> AnimatedTextureAtlas::get_texture_coordinates_of_current_animation_frame(double ms_curr_time) {
    if (ms_prev_time == 0.0) {
        // animation hasn't run yet
        ms_prev_time = ms_curr_time;
    }

    ms_accumulated_time += ms_curr_time - ms_prev_time;
    ms_prev_time = ms_curr_time;

    while (ms_accumulated_time >= ms_per_animation_frame) {
        curr_animation_frame = curr_animation_frame + 1;
        if (looping) {
            curr_animation_frame %= total_animation_frames;
        } else {
            // will be locked on the last frame

            curr_animation_frame = std::min(total_animation_frames - 1, curr_animation_frame);

            if (curr_animation_frame == total_animation_frames - 1) { // on the last frame
                animation_is_complete = true;
            }
        }
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
