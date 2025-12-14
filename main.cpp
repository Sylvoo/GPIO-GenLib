#include <iostream>
#include <cstdint>
#include <type_traits>
#include <stdexcept>
#include <concepts>  

template <typename T>
concept McuType = std::same_as<std::remove_cv_t<T>, uint8_t> || std::same_as<std::remove_cv_t<T>, uint32_t>;
// std::remove_cv_t jest po to aby akceptowalo volatile, const uint8_t, uint32_t i czyscilo



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


template<McuType T>
class GPIO_port
{
    protected:
        using reg_t = std::remove_cv_t<T>;

        volatile reg_t& DDRx;
        volatile reg_t& PORTx;
        volatile reg_t& PINx;

        static constexpr reg_t bitMask(reg_t bit) // alternatywa w klasach dla #define
        {
            return (reg_t)(1 << bit);
        }
    public:
        bool validateBit(reg_t bit) const 
        {
            if constexpr(std::is_same_v<reg_t, uint8_t>)
            {
                if(bit > 7 || bit < 0) 
                {
                    throw std::out_of_range("Pin out of range!");
                    return false;
                }
                else
                {
                    return true;
                } 
            }
            else if constexpr (std::is_same_v<T, uint32_t>)
            {
                if(bit > 31 || bit < 0) 
                {
                    throw std::out_of_range("Pin out of range!");
                    return false;
                }
                else
                {
                    return true;
                } 
            }
        }

    GPIO_port(volatile reg_t& ddr, volatile reg_t& port, volatile reg_t& pin) : DDRx(ddr), PORTx(port), PINx(pin) {}; 
    
    void setDirection(reg_t bit, bool is_output)
    {
        if(validateBit(bit))
        {
            if(is_output)
            {
                DDRx |= (reg_t)bitMask(bit);
            }
            else
            {
                DDRx &= (reg_t)(~bitMask(bit));
            }
        }
        else 
        {
            throw std:: invalid_argument("Cannot set Direction to a %x Bit", bit);
        }
    }

    void setBit(reg_t bit, bool to_high)
    {
        if(validateBit(bit))
        {
            if(to_high)
            {
                PORTx |= (reg_t)bitMask(bit);
            }
            else
            {
                PORTx &= (reg_t)~(bitMask(bit));
            }
        }
        else 
        {
            throw std:: invalid_argument("Cannot set %x Bit", bit);
        }
    }

    bool readBit(const reg_t bit) const
    {
        if(validateBit(bit))
        {
            return (PINx & (reg_t)bitMask(bit));
        }
        else 
        {
            throw std:: invalid_argument("Cannot read %x Bit", bit);
        }
    }

    void pullUpBit(reg_t bit, bool is_pullUp)
    {
        if(validateBit(bit))
        {
            if(is_pullUp)
            {
                DDRx &= (reg_t)~(bitMask(bit));
                PORTx |= (reg_t)bitMask(bit);
            }   
            else
            {
                PORTx &= (reg_t)~(bitMask(bit));
            }
        }
        else 
        {
            throw std:: invalid_argument("Cannot do PullUp operation on %x Bit", bit);
        }

    }
};
template <McuType T>
class GPIO_pin : public GPIO
{
    using reg_t = std::remove_cv_t<T>;
    GPIO_port<reg_t>& port;
    const reg_t bit;

    public:
    GPIO_pin(GPIO_port<reg_t>& portx, const reg_t pbit) : port(portx), bit(pbit)
    {
        try
        {
            if(!port.validateBit(pbit)){
                throw std::invalid_argument("Pin number out of range");
            }
        }
        catch(std::invalid_argument err)
        {
            printf("Error: %s", err.what()); // ktore najlepsze z profesjonalnego punktu widzenia ? 
            // std::cerr << err.what() << '\n'; 
            // std::cout<< err.what()<<"\n";
        }
    }; 

    void setDirection(Direction direction) override
    {
       port.setDirection(bit, direction == Direction::output); // true jesli output false jesli input
    }

    void setPinState(PinState state) override
    {
        switch(state)
        {
            case PinState::high:
                port.setBit(bit, true);
                break;
            case PinState::low:
                port.setBit(bit, false);
                break;
            default:
                throw std::invalid_argument("wanted invalid state of a pin"); // usseles raczej ale dla picu jest (na razie)
                break;
        }
    }

    void setPinMode(PinMode state) override
    {
        switch(state)
        {
            case PinMode::input:
                port.setDirection(bit, false);
                break;
            case PinMode::output:
                port.setDirection(bit, true);
                break;
            case PinMode::inputPullUp: 
                port.pullUpBit(bit, true);
                break;
            default:
                throw std::invalid_argument("wanted invalid mode of a pin"); // usseles raczej ale dla picu jest (na razie)
                break;
        }
    }

    void setPullMode(PullMode state) override
    {
        switch(state)
        {
            case PullMode::noPull:
                port.pullUpBit(bit, false);
                break;
            case PullMode::pull:
                port.pullUpBit(bit, true);
                break;
            default:
                throw std::invalid_argument("wanted invalid pull mode of a pin"); // usseles raczej ale dla picu jest (na razie)
                break;
        }

    }
    
    bool read() const override
    {
        return port.readBit(bit);
    }

    void init() override
    {
        setDirection(Direction::input); // domyslnie 0
        setPullMode(PullMode::noPull); // domyslnie pullUp wylaczony 
    }
};


int main()
{

    std::cout<<"Elo elo\n";
}