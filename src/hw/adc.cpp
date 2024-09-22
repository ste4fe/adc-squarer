#include "adc.h"


HW::ADCSensor HW::ADCSensorInstance;

/**
 * @brief Первичная инициализация канала ацп
 * 
 * @param channel номер канала
 */
void HW::ADCSensor::init(ADC_CHANNEL_T channel) {
    ADC_Config_T adcConfig;

    /* ADC configuration */
    ADC_Reset(ADC1);
    ADC_ConfigStructInit(&adcConfig);
    adcConfig.mode                  = ADC_MODE_INDEPENDENT;
    adcConfig.scanConvMode          = DISABLE;
    adcConfig.continuosConvMode     = ENABLE;
    adcConfig.externalTrigConv      = ADC_EXT_TRIG_CONV_None;
    adcConfig.dataAlign             = ADC_DATA_ALIGN_RIGHT;
    /* channel number */
    adcConfig.nbrOfChannel          = channel;
    ADC_Config(ADC1, &adcConfig);

    /* ADC channel Convert configuration */
    ADC_ConfigRegularChannel(ADC1, ADC_CHANNEL_0, 1, ADC_SAMPLETIME_13CYCLES5);
    
    /* Enable complete conversion interupt */
    ADC_EnableInterrupt(ADC1, ADC_INT_EOC);

    /* NVIC configuration */
    NVIC_EnableIRQ(ADC1_2_IRQn);
    
    /* Enable ADC */
    ADC_Enable(ADC1);

    /* Enable ADC1 reset calibration register */
    ADC_ResetCalibration(ADC1);
    /* Check the end of ADC1 reset calibration register */
    while(ADC_ReadResetCalibrationStatus(ADC1));

    /* Start ADC1 calibration */
    ADC_StartCalibration(ADC1);
    /* Check the end of ADC1 calibration */
    while(ADC_ReadCalibrationStartFlag(ADC1));

    /* Start ADC1 Software Conversion */
    ADC_EnableSoftwareStartConv(ADC1);

    HW_ADC_CurrentHandler = &this->LLHandlerInstance;
}

/**
 * @brief Получить результат счета ацп
 * 
 * @return uint16_t 
 */
uint16_t HW::ADCSensor::getResult() {
    return this->LLHandlerInstance.conversionValue;
}
