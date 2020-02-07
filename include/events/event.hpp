#ifndef EVENT_H
#define EVENT_H

namespace nitro
{
    namespace events
    {
        struct Event
        {
            virtual ~Event() = default;
        };
    }
}

#endif