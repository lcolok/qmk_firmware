#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "quantum.h"
#include "via.h"
static uint32_t now, current_layer;
uint32_t timer_read32(void) { return now; }
uint32_t timer_elapsed32(uint32_t start) { return now - start; }
void default_layer_set(uint32_t layer) { current_layer = layer; }
bool q3p_dip_policy(uint8_t index, bool active);
void via_custom_value_command_kb(uint8_t *data, uint8_t length);
void housekeeping_task_user(void);
static uint8_t packet[32];
static void get(void) {
    memset(packet, 0, sizeof(packet));
    packet[0] = 8; packet[2] = 0x10;
    via_custom_value_command_kb(packet, sizeof(packet));
    assert(packet[0] == 8);
}
static void set(uint8_t layer, uint8_t seconds) {
    memset(packet, 0, sizeof(packet));
    packet[0] = 7; packet[2] = 0x10;
    packet[3] = layer; packet[4] = seconds;
    via_custom_value_command_kb(packet, sizeof(packet));
}
int main(void) {
    assert(!q3p_dip_policy(0, false));
    set(6, 7); assert(packet[0] == 255);
    assert(q3p_dip_policy(0, true)); assert(current_layer == 1);
    set(6, 0); assert(packet[0] == 7 && current_layer == (1U << 6));
    get(); assert(packet[5] == 7 && packet[6] == 1);
    now = 6999; housekeeping_task_user(); assert(current_layer == (1U << 6));
    now = 7000; housekeeping_task_user(); assert(current_layer == 1);
    get(); assert(packet[6] == 0);
    set(8, 7); assert(packet[0] == 255 && current_layer == 1);
    set(7, 255); now += 70000; get(); assert(packet[5] == 185 && packet[6] == 1);
    now += 185000; get(); assert(packet[6] == 0 && current_layer == 1);
    set(6, 7); now += 2000; set(6, 7);
    now += 5000; housekeeping_task_user(); assert(current_layer == (1U << 6));
    now += 2000; housekeeping_task_user(); assert(current_layer == 1);
    set(6, 7); assert(!q3p_dip_policy(0, false));
    current_layer = 4; get(); assert(packet[3] == 0 && packet[6] == 0 && current_layer == 4);
    assert(q3p_dip_policy(0, true)); assert(current_layer == 1);
    now = UINT32_MAX - 1000; set(7, 7); now += 7000;
    housekeeping_task_user(); assert(current_layer == 1);
    uint8_t short_get[] = {8, 0, 0x10};
    via_custom_value_command_kb(short_get, sizeof(short_get)); assert(short_get[0] == 255);
    uint8_t short_set[] = {7, 0, 0x10, 6};
    via_custom_value_command_kb(short_set, sizeof(short_set)); assert(short_set[0] == 255);
    via_custom_value_command_kb(NULL, 0);
    assert(!q3p_dip_policy(1, true));
    puts("PASS：手动拒绝、8 层边界、7 秒超时、255 秒租约、heartbeat 续租、模式切换、32 位回绕、短包");
}
