mapgraph: mapgraph.r display.r mapscreen.r dirgraph.r bitmap.r
  cc -f=mapgraph mapgraph.r mapscreen.r display.r dirgraph.r bitmap.r

toast: toast.r display.r asmutil.r mapscreen.r dirgraph.r
  cc -t -f=toast mapscreen.r toast.r display.r asmutil.r dirgraph.r

dirgraph.r: dirgraph.c
  cc -r dirgraph.c

mapscreen.r: mapscreen.c
  cc -r mapscreen.c

mapgraph.r: mapgraph.c
  cc -r -A mapgraph.c

test.r: test.c
  cc -r test.c

toast.r: toast.c screen.h display.h
  cc -s -r -A toast.c

display.r: display.c display.h
  cc -r -A display.c

asmutil.r: asmutil.c
  cc -r -s asmutil.c
