#ifndef _COMMUNICATE_ACTION_STUB_H
#define _COMMUNICATE_ACTION_STUB_H
#include <atomic>

class action_stub
{
private:
    std::atomic_ullong last_snd_timestamp;
    std::atomic_ullong last_rcv_timestamp;
    std::atomic_ullong snd_num;
    std::atomic_ullong rcv_num;
public:
    virtual ~action_stub() = default;
    action_stub();

    action_stub(const action_stub &other);

    void mark_sent();

    void mark_received();

    const unsigned long get_time_frame_since_last_snd() const;

    const unsigned long get_time_frame_since_last_rcv() const;

    const unsigned long get_snd_num() const;

    const unsigned long get_rcv_num() const;

    void reset();
};



#endif //TEST_COMMUNICATE_ACTION_STUB_H
