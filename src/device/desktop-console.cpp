#include "desktop-console.h"

#include <string.h>

size_t device::DesktopConsole::getLineSize() const {
    const char *pLineEnd =
        (const char *)memchr(this->_pDMABuffer, '\n', this->_sDMABuffer);

    if (pLineEnd == nullptr) {
        return 0;
    }

    return pLineEnd - this->_pDMABuffer;
}

/**
 * @brief
 *
 * @param[out] pDst Куда записывать получившееся сообщение. Размер буффера
 * должен быть не меньше `sDMABuffer`. Последний байт окончания линии будет
 * удален.
 * @return size_t Размер строки
 */
size_t device::DesktopConsole::getLine(char *pDst) {
    size_t takedStringSize = this->getLineSize();

    if (takedStringSize == 0) {
        return 0;
    }

    memcpy(pDst, this->_pDMABuffer, takedStringSize - 1);

    memset(pDst, 0, takedStringSize);

    return takedStringSize;
}

void device::DesktopConsole::loadAnswer(const char *answer) {}

device::DesktopConsole::DesktopConsole(char *pDMABuffer, size_t sDMABuffer) {
    memset(pDMABuffer, 0, sDMABuffer);

    this->_pDMABuffer = pDMABuffer;
    this->_sDMABuffer = sDMABuffer;
}
