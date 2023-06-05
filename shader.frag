#version 330 core
layout(location = 0) out vec4 color;
uniform float time;

void main() {
    vec2 coord = gl_FragCoord.xy;
    vec2 res = vec2(1920, 1080);
    vec2 uv = (coord * 2.0 - res) / res.y;

    vec3 col = vec3(1.0, 0.0, 0.0);

    float d = length(uv);

    d = sin(d*8.0 + time)/8.0;
    d = abs(d);

    d = 0.02 / d;

    col *= d;

    color = vec4(col, 1.0);
}
