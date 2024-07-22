#pragma once
#include <string>
#include <vector>

class DBManager 
{
public:
    struct Player 
    {
        int id;
        std::wstring playerId;
        std::wstring playerPassword;
        std::wstring playerNickname;

        Player(
            int id, 
            std::wstring playerId, 
            std::wstring playerPassword, 
            std::wstring playerNickname
        ) :
            id(id), 
            playerId(playerId), 
            playerPassword(playerPassword), 
            playerNickname(playerNickname)
        { }
    };

    std::vector<Player> player_data;

    void add_player_record(const Player& record) 
    {
        player_data.push_back(record);
    }

    std::vector<Player> find_player_by_id(const int& value) 
    {
        std::vector<Player> results;
        for (const auto& record : player_data) 
        {
            if (record.id == value) 
            {
                results.push_back(record);
            }
        }
        return results;
    }

    void update_player_by_id(const int& key, const Player& new_record) 
    {
        for (auto& record : player_data) 
        {
            if (record.id == key) 
            {
                record = new_record;
                return;
            }
        }
    }

    void delete_player_by_id(const int& key) 
    {
        auto it = std::remove_if(player_data.begin(), player_data.end(),
            [&key](const Player& record) 
            {
                return record.id == key;
            });
        player_data.erase(it, player_data.end());
    }
    struct Friends 
    {
        std::wstring UserId;
        std::wstring FriendId;

        Friends(
            std::wstring UserId, 
            std::wstring FriendId
        ) :
            UserId(UserId), 
            FriendId(FriendId)
        { }
    };

    std::vector<Friends> friends_data;

    void add_friends_record(const Friends& record) 
    {
        friends_data.push_back(record);
    }

    std::vector<Friends> find_friends_by_UserId(const std::wstring& value) 
    {
        std::vector<Friends> results;
        for (const auto& record : friends_data) 
        {
            if (record.UserId == value) 
            {
                results.push_back(record);
            }
        }
        return results;
    }

    void update_friends_by_UserId(const std::wstring& key, const Friends& new_record) 
    {
        for (auto& record : friends_data) 
        {
            if (record.UserId == key) 
            {
                record = new_record;
                return;
            }
        }
    }

    void delete_friends_by_UserId(const std::wstring& key) 
    {
        auto it = std::remove_if(friends_data.begin(), friends_data.end(),
            [&key](const Friends& record) 
            {
                return record.UserId == key;
            });
        friends_data.erase(it, friends_data.end());
    }

    void Initialize();
};

extern DBManagerRef GDBManager;