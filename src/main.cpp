/*
__AUTHOR__: Simon Wekesa
Smart Scheduler Main File
*/

#include<iostream>
#include<fstream>
#include<vector>
#include<ctime>
#include<sstream>

#include"json.hpp"
#include"scheduler_algo.hpp"

using namespace std;
using json = nlohmann::json;

// Helper function to convert date string (YYYY-MM-DD) to time_t
time_t parse_date(const string& date_str) {
    struct tm tm_time = {};
    sscanf_s(date_str.c_str(), "%d-%d-%d", &tm_time.tm_year, &tm_time.tm_mon, &tm_time.tm_mday);
    tm_time.tm_year -= 1900;  // years since 1900
    tm_time.tm_mon -= 1;      // months are 0-11
    tm_time.tm_hour = 0;
    tm_time.tm_min = 0;
    tm_time.tm_sec = 0;
    return mktime(&tm_time);
}

int main(){
    cout << "Smart Scheduler C++\n";

    cout << "Loading lessons and preferences...\n";
    ifstream lessons_file("../data/mock_data.json");

    if (!lessons_file.is_open())
    {
        cerr << "Failed to open lessons file." << endl;
        return 1;
    }
    
    json data;
    lessons_file >> data;
    lessons_file.close();
    vector<json> lessons = data["lessons"].get<vector<json>>();

    json preferences;
    preferences["start_date"] = parse_date(data["startDate"].get<string>());
    preferences["end_date"] = parse_date(data["endDate"].get<string>());
    preferences["daily_hours"] = data["dailyHours"];
    preferences["working_days"] = data["workingDays"];
    preferences["break_between_lessons"] = data["breakBetweenLessons"];
    preferences["room"] = data["room"];

    cout << "Initializing scheduler...\n";

    // intialize scheduler
    scheduler_algorithm scheduler(lessons,preferences);
    cout << "Generating schedule...\n";
    vector<json> schedule = scheduler.generate_schedule();

    cout << "Generated Schedule saved:\n";

    json schedule_json = schedule;
    
    ofstream schedule_file("../data/generated_schedule.json");
    if (!schedule_file.is_open()){
        cerr << "Failed to open schedule file for writing." << endl;
        return 1;

    }

    schedule_file << schedule_json.dump(4) << endl;
    schedule_file.close();  

    cout << "Schedule generation complete. Check 'data/generated_schedule.json' for the output.\n";
    

    return 0;
}