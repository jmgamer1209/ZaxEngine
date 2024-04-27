#version 330 core

//KEYWORD:SHADOWCUBE

#ifdef SHADOWCUBE

struct Light
{
   vec3 position;
   float range;
};
uniform Light light;

#endif

in vec4 fragPos;

void main()
{

#ifdef SHADOWCUBE

   float shadowLinearDepth = distance(vec3(fragPos),light.position); 
   gl_FragDepth = shadowLinearDepth / light.range;
   
#else 
   // {
   //    gl_FragDepth = gl_FragCoord.z;
   // }
#endif
}