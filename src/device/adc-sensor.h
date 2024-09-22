
namespace device {

class ADCSensor {
   public:
    enum class Status {
        Free,
        Processing,
        Ready,
    };

    void setup();
    ADCSensor::Status getState();
    float getVoltage();
};
}  // namespace device
