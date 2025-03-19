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
    bool use_arrow_keys;
    static Settings default_settings()
    {
        Settings settings;
        settings.width = DEFAULT_WIDTH;
        settings.height = DEFAULT_HEIGHT;
        settings.fps_hud = false;
        settings.sfx_volume = 100;
        settings.music_volume = 70;
        settings.use_arrow_keys = true;
        return settings;
        // return Settings{DEFAULT_WIDTH, DEFAULT_HEIGHT, false, 100, 70, true};
    }
    static Settings from_json(const nlohmann::json &j)
    {
        Settings settings;
        j.at("width").get_to(settings.width);
        j.at("height").get_to(settings.height);
        j.at("fps_hud").get_to(settings.fps_hud);
        j.at("sfx_volume").get_to(settings.sfx_volume);
        j.at("music_volume").get_to(settings.music_volume);
        j.at("use_arrow_keys").get_to(settings.use_arrow_keys);
        return settings;
        // return Settings{j.at("width").get<int>(), j.at("height").get<int>(), j.at("fps_hud").get<bool>(), j.at("sfx_volume").get<int>(), j.at("music_volume").get<int>(), j.at("use_arrow_keys").get<bool>()};
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
        return nlohmann::json{{"width", width}, {"height", height}, {"fps_hud", fps_hud}, {"sfx_volume", sfx_volume}, {"music_volume", music_volume}, {"use_arrow_keys", use_arrow_keys}};
    }
    void save() const
    {
        std::ofstream file("settings.json");
        file << to_json().dump(4);
    }
};

// Save and required functions
struct Save
{
    long long date;
    int score;
    std::vector<std::vector<int>> grid;
};

void to_json(nlohmann::json &j, const Save &save)
{
    j = nlohmann::json{
        {"date", save.date},
        {"score", save.score},
        {"grid", save.grid}};
}
void from_json(const nlohmann::json &j, Save &save)
{
    j.at("score").get_to(save.score);
    j.at("grid").get_to(save.grid);
    j.at("date").get_to(save.date);
}

struct UserData
{
public:
    int best_score;
    std::vector<Save> saves;
    static UserData default_user_data()
    {
        auto empty = std::vector<Save>();
        UserData user_data;
        user_data.best_score = 0;
        user_data.saves = empty;
        return user_data;
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
