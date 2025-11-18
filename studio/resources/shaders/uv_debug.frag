#version 450 core

in vec2 fragUV;
out vec4 outColor;

void main() {
    outColor = vec4(fragUV.x, fragUV.y, 0.0, 1.0);
}
