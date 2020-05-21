
//----------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>

#include "line.h"

//----------------------------------------------------------
#define INSIDE (0)
#define LEFT   (1)
#define RIGHT  (2)
#define BELOW  (4)
#define ABOVE  (8)

#define EPSILON (1e-13)

int _regionCode( double x, double y, double xMin, double yMin, double xMax, double yMax );

//----------------------------------------------------------
int clipLine( Line *l, double xMin, double yMin, double xMax, double yMax )
{
  // TODO: Implement the clipLine algorithm here.
  //       Arguments:
  //           l : a pointer to the line to clip.
  //
  //           xMin, yMin, xMax, yMax
  //             : the four bounding lines of the view region to
  //               clip against.
  //
  //       Compute the codes of the two endpoints of the line l
  //         using _regionCode().
  //
  //       In an infinite while loop,
  //         Do a trivial accept if possible.
  //         Do a trivial reject if possible.
  //         If neither is possible, select one of the point
  //           codes that's not in the INSIDE region.
  //         If it's ABOVE, slide the point to the yMax line.
  //         ElIf it's BELOW, slide the point to the yMin line.
  //         ElIf it's RIGHT, slide the point to the xMax line.
  //         Elif it's LEFT, slide the point to the xMin line.
  //         Else an error occurred.  Print a message and exit.
  //         Update whichever point got slid in l.
  //         Recompute the code of that point.
  //
  // This routine should return 1 if trivial accept happens, 0 if
  // trivial reject happens.
  //
  // IMPORTANT -- IMPORTANT -- IMPORTANT -- IMPORTANT -- IMPORTANT
  //   You can look at the code in the Clipping handout as a
  //   reference, but remember that code is written in Python and
  //   this is C.
  // IMPORTANT -- IMPORTANT -- IMPORTANT -- IMPORTANT -- IMPORTANT
  // - Forty-one (41) lines of code.

    int p1Out;
    int p2Out;
    int clip = 0;
    
    p1Out = _regionCode( l->p1X, l->p1Y, xMin, yMin, xMax, yMax);
    p2Out = _regionCode( l->p2X, l->p2Y, xMin, yMin, xMax, yMax);
    
    while(1)
    {
        if((p1Out | p2Out) == INSIDE)
        {
            clip = 1;
            break;
        }
        if((p1Out & p2Out) != INSIDE)
        {
            clip = 0;
            break;
        }
        
        double x;
        double y;
        int anOutCode;
        
        if(p1Out == INSIDE)
        {
            anOutCode = p2Out;
        }
        else
        {
            anOutCode = p1Out;
        }
        
        if (anOutCode & ABOVE)
        {
            x = l->p1X + (l->p2X - l->p1X) * (yMax - l->p1Y) / (l->p2Y - l->p1Y);
            y = yMax;
        }
        else if (anOutCode & BELOW)
        {
            x = l->p1X + (l->p2X - l->p1X) * (yMin - l->p1Y) / (l->p2Y - l->p1Y);
            y = yMin;
        }
        else if (anOutCode & RIGHT)
        {
            x = xMax;
            y = l->p1Y + (l->p2Y - l->p1Y) * (xMax - l->p1X) / (l->p2X - l->p1X);
        }
        else if (anOutCode & LEFT)
        {
            x = xMin;
            y = l->p1Y + (l->p2Y - l->p1Y) * (xMin - l->p1X) / (l->p2X - l->p1X);
        }
        else
        {
            fprintf(stderr, "Code did not match any region.");
            exit(1);
        }
        
        if(anOutCode == p1Out)
        {
            l->p1X = x;
            l->p1Y = y;
            p1Out = _regionCode (l->p1X, l->p1Y, xMin, yMin,xMax, yMax);
        }
        else
        {
            l->p2X = x;
            l->p2Y = y;
            p2Out = _regionCode (l->p2X, l->p2Y, xMin, yMin,xMax, yMax);
        }
    }
    return clip;
}

//----------------------------------------------------------
void dumpLine( Line *l )
{
  // TODO: Print an 'l' line for the given Line l.
  //Two (2) lines of code.
    printf("l%9.1f%9.1f%9.1f%9.1f\n",
           l->p1X, l->p1Y, l->p2X, l->p2Y);
}

//----------------------------------------------------------
int _regionCode( double x, double y, double xMin, double yMin, double xMax, double yMax )
{
  // TODO: Implement the region code computation here.
  //       Arguments:
  //           x, y
  //             : the point whose region must be computed.
  //
  //           xMin, yMin, xMax, yMax
  //             : the four bounding lines of the view region to
  //               check against.
  //
  //       Assume the region is INSIDE to start.
  //       Check against xMin to see if the point is LEFT.
  //       Check against xMax to see if the point is RIGHT.
  //       Check against yMin to see if the point is BELOW.
  //       Check against yMax to see if the point is ABOVE.
  //
  // This routine should return the code with the proper bits set.
  // (The bits are already defined above for your use.)

  // IMPORTANT -- IMPORTANT -- IMPORTANT -- IMPORTANT -- IMPORTANT
  //   (1) Ensure that you do EPSILON comparisions when
  //       checking against xMin, yMin, xMax, yMax.  EPSILON is
  //       already defined above for your use.
  //   (2) You can look at the code in the Clipping handout as a
  //       reference, but remember that code is written in Python
  //       and this is C.
  // IMPORTANT -- IMPORTANT -- IMPORTANT -- IMPORTANT -- IMPORTANT
  // - Twelve (12) lines of code.
    
    int code = INSIDE;
    
    if((xMin - x) > EPSILON)
    {
        code = code | LEFT;
    }
    else if ((x - xMax) > EPSILON)
    {
        code = code | RIGHT;;
    }
    
    if((yMin - y) > EPSILON)
    {
        code = code | BELOW;
    }
    else if((y - yMax) > EPSILON)
    {
        code = code | ABOVE;
    }
    return code;
}

//----------------------------------------------------------
