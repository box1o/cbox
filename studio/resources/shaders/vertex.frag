#version 450 core
in vec2 fragUV;
out vec4 outColor;

uniform sampler2D uTex;

void main() {
    // outColor = texture(uTex, fragUV);
    outColor = vec4(fragUV, 0.0, 1.0); // Should show gradient
}
