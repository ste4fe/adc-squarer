#include "usart.h"

#include <memory.h>
#include <string.h>

using namespace HW;

USARTConsole HW::USARTConsoleInstance;

/**
 * @brief Первичная инициализация консоли
 * 
 * @param USART Указатель на USART
 * @param rxDMA DMA приемника
 * @param txDMA DMA передатчика
 */
void HW::USARTConsole::init(USART_T *USART, DMA_Channel_T *rxDMA, DMA_Channel_T *txDMA) {
    DMA_Config_T DMAConfig;
    USART_Config_T USARTConfig;

    this->_USART = USART;
    this->_rxDMA = rxDMA;
    this->_txDMA = txDMA;

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
    DMAConfig.bufferSize = USARTbufferSize;
    DMAConfig.memoryBaseAddr = (uint32_t)this->_pDMABuffer;
    DMAConfig.loopMode = DMA_MODE_CIRCULAR;
    DMAConfig.dir = DMA_DIR_PERIPHERAL_SRC;

    DMA_Disable(this->_rxDMA);
    DMA_Config(this->_rxDMA, &DMAConfig);
    this->_rxDMA->CHCFG_B.CHEN = true;

    USART_Disable(this->_USART);
    USART_Reset(this->_USART);
    USART_Config(this->_USART, &USARTConfig);

    USART_EnableDMA(this->_USART, USART_DMA_TX_RX);
    USART_Enable(this->_USART);

    memset(this->_pDMABuffer, 0, USARTbufferSize);
}

/**
 * @brief Отправить ответ
 * 
 * @param answer Строка ответа
 */
void HW::USARTConsole::loadAnswer(const char *answer) {
    this->_txDMA->CHCFG_B.CHEN = DISABLE;
    this->_txDMA->CHMADDR = (uint32_t)answer;
    this->_txDMA->CHNDATA = strlen(answer);
    this->_txDMA->CHCFG_B.CHEN = ENABLE;

    // while(*answer != 0) {
    //     while (USART_ReadStatusFlag(this->_USART, USART_FLAG_TXBE) == RESET)
    //     {} USART_TxData(this->_USART, *answer); answer++;
    // }

    this->_rxDMA->CHCFG_B.CHEN = DISABLE;
    this->_rxDMA->CHNDATA = USARTbufferSize;
    this->_rxDMA->CHCFG_B.CHEN = ENABLE;
}

/**
 * @brief Занята ли консоль приемом/передачей
 * 
 * @return true 
 * @return false 
 */
bool HW::USARTConsole::busy() {
    if (USART_ReadStatusFlag(this->_USART, USART_FLAG_TXBE)) {
        return false;
    } else {
        return true;
    }
}

/**
 * @brief Получить длинну принятого сообщения
 * 
 * @return size_t 
 */
size_t HW::USARTConsole::getLineSize() const {
    const char *pLineEnd =
        (const char *)memchr(this->_pDMABuffer, '\n', USARTbufferSize);

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
size_t HW::USARTConsole::getLine(char *pDst) {
    size_t takedStringSize = this->getLineSize();

    if (takedStringSize == 0) {
        return 0;
    }

    memcpy(pDst, this->_pDMABuffer, takedStringSize);

    memset(this->_pDMABuffer, 0, takedStringSize + 1);

    return takedStringSize;
}
