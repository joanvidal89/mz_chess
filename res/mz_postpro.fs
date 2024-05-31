#version 330

in vec2 fragTexCoord;
uniform sampler2D texture0;
uniform vec2 screenSize;
out vec4 finalColor;

//------------------------------------------------------------------------------------
// SCREEN DISTORTION
//------------------------------------------------------------------------------------
const vec2 warpAmount = vec2(1.0 / 48.0, 1.0 / 32.0); // 1.0 / 32.0, 1.0 / 24.0
vec2 warp(vec2 pos) {
    pos = pos * 2.0 - 1.0;
    pos *= vec2(1.0 + (pos.y * pos.y) * warpAmount.x, 1.0 + (pos.x * pos.x) * warpAmount.y);
    return pos * 0.5 + 0.5;
}

//------------------------------------------------------------------------------------
// CLEAN EDGES
//------------------------------------------------------------------------------------
float clean(vec2 uv) {
    float outerClean = step(0.0, uv.x) * step(0.0, uv.y);
    outerClean *= 1.0 - step(1.0, uv.x) * 1.0 - step(1.0, uv.y);

    return outerClean;
}

//------------------------------------------------------------------------------------
// VIGNETTE
//------------------------------------------------------------------------------------
float vignette(vec2 uv) {
    uv *= 1.0 - uv.yx;
    float vig = uv.x * uv.y * 15.0; //15.0
    vig = pow(vig, 0.25); //0.25
    return vig;
}

//------------------------------------------------------------------------------------
// CHROMATIC ABERRATION
//------------------------------------------------------------------------------------
const float chromaAmount = 5.0;
vec3 chromatic(vec2 uv, vec3 color) {
    color.r = texture(texture0, (uv - 0.5) * (1.0 + chromaAmount / screenSize.xy) + 0.5).r;
    color.b = texture(texture0, (uv - 0.5) * (1.0 - chromaAmount / screenSize.xy) + 0.5).b;
    return color;
}

//------------------------------------------------------------------------------------
// FXAA
//------------------------------------------------------------------------------------
const float FXAA_SPAN_MAX = 8.0;
const float FXAA_REDUCE_MUL = 1.0 / 8.0;
const float FXAA_REDUCE_MIN = 1.0 / 128.0;

vec3 FXAA(vec2 texCoords) {
    vec2 inverse_resolution = vec2(1.0/screenSize.x,1.0/screenSize.y);
    
    vec3 rgbNW = texture2D(texture0, texCoords.xy + (vec2(-1.0,-1.0)) * inverse_resolution).xyz;
    vec3 rgbNE = texture2D(texture0, texCoords.xy + (vec2(1.0,-1.0)) * inverse_resolution).xyz;
    vec3 rgbSW = texture2D(texture0, texCoords.xy + (vec2(-1.0,1.0)) * inverse_resolution).xyz;
    vec3 rgbSE = texture2D(texture0, texCoords.xy + (vec2(1.0,1.0)) * inverse_resolution).xyz;
    
    vec3 rgbM  = texture2D(texture0,  fragTexCoord.xy).xyz;
    
    vec3 luma = vec3(0.299, 0.587, 0.114);
 
    float lumaNW = dot(rgbNW, luma);
    float lumaNE = dot(rgbNE, luma);
    float lumaSW = dot(rgbSW, luma);
    float lumaSE = dot(rgbSE, luma);
    float lumaM  = dot(rgbM,  luma);
    float lumaMin = min(lumaM, min(min(lumaNW, lumaNE), min(lumaSW, lumaSE)));
    float lumaMax = max(lumaM, max(max(lumaNW, lumaNE), max(lumaSW, lumaSE))); 
 
    vec2 dir;
    dir.x = -((lumaNW + lumaNE) - (lumaSW + lumaSE));
    dir.y =  ((lumaNW + lumaSW) - (lumaNE + lumaSE));
 
    float dirReduce = max((lumaNW + lumaNE + lumaSW + lumaSE) * (0.25 * FXAA_REDUCE_MUL),FXAA_REDUCE_MIN);
    float rcpDirMin = 1.0/(min(abs(dir.x), abs(dir.y)) + dirReduce);
 
    dir = min(vec2( FXAA_SPAN_MAX,  FXAA_SPAN_MAX),max(vec2(-FXAA_SPAN_MAX, -FXAA_SPAN_MAX),dir * rcpDirMin)) * inverse_resolution;
 
    vec3 rgbA = 0.5 * (texture2D(texture0,   texCoords.xy   + dir * (1.0/3.0 - 0.5)).xyz + texture2D(texture0,   texCoords.xy   + dir * (2.0/3.0 - 0.5)).xyz);
    vec3 rgbB = rgbA * 0.5 + 0.25 * (texture2D(texture0,  texCoords.xy   + dir *  - 0.5).xyz + texture2D(texture0,  texCoords.xy   + dir * 0.5).xyz);
    
    float lumaB = dot(rgbB, luma);
 
    if((lumaB < lumaMin) || (lumaB > lumaMax)) 
    {
        return rgbA;
    } 
    else 
    {
        return rgbB;
    }
}

//------------------------------------------------------------------------------------
// BLOOM
//------------------------------------------------------------------------------------
const float samples = 5.0; // Pixels per axis; higher = bigger glow, worse performance
const float quality = 2.5; // Size factor: Lower = smaller glow, better quality

vec4 bloom(sampler2D tex, vec2 texCoords) {
    vec4 sum = vec4(0);
    vec2 sizeFactor = vec2(1) / screenSize * quality;
    const int range = 2;

    for(int x = -range; x <= range; x++) {
        for(int y = -range; y <= range; y++) {
            //sum += clamp(texture(tex, texCoords + vec2(x, y)*sizeFactor) - threshold,0.0,1.0)*1.0/(1.0-threshold);
            sum += texture(tex, texCoords + vec2(x, y) * sizeFactor);
        }
    }
    return sum / (samples * samples);
}

//------------------------------------------------------------------------------------
// BLOOM2
//------------------------------------------------------------------------------------
const float threshold = 0.2;
const float pi2 = 6.28318530718;

const float blurDirections = 16.0;
const float blurQuality = 4.0;
const float blurSize = 8.0;

vec4 bloom2(sampler2D tex, vec2 texCoords) {

    vec2 radius = blurSize / screenSize;
    vec4 color = texture(tex, texCoords);

    for(float d = 0.0; d < pi2; d += pi2 / blurDirections) {
        for(float i = 1.0 / blurQuality; i <= 1.0; i += 1.0 / blurQuality) {
            color += clamp(texture(tex, texCoords + vec2(cos(d), sin(d)) * radius * i) - threshold, 0.0, 1.0) * 1.0 / (1.0 - threshold);
        }
    }

    color /= blurQuality * blurDirections - 15.0;
    return color;
}

//------------------------------------------------------------------------------------
// MAIN
//------------------------------------------------------------------------------------
void main() {
    vec2 uv = warp(fragTexCoord);
    //vec2 uv = fragTexCoord;
    finalColor = texture(texture0, uv);
    finalColor.rgb = FXAA(uv);
    finalColor.rgb = chromatic(uv, finalColor.rgb);
    finalColor.rgb += bloom2(texture0, uv).rgb;
    finalColor.rgb *= clean(uv);
    finalColor.rgb *= vignette(uv);
    finalColor.a = 1;
}
