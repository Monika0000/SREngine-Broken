#version 330 core
out vec4 FragColor;
in vec4 gl_FragCoord;

void main(void) {  
    FragColor = vec4(
        gl_FragCoord.x / 1600.0, 
        gl_FragCoord.y / 1600.0, 
        gl_FragCoord.z / 1600.0, 
    1);
}
