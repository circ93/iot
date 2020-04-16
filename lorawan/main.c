#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include <inttypes.h>

#include "msg.h"
#include "shell.h"
#include "fmt.h"

#include "hts221.h"
#include "hts221_params.h"

#include "net/loramac.h"
#include "semtech_loramac.h"

/* Messages are sent every 20s to respect the duty cycle on each channel */
#define PERIOD              (20U)

semtech_loramac_t loramac;

static hts221_t hts221;

/* Application key is 16 bytes long (e.g. 32 hex chars), and thus the longest
   possible size (with application session and network session keys) */
static char print_buf[LORAMAC_APPKEY_LEN * 2 + 1];


static void _cmd_usage(void)
{
    puts("Usage: <config|start>");
}

static void _config_usage(void)
{
    puts("Usage: config set/get <deveui|appeui|appkey|dr>" "<value>");
}

static void _tx_usage(void)
{
    puts("Usage: start tx <id_station>");
}

static void _join_usage(void)
{
    puts("Usage: start join <otaa>");
}

void get_data(char * msg, char * id) {
    /* random measurements */
    int win_dir = rand() % 360 +1 ;
    int win_int = rand()% 100 +1 ;
    int rain = rand() % 50 +1 ;

    uint16_t humidity = 0;
    int16_t temperature = 0;
    /* measurements from real sensors */
    if (hts221_read_humidity(&hts221, &humidity) != HTS221_OK) {
        puts(" -- failed to read humidity!");
    }
    if (hts221_read_temperature(&hts221, &temperature) != HTS221_OK) {
        puts(" -- failed to read temperature!");
    }
    /* string of measurements */
    sprintf (msg, "{\"id\": %s,\"temperature\": %d,\"humidity\": %d,\"wind_direction\": %d,\"wind_intensity\": %d,\"rain_height\": %d}", id, temperature%50,humidity%100,win_dir,win_int,rain);

}
/* configuration device iot-lab */
static int _cmd_config(int argc, char **argv) {
    if (argc < 2) {
        _cmd_usage();
        return 1;
    }

    if (strcmp(argv[0], "config") == 0) {
        if (argc < 3) {
            _config_usage();
            return 1;
        }
        else if (strcmp(argv[1], "set") == 0) {
            if (argc < 3) {
                _config_usage();
                return 1;
            }

            if (strcmp("deveui", argv[2]) == 0) {
                if ((argc < 4) || (strlen(argv[3]) != LORAMAC_DEVEUI_LEN * 2)) {
                    puts("Usage: loramac set deveui <16 hex chars>");
                    return 1;
                }
                uint8_t deveui[LORAMAC_DEVEUI_LEN];
                fmt_hex_bytes(deveui, argv[3]);
                semtech_loramac_set_deveui(&loramac, deveui);
            }
            else if (strcmp("appeui", argv[2]) == 0) {
                if ((argc < 4) || (strlen(argv[3]) != LORAMAC_APPEUI_LEN * 2)) {
                    puts("Usage: loramac set appeui <16 hex chars>");
                    return 1;
                }
                uint8_t appeui[LORAMAC_APPEUI_LEN];
                fmt_hex_bytes(appeui, argv[3]);
                semtech_loramac_set_appeui(&loramac, appeui);
            }
            else if (strcmp("appkey", argv[2]) == 0) {
                if ((argc < 4) || (strlen(argv[3]) != LORAMAC_APPKEY_LEN * 2)) {
                    puts("Usage: loramac set appkey <32 hex chars>");
                    return 1;
                }
                uint8_t appkey[LORAMAC_APPKEY_LEN];
                fmt_hex_bytes(appkey, argv[3]);
                semtech_loramac_set_appkey(&loramac, appkey);
            }
            else if (strcmp("dr", argv[2]) == 0) {
                if (argc < 4) {
                    puts("Usage: loramac set dr <0..16>");
                    return 1;
                }
                uint8_t dr = atoi(argv[3]);
                if (dr > LORAMAC_DR_15) {
                    puts("Usage: loramac set dr <0..16>");
                    return 1;
                }
                semtech_loramac_set_dr(&loramac, dr);
            }
            else {
                _config_usage();
                return 1;
            }
        }

        if (strcmp(argv[1], "get") == 0) {
            if (argc < 3) {
                _config_usage();
                return 1;
            }

            if (strcmp("deveui", argv[2]) == 0) {
                uint8_t deveui[LORAMAC_DEVEUI_LEN];
                semtech_loramac_get_deveui(&loramac, deveui);
                fmt_bytes_hex(print_buf, deveui, LORAMAC_DEVEUI_LEN);
                print_buf[LORAMAC_DEVEUI_LEN * 2] = '\0';
                printf("DEVEUI: %s\n", print_buf);
                return 1;
            }
            else if (strcmp("appeui", argv[2]) == 0) {
                uint8_t appeui[LORAMAC_APPEUI_LEN];
                semtech_loramac_get_appeui(&loramac, appeui);
                fmt_bytes_hex(print_buf, appeui, LORAMAC_APPEUI_LEN);
                print_buf[LORAMAC_APPEUI_LEN * 2] = '\0';
                printf("APPEUI: %s\n", print_buf);
                return 1;
            }
            else if (strcmp("appkey", argv[2]) == 0) {
                uint8_t appkey[LORAMAC_APPKEY_LEN];
                semtech_loramac_get_appkey(&loramac, appkey);
                fmt_bytes_hex(print_buf, appkey, LORAMAC_APPKEY_LEN);
                print_buf[LORAMAC_APPKEY_LEN * 2] = '\0';
                printf("APPKEY: %s\n", print_buf);
                return 1;
            }
            else if (strcmp("dr", argv[2]) == 0) {
                printf("DATARATE: %d\n",
                   semtech_loramac_get_dr(&loramac));
                return 1;
            }
        }

    }
    return 0;
}
/* sending measurements */
static int _cmd_lora(int argc, char **argv)
{
    if (argc < 2) {
        _cmd_usage();
        return 1;
    }

    if (strcmp(argv[0], "start") == 0) {
        if (argc < 3) {
            _config_usage();
            return 1;
        }

        else if (strcmp(argv[1], "join") == 0) {
            if (argc < 3) {
                _join_usage();
                return 1;
            }

            uint8_t join_type;
            if (strcmp(argv[2], "otaa") == 0) {
                join_type = LORAMAC_JOIN_OTAA;
            }
            else {
                _join_usage();
                return 1;
            }

            switch (semtech_loramac_join(&loramac, join_type)) {
                case SEMTECH_LORAMAC_DUTYCYCLE_RESTRICTED:
                    puts("Cannot join: dutycycle restriction");
                    return 1;
                case SEMTECH_LORAMAC_BUSY:
                    puts("Cannot join: mac is busy");
                    return 1;
                case SEMTECH_LORAMAC_JOIN_FAILED:
                    puts("Join procedure failed!");
                    return 1;
                case SEMTECH_LORAMAC_ALREADY_JOINED:
                    puts("Warning: already joined!");
                    return 1;
                case SEMTECH_LORAMAC_JOIN_SUCCEEDED:
                    puts("Join procedure succeeded!");
                    break;
                default: /* should not happen */
                    break;
            }
            return 0;
        }
    
        else if (strcmp(argv[1], "tx") == 0) {
            if (argc < 3) {
                _tx_usage();
                return 1;
            }

            char *id = NULL;
            id = argv[2];

            /* Real sensors */
            if (hts221_init(&hts221, &hts221_params[0]) != HTS221_OK) {
                puts("Sensor initialization failed");
                LED3_TOGGLE;
                return 1;
            }
            if (hts221_power_on(&hts221) != HTS221_OK) {
                puts("Sensor initialization power on failed");
                LED3_TOGGLE;
                return 1;
            }
            if (hts221_set_rate(&hts221, hts221.p.rate) != HTS221_OK) {
                puts("Sensor continuous mode setup failed");
                LED3_TOGGLE;
                return 1;
            }

            semtech_loramac_set_tx_mode(&loramac, LORAMAC_DEFAULT_TX_MODE);
            semtech_loramac_set_tx_port(&loramac, LORAMAC_DEFAULT_TX_PORT);

            /* send message */
            while(1) {
                char msg[100];
                get_data(msg, id);
                printf("Message send: '%s'\n", msg);

                switch (semtech_loramac_send(&loramac,
                                     (uint8_t *)msg, strlen(msg))) {
                    case SEMTECH_LORAMAC_NOT_JOINED:
                        puts("Cannot send: not joined");
                        return 1;

                    case SEMTECH_LORAMAC_DUTYCYCLE_RESTRICTED:
                        puts("Cannot send: dutycycle restriction");
                        return 1;

                    case SEMTECH_LORAMAC_BUSY:
                        puts("Cannot send: MAC is busy");
                        return 1;

                    case SEMTECH_LORAMAC_TX_ERROR:
                        puts("Cannot send: error");
                        return 1;
                }

                /* wait for receive windows */
                switch (semtech_loramac_recv(&loramac)) {
                    case SEMTECH_LORAMAC_DATA_RECEIVED:
                        loramac.rx_data.payload[loramac.rx_data.payload_len] = 0;
                        printf("Data received: %s, port: %d\n",
                               (char *)loramac.rx_data.payload, loramac.rx_data.port);
                        break;

                    case SEMTECH_LORAMAC_DUTYCYCLE_RESTRICTED:
                        puts("Cannot send: dutycycle restriction");
                        return 1;

                    case SEMTECH_LORAMAC_BUSY:
                        puts("Cannot send: MAC is busy");
                        return 1;

                    case SEMTECH_LORAMAC_TX_ERROR:
                        puts("Cannot send: error");
                        return 1;

                    case SEMTECH_LORAMAC_TX_DONE:
                        puts("TX complete, no data received");
                        break;
                }
                /* waiting to send new message */
                xtimer_sleep(20);
            }
            return 0;
        }
    }
    else {
        _cmd_usage();
        return 1;
    }
    return 0;
}

static const shell_command_t shell_commands[] = {
    { "start", "run the station", _cmd_lora },
    { "config", "run the station", _cmd_config },
    { NULL, NULL, NULL }
};

int main(void)
{
    semtech_loramac_init(&loramac);

    puts("=====================================");
    puts("=                                   =");
    puts("=   Starting LoRaWAN application    =");
    puts("=                                   =");
    puts("=====================================");
    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(shell_commands, line_buf, SHELL_DEFAULT_BUFSIZE);
}
