#ifndef MODE7_CONSTANTS_H_
# define MODE7_CONSTANTS_H_            202506L

# define MODE7_WIDTH                   40   /* rows are 40 characters wide */
# define MODE7_TEXTROWS                24   /* text rows are 1..24 (24 is FastText) */
# define MODE7_PACKETSIZE              45   /* packets are 45 bytes */
# define MODE7_MAGAZINES               8    /* magazines are 1..8 (stored as 0..7) */
# define MODE7_PAGES                   0xff /* pages are 00 .. ff */
# define MODE7_FRAMES                  255  /* frames are 0 .. 255 */
# define MODE7_RUNIN                   0x55 /* sync pattern (bytes 0, 1)*/
# define MODE7_FRAMINGCODE             0x27 /* sync pattern (byte 2) */
# define MODE7_HEADER                  32   /* width of the header line */

/* Packet IDs */
# define MODE7_PKT_HEADER              0    /* Packet 0 is page header */
# define MODE7_PKT_TEXT1               1    /* Packet 1 is text row 1 */
# define MODE7_PKT_TEXTMAX             MODE7_TEXTROWS /* Packet 24 is text row 24 */
# define MODE7_PKT_UNUSED              25   /* ??? check a spec newer than 1974 */
# define MODE7_PKT_PAGE                26   /* ??? */
# define MODE7_PKT_LINK                27   /* Page linking and navigation */
# define MODE7_PKT_PAGE_ENH            28   /* Page-level enhancement data */
# define MODE7_PKT_MAG_ENH             29   /* Magazine-level enhancement data */
# define MODE7_PKT_BSDP                30   /* Broadcast Service Data Packet */
# define MODE7_PKT_MAX                 31   /* at most MODE7_PKT_MAX+1 packets per page */

/* Byte offsets within packets */

# define MODE7_BYTE_SYNC1              0    /* first synchronisation byte (start of T45 packet) */
# define MODE7_BYTE_SYNC2              1    /* second synchronisation byte */
# define MODE7_BYTE_SYNC3              2    /* third synchronisation byte */
# define MODE7_BYTE_ADDR1              3    /* first address byte (start of T42 packet) */
# define MODE7_BYZE_ADDR2              4    /* second address byte */
# define MODE7_BYTE_TEXT               5    /* displayable text within rows 1-24 */

# define MODE7_BYTE_HDRTEXT            13   /* start of displayable text in the header row */
# define MODE7_BYTE_MAX                (MODE7_PACKETSIZE - 1) /* index of last byte in backet (44) */

/* Language/Nation IDs */
# define MODE7_LANG_ENG                0    /* English */
# define MODE7_LANG_DEU                1    /* German */
# define MODE7_LANG_SVE                2    /* Swedish */
# define MODE7_LANG_ITA                3    /* Italian */
# define MODE7_LANG_FRA                4    /* French */
# define MODE7_LANG_ESP                5    /* Spanish */
# define MODE7_LANG_CZE                6    /* Czech */

#endif /*!MODE7_CONSTANTS_H_*/
