#pragma once

class light_sensor final
{
public:
    light_sensor() 
    {

    } // 默认构造函数

    ~light_sensor() 
    {
        
    } // 默认析构函数

    float get_light_intensity()
    {
        // 模拟获取光照强度的操作，这里直接返回一个固定值
        return 1000; // 假设光照强度为1000流明
    }
};
