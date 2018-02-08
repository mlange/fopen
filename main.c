#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#ifdef UNIX
# include <unistd.h>
#else
# include <windows.h>
# include <wtypes.h>
# include <winnls.h>
# include <mbctype.h>
#endif


static void DumpCodePoints(unsigned char *ptr)
{
    printf("Code Points: ");

    while (*ptr)
        printf("%x ", *ptr++);

    printf("\n"); 
}

static wchar_t *Convert(char *inBuf)
{
    int wideSize;

    wchar_t *wideBuf;

    wideSize = 2 * (strlen(inBuf) + 1);

    /* Allocate space for the wide-character string. */
    wideBuf = (wchar_t *) malloc(wideSize);
    if (!wideBuf)
        return NULL;

    /* Convert the given string to a wide-character string. */
    wideSize = MultiByteToWideChar(CP_UTF8, 0, inBuf, -1, wideBuf, wideSize);
    if (!wideSize)
        return NULL;

    return wideBuf;
}

int main(int argc, char *argv[])
{
    unsigned char temp[] = { 0xC2, 0xA2, 0xC2, 0xA2, 0xC2, 0xA2, 0x0 };
    unsigned char *filename;
    wchar_t *wfilename;
    FILE *outfile;

    filename = argv[1] ? argv[1] : temp;

    DumpCodePoints(filename);

    printf("Filename: %s\n", filename);

    wfilename = Convert(filename);
    
    /* Open the file. */
    outfile = _wfopen(wfilename, L"a+");
    if (!outfile)
    {
	perror("_wfopen");
	exit(1);
    }

    /* Write to the file. */
    fprintf(outfile, "Hello, world\n");

    /* Close the file. */
    fclose(outfile);

    exit(0);
}

