// Source code of fragment shader
#version 130

// Attributes passed from the vertex shader
in vec4 color_interp;
in vec2 uv_interp;

// Texture sampler
uniform sampler2D onetex;
uniform float doGreyscale;

void main()
{
    // Sample texture
    vec4 color = texture2D(onetex, fract(uv_interp));
    

    // Assign color to fragment
    if(doGreyscale == 1){
        float greyscale = 0.3*color.r + 0.59*color.g + 0.11*color.b;
        gl_FragColor = vec4(greyscale, greyscale, greyscale, color.a);
    } else {
        gl_FragColor = vec4(color.r, color.g, color.b, color.a);
    }
    

    // Check for transparency
    if(color.a < 1.0)
    {
         discard;
    }
}
