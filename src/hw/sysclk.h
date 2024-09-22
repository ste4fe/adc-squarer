#include <stdint.h>

namespace HW {
namespace SysCLK {

void init();

uint32_t getTime();

void hardDelay(uint32_t ms);

}  // namespace SysCLK
}  // namespace HW
