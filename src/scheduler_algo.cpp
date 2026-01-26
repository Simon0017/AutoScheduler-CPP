#include<vector>
#include<ctime>
#include<set>
#include<algorithm>
#include<string>
#include<chrono>
#include<iostream>
#include<random>

#include"scheduler_algo.hpp"
#include"json.hpp"

using namespace std;
using namespace std::chrono;


using json = nlohmann::json;

string format_time(time_t time);

scheduler_algorithm::scheduler_algorithm(vector<json> lessons,json preferences){
    this->lessons = lessons;
    this->preferences = preferences;
}

vector<json> scheduler_algorithm::generate_schedule(){
    this->generated_schedule.clear();

    vector<json> sorted_lessons = prioritize_lessons(this->lessons);

    vector<json> time_slots = generate_time_slots();

    assign_lessons_to_slots(sorted_lessons,time_slots);

    return this->generated_schedule;
}

vector<json> scheduler_algorithm::prioritize_lessons(vector<json> lessons){
    // Map priority strings to numeric values
    auto priority_value = [](const string& p) { 
        if (p == "high") return 3;
        if (p == "medium") return 2;
        return 1;
    };

    sort(lessons.begin(),lessons.end(),
        [&priority_value](const json& a,const json& b){
            return priority_value(a["priority"].get<string>()) > priority_value(b["priority"].get<string>());
        });

    
    sort(lessons.begin(),lessons.end(),
        [](const json& a,const json& b){
            return a["sessionDuration"].get<int>() > b["sessionDuration"].get<int>();
        });

    return lessons;
}

vector<json> scheduler_algorithm::generate_time_slots(){
    vector<json> slots;
    time_t start_date = this->preferences["start_date"].get<time_t>();
    time_t end_date = this->preferences["end_date"].get<time_t>();
    json daily_hours = this->preferences["daily_hours"].get<json>();
    vector<int> working_days = this->preferences["working_days"].get<vector<int>>();

    time_t current_date = start_date;

    while (current_date <= end_date)
    {       
        tm local{};
        localtime_s(&local,&current_date);
        int weekday = local.tm_wday;

        bool weekday_exists = find(working_days.begin(),working_days.end(),weekday) != working_days.end();

        if (weekday_exists)
        {
            vector<json> day_slots = this->generate_day_slots(current_date,daily_hours);
            slots.insert(slots.begin(),day_slots.begin(),day_slots.end());
        }
        
        auto tp = system_clock::from_time_t(current_date);
        
        typedef duration<int,ratio<86400>> days;

        // increment the time by one day
        tp += days(1);

        current_date = system_clock::to_time_t(tp);
    }
    
    return slots;
}

vector<json> scheduler_algorithm::generate_day_slots(time_t current_date,json daily_hours){
    vector<json> slots;
    int start_hour = daily_hours["start"].get<int>();
    int end_hour = daily_hours["end"].get<int>();

    for (int hour = start_hour; hour < end_hour; hour++)
    {
        for (int minute = 0; minute < 60; minute += 30)
        {
            tm local{};
            localtime_s(&local,&current_date);
            local.tm_hour = hour;
            local.tm_min = minute;
            local.tm_sec = 0;

            time_t slot_time = mktime(&local);

            json slot;
            slot["start"] = slot_time;
            slot["end"] = slot_time + 30 * 60; // 30 minutes later
            slot["available"] = true;

            slots.push_back(slot);
        }
    }

    return slots;
}

void scheduler_algorithm::assign_lessons_to_slots(vector<json> lessons,vector<json> time_slots){
    set<json> used_slots;

    for (const auto& lesson:lessons)
    {
        int sessions_needed = (int)(lesson["totalHours"].get<double>() * 60 / lesson["sessionDuration"].get<int>());
        int sessions_assigned = 0;

        for (size_t i = 0; i < sessions_needed; i++)
        {
            json optimal_slot = find_optimal_slot(lesson,time_slots,used_slots);

            if (optimal_slot["available"].get<bool>())
            {
                this->generated_schedule.push_back({
                    {"ID",lesson["id"]},
                    {"Title",lesson["name"]},
                    {"Start",format_time(optimal_slot["start"].get<time_t>())},
                    {"End",format_time(optimal_slot["end"].get<time_t>())},
                    {"Priority",optimal_slot["priority"]},
                    {"Description",lesson["description"]}
                });

                used_slots.insert(optimal_slot);

                // Mark the slot as used
                for (auto& slot:time_slots)
                {
                    if (slot["start"] == optimal_slot["start"])
                    {
                        slot["available"] = false;
                        break;
                    }
                }

                sessions_assigned++;
            }
            
        }
        
    }
    

};

json scheduler_algorithm::find_optimal_slot(json lesson,vector<json> time_slots,set<json>& used_slots){
    int best_score = -1;
    json best_slot;

    for (const json& slot:time_slots){
        if (used_slots.find(slot) != used_slots.end())
        {
            continue; // Slot already used

        }

        double score = score_time_slot(slot,lesson);

        if (score > best_score)
        {
            best_score = score;
            best_slot = slot;
        }
    }

    return best_slot;
    
};

double scheduler_algorithm::score_time_slot(json slot,json lesson){
    int score = 0;
    time_t slot_time = slot["start"].get<time_t>();
    
    tm local{};
    localtime_s(&local,&slot_time);
    int hour = local.tm_hour;
    int weekday  = local.tm_wday;

    string priority = lesson["priority"].get<string>();
    
    if (priority == "high" and hour > 8 and hour < 11)
    {
        score +=10;
    }

    if (priority == "medium" and hour > 12 and hour < 16)
    {
        score += 8;
    }

    if (priority == "low" and (hour < 8 or hour > 17))
    {
        score -=5;
    }

    if (weekday > -1 and weekday <= 5)
    {
        score += 5;
    }
    
    // introduce randomness to reduce clustering
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(-2,2);
    score += dis(gen);

    return score;
};


string format_time(time_t time){
    char buffer[64];
    tm local{};
    localtime_s(&local,&time);
    strftime(buffer,sizeof(buffer),"%Y-%m-%d %A %I:%M %p",&local);
    return string(buffer);
}