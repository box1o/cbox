#version 330 core

in vec3 v_Normal;
in vec2 v_TexCoord;

uniform sampler2D u_Texture;

out vec4 FragColor;

void main() {
    vec3 light_dir = normalize(vec3(0.5, 1.0, 0.3));
    float diffuse = max(dot(normalize(v_Normal), light_dir), 0.0) * 0.8 + 0.2;
    
    vec4 tex_color = texture(u_Texture, v_TexCoord);
    FragColor = vec4(tex_color.rgb * diffuse, tex_color.a);
}
