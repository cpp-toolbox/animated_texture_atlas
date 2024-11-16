#ifndef ANIMATED_TEXTURE_ATLAS_HPP
#define ANIMATED_TEXTURE_ATLAS_HPP

#include <string>
#include <map>
#include <vector>
#include <glm/glm.hpp>
#include <nlohmann/json.hpp>

#include "sbpt_generated_includes.hpp"

class AnimatedTextureAtlas {
  public:
    AnimatedTextureAtlas(const std::string &json_path, const std::string &spritesheet_path, double ms_per_sprite);

    std::vector<glm::vec2> get_texture_coordinates_of_sprite(double ms_curr_time);

  private:
    TextureAtlas texture_atlas;
    double ms_per_sprite;
    double ms_prev_time;
    double ms_accumulated_time;
    unsigned int curr_sprite;
    unsigned int total_sprites;
};

#endif // ANIMATED_TEXTURE_ATLAS_HPP
