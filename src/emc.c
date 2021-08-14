/*
 * Gemu - Nintendo Game Boy Emulator
 * Copyright (C) 2017  Alex Dempsey

 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see http://www.gnu.org/licenses/
 *
 */

#include <emscripten.h>
#include "cpu.h"

unsigned char pixels[144 * 160 * 3] = {};

EMSCRIPTEN_KEEPALIVE
int * getScreenSurfacePtr() {
	return (void *)pixels;
}

EMSCRIPTEN_KEEPALIVE
void getNextFrame()
{
	get_next_frame();
}

EMSCRIPTEN_KEEPALIVE
void startCpu()
{
	start_cpu(NULL);
}

EMSCRIPTEN_KEEPALIVE
void startDK()
{
	start_cpu("games/donkeykongland.gb");
}

EMSCRIPTEN_KEEPALIVE
void startMK2()
{
	start_cpu("games/mk2.gb");
}

EMSCRIPTEN_KEEPALIVE
void startTetris()
{
	start_cpu("games/tetris.gb");
}

EMSCRIPTEN_KEEPALIVE
void startGame(int code)
{
	switch (code) {
		case 1: start_cpu("games/donkeykongland2.gb"); break;
		case 2: start_cpu("games/donkeykongland3.gb"); break;
		case 3: start_cpu("games/supermarioland.gb"); break;
		case 4: start_cpu("games/supermarioland2.gb"); break;
		case 5: start_cpu("games/supermarioland3.gb"); break;
		case 6: start_cpu("games/kidicarus.gb"); break;
		case 7: start_cpu("games/kirby.gb"); break;
		case 8: start_cpu("games/qix.gb"); break;
		case 9: start_cpu("games/drmario.gb"); break;
		case 10: start_cpu("games/legendofzelda.gb"); break;
		case 11: start_cpu("games/legendoftheriverking.gb"); break;
		case 12: start_cpu("games/solarstriker.gb"); break;
		case 13: start_cpu("games/tmnt1.gb"); break;
		case 14: start_cpu("games/tmnt2.gb"); break;
		case 15: start_cpu("games/tmnt3.gb"); break;
		case 16: start_cpu("games/contra.gb"); break;
		case 17: start_cpu("games/catrap.gb"); break;
		case 18: start_cpu("games/megaman1.gb"); break;
		case 19: start_cpu("games/megaman2.gb"); break;
		case 20: start_cpu("games/megaman3.gb"); break;
		case 21: start_cpu("games/megaman4.gb"); break;
		case 22: start_cpu("games/megaman5.gb"); break;
		case 23: start_cpu("games/harvestmoon.gb"); break;
		case 24: start_cpu("games/lemmings1.gb"); break;
		case 25: start_cpu("games/lemmings2.gb"); break;
		case 26: start_cpu("games/batman.gb"); break;
		case 27: start_cpu("games/operationc.gb"); break;
		case 28: start_cpu("games/tetris2.gb"); break;
		case 29: start_cpu("games/tetrisattack.gb"); break;
		case 30: start_cpu("games/tetrisblast.gb"); break;
		case 31: start_cpu("games/tetrisplus.gb"); break;
		case 32: start_cpu("games/fflegend.gb"); break;
		case 33: start_cpu("games/fflegend2.gb"); break;
		case 34: start_cpu("games/fflegend3.gb"); break;
		case 35: start_cpu("games/ff4.gb"); break;
		case 36: start_cpu("games/ffadventure.gb"); break;
		case 37: start_cpu("games/castlevania2.gb"); break;
		case 38: start_cpu("games/gargoylesquest.gb"); break;
		case 39: start_cpu("games/ducktales.gb"); break;
		case 40: start_cpu("games/ducktales2.gb"); break;
		case 41: start_cpu("games/pokemonblue.gb"); break;
		case 42: start_cpu("games/pokemonred.gb"); break;
		case 43: start_cpu("games/tetris.gb"); break;
		case 44: start_cpu("games/donkeykongland.gb"); break;
		case 45: start_cpu("games/mk2.gb"); break;
		case 46: start_cpu("games/warioland2.gb"); break;
		case 47: start_cpu("games/tetrisdx.gbc"); break;
		case 48: start_cpu("games/donkeykongcountry.gbc"); break;
		case 49: start_cpu("games/crazycastle3.gbc"); break;
	}
}

int main(int argc, char** args)
{
	// start_cpu();
}