#pragma once
#include <string>
#include <vector>

class DBConnection;

class DBManager 
{
public:
    {%- for table in tables %}
    struct {{ table.name }} 
    {
        {%- for column in table.columns %}
        {{ column.type }} {{ column.name }};
        {%- endfor %}

        {{ table.name }}(
            {%- for column in table.columns %}
            {{ column.type }} {{ column.name }}{%- if not loop.last %}, {% endif %}
            {%- endfor %}
        ) :
            {%- for column in table.columns %}
            {{ column.name }}({{ column.name }}){%- if not loop.last %}, {% endif %}
            {%- endfor %}
        { }
    };

    std::vector<{{ table.name }}> {{ table.name.lower() }}_data;

    void add_{{ table.name.lower() }}_record(const {{ table.name }}& record) 
    {
        {{ table.name.lower() }}_data.push_back(record);
    }

    std::vector<{{ table.name }}> find_{{ table.name.lower() }}_by_{{ table.columns[0].name }}(const {{ table.columns[0].type }}& value) 
    {
        std::vector<{{ table.name }}> results;
        for (const auto& record : {{ table.name.lower() }}_data) 
        {
            if (record.{{ table.columns[0].name }} == value) 
            {
                results.push_back(record);
            }
        }
        return results;
    }

    void update_{{ table.name.lower() }}_by_{{ table.columns[0].name }}(const {{ table.columns[0].type }}& key, const {{ table.name }}& new_record) 
    {
        for (auto& record : {{ table.name.lower() }}_data) 
        {
            if (record.{{ table.columns[0].name }} == key) 
            {
                record = new_record;
                return;
            }
        }
    }

    void delete_{{ table.name.lower() }}_by_{{ table.columns[0].name }}(const {{ table.columns[0].type }}& key) 
    {
        auto it = std::remove_if({{ table.name.lower() }}_data.begin(), {{ table.name.lower() }}_data.end(),
            [&key](const {{ table.name }}& record) 
            {
                return record.{{ table.columns[0].name }} == key;
            });
        {{ table.name.lower() }}_data.erase(it, {{ table.name.lower() }}_data.end());
    }

    void Initialize{{ table.name.lower() }}();

    std::vector<{{ table.name }}> get_all_{{ table.name.lower() }}() 
    {
        return {{ table.name.lower() }}_data;
    }

    {%- endfor %}

    void Initialize();
    DBConnection* GetDBConn() { return m_DbConn;  }

private:
    DBConnection* m_DbConn = nullptr;
};

extern DBManagerRef GDBManager;