#pragma once
#include<vector>
#include<ctime>
#include<set>

#include"json.hpp"
using namespace std;

using json = nlohmann::json;

class scheduler_algorithm
/*
Scheduler for lessons written in c++;
Author: Simon Wekesa
*/
{
private:
    vector<json> lessons;
    json preferences;
    vector<json> generated_schedule;
public:
    scheduler_algorithm(vector<json> lessons,json preferences);
    ~scheduler_algorithm() {}

    vector<json> generate_schedule();
    vector<json> prioritize_lessons(vector<json> lessons);
    vector<json> generate_time_slots();
    vector<json> generate_day_slots(time_t current_date,json daily_hours);
    void assign_lessons_to_slots(vector<json> lessons,vector<json> time_slots);
    json find_optimal_slot(json lesson,vector<json> time_slots,set<json>& used_slots);
    double score_time_slot(json slot,json lesson);

};