#ifndef PROJDEFS_H_
#define PROJDEFS_H_

#define MAX_DISTANCE 450 				// Maximum distance (in cm) to ping.
#define PING_INTERVAL 29 			    // Milliseconds between sensor pings (29ms is about the min to avoid cross-sensor echo).

#define FRONT_SENSOR_COMMAND               0x01
#define REAR_SENSOR_COMMAND                0x02
#define LEFT_SENSOR_COMMAND                0x03
#define RIGHT_SENSOR_COMMAND               0x04
#define TOP_SENSOR_COMMAND	               0x05
#define BOTTOM_SENSOR_COMMAND              0x06

#define START_LANDING_COMMAND			   0x07
#define STOP_LANDING_COMMAND			   0x08
#define	LANDING_CONFIRM_COMMAND			   0x09
#define GET_VOLTAGE_COMMAND				   0x0A
#define GET_CURRENT_COMMAND				   0x0B
#define GET_BATTERY_LEVEL_COMMAND		   0x0C
#define CELL_VOLTAGE_ALERT_COMMAND		   0x0D
#define LEVEL_BATTERY_ALERT			   	   0x0E
#define	RESET_DATA_BATTERY_COMMAND		   0x0F
#define VOLTAGE_CELL1_COMMAND			   0x10
#define VOLTAGE_CELL2_COMMAND			   0x11
#define VOLTAGE_CELL3_COMMAND			   0x12

#define ECHO_FRONT_PIN                     42
#define ECHO_REAR_PIN                      44
#define ECHO_LEFT_PIN                      46
#define ECHO_RIGHT_PIN                     48
#define ECHO_TOP_PIN                       50
#define ECHO_BOTTOM_PIN                    52

#define TRIGGER_FRONT_PIN                  43
#define TRIGGER_REAR_PIN                   45
#define TRIGGER_LEFT_PIN                   47
#define TRIGGER_RIGHT_PIN                  49
#define TRIGGER_TOP_PIN                    51
#define TRIGGER_BOTTOM_PIN                 53

#define VOLTAGE_CELL_1_PIN				   A0
#define VOLTAGE_CELL_2_PIN				   A1
#define VOLTAGE_CELL_3_PIN				   A2
#define CURRENT_BATTERY_PIN				   A3	


#define MAX_SENSOR_NUMBER 		   		   6

#define DEBUG_LED_PIN                      13

#pragma pack(1)
typedef struct COMMAND_DATA
{
    uint8_t command;
    uint32_t argument;   
};
#pragma pack()


#pragma pack(1)
typedef struct COMMAND_DATA_F
{
    uint8_t command;
    float argument;   
};
#pragma pack()


#define PROTOCOL_SIZE_IN_BYTES			   sizeof(COMMAND_DATA)

#endif //PROJDEFS_H_
