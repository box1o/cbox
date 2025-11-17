#version 450 core

layout(location = 0) in vec3 v_Normal;
layout(location = 1) in vec2 v_TexCoord;

layout(binding = 0) uniform sampler2D u_Texture;

layout(location = 0) out vec4 FragColor;

void main() {
    vec3 light_dir = normalize(vec3(0.5, 1.0, 0.3));
    float diffuse = max(dot(normalize(v_Normal), light_dir), 0.0) * 0.8 + 0.2;
    
    vec4 tex_color = texture(u_Texture, v_TexCoord);
    FragColor = vec4(tex_color.rgb * diffuse, tex_color.a);
}
