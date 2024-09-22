/**
 * @file desktop-console.h
 * @author your name (you@domain.com)
 * @brief Класс обработки байтовой информации
 * @version 0.1
 * @date 2024-09-18
 *
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include <cstddef>
namespace device {
class DesktopConsole {
   public:
    DesktopConsole(char *pDMABuffer, size_t sDMABuffer);

    size_t getLineSize() const;
    size_t getLine(char *pDst);
    // virtual void loadAnswer(const char *answer);

   protected:
    char *_pDMABuffer;
    size_t _sDMABuffer;
};
}  // namespace device
