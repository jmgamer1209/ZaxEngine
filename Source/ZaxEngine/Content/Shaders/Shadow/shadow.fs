#version 330 core

struct Light
{
    int type;
    float near;
    float far;
};
uniform Light light;

void main()
{

   if (light.type == 1)
   {
      float shadowDepthInNDC = gl_FragCoord.z * 2.0 - 1.0;
      float shadowLinearDepth = (2.0 * light.near * light.far) / (light.far + light.near - shadowDepthInNDC * (light.far - light.near)); 
      float depth = shadowLinearDepth / light.far;
      gl_FragDepth = depth;
   }
   else 
   {
      gl_FragDepth = gl_FragCoord.z;
   }
     // float shadowDepthInNDC = shadowDepth * 2.0 - 1.0;

        // if (light.type == 0) 
        // {
        //     shadowDepth = 0.5 * (shadowDepthInNDC * (light.far - light.near) + (light.far + light.near));
        // }
        // else if (light.type == 2) 
        // {
        //     shadowDepth = (2.0 * light.near * light.far) / (light.far + light.near - shadowDepthInNDC * (light.far - light.near)); 
        // }
   //FragColor = vec4(vec3(gl_FragCoord.z), 1.0);
}