#pragma once

#include <stdint.h>
#include "main.h"

#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

#define CAN_RX_QUEUE_SIZE 16
#define CAN_TX_QUEUE_SIZE 16
#define CAN_TX_TASK_STACK_SIZE 128

extern CAN_HandleTypeDef hcan;

typedef struct {
    uint16_t            id;     //11-bit ID
    uint8_t             len;     //length of message in bytes (0 - 8)
    uint8_t             data[8]; //data to send (up to 8 bytes)
} CAN_msg_t;

typedef struct {
    CAN_HandleTypeDef*  hcan;
    uint16_t            node_id;

    //RTOS stuff
    QueueHandle_t       rx_queue;
    QueueHandle_t       tx_queue;
    TaskHandle_t        tx_task_handle;
} CAN_al_t;

typedef struct {
    CAN_HandleTypeDef*  hcan;
    uint8_t*            filter_ids;
    uint16_t            node_id;
} CAN_al_config;

// Setup
/// @brief Initialize CAN connection, queues, and TX task
/// @param can Pointer to CAN context
/// @param config Pointer to configuration setup 
/// @param filter_ids list of ids to enable receiving CAN messages from.
/// @param num_of_ids number of ids in the list of filter_ids.
/// @return 0 on success, else non-zero
int CAN_al_init(CAN_al_t* can, CAN_al_config* config, uint8_t* filter_ids, int num_of_ids);

// Sending data
/// @brief Add message to CAN sending queue.
/// @param can Pointer to CAN context
/// @param msg message to send
/// @return 0 if successful, else 1
int CAN_tx_enqueue(CAN_al_t* can, CAN_msg_t* msg);

// Getting data
/// @brief Waits to retreive a message from the RX queue
/// @param can Pointer to CAN context
/// @param msg Pointer to store received message
/// @return 0 if successful, else 1
int CAN_rx_get_from_queue(CAN_al_t* can, CAN_msg_t* msg);

// ISR Callback
void HAL_CAN_RxEventCallback(CAN_HandleTypeDef* hcan, uint16_t size);

// Send Task Creation
/// @brief Transmit task that physically sends messages from tx_queue 
/// @param pvParameters Pointer to the CAN_al_t context.
void CAN_tx_task(void *pvParameters);