// 来源 https://www.jianshu.com/p/6e0b612ce6ba
varying vec2 textureCoordinate;
uniform sampler2D inputImageTexture;
uniform sampler2D maskTexture;
uniform float strength;
void main() {
    vec4 inputImageColor = texture2D(inputImageTexture, textureCoordinate);
    vec4 maskColor = texture2D(maskTexture, textureCoordinate);
    
//    gl_FragColor = vec4(vec3(1. * inputImageColor * maskColor), 1.0);
//    gl_FragColor = vec4(vec3(maskColor - inputImageColor), 1.0);
//    gl_FragColor = vec4(vec3(inputImageColor / maskColor), 1.0);
    float alpha = 0.7;
//    gl_FragColor = vec4(vec3(inputImageColor * (1.0 - alpha) + maskColor * alpha), 1.0);
    
    // 图像叠加
    // 如果x < 1.0 - strength 为 0 全透明 否则为1.0全不透明,显示最后一个
//    float mask = (textureCoordinate.x < 1.0 - strength) ? 0.0 : 1.0;
//    gl_FragColor = mix(inputImageColor, maskColor, mask);
    
    gl_FragColor = vec4(1.0 - maskColor.rgb, maskColor.a);
}
