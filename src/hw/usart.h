#pragma once

#include <stddef.h>

#include "apm32f10x_dma.h"
#include "apm32f10x_usart.h"

namespace HW {
    static auto *const defaultUSART = USART1;
    static auto *const defaultTxDMA = DMA1_Channel4;
    static auto *const defaultRxDMA = DMA1_Channel5;

    constexpr size_t USARTbufferSize = 64;
    constexpr uint32_t baudRate = 38400;

class USARTConsole {

   public:

    void init(USART_T *dUSART, DMA_Channel_T *rxDMA, DMA_Channel_T *txDMA);
    void loadAnswer(const char *answer);
    bool busy();

    size_t getLineSize() const;
    size_t getLine(char *pDst);

   private:
    USART_T *_USART;
    DMA_Channel_T *_rxDMA;
    DMA_Channel_T *_txDMA;
    char _pDMABuffer[HW::USARTbufferSize];
};

extern USARTConsole USARTConsoleInstance;

}  // namespace HW