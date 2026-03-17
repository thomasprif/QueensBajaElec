#include "can_al_freertos.h"

extern CAN_al_t *can_ptr;

int CAN_al_init(CAN_al_t *can, CAN_al_config *config, uint8_t *filter_ids, int num_of_ids) {
    can->hcan = config->hcan;
    can->node_id = config->node_id;
    can_ptr = can;

    // Setup send and receive queues
    can->rx_queue = xQueueCreate(CAN_RX_QUEUE_SIZE, sizeof(CAN_msg_t));
    can->tx_queue = xQueueCreate(CAN_TX_QUEUE_SIZE, sizeof(CAN_msg_t));

    if (can->rx_queue == NULL || can->tx_queue == NULL) return 1; // Error creating queue

    // Setup transmit task
    // xTaskCreate()?


    // Configure the filters


    // Start CAN
    if (HAL_CAN_Start(can->hcan) != HAL_OK) return 3;

    if (HAL_CAN_ActivateNotification(can->hcan, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK) return 4;

    return 0; // Success
}

int CAN_tx_enqueue(CAN_al_t *can, CAN_msg_t *msg) {
    if (xQueueSend(can->tx_queue, msg, pdMS_TO_TICKS(10)) == pdTRUE) return 0;
    return 1; // Send queue full
}

int CAN_rx_get_from_queue(CAN_al_t *can, CAN_msg_t *msg) {
    if (xQueueReceive(can->rx_queue, msg, portMAX_DELAY) == pdTRUE) return 0;
    return 1; //Error retrieving from queue
}

void HAL_CAN_RxEventCallback(CAN_HandleTypeDef *hcan, uint16_t size) {
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
