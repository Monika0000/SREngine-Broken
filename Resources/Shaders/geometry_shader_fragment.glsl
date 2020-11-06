#version 330 core
out vec4 FragColor;
in vec4 gl_FragCoord;

in VS_OUT {
    vec3 FragPos;
    vec2 TexCoord0;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
    vec3 Normal0;
} fs_in;

uniform sampler2D DiffuseMap;
uniform sampler2D NormalMap;
uniform sampler2D SpecularMap;

void main(void) {  
    vec3 ambient = texture(DiffuseMap, fs_in.TexCoord0).rgb;
    float alpha = texture(DiffuseMap, fs_in.TexCoord0).a;

    FragColor = vec4(ambient, alpha);
   // FragColor = vec4(1.0, 0.5, 0.2, 1.0);
    //FragColor = vec4(
    //    gl_FragCoord.x / 1600.0, 
    //    gl_FragCoord.y / 1600.0, 
    //    gl_FragCoord.z / 1600.0, 
    //1);
}
