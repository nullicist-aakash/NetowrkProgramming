#pragma once

#include <pthread.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <chrono>
#include <string>
#include <unordered_map>
#include <queue>

using namespace std;


#define MESSAGE_TIME_LIMIT 60
#define BULK_LIMIT 10
using short_time = std::chrono::_V2::system_clock::time_point;

class Database
{
private:
    using pcs = pair<short_time, string>;

    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    unordered_map<string, priority_queue<pcs, vector<pcs>, greater<pcs>>> mp;

    inline void lock()
    {
        pthread_mutex_lock(&mutex);
    }

    inline void unlock()
    {
        pthread_mutex_unlock(&mutex);
    }

    void removeOldMessages(const string &topic);

    Database() {}

public:
    Database(Database const&)        = delete;
    void operator=(Database const&)  = delete;

    static Database& getInstance();

    int addTopic(const char* in_topic);

    bool topicExists(const char* in_topic) const;

    int addMessage(const char* in_topic, const char* in_message, short_time&);

    int addMessages(const char* in_topic, const vector<string> &msgs, short_time&);

    string getNextMessage(const char* in_topic, short_time &clk);

    const vector<pair<short_time, string>> getBulkMessages(const char* in_topic, short_time &clk);

    const vector<string> getAllTopics() const;
};