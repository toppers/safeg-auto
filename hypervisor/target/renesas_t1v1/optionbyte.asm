;/*
 ;*  オプションバイト用のセクション
 ;*/
#if 0
    .section ".csa.rodata", const
    .align 4
    .dw 0x00000000 ;/* Reset Vector PE0 */
    .dw 0x00000000 ;/* Reset Vector PE1 */
    .dw 0x00800000 ;/* Reset Vector PE2 */
    .dw 0x00800000 ;/* Reset Vector PE3 */
    .dw 0xFFFFFFFF 
    .dw 0xFFFFFFFF 
    .dw 0xFFFFFFFF 
    .dw 0xFFFFFFFF
    .dw 0x3FF30010 ;/* OPBT0 */
    .dw 0xF0FB0000 ;/* OPBT1 */ 
    .dw 0x7FFFFFFF ;/* OPBT2 */
    .dw 0xF1FFFEFC ;/* OPBT3 Normal Operating Mode,  Shipping : 0xF1FFFEFE */
    .dw 0x0C0C0C0F ;/* OPBT4 */ 
    .dw 0xFFFFFFFF
    .dw 0xFFFF0FC3 ;/* OPBT6 */  
    .dw 0xFFFFFFFF ;/* OPBT7 */  
    .dw 0xFFFFFFFE ;/* OPBT8 */ 
    .dw 0xFFF1FFFF ;/* OPBT9 */
    .dw 0xF9FD288E ;/* OPBT10 MainOSC 20MHz/ Shipping : 0xFBFD288E */ 
    .dw 0xFFFFFFFF ;/* OPBT11 */
    .dw 0xFFFFFFFD ;/* OPBT12 */ 
    .dw 0xFFFFFFFF ;/* OPBT13 */ 
    .dw 0x00000002 ;/* OPBT14 */ ;/* 0192 */
    .dw 0xFFFFFFFF
    .dw 0xFFFFFF12 ;/* OPBT16 0x6FFFFFFF */ 
    .dw 0xF97DFCFC ;/* OPBT17 0xFFFFFFFF */  
    .dw 0xC736C561 ;/* OPBT18 0xFFFFFFFF */  
    .dw 0xFFFFC5FE ;/* OPBT19 0xFFFFFFFF */ 
    .dw 0x6088FFED ;/* OPBT20 0xF088FF00 */  
    .dw 0x068203CF ;/* OPBT21 0x000000CC */
    .dw 0xF8C9FA9E ;/* OPBT22 0xC000C000 */ 
    .dw 0xFFFFFA01 ;/* OPBT23 0xFFFFC000 */
$endif

#if 0
;/*
 ;* Y-RH850-U2A-516PIN-PB-T1-V1用
 ;*/
    .section ".csa.rodata", const
    .align 4
    .dw 0x00000000 ;/* Reset Vector PE0 */
    .dw 0x00000000 ;/* Reset Vector PE1 */
    .dw 0x00800000 ;/* Reset Vector PE2 */
    .dw 0x00800000 ;/* Reset Vector PE3 */
    .dw 0xFFFFFFFF 
    .dw 0xFFFFFFFF 
    .dw 0xFFFFFFFF 
    .dw 0xFFFFFFFF
    .dw 0x3FF30010 ;/* OPBT0 */
    .dw 0xF0FB0000 ;/* OPBT1 */ 
    .dw 0x7FFFFFFF ;/* OPBT2 */
    .dw 0xF1FFFEFC ;/* OPBT3 Normal Operating Mode,  Shipping : 0xF1FFFEFE */
    .dw 0x0C0C0C0F ;/* OPBT4 */ 
    .dw 0xFFFFFFFF
    .dw 0xFFFF0FC3 ;/* OPBT6 */  
    .dw 0xFFFFFFFF ;/* OPBT7 */  
    .dw 0xFFFFFFFE ;/* OPBT8 */ 
    .dw 0xFFF1FFFF ;/* OPBT9 */
    .dw 0xFBFD288E ;/* OPBT10 */ 
    .dw 0xFFFFFFFF ;/* OPBT11 */
    .dw 0xFFFFFFFD ;/* OPBT12 */ 
    .dw 0xFFFFFFFF ;/* OPBT13 */ 
    .dw 0x00000002 ;/* OPBT14 */
    .dw 0xFFFFFFFF
    .dw 0x6FFFFFFF ;/* OPBT16  */ 
    .dw 0xFFFFFFFF ;/* OPBT17  */  
    .dw 0xFFFFFFFF ;/* OPBT18  */  
    .dw 0xFFFFFFFF ;/* OPBT19  */ 
    .dw 0xF088FF00 ;/* OPBT20  */  
    .dw 0x000000CC ;/* OPBT21  */
    .dw 0xC000C000 ;/* OPBT22  */ 
    .dw 0xFFFFC000 ;/* OPBT23  */
$endif
