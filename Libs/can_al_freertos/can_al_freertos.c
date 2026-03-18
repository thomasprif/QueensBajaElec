#include "can_al_freertos.h"

#include <string.h>

CAN_al_t *can_ptr;

int CAN_al_init(CAN_al_t *can, CAN_al_config *config, uint16_t *filter_ids, int num_of_ids) {
    can->hcan = config->hcan;
    can->node_id = config->node_id;
    can_ptr = can;

    // Setup send and receive queues
    can->rx_queue = xQueueCreate(CAN_RX_QUEUE_SIZE, sizeof(CAN_msg_t));
    can->tx_queue = xQueueCreate(CAN_TX_QUEUE_SIZE, sizeof(CAN_msg_t));

    if (can->rx_queue == NULL || can->tx_queue == NULL) return 1; // Error creating queue

    // Setup transmit task
    if (xTaskCreate(
        CAN_tx_task,
        "CAN_tx_task",
        CAN_TX_TASK_STACK_SIZE,
        can_ptr,
        tskIDLE_PRIORITY + 1,
        NULL
    ) != pdPASS) return 2;

    // Configure the filters
    CAN_Add_Filter_Discrete(can->hcan, num_of_ids, filter_ids);

    // Start CAN
    if (HAL_CAN_Start(can->hcan) != HAL_OK) return 3;

    if (HAL_CAN_ActivateNotification(can->hcan, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK) return 4;

    return CAN_OK; // Success
}

HAL_StatusTypeDef CAN_Add_Filter_Discrete(CAN_HandleTypeDef *hcan, int length, uint16_t *ids) {
    int ids_left = length;

    CAN_FilterTypeDef CAN_Filter_Config;

    CAN_Filter_Config.FilterActivation = CAN_FILTER_ENABLE;
    CAN_Filter_Config.FilterFIFOAssignment = CAN_FILTER_FIFO0;
    CAN_Filter_Config.FilterMode = CAN_FILTERMODE_IDLIST;
    CAN_Filter_Config.FilterScale = CAN_FILTERSCALE_16BIT;

    for (int i = 0; i < ((length - 1) / 4) + 1; i++) {
        CAN_Filter_Config.FilterBank = i;

        if (ids_left-- > 0)
            CAN_Filter_Config.FilterIdHigh = (ids[4 * i] << 5);

        if (ids_left-- > 0)
            CAN_Filter_Config.FilterIdLow = (ids[4 * i + 1] << 5);

        if (ids_left-- > 0)
            CAN_Filter_Config.FilterMaskIdHigh = (ids[4 * i + 2] << 5);

        if (ids_left-- > 0)
            CAN_Filter_Config.FilterMaskIdLow = (ids[4 * i + 3] << 5);

        return HAL_CAN_ConfigFilter(hcan, &CAN_Filter_Config);
    }
    return 1;
}

int CAN_tx_enqueue(CAN_al_t *can, CAN_msg_t *msg) {
    if (xQueueSend(can->tx_queue, msg, pdMS_TO_TICKS(10)) == pdTRUE) return CAN_OK;
    return 1; // Send queue full
}

int CAN_rx_get_from_queue(CAN_al_t *can, CAN_msg_t *msg) {
    if (xQueueReceive(can->rx_queue, msg, portMAX_DELAY) == pdTRUE) return CAN_OK;
    return 1; //Error retrieving from queue
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
    CAN_RxHeaderTypeDef RxHeader;
    uint8_t data[8];
    CAN_msg_t msg;

    HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, data);

    msg.id = RxHeader.StdId;
    msg.len = RxHeader.DLC;
    memcpy(msg.data, data, msg.len);

    xQueueSendFromISR(can_ptr->rx_queue, &msg, NULL);
}

void CAN_tx_task(void *pvParameters) {
    CAN_al_t *can = (CAN_al_t *) pvParameters;
    CAN_msg_t tx_msg;
    CAN_TxHeaderTypeDef TxHeader;
    uint32_t TxMailbox;

    // Config header
    TxHeader.IDE = CAN_ID_STD;
    TxHeader.RTR = CAN_RTR_DATA;

    while (1) {
        // Check if there is a message in queue to be sent
        if (xQueueReceive(can->tx_queue, &tx_msg, portMAX_DELAY) == pdTRUE) {
            TxHeader.StdId = tx_msg.id;
            TxHeader.DLC = tx_msg.len;

            // Ensure open mailbox before try to add msg
            while (HAL_CAN_GetTxMailboxesFreeLevel(can->hcan) == 0) {
                vTaskDelay(pdMS_TO_TICKS(1));
            }

            // Add message to open mailbox
            HAL_CAN_AddTxMessage(can->hcan, &TxHeader, tx_msg.data, &TxMailbox);
        }
    }
}
