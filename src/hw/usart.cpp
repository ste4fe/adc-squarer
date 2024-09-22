#include "usart.h"

#include <memory.h>
#include <string.h>

using namespace HW;

char usartBuffer[bufferSize];
USARTConsole HW::Console(
    defaultUSART,
    defaultRxDMA,
    defaultTxDMA,
    usartBuffer,
    sizeof(usartBuffer)
);

HW::USARTConsole::USARTConsole(
    USART_T *dUSART,
    DMA_Channel_T *rxDMA,
    DMA_Channel_T *txDMA,
    char *pBuffer,
    size_t sBuffer
)
    : DesktopConsole(pBuffer, sBuffer) {
    this->_USART = USART1;
    this->_rxDMA = rxDMA;
    this->_txDMA = txDMA;
}

void HW::USARTConsole::init() {
    DMA_Config_T DMAConfig;
    USART_Config_T USARTConfig;

    USARTConfig.baudRate = baudRate;
    USARTConfig.hardwareFlow = USART_HARDWARE_FLOW_NONE;
    USARTConfig.mode = USART_MODE_TX_RX;
    USARTConfig.parity = USART_PARITY_NONE;
    USARTConfig.stopBits = USART_STOP_BIT_1;
    USARTConfig.wordLength = USART_WORD_LEN_8B;

    DMAConfig.M2M = DMA_M2MEN_DISABLE;
    DMAConfig.memoryDataSize = DMA_MEMORY_DATA_SIZE_BYTE;
    DMAConfig.memoryInc = DMA_MEMORY_INC_ENABLE;
    DMAConfig.priority = DMA_PRIORITY_HIGH;
    DMAConfig.peripheralDataSize = DMA_PERIPHERAL_DATA_SIZE_BYTE;
    DMAConfig.peripheralBaseAddr = (uint32_t) & this->_USART->DATA;
    DMAConfig.peripheralInc = DMA_PERIPHERAL_INC_DISABLE;

    // TX DMA channel config
    DMAConfig.bufferSize = 0;
    DMAConfig.memoryBaseAddr = (uint32_t) nullptr;
    DMAConfig.loopMode = DMA_MODE_NORMAL;
    DMAConfig.dir = DMA_DIR_PERIPHERAL_DST;

    DMA_Disable(this->_txDMA);
    DMA_Config(this->_txDMA, &DMAConfig);

    // RX DMA channel config
    DMAConfig.bufferSize = bufferSize;
    DMAConfig.memoryBaseAddr = (uint32_t)this->_pDMABuffer;
    DMAConfig.loopMode = DMA_MODE_CIRCULAR;
    DMAConfig.dir = DMA_DIR_PERIPHERAL_SRC;

    DMA_Disable(this->_rxDMA);
    DMA_Config(this->_rxDMA, &DMAConfig);
    DMA_Enable(this->_rxDMA);

    USART_Disable(this->_USART);
    USART_Reset(this->_USART);
    USART_Config(this->_USART, &USARTConfig);

    USART_EnableDMA(this->_USART, USART_DMA_TX_RX);
    USART_Enable(this->_USART);

    memset(this->_pDMABuffer, 0, this->_sDMABuffer);
}

void HW::USARTConsole::loadAnswer(const char *answer) {
    this->_txDMA->CHCFG_B.CHEN = DISABLE;
    this->_txDMA->CHMADDR = (uint32_t)answer;
    this->_txDMA->CHNDATA = strlen(answer);
    this->_txDMA->CHCFG_B.CHEN = ENABLE;
}
