#include "contiki.h"
#include "net/rime.h"
#include "random.h"
#include "dev/light-sensor.h"
#include "dev/button-sensor.h"
#include "dev/leds.h"
#include "sys/node-id.h"
#include <stdio.h>

PROCESS(broadcast_button_process, "Broadcast Button");
AUTOSTART_PROCESSES(&broadcast_button_process);

int button_pressed_counter=0;
int messages_received_counter=0;

static void broadcast_recv(struct broadcast_conn *c, const rimeaddr_t *from)
{
        messages_received_counter++;

        //printing the event message of the sender node and the total number of messages received
        printf("Broadcast message received from %d.%d: '%s'\n", from->u8[0], from->u8[1], (char *)packetbuf_dataptr());
        printf("Button pressed on node %d.%d - %d messages received in total\n", from->u8[0], from->u8[1], messages_received_counter);

        //changing the modes of the green and blue LEDs
        leds_toggle(LEDS_GREEN);
        leds_toggle(LEDS_BLUE);
}

static const struct broadcast_callbacks broadcast_call = {broadcast_recv};
static struct broadcast_conn broadcast;

PROCESS_THREAD(broadcast_button_process, ev, data)
{
        static struct etimer timer;
        PROCESS_EXITHANDLER(broadcast_close(&broadcast);)

        PROCESS_BEGIN();

                SENSORS_ACTIVATE(button_sensor);
                SENSORS_ACTIVATE(light_sensor);

                broadcast_open(&broadcast, 129, &broadcast_call);

                while(1)   //always waiting for a button click
                {
                        PROCESS_WAIT_EVENT_UNTIL(ev == sensors_event && data == &button_sensor);

                        button_pressed_counter++;
                        printf("Node %d: my button was pressed - %d times in total\n", node_id, button_pressed_counter);

                        leds_on(LEDS_RED);      //switching on the red LED after the sensor's button is pressed

                        //waiting for 5 seconds
                        etimer_set(&timer, CLOCK_SECOND * 5);
                        PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_TIMER);

                        leds_off(LEDS_RED);     //switching off the red LED after 5 seconds

                        //broadcasting the event message to the neighbors
                        packetbuf_copyfrom("button pressed", 16);
                        broadcast_send(&broadcast);
                }

        PROCESS_END();
}
