#pragma once

class GPIO
{
    public:
        enum class Direction
        {
            input,
            output
        };

        enum class PinState
        {
            low = 0,
            high = 1
        };

        enum class PinMode
        {
            input,
            inputPullUp,
            output
        };

        enum class PullMode
        {
            pull,
            noPull,
            // pullDown
        };

        virtual ~GPIO(){};
        
        virtual void init() = 0;
        virtual void setDirection(Direction direction) = 0;
        virtual void setPinState(PinState state) = 0;
        virtual void setPinMode(PinMode state) = 0;
        virtual void setPullMode(PullMode state) = 0;
        virtual bool read() const = 0;

};