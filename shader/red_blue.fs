varying vec2 textureCoordinate;
varying vec2 textureCoordinate2;

uniform sampler2D inputImageTexture;
uniform sampler2D inputImageTextureLookup;
uniform sampler2D inputImageTextureLast;

const vec3 blend = vec3(0.05, 0.2, 0.5);

void main() {
    vec4 textureColor = texture2D(inputImageTexture, textureCoordinate);
    
    textureColor = clamp(textureColor, 0.0, 1.0);
    
    float blueColor = textureColor.b * 63.0;
    
    vec2 quad1;
    quad1.y = floor(floor(blueColor) / 8.0);
    quad1.x = floor(blueColor) - (quad1.y * 8.0);
    
    vec2 quad2;
    quad2.y = floor(ceil(blueColor) / 8.0);
    quad2.x = ceil(blueColor) - (quad2.y * 8.0);
    
    vec2 texPos1;
    texPos1.x = (quad1.x * 0.125) + 0.5 / 512.0 + ((0.125 - 1.0 / 512.0) * textureColor.r);
    texPos1.y = (quad1.y * 0.125) + 0.5 / 512.0 + ((0.125 - 1.0 / 512.0) * textureColor.g);
    
    vec2 texPos2;
    texPos2.x = (quad2.x * 0.125) + 0.5 / 512.0 + ((0.125 - 1.0 / 512.0) * textureColor.r);
    texPos2.y = (quad2.y * 0.125) + 0.5 / 512.0 + ((0.125 - 1.0 / 512.0) * textureColor.g);
    
    vec4 newColor1 = texture2D(inputImageTextureLookup, texPos1);
    vec4 newColor2 = texture2D(inputImageTextureLookup, texPos2);
    
    vec4 lookupColor = mix(newColor1, newColor2, fract(blueColor));
    
    vec4 textureColorLast = texture2D(inputImageTextureLast, textureCoordinate);
    gl_FragColor = vec4(mix(textureColorLast.rgb, lookupColor.rgb, blend), textureColor.w);
}

