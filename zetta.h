/* Copyright 2020 TumoiYorozu
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "quantum.h"

/* This is a shortcut to help you visually see your layout.
 *
 * The first section contains all of the arguments representing the physical
 * layout of the board and position of the keys.
 *
 * The second converts the arguments into a two-dimensional array which
 * represents the switch matrix.
 */
/*
#define LAYOUT( \
    k00, k01, k02, \
      k10,  k12    \
) { \
    { k00, k01,   k02 }, \
    { k10, KC_NO, k12 }  \
}
*/


#define LAYOUT(\
    L21, L12, L13, L14, L15, L16, L17,      \
    L31, L32, L23, L24, L25, L26, L27,      \
    L41, L42, L43, L34, L35, L36,   L37,    \
    L51, L52, L53, L54, L45, L46, L47,      \
    L61, L62, L63, L64, L65, L56,   L57,    \
    L71, L72, L73, L74, L75,      L76, L67, \
\
    R12, R13, R14, R15, R16, R17, R18, R19, R71,   R81, R91, R85, R95, \
    R21, R23, R24, R25, R26, R27, R28, R29, R72,   R82, R92, R86, R96, \
    R31, R32, R34, R35, R36, R37, R38, R39,        R83, R93, R87, R97, \
    R51, R41, R42, R43, R45, R46, R47, R48, R49,   R84, R94, R89, R98, \
    R52, R53, R54, R56, R57, R58, R59, R69,     R73,    R78, R79,      \
    R61, R62, R63, R64, R65, R67,          R74, R75, R76               \
) { \
    {KC_NO,   L21,   L31,   L41,   L51,   L61,   L71, KC_NO, KC_NO},\
    {  L12, KC_NO,   L32,   L42,   L52,   L62,   L72, KC_NO, KC_NO},\
    {  L13,   L23, KC_NO,   L43,   L53,   L63,   L73, KC_NO, KC_NO},\
    {  L14,   L24,   L34, KC_NO,   L54,   L64,   L74, KC_NO, KC_NO},\
    {  L15,   L25,   L35,   L45, KC_NO,   L65,   L75, KC_NO, KC_NO},\
    {  L16,   L26,   L36,   L46,   L56, KC_NO,   L76, KC_NO, KC_NO},\
    {  L17,   L27,   L37,   L47,   L57,   L67, KC_NO, KC_NO, KC_NO},\
    {KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO},\
    {KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO},\
    \
    {KC_NO,   R21,   R31,   R41,   R51,   R61,   R71,   R81,   R91},\
    {  R12, KC_NO,   R32,   R42,   R52,   R62,   R72,   R82,   R92},\
    {  R13,   R23, KC_NO,   R43,   R53,   R63,   R73,   R83,   R93},\
    {  R14,   R24,   R34, KC_NO,   R54,   R64,   R74,   R84,   R94},\
    {  R15,   R25,   R35,   R45, KC_NO,   R65,   R75,   R85,   R95},\
    {  R16,   R26,   R36,   R46,   R56, KC_NO,   R76,   R86,   R96},\
    {  R17,   R27,   R37,   R47,   R57,   R67, KC_NO,   R87,   R97},\
    {  R18,   R28,   R38,   R48,   R58, KC_NO,   R78, KC_NO,   R98},\
    {  R19,   R29,   R39,   R49,   R59,   R69,   R79,   R89, KC_NO}\
}

#define LAYOUT_kc(\
    L21, L12, L13, L14, L15, L16, L17,      \
    L31, L32, L23, L24, L25, L26, L27,      \
    L41, L42, L43, L34, L35, L36,   L37,    \
    L51, L52, L53, L54, L45, L46, L47,      \
    L61, L62, L63, L64, L65, L56,   L57,    \
    L71, L72, L73, L74, L75,      L76, L67, \
\
    R12, R13, R14, R15, R16, R17, R18, R19, R71,   R81, R91, R85, R95,  \
    R21, R23, R24, R25, R26, R27, R28, R29, R72,   R82, R92, R86, R96,  \
    R31, R32, R34, R35, R36, R37, R38, R39,        R83, R93, R87, R97,  \
    R51, R41, R42, R43, R45, R46, R47, R48, R49,   R84, R94, R89, R98,  \
    R52, R53, R54, R56, R57, R58, R59, R69,     R73,    R78, R79,       \
    R61, R62, R63, R64, R65, R67,          R74, R75, R76                \
) { \
    {   KC_NO, KC_##L21, KC_##L31, KC_##L41, KC_##L51, KC_##L61, KC_##L71,  KC_NO,  KC_NO},\
    {KC_##L12,    KC_NO, KC_##L32, KC_##L42, KC_##L52, KC_##L62, KC_##L72,  KC_NO,  KC_NO},\
    {KC_##L13, KC_##L23,    KC_NO, KC_##L43, KC_##L53, KC_##L63, KC_##L73,  KC_NO,  KC_NO},\
    {KC_##L14, KC_##L24, KC_##L34,    KC_NO, KC_##L54, KC_##L64, KC_##L74,  KC_NO,  KC_NO},\
    {KC_##L15, KC_##L25, KC_##L35, KC_##L45,    KC_NO, KC_##L65, KC_##L75,  KC_NO,  KC_NO},\
    {KC_##L16, KC_##L26, KC_##L36, KC_##L46, KC_##L56,    KC_NO, KC_##L76,  KC_NO,  KC_NO},\
    {KC_##L17, KC_##L27, KC_##L37, KC_##L47, KC_##L57, KC_##L67,    KC_NO,  KC_NO,  KC_NO},\
    {   KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,  KC_NO,  KC_NO},\
    {   KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,  KC_NO,  KC_NO},\
    \
    {   KC_NO, KC_##R21, KC_##R31, KC_##R41, KC_##R51, KC_##R61, KC_##R71, KC_##R81, KC_##R91},\
    {KC_##R12,    KC_NO, KC_##R32, KC_##R42, KC_##R52, KC_##R62, KC_##R72, KC_##R82, KC_##R92},\
    {KC_##R13, KC_##R23,    KC_NO, KC_##R43, KC_##R53, KC_##R63, KC_##R73, KC_##R83, KC_##R93},\
    {KC_##R14, KC_##R24, KC_##R34,    KC_NO, KC_##R54, KC_##R64, KC_##R74, KC_##R84, KC_##R94},\
    {KC_##R15, KC_##R25, KC_##R35, KC_##R45,    KC_NO, KC_##R65, KC_##R75, KC_##R85, KC_##R95},\
    {KC_##R16, KC_##R26, KC_##R36, KC_##R46, KC_##R56,    KC_NO, KC_##R76, KC_##R86, KC_##R96},\
    {KC_##R17, KC_##R27, KC_##R37, KC_##R47, KC_##R57, KC_##R67,    KC_NO, KC_##R87, KC_##R97},\
    {KC_##R18, KC_##R28, KC_##R38, KC_##R48, KC_##R58,    KC_NO, KC_##R78,    KC_NO, KC_##R98},\
    {KC_##R19, KC_##R29, KC_##R39, KC_##R49, KC_##R59, KC_##R69, KC_##R79, KC_##R89,    KC_NO}\
}

