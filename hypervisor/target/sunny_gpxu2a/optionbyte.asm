;/*
 ;*  SVR未使用
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
    .dw 0xFDFFFFFE ;/* OPBT8 ETNB0 MII / Shipping : 0xFFFFFFFE */ 
    .dw 0xFFF1FFFF ;/* OPBT9 */
    .dw 0xF9FDA88E ;/* OPBT10 MOSC_FREQ 20Mhz(B->9), MOSC_CAP_SEL (2->A) Shipping : 0xFBFD288E */ 
    .dw 0xEFFFFFFF ;/* OPBT11 STARTUPPLL 1  Shipping : 0xFFFFFFFF */
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
