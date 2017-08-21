//adapted from shaders example

varying vec3 floorColor1;
varying vec3 floorColor2;
varying vec3 floorColor3;

//  Model coordinates and light from vertex shader
varying float LightIntensity;
varying vec2  ModelPos;

void main()
{
   vec2 position = ModelPos;
   for (float i = 0.0; i <= 1.0; i = i + 0.01)
   {
   if ((position.y)>(-0.5+i) && (position.y)<=(0.1))
     {
     vec3 floorColor1  = vec3(0,0.0+i,1.0-i);
     vec3 color  = mix(floorColor1, floorColor1, floorColor1);
     color *= LightIntensity;
     gl_FragColor = vec4(color,1.0);
     }
   if ((position.y)>(0.0+i) && (position.y)<=(0.7))
    {
     vec3 floorColor2  = vec3(0.0+i,0.5+i,0);
     vec3 color  = mix(floorColor2, floorColor2, floorColor2);
     color *= LightIntensity;
     gl_FragColor = vec4(color,1.0);
     }
   if ((position.y)>(0.7+i) && (position.y)<=(1.7))
     {
     vec3 floorColor3  = vec3(0.5+i,0.6-i,0.0);
     vec3 color  = mix(floorColor3, floorColor3, floorColor3);
     color *= LightIntensity;
     gl_FragColor = vec4(color,1.0);
     }
   }
}
