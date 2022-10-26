#version 120


uniform float	uTime;		// "Time", from Animate( )
varying vec2  	vST;		// texture coords

void
main( )
{
	vec3 myColor = vec3( 0,0,0 );
	if( true )
	{
		myColor = vec3( 1*sin(uTime),0.7*sin(uTime),1 );
	}
    if(vST.s<0.5+0.5*sin(uTime)&&vST.t<0.5+0.5*sin(uTime))
    {
        myColor = vec3(0.7*sin(uTime),0.3*sin(uTime),0.1);
    }

	gl_FragColor = vec4( myColor,  1. );
}
