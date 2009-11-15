/* 
**  (C) by Remo Dentato (rdentato@gmail.com)
** 
** This sofwtare is distributed under the terms of the BSD license:
**   http://creativecommons.org/licenses/BSD/
**   http://opensource.org/licenses/bsd-license.php 
*/

#define UTL_UNITTEST
#include "libutl.h"

#include <assert.h>

char *lorem =
  "Lorem ipsum dolor sit amet, consectetuer adipiscing elit. "
  "Sed libero sapien, sollicitudin consequat, tempor ut, elementum a, "
  "diam. Morbi eu risus sed felis porta fermentum. Phasellus sit amet."
#if 0  
  "Diam in gravida. Nunc et arcu vel turpis porttitor semper. Nulla "
  "facilisi. Integer lobortis urna in dolor. Nam eu nisl ac nibh "
  "elementum molestie. Proin eros. Cras ornare aliquam risus. Nulla sem "
  "tortor, pulvinar ac, elementum et, convallis a, velit. Proin mollis "
  "elit non augue. Ut laoreet nunc. Etiam imperdiet iaculis nibh. Donec "
  "nec eros. Vivamus posuere gravida sapien. Donec eros. Praesent ut "
  "pede at leo laoreet consequat. Sed in sem. "
#endif
   ;

char *uppers = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

int main(void)
{
  int k;
  chs_t buf;
  FILE *f;
  size_t pos;
  char ln[256];
 
  TSTTITLE("");
  
  TSTSECTION("char strings (chs)");
  
  TSTGROUP("Create/Destroy");
 
  buf = chsNew();
  TST("String created", buf != NULL);
  TST("Size is one slot", chsSize(buf) == chsBlksz);
  TST("Length is 0", chsLen(buf) == 0);
  TST("Cur is 0", chsCur(buf) == 0);
  
  buf = chsCpy(buf, uppers);
  k = chsLen(buf);
  TST("CHS length is correct", k == strlen(uppers));
  TSTWRITE("# Next two lines should be identical\n");
  TSTWRITE("# >>  \"%s\"\n",uppers);
  TSTWRITE("# >>  \"%s\"\n",buf);

  buf=chsDel(buf,'M'-'A','P'-'A');
  TST("Deleted \"MNOP\". Length is correct", k == chsLen(buf)+4);
  TSTWRITE("# Check that next lines don't have MNOP in it!\n");
  TSTWRITE("# >>  \"%s\"\n",buf);

  k = chsGetChr(buf);
  TST("Cur char is Q", k == 'Q');
  
  k = chsChrAt(buf,'L'-'A');
  TST("ChrAt in the middle", k == 'L');
  k = chsGetChr(buf);
  TST("GetChr after ChrAt ", k == 'Q');

  k = chsLen(buf);
  buf=chsDel(buf, -4, -1);
  TST("Deleted \"WXYZ\" at the end. Length is correct", k == chsLen(buf)+4);
  TSTWRITE("# >>  \"%s\"\n",buf);

  k = chsLen(buf);
  buf = chsDel(buf, 0, 3);
  TST("Deleted \"ABCD\" at the beginning. Length is correct", k == chsLen(buf)+4);
  TSTWRITE("# >> [%d]  \"%s\"\n",chsLen(buf),buf);

  k = chsLen(buf);
  buf = chsInsStr(buf, chsLen(buf), "ABCD");
  TST("Added \"ABCD\" at the end. Length is correct", k == chsLen(buf)-4);
  TSTWRITE("# >> [%d]  \"%s\"\n",chsLen(buf),buf);
  
  k = chsLen(buf);
  buf = chsInsStr(buf, 0, "WXYZ");
  TST("Added \"WXYZ\" at the beginning. Length is correct", k == chsLen(buf)-4);
  TSTWRITE("# >> [%d]  \"%s\"\n",chsLen(buf),buf);
  
  k = chsLen(buf);
  buf = chsInsStr(buf,'M'-'A', "MNOP");
  TST("Added \"MNOP\" in the middle. Length is correct", k == chsLen(buf)-4);
  TSTWRITE("# >> [%d]  \"%s\"\n",chsLen(buf),buf);
  
  buf = chsSubStr(buf ,0, "<=EGIKM>","++");
  TST("Replacing <=EGIKM> with '++'",1);
  TSTWRITE("# >>  \"%s\"\n",buf);

  buf = chsSubStr(buf ,0, "<=PQRS>","[&0&0]");
  TST("Replacing <=PQRS> with '[&0&0]'", 4);
  TSTWRITE("# >>  \"%s\"\n",buf);
  
  buf = chsFree(buf);
  TST("CHS destroyed", buf == NULL);
  
  TSTGROUP("Basic Read/writing into a CHS");
  f = fopen("txt.txt","r");
  if (f) {
    TST("File reading", 1);
    buf = chsRead(buf,f,'r');
    TSTWRITE("[[**************************\n");
    TSTWRITE("%s\n",buf);
    TSTWRITE("]]**************************\n");
    fclose(f);
  }
  else  TST("File reading", 0);
  
  f = fopen("txt.txt","r");
  if (f) {
    TST("File reading", 1);
    buf = chsRead(buf,f,'a');
    TSTWRITE("[[**************************\n");
    TSTWRITE("%s\n",buf);
    TSTWRITE("]]**************************\n");
    fclose(f);
  }
  else  TST("File reading", 0);
  
 #if 10
  f = fopen("txt.txt","r");
  if (f) {
    TST("File reading", 1);
    TSTWRITE("[[**************************\n");
    do {
      buf = chsReadln(buf,f,'w');
      if (chsLen(buf) == 0) break;
      TSTWRITE("{%s}",buf);
    } while (1);
    TSTWRITE("]]**************************\n");
    fclose(f);
  } 
  else  TST("File reading", 0);
  #endif
  buf = chsFree(buf);
  assert(buf == NULL);  
  
  TSTDONE();
  
  return 0;
}