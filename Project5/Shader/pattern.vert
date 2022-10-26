#version 120


uniform float	uTime;		// "Time", from Animate( )
varying vec2  	vST;		// texture coords

const float PI = 	3.14159265;
const float AMP = 	0.2;		// amplitude
const float W = 	2.;		// frequency

void
main( )
{
    vST = gl_MultiTexCoord0.st;
	vec3 vert = gl_Vertex.xyz;
    vert.x *= (sin(uTime));
    vert.y *= (sin(uTime));
    vert.z *= (sin(uTime));
	gl_Position = gl_ModelViewProjectionMatrix * vec4( vert, 1. );
}
