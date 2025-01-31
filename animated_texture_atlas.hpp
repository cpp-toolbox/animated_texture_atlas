#ifndef ANIMATED_TEXTURE_ATLAS_HPP
#define ANIMATED_TEXTURE_ATLAS_HPP

#include <string>
#include <map>
#include <vector>
#include <glm/glm.hpp>
#include <nlohmann/json.hpp>
#include <optional>

#include "sbpt_generated_includes.hpp"

class AnimatedTextureAtlas {
  public:
    AnimatedTextureAtlas(const std::string &json_path, const std::string &animated_texture_atlas_path,
                         double ms_per_animation_frame, bool looping = true,
                         std::optional<TexturePacker> texture_packer = std::nullopt);

    std::vector<glm::vec2> get_texture_coordinates_of_current_animation_frame(double ms_curr_time);
    void reset_processed_state();

  private:
    TextureAtlas texture_atlas;
    // note that if you have the texture_packer, then we don't use the texture_atlas, it has a superset of the atlases
    // functionality
    std::optional<TexturePacker> texture_packer;
    std::string animated_texture_atlas_path;
    double ms_per_animation_frame;
    double ms_prev_time;
    double ms_accumulated_time;
    unsigned int curr_animation_frame;
    unsigned int total_animation_frames;
    bool looping;
};

#endif // ANIMATED_TEXTURE_ATLAS_HPP
