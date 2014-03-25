/*
 * test2.cc
 *
 *  Created on: Sep 16, 2013
 *      Author: keevi7c3l
 */

#include <avr/io.h>
#include <util/delay.h>

void InitGpio(void)
{
/*端口映射表(Port map) */
// GPIO0 - PE5
// GPIO1 - PE4
// GPIO2 - PE7
// GPIO3 - PE6
// 将 GPIO0,GPIO1 的方向设置为输入
// (Set general digital I/O port 0 and port 1 to input mode)
DDRE &= ~(_BV(PE5) | _BV(PE4));
// 将 GPIO0,GPIO1 的状态设置为上拉
// (Set Set general digital I/O port 0 and pull-up state)
PORTE |= (_BV(PE5) | _BV(PE4));
}

int main(void)
{
while (1)
{
;
}
}


