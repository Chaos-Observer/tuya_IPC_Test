/**
* @file tuya_os_adapter_error_code.h
* @brief Common process - Initialization
* @version 0.1
* @date 2020-11-09
*
* @copyright Copyright 2020-2021 Tuya Inc. All Rights Reserved.
*
*/
#ifndef TUYA_OS_ADAPTER_ERROR_CODE_H
#define TUYA_OS_ADAPTER_ERROR_CODE_H


#ifdef __cplusplus
extern "C" {
#endif

#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

#ifndef INOUT
#define INOUT
#endif

/**
 * @brief tuya sdk definition of socket errno
 */
typedef int UNW_ERRNO_T;
#define UNW_SUCCESS       0
#define UNW_FAIL          -1
#define UNW_EINTR         -2
#define UNW_EBADF         -3
#define UNW_EAGAIN        -4
#define UNW_EFAULT        -5
#define UNW_EBUSY         -6
#define UNW_EINVAL        -7
#define UNW_ENFILE        -8
#define UNW_EMFILE        -9
#define UNW_ENOSPC        -10
#define UNW_EPIPE         -11
#define UNW_EWOULDBLOCK   -12
#define UNW_ENOTSOCK      -13
#define UNW_ENOPROTOOPT   -14
#define UNW_EADDRINUSE    -15
#define UNW_EADDRNOTAVAIL -16
#define UNW_ENETDOWN      -17
#define UNW_ENETUNREACH   -18
#define UNW_ENETRESET     -19
#define UNW_ECONNRESET    -20
#define UNW_ENOBUFS       -21
#define UNW_EISCONN       -22
#define UNW_ENOTCONN      -23
#define UNW_ETIMEDOUT     -24
#define UNW_ECONNREFUSED  -25
#define UNW_EHOSTDOWN     -26
#define UNW_EHOSTUNREACH  -27
#define UNW_ENOMEM        -28
#define UNW_EMSGSIZE      -29

#define OPRT_OS_INTF_INVALID   -200  //Component interface service is invalid

/**
 * @brief common error code
 */
#define OPRT_OS_ADAPTER_OK                      0
#define OPRT_OS_ADAPTER_COM_ERROR               -1
#define OPRT_OS_ADAPTER_INVALID_PARM            -2
#define OPRT_OS_ADAPTER_MALLOC_FAILED           -3
#define OPRT_OS_ADAPTER_NOT_SUPPORTED           -4
#define OPRT_OS_ADAPTER_NETWORK_ERROR           -5

/**
 * @brief error code of mutex
 */
#define OPRT_OS_ADAPTER_ERRCODE_MUTEX           10100
#define OPRT_OS_ADAPTER_MUTEX_CREAT_FAILED      -(OPRT_OS_ADAPTER_ERRCODE_MUTEX + 1)
#define OPRT_OS_ADAPTER_MUTEX_LOCK_FAILED       -(OPRT_OS_ADAPTER_ERRCODE_MUTEX + 2)
#define OPRT_OS_ADAPTER_MUTEX_UNLOCK_FAILED     -(OPRT_OS_ADAPTER_ERRCODE_MUTEX + 3)
#define OPRT_OS_ADAPTER_MUTEX_RELEASE_FAILED    -(OPRT_OS_ADAPTER_ERRCODE_MUTEX + 4)

/**
 * @brief error code of semaphore
 */
#define OPRT_OS_ADAPTER_ERRCODE_SEM             10110
#define OPRT_OS_ADAPTER_SEM_CREAT_FAILED        -(OPRT_OS_ADAPTER_ERRCODE_SEM + 1)
#define OPRT_OS_ADAPTER_SEM_WAIT_FAILED         -(OPRT_OS_ADAPTER_ERRCODE_SEM + 2)
#define OPRT_OS_ADAPTER_SEM_POST_FAILED         -(OPRT_OS_ADAPTER_ERRCODE_SEM + 3)
#define OPRT_OS_ADAPTER_SEM_RELEASE_FAILED      -(OPRT_OS_ADAPTER_ERRCODE_SEM + 4)

/**
 * @brief error code of queue
 */
#define OPRT_OS_ADAPTER_ERRCODE_QUEUE           10120
#define OPRT_OS_ADAPTER_QUEUE_CREAT_FAILED      -(OPRT_OS_ADAPTER_ERRCODE_QUEUE + 1)
#define OPRT_OS_ADAPTER_QUEUE_SEND_FAIL         -(OPRT_OS_ADAPTER_ERRCODE_QUEUE + 2)
#define OPRT_OS_ADAPTER_QUEUE_RECV_FAIL         -(OPRT_OS_ADAPTER_ERRCODE_QUEUE + 3)

/**
 * @brief error code of thread
 */
#define OPRT_OS_ADAPTER_ERRCODE_THRD            10130
#define OPRT_OS_ADAPTER_THRD_CREAT_FAILED       -(OPRT_OS_ADAPTER_ERRCODE_THRD + 1)
#define OPRT_OS_ADAPTER_THRD_RELEASE_FAILED     -(OPRT_OS_ADAPTER_ERRCODE_THRD + 2)
#define OPRT_OS_ADAPTER_THRD_JUDGE_SELF_FAILED  -(OPRT_OS_ADAPTER_ERRCODE_THRD + 3)

/**
 * @brief error code of system
 */
#define OPRT_OS_ADAPTER_ERRCODE_SYSTEM          10140
#define OPRT_OS_ADAPTER_CPU_LPMODE_SET_FAILED   -(OPRT_OS_ADAPTER_ERRCODE_SYSTEM + 1)

/**
 * @brief error code of output
 */
#define OPRT_OS_ADAPTER_ERRCODE_LOG             10150
#define OPRT_OS_ADAPTER_LOG_CLOSE_FAILED        -(OPRT_OS_ADAPTER_ERRCODE_LOG + 1)
#define OPRT_OS_ADAPTER_LOG_OPEN_FAILED         -(OPRT_OS_ADAPTER_ERRCODE_LOG + 2)

/**
 * @brief error code of wifi
 */
#define OPRT_OS_ADAPTER_ERRCODE_WIFI            10160
#define OPRT_OS_ADAPTER_AP_NOT_FOUND            -(OPRT_OS_ADAPTER_ERRCODE_WIFI + 1)
#define OPRT_OS_ADAPTER_AP_SCAN_FAILED          -(OPRT_OS_ADAPTER_ERRCODE_WIFI + 2)
#define OPRT_OS_ADAPTER_AP_RELEASE_FAILED       -(OPRT_OS_ADAPTER_ERRCODE_WIFI + 3)
#define OPRT_OS_ADAPTER_CHAN_SET_FAILED         -(OPRT_OS_ADAPTER_ERRCODE_WIFI + 4)
#define OPRT_OS_ADAPTER_CHAN_GET_FAILED         -(OPRT_OS_ADAPTER_ERRCODE_WIFI + 5)
#define OPRT_OS_ADAPTER_IP_GET_FAILED           -(OPRT_OS_ADAPTER_ERRCODE_WIFI + 6)
#define OPRT_OS_ADAPTER_MAC_SET_FAILED          -(OPRT_OS_ADAPTER_ERRCODE_WIFI + 7)
#define OPRT_OS_ADAPTER_MAC_GET_FAILED          -(OPRT_OS_ADAPTER_ERRCODE_WIFI + 8)
#define OPRT_OS_ADAPTER_WORKMODE_SET_FAILED     -(OPRT_OS_ADAPTER_ERRCODE_WIFI + 9)
#define OPRT_OS_ADAPTER_WORKMODE_GET_FAILED     -(OPRT_OS_ADAPTER_ERRCODE_WIFI + 10)
#define OPRT_OS_ADAPTER_SNIFFER_SET_FAILED      -(OPRT_OS_ADAPTER_ERRCODE_WIFI + 11)
#define OPRT_OS_ADAPTER_AP_START_FAILED         -(OPRT_OS_ADAPTER_ERRCODE_WIFI + 12)
#define OPRT_OS_ADAPTER_AP_STOP_FAILED          -(OPRT_OS_ADAPTER_ERRCODE_WIFI + 13)
#define OPRT_OS_ADAPTER_APINFO_GET_FAILED       -(OPRT_OS_ADAPTER_ERRCODE_WIFI + 14)
#define OPRT_OS_ADAPTER_FAST_CONN_FAILED        -(OPRT_OS_ADAPTER_ERRCODE_WIFI + 15)
#define OPRT_OS_ADAPTER_CONN_FAILED             -(OPRT_OS_ADAPTER_ERRCODE_WIFI + 16)
#define OPRT_OS_ADAPTER_DISCONN_FAILED          -(OPRT_OS_ADAPTER_ERRCODE_WIFI + 17)
#define OPRT_OS_ADAPTER_RSSI_GET_FAILED         -(OPRT_OS_ADAPTER_ERRCODE_WIFI + 18)
#define OPRT_OS_ADAPTER_BSSID_GET_FAILED        -(OPRT_OS_ADAPTER_ERRCODE_WIFI + 19)
#define OPRT_OS_ADAPTER_STAT_GET_FAILED         -(OPRT_OS_ADAPTER_ERRCODE_WIFI + 20)
#define OPRT_OS_ADAPTER_CCODE_SET_FAILED        -(OPRT_OS_ADAPTER_ERRCODE_WIFI + 21)
#define OPRT_OS_ADAPTER_MGNT_SEND_FAILED        -(OPRT_OS_ADAPTER_ERRCODE_WIFI + 22)
#define OPRT_OS_ADAPTER_MGNT_REG_FAILED         -(OPRT_OS_ADAPTER_ERRCODE_WIFI + 23)
#define OPRT_OS_ADAPTER_WF_LPMODE_SET_FAILED    -(OPRT_OS_ADAPTER_ERRCODE_WIFI + 24)

/**
 * @brief error code of flash
 */
#define OPRT_OS_ADAPTER_ERRCODE_FLASH           10210
#define OPRT_OS_ADAPTER_FLASH_READ_FAILED       -(OPRT_OS_ADAPTER_ERRCODE_FLASH + 1)
#define OPRT_OS_ADAPTER_FLASH_WRITE_FAILED      -(OPRT_OS_ADAPTER_ERRCODE_FLASH + 2)
#define OPRT_OS_ADAPTER_FLASH_ERASE_FAILED      -(OPRT_OS_ADAPTER_ERRCODE_FLASH + 3)

/**
 * @brief error code of ota
 */
#define OPRT_OS_ADAPTER_ERRCODE_OTA             10220
#define OPRT_OS_ADAPTER_OTA_START_INFORM_FAILED -(OPRT_OS_ADAPTER_ERRCODE_OTA + 1)
#define OPRT_OS_ADAPTER_OTA_PKT_SIZE_FAILED     -(OPRT_OS_ADAPTER_ERRCODE_OTA + 2)
#define OPRT_OS_ADAPTER_OTA_PROCESS_FAILED      -(OPRT_OS_ADAPTER_ERRCODE_OTA + 3)
#define OPRT_OS_ADAPTER_OTA_VERIFY_FAILED       -(OPRT_OS_ADAPTER_ERRCODE_OTA + 4)
#define OPRT_OS_ADAPTER_OTA_END_INFORM_FAILED   -(OPRT_OS_ADAPTER_ERRCODE_OTA + 5)

/**
 * @brief error code of watchdog
 */
#define OPRT_OS_ADAPTER_ERRCODE_WD              10230
#define OPRT_OS_ADAPTER_WD_INIT_FAILED          -(OPRT_OS_ADAPTER_ERRCODE_WD + 1)

/**
 * @brief error code of gpio
 */
#define OPRT_OS_ADAPTER_ERRCODE_GPIO            10240
#define OPRT_OS_ADAPTER_GPIO_INOUT_SET_FAILED   -(OPRT_OS_ADAPTER_ERRCODE_GPIO + 1)
#define OPRT_OS_ADAPTER_GPIO_MODE_SET_FAILED    -(OPRT_OS_ADAPTER_ERRCODE_GPIO + 2)
#define OPRT_OS_ADAPTER_GPIO_WRITE_FAILED       -(OPRT_OS_ADAPTER_ERRCODE_GPIO + 3)
#define OPRT_OS_ADAPTER_GPIO_IRQ_INIT_FAILED    -(OPRT_OS_ADAPTER_ERRCODE_GPIO + 4)

/**
 * @brief error code of uart
 */
#define OPRT_OS_ADAPTER_ERRCODE_UART            10250
#define OPRT_OS_ADAPTER_UART_INIT_FAILED        -(OPRT_OS_ADAPTER_ERRCODE_UART + 1)
#define OPRT_OS_ADAPTER_UART_DEINIT_FAILED      -(OPRT_OS_ADAPTER_ERRCODE_UART + 2)
#define OPRT_OS_ADAPTER_UART_SEND_FAILED        -(OPRT_OS_ADAPTER_ERRCODE_UART + 3)
#define OPRT_OS_ADAPTER_UART_READ_FAILED        -(OPRT_OS_ADAPTER_ERRCODE_UART + 4)

/**
 * @brief error code of i2c
 */
#define OPRT_OS_ADAPTER_ERRCODE_I2C             10260
#define OPRT_OS_ADAPTER_I2C_OPEN_FAILED         -(OPRT_OS_ADAPTER_ERRCODE_I2C + 1)
#define OPRT_OS_ADAPTER_I2C_CLOSE_FAILED        -(OPRT_OS_ADAPTER_ERRCODE_I2C + 2)
#define OPRT_OS_ADAPTER_I2C_READ_FAILED         -(OPRT_OS_ADAPTER_ERRCODE_I2C + 3)
#define OPRT_OS_ADAPTER_I2C_WRITE_FAILED        -(OPRT_OS_ADAPTER_ERRCODE_I2C + 4)

/**
 * @brief error code of bluetooth
 */
#define OPRT_OS_ADAPTER_ERRCODE_BT              10270
#define OPRT_OS_ADAPTER_BT_INIT_FAILED          -(OPRT_OS_ADAPTER_ERRCODE_BT + 1)
#define OPRT_OS_ADAPTER_BT_DEINIT_FAILED        -(OPRT_OS_ADAPTER_ERRCODE_BT + 2)
#define OPRT_OS_ADAPTER_BT_DISCONN_FAILED       -(OPRT_OS_ADAPTER_ERRCODE_BT + 3)
#define OPRT_OS_ADAPTER_BT_SEND_FAILED          -(OPRT_OS_ADAPTER_ERRCODE_BT + 4)
#define OPRT_OS_ADAPTER_BT_ADV_RESET_FAILED     -(OPRT_OS_ADAPTER_ERRCODE_BT + 5)
#define OPRT_OS_ADAPTER_BT_RSSI_GET_FAILED      -(OPRT_OS_ADAPTER_ERRCODE_BT + 6)
#define OPRT_OS_ADAPTER_BT_ADV_START_FAILED     -(OPRT_OS_ADAPTER_ERRCODE_BT + 7)
#define OPRT_OS_ADAPTER_BT_ADV_STOP_FAILED      -(OPRT_OS_ADAPTER_ERRCODE_BT + 8)
#define OPRT_OS_ADAPTER_BT_SCAN_FAILED          -(OPRT_OS_ADAPTER_ERRCODE_BT + 9)

#ifdef __cplusplus
}
#endif
#endif
