#version 300 es
// Vertex information
layout(location = 0) in vec4  VertexPosition;
layout(location = 1) in vec3  Normal;
layout(location = 2) in vec2  TexCoords;


// Model View Project matrix
uniform mat4    MODELVIEWPROJECTIONMATRIX;
uniform mat4    MODELVIEWMATRIX;
uniform mat3    NormalMatrix;

out vec3    nNormal;
out vec3    pos;
out vec2    TextureCoord;

uniform mediump vec3 LightPosition;

out vec3 lightVec;//lightDir; 	  // interpolated surface local coordinate light direction
out vec3 eyeVec;//viewDir;     // interpolated surface local coordinate view direction

void main()
{
    vec3 b;
    vec3 n;
    vec3 t;
    vec3 r;
    vec3 v;

    nNormal      = normalize ( NormalMatrix * Normal );
    pos          = vec3 ( MODELVIEWMATRIX * VertexPosition );
    TextureCoord = TexCoords;

    vec3 tangent;
    vec3 v1 = cross(Normal, vec3(0.0, 0.0, 1.0));
    vec3 v2 = cross(Normal, vec3(0.0, -1.0, 0.0));
    if( length(v1) > length(v2) )
        tangent = v1;
    else
        tangent = v2;
    
    // Transform the input vectors
    t = normalize(NormalMatrix * tangent);
    n = normalize(NormalMatrix * Normal);
    b = cross(n, t);
    
    
    mat3 mat = mat3(t.x, b.x, n.x, t.y, b.y, n.y, t.z, b.z, n.z);
    
    vec3 vVertex = pos;
	vec3 tmpVec = normalize( LightPosition - vVertex);
    lightVec = mat * tmpVec;
	/*lightVec.x = dot(tmpVec, t);
	lightVec.y = dot(tmpVec, b);
	lightVec.z = dot(tmpVec, n);*/
    eyeVec = pos;
//	tmpVec = -pos;
  //  eyeVec = mat * tmpVec;
	
    t = normalize(NormalMatrix * vec3(0.0, 0.0, 1.0));
    b = normalize(NormalMatrix * vec3(0.0, -1.0, 0.0));
    n = normalize(NormalMatrix * Normal);
    
    /*eyeVec.x = dot(tmpVec, t);
	eyeVec.y = dot(tmpVec, b);
	eyeVec.z = dot(tmpVec, n);*/
    // Transform light position into surface local coordinates
    v.x = dot(LightPosition, t);
    v.y = dot(LightPosition, b);
    v.z = dot(LightPosition, n);
    lightVec = normalize(v);
    
    v.x = dot(pos, t);
    v.y = dot(pos, b);
    v.z = dot(pos, n);
    eyeVec = normalize(v);

    gl_Position = MODELVIEWPROJECTIONMATRIX * VertexPosition;
}