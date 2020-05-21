
//----------------------------------------------------------
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "view.h"

//----------------------------------------------------------
View *allocView()
{
  // TODO: Allocate a View structure.
  //       Return a pointer to the View structure as the
  //         result of this function.
  // (You can use calloc() to allocate zeroed out memory.)
    View *v;
    v = (View *) calloc (1,sizeof(View));
    if(v == NULL)
    {
        fprintf(stderr,"Unable to allocate View");
        exit(1);
    }
    return v;
}

//----------------------------------------------------------
void dumpView( View *view )
{
  printf( "#- View parameters ---------------------\n" );
  printf( "# Bézier resolution : %5d\n",
    view->m_resolution );

  printf( "# Euler angles      : %13.6f, %13.6f, %13.6f\n",
    view->m_phi, view->m_theta, view->m_psi );

  printf( "# World limits      : %13.6f, %13.6f, %13.6f, %13.6f\n",
    view->m_worldXMin, view->m_worldYMin,
    view->m_worldXMax, view->m_worldYMax );

  printf( "# Camera distance   : %13.6f\n", view->m_cameraDistance );

  printf( "# Canvas width      : %5d\n", view->m_width );
  printf( "# Canvas height     : %5d\n", view->m_height );

  printf( "# Screen limits     : %13.6f, %13.6f, %13.6f, %13.6f\n",
    view->m_viewportXMin, view->m_viewportYMin,
    view->m_viewportXMax, view->m_viewportYMax );

  printf( "# Portal pixels     : %4d, %4d, %4d, %4d\n",
    view->m_portalXMin, view->m_portalXMax,
    view->m_portalYMin, view->m_portalYMax );

  printf( "#---------------------------------------\n" );
}

//----------------------------------------------------------
void freeView( View *view )
{
  // TODO: Free the view structure.
    if (view != NULL)
    {
        free(view);
    }
}

//----------------------------------------------------------
View *loadView( char *fName )
{
  // TODO: Open the file indicated by fName.
  //       Allocate a View structure.
  //       Process each 'c', 'd', 's', and 'w' line.
  //       Compute the xMin, xMax, yMin, and yMax of the viewport
  //         and save the values in the m_portal... slots.
  //       Return a pointer to the allocated View structure as
  //         the value of this function.
  //
  // This code is very similar to what was done in loadModel().
  // The primary difference is that a view file has different
  // kinds of lines in it and therefore the line processing is
  // different.
    FILE *fp = fopen( fName, "r" );
    if ( fp == NULL )
    {
      fprintf( stderr, "loadView: Unable to open '%s' for read.\n", fName );
      exit( 1 );
    }
    
    View *v = allocView();
    char *line = NULL;
    size_t len = 0;
    size_t read;
    int count;
    
    while ( ( read = getline( &line, &len, fp ) ) != -1 )
    {
        count++;
        char *ptr = line;
        while ( *ptr && isspace( *ptr ) ) ptr++;
        
        if ( *ptr == 'c' )
        {
          int conv = sscanf( ptr+1, "%d %d", &v->m_width, &v->m_height );
            
          if ( conv != 2 )
          {
            fprintf( stderr, "loadView: Unable to load canvas size at line %d.", count);
          }

          continue;
        }
        
        if ( *ptr == 'd' )
        {
            int conv = sscanf( ptr+1, "%lf", &v->m_cameraDistance );

            if ( conv != 1 )
            {
              fprintf( stderr, "loadView: Unable to load camera distance at line %d.", count );
            }

            continue;
        }

        if ( *ptr == 'e' )
        {
            int conv = sscanf( ptr+1, "%lf %lf %lf",&v->m_phi, &v->m_theta, &v->m_psi );

            if ( conv != 3 )
            {
              fprintf( stderr, "loadView: Unable to load Euler angles at line %d.", count );
            }

            continue;
        }

        if ( *ptr == 's' )
        {
            int conv = sscanf( ptr+1, "%lf %lf %lf %lf", &v->m_viewportXMin, &v->m_viewportYMin, &v->m_viewportXMax, &v->m_viewportYMax );

            if ( conv != 4 )
            {
              fprintf( stderr, "loadView: Unable to load viewport limits at line %d.", count );
            }

            continue;
        }

        if ( *ptr == 'w' )
        {
            int conv = sscanf( ptr+1, "%lf %lf %lf %lf",&v->m_worldXMin, &v->m_worldYMin,&v->m_worldXMax, &v->m_worldYMax );

            if ( conv != 4 )
            {
              fprintf( stderr, "loadView: Unable to load world limits at line %d.", count );
            }

            continue;
        }
    }
    
    v->m_portalXMin = v->m_width * v->m_viewportXMin;
    v->m_portalYMin = v->m_height * v->m_viewportYMin;
    v->m_portalXMax = v->m_width * v->m_viewportXMax;
    v->m_portalYMax = v->m_height * v->m_viewportYMax;
    
    if ( line != NULL )
    {
      free( line );
    }

    fclose( fp );
    
    return v;
}

//----------------------------------------------------------

