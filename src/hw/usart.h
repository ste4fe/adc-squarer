#pragma once

#include "device/desktop-console.h"

#include "apm32f10x_dma.h"
#include "apm32f10x_usart.h"

namespace HW {
static auto *const defaultUSART = USART1;
static auto *const defaultTxDMA = DMA1_Channel4;
static auto *const defaultRxDMA = DMA1_Channel5;
static const size_t bufferSize = 64;
static const uint32_t baudRate = 115200;

class USARTConsole : public device::DesktopConsole {
   public:
    USARTConsole(
        USART_T *dUSART,
        DMA_Channel_T *rxDMA,
        DMA_Channel_T *txDMA,
        char *pBuffer,
        size_t sBuffer
    );
    // USARTConsole() = default;
    void init();
    void loadAnswer(const char *answer) override;

   private:
    USART_T *_USART;
    DMA_Channel_T *_rxDMA;
    DMA_Channel_T *_txDMA;
};

extern USARTConsole Console;

}  // namespace HW