#version 120
#extension GL_ARB_texture_rectangle : enable

/*
 * normals calc
 * code by
 * Inigo Quilez
 * http://www.iquilezles.org/www/articles/normals/normals.htm
 *
 * in: 
 *  3d points in metric space
 * out: 
 *  normals
 */

uniform sampler2DRect mesh_3d;

const int kernel = 1;

void main( void ) 
{
  vec2 p2 = gl_TexCoord[0].xy;
  vec3 p3 = texture2DRect( mesh_3d, p2 ).xyz;

  vec3 normal = vec3(0.);

  for ( int i = 1; i <= kernel; i++ )
  {
    /* south */
    vec3 s = vec3(texture2DRect(mesh_3d,vec2( p2.x, p2.y + i )));

    /* north */
    vec3 n = vec3(texture2DRect(mesh_3d,vec2( p2.x, p2.y - i )));

    /* east */
    vec3 e = vec3(texture2DRect(mesh_3d,vec2( p2.x + i, p2.y )));

    /* west */
    vec3 w = vec3(texture2DRect(mesh_3d,vec2( p2.x - i, p2.y ))); 

    /* edges */
    vec3 xe = p3 - e;
    vec3 xw = p3 - w;

    /* faces normals */
    vec3 xen = cross( xe, n - e );
    vec3 xws = cross( xw, s - w );
    vec3 xes = cross( xe, e - s );
    vec3 xwn = cross( xw, w - n );

    normal += ( xen + xws + xes + xwn );
  }

  normal = normalize( normal );

  gl_FragColor = vec4( normal, 1. );
}

