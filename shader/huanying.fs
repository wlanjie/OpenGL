// 来源 https://www.jianshu.com/p/6e0b612ce6ba
varying vec2 textureCoordinate;
varying vec2 textureCoordinate2;
uniform sampler2D inputImageTexture;
uniform sampler2D inputImageTexture2;
vec3 blend = vec3(0.05, 0.2, 0.5);
void main() {
    vec4 textureColor = texture2D(inputImageTexture, textureCoordinate);
    vec4 textureColor2 = texture2D(inputImageTexture2, textureCoordinate);
    gl_FragColor = vec4(mix(textureColor2.rgb, textureColor.rgb, blend), 0.5);
}
