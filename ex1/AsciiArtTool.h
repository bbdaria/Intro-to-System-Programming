#ifndef ASCII_ART_TOOL_H
#define ASCII_ART_TOOL_H

#include <stdio.h>
#include "RLEList.h"
#include <stdlib.h>

/**
 * implements an AsciiArtTool for using RLELists.
 *
 * The following functions are available:
 *   asciiArtRead	        - reads given file and transforms it into a compressed RLEList
 *   asciiArtPrint		    - writes a RLEList picture into file
 *   asciiArtPrintEncoded           - writes a RLEList picture into file in compressed form
 */

/**
 * asciiArtRead: reads given file and transforms it into a compressed RLEList
 *
 * @param in_stream file object containing the image we want to compress
 * @return
 *      RLEList containig all characters of the image
 */
RLEList asciiArtRead(FILE *in_stream);

/**
 * asciiArtPrint: writes a RLEList picture into file
 *
 * @param list RLEList containig all characters of the image
 * @param out_stream file object we will write the image into
 * @return
 *      RLE_LIST_NULL_ARGUMENT if one of the paramaters are NULL
 *      RLE_LIST_SUCCESS if writing succeeded
 */
RLEListResult asciiArtPrint(RLEList list, FILE *out_stream);

/**
 * asciiArtPrintEncoded: writes a RLEList picture into file in compressed form
 *
 * @param list RLEList containig all characters of the image
 * @param out_stream file object we will write the compressed image into
 * @return
 *      RLE_LIST_NULL_ARGUMENT if one of the paramaters are NULL
 *      RLE_LIST_SUCCESS if writing succeeded
 */
RLEListResult asciiArtPrintEncoded(RLEList list, FILE *out_stream);

#endif
