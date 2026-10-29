#ifndef TIME_SLOT_HPP
#define TIME_SLOT_HPP

#include <ctime>
#include <string>
#include <iomanip>
#include <sstream>

class TimeSlot {
private:
    time_t startTime;
    time_t endTime;

public:
    TimeSlot(time_t startTime, time_t endTime)
        : startTime(startTime), endTime(endTime) {}

    TimeSlot(time_t startTime, int durationMinutes)
        : startTime(startTime), endTime(startTime + durationMinutes * 60) {}

    // Getters
    time_t getStartTime() const { return startTime; }
    time_t getEndTime() const { return endTime; }
    
    int getDurationMinutes() const {
        return static_cast<int>(difftime(endTime, startTime) / 60);
    }

    bool overlaps(const TimeSlot& other) const {
        return !(endTime <= other.startTime || startTime >= other.endTime);
    }

    bool isBefore(const TimeSlot& other) const {
        return startTime < other.startTime;
    }

    std::string toString() const {
        std::stringstream ss;
        struct tm* start_tm = localtime(&startTime);
        ss << std::setfill('0') << std::setw(2) << start_tm->tm_hour << ":"
           << std::setfill('0') << std::setw(2) << start_tm->tm_min;
        ss << " - ";
        struct tm* end_tm = localtime(&endTime);
        ss << std::setfill('0') << std::setw(2) << end_tm->tm_hour << ":"
           << std::setfill('0') << std::setw(2) << end_tm->tm_min;
        return ss.str();
    }

    std::string getDateString() const {
        struct tm* tm_info = localtime(&startTime);
        std::stringstream ss;
        ss << (tm_info->tm_year + 1900) << "-"
           << std::setfill('0') << std::setw(2) << (tm_info->tm_mon + 1) << "-"
           << std::setfill('0') << std::setw(2) << tm_info->tm_mday;
        return ss.str();
    }
};

#endif // TIME_SLOT_HPP
