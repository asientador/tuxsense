#include <dlfcn.h>
#include <link.h> //libgcc

#include "sdk.h"
#include "memory.h"

//glibc - https://codebrowser.dev/glibc/glibc/include/link.h.html

struct LinkMap{
    ELFW(Addr)         l_addr;
    ELFW(Dyn)        * l_ld;
    struct LinkMap   * l_next, * l_prev;
    const ElfW(Phdr) * phdr; /* Pointer to program header table in core.
 *                                This is pointing to an Array of LinkMap structs  */

};

static void * find(const char * moduleName, const char * pattern)
{
    LinkMap * linkMap = dlopen(moduleName,RTLD_NOLOAD | RTLD_NOW);

    if(!linkMap)
        return 0;



    char * start = (char * ) linkMap->l_addr;
    char * end = start + linkMap->phdr[0].p_memsz;

    dlclose(linkMap);

    const char * iterator = start;
    const char * patternIterator = pattern;

    while(iterator < end && *patternIterator)
    {
        if(*iterator == *patternIterator || *patternIterator == '?')
        {
            iterator++;
            patternIterator++;
        }
        else
        {
            iterator = ++start;
            patternIterator = pattern;
        }
    }

    if (!*patternIterator)
        return start;

    return 0;
}