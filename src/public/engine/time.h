#ifndef TIME_H
#define TIME_H

namespace TIME
{
    float delta_time = 0.0f; // 当前帧与上一帧的时间差
    float current_time = 0.0f; // 上一帧的时间

    void updateTime(float _current_time)
    {
        delta_time = _current_time - current_time;
        current_time = _current_time;
    }
} // namespace TIME


#endif