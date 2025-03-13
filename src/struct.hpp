#include <fstream>
#include <vector>
#include <nlohmann/json.hpp>
#include "constants.hpp"

struct Settings
{
public:
    int width, height;
    bool fps_hud;
    int sfx_volume, music_volume;
    static Settings default_settings()
    {
        return Settings{DEFAULT_WIDTH, DEFAULT_HEIGHT, false, 100, 100};
    }
    static Settings from_json(const nlohmann::json &j)
    {
        return Settings{j.at("width").get<int>(), j.at("height").get<int>(), j.at("fps_hud").get<bool>(), j.at("sfx_volume").get<int>(), j.at("music_volume").get<int>()};
    }
    static Settings load()
    {
        std::ifstream file("settings.json");
        if (!file.is_open())
        {
            return default_settings();
        }
        nlohmann::json j;
        file >> j;
        return from_json(j);
    }
    nlohmann::json to_json() const
    {
        return nlohmann::json{{"width", width}, {"height", height}, {"fps_hud", fps_hud}, {"sfx_volume", sfx_volume}, {"music_volume", music_volume}};
    }
    void save() const
    {
        std::ofstream file("settings.json");
        file << to_json().dump(4);
    }
};

// Save and required functions
void to_json(nlohmann::json &j, const Save &save)
{
    j = nlohmann::json{
        {"score", save.score},
        {"grid", save.grid}};
}
void from_json(const nlohmann::json &j, Save &save)
{
    j.at("score").get_to(save.score);
    j.at("grid").get_to(save.grid);
}
struct Save
{
    int score;
    std::vector<std::vector<int>> grid;
};

struct UserData
{
public:
    int best_score;
    std::vector<Save> saves;
    static UserData default_user_data()
    {
        auto empty = std::vector<Save>();
        return UserData{
            best_score : 0,
            saves : empty
        };
    }
    static UserData from_json(const nlohmann::json &j)
    {
        UserData data;
        j.at("best_score").get_to(data.best_score);
        j.at("saves").get_to(data.saves);
        return data;
    }
    static UserData load()
    {
        std::ifstream file("user_data.json");
        if (!file.is_open())
        {
            return default_user_data();
        }
        nlohmann::json j;
        file >> j;
        return from_json(j);
    }
    nlohmann::json to_json() const
    {
        return nlohmann::json{{"best_score", best_score}, {"saves", saves}};
    }
    void save() const
    {
        std::ofstream file("user_data.json");
        file << to_json().dump(4);
    }
};
